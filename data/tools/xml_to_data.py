#! /usr/bin/env python

from os import remove
from pprint import pprint
from shutil import copyfileobj
from struct import pack
from sys import exit
from types import ListType, StringTypes
from xml.dom.minidom import Node, parse
from optparse import OptionParser

DarkYellow = "\033[33m"
LightYellow = "\033[33;1m"
DarkGreen = "\033[32m"
LightGreen = "\033[32;1m"
Gray = "\033[30;1m"
LightRed = "\033[31;1m"
DarkRed = "\033[31m"
White = "\033[0m"
DarkBlue = "\033[34m"
LightBlue = "\033[34;1m"

class ConvertException(Exception):
	pass

parser = OptionParser()
parser.add_option("-d", "--declarations", dest="declarations", help="Load the declarations from the file.")
parser.add_option("-i", "--in", dest="in_file", help="The file to convert to binary form.")
parser.add_option("-o", "--out", dest="out_file", help="The file to put the binary form into.")

# read the options and validate
(options, args) = parser.parse_args()
if options.in_file == None:
	print LightRed + "Usage" + White + ": Set the file to convert to binary form with " + LightYellow + "--in" + White + "."
	exit(1)
if options.out_file == None:
	print LightRed + "Usage" + White + ": Set the file to write the binary form to with " + LightYellow + "--out" + White + "."
	exit(1)

# read the declarations first
declarations = dict();
if options.declarations != None:
	declarations_document = parse(options.declarations)
	declarations_element = declarations_document.documentElement
	for declaration_element in declarations_element.childNodes:
		if declaration_element.nodeType == Node.ELEMENT_NODE and declarations.has_key(declaration_element.tagName) == False:
			if declaration_element.attributes.has_key("is") == True:
				declarations[declaration_element.tagName] = declaration_element.attributes.get("is").nodeValue
			else:
				declaration = list()
				for part_element in declaration_element.childNodes:
					if part_element.nodeType == Node.ELEMENT_NODE and part_element.attributes.has_key("identifier") == True:
						declaration.append((part_element.attributes.get("identifier").nodeValue, part_element.tagName))
				declarations[declaration_element.tagName] = declaration

# now open the out file for writing binary
out_file = open(options.out_file, "wb")

# the out() call stack
out_call_stack = list()

# the recursive output function
def out(data_type, node):
	out_call_stack.append(node.tagName)
	stack_path = ""
	for stack_entry in out_call_stack:
		stack_path += "/" + stack_entry
	if data_type == "string":
		if node.firstChild != None:
			out_file.write(node.firstChild.nodeValue)
		out_file.write(pack('B', 0))
	elif data_type == "boolean":
		if node.firstChild.nodeValue == "true":
			out_file.write(pack('B', 1))
		else:
			out_file.write(pack('B', 0))
	elif data_type == "float":
		out_file.write(pack('f', float(node.firstChild.nodeValue)))
	elif data_type == "u1byte":
		try:
			out_file.write(pack('B', int(node.firstChild.nodeValue)))
		except ValueError:
			print LightRed + "Error" + White + ": In file " + LightYellow + options.in_file + White + " for " + LightYellow + stack_path + White + " of type " + DarkYellow + data_type + White + " I found an invalid value: " + LightRed + node.firstChild.nodeValue + White
			raise ConvertException()
	elif data_type == "u4byte":
		try:
			out_file.write(pack('I', long(node.firstChild.nodeValue)))
		except ValueError:
			print LightRed + "Error" + White + ": In file " + LightYellow + options.in_file + White + " for " + LightYellow + stack_path + White + " of type " + DarkYellow + data_type + White + " I found an invalid value: " + LightRed + node.firstChild.nodeValue + White
			raise ConvertException()
	elif data_type == "array":
		count = 0
		for node_part in node.childNodes:
			if node_part.nodeType == Node.ELEMENT_NODE:
				count += 1
		out_file.write(pack('I', long(count)))
		for node_part in node.childNodes:
			if node_part.nodeType == Node.ELEMENT_NODE:
				out(node_part.tagName, node_part)
	elif data_type == "file":
		inline_file_name = node.firstChild.nodeValue
		try:
			inline_file = open(inline_file_name, "rb")
			copyfileobj(inline_file, out_file)
		except IOError:
			print LightRed + "Error" + White + ": In file " + LightYellow + options.in_file + White + " for " + LightYellow + stack_path + White + " of type " + DarkYellow + data_type + White + " I couldn't open the file: " + LightRed + inline_file_name + White
			raise ConvertException()
	elif declarations.has_key(data_type) == True:
		if isinstance(declarations[data_type], ListType):
			for child_node in node.childNodes:
				if child_node.nodeType != Node.ELEMENT_NODE:
					node.removeChild(child_node)
			parts = map(None, declarations[data_type], node.childNodes)
			for declaration, definition_node in parts:
				# safe-guard
				assert declaration != None or definition_node != None
				if declaration == None:
					print LightRed + "Error" + White + ": In file " + LightYellow + options.in_file + White + " for " + LightYellow + stack_path + "/" + definition_node.tagName + White + " I could not find any declaration."
					raise ConvertException()
				declaration_name, declaration_type = declaration
				if definition_node == None:
					print LightRed + "Error" + White + ": In file " + LightYellow + options.in_file + White + " for " + LightYellow + stack_path + "/" + declaration_name + White + " of type " + DarkYellow + declaration_type + White + " I could not find any definition."
					raise ConvertException()
				if declaration_name != definition_node.tagName:
					print LightRed + "Error" + White + ": In file " + LightYellow + options.in_file + White + " the definition for " + LightYellow + stack_path + "/" + definition_node.tagName + White + " does not belong there."
					print "               Expected to find a definition for " + LightYellow + stack_path + "/" + declaration_name + White + " of type " + DarkYellow + declaration_type + White + "'."
					raise ConvertException()
				else:
					out(declaration_type, definition_node)
		elif isinstance(declarations[data_type], StringTypes):
			out(declarations[data_type], node)
	else:
		print LightRed + "Error" + White + ": In file " + LightYellow + options.in_file + White + " for " + LightYellow + stack_path + White + " of type " + DarkYellow + data_type + White + " I could not find a suitable declaration."
		raise ConvertException()
	out_call_stack.pop()

# now parse the in file
in_document = parse(options.in_file)
in_element = in_document.documentElement
if in_element.nodeType == Node.ELEMENT_NODE:
	try:
		out(in_element.tagName, in_element)
	except ConvertException:
		out_file.close()
		remove(options.out_file)
		exit(1)
