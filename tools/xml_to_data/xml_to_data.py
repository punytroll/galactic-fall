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

class Declaration(object):
	def __init__(self, identifier):
		self.__identifier = identifier
		self.__is_declaration = None
		self.__parts = list()
		self.__sub_type = None
		self.__type = None
	
	def __get_identifier(self):
		return self.__identifier
	
	def __get_is_declaration(self):
		return self.__is_declaration
	
	def __get_parts(self):
		return self.__parts
	
	def __get_sub_type(self):
		return self.__sub_type
	
	def __get_type(self):
		return self.__type
	
	def __set_is_declaration(self, is_declaration):
		self.__is_declaration = is_declaration
	
	def __set_sub_type(self, sub_type):
		self.__sub_type = sub_type
	
	def __set_type(self, type):
		self.__type = type
	
	identifier = property(__get_identifier)
	is_declaration = property(__get_is_declaration, __set_is_declaration)
	parts = property(__get_parts)
	sub_type = property(__get_sub_type, __set_sub_type)
	type = property(__get_type, __set_type)

class Part(object):
	def __init__(self, name, type_identifier):
		self.__type_identifier = type_identifier
		self.__name = name
	
	def __get_type_identifier(self):
		return self.__type_identifier
	
	def __get_name(self):
		return self.__name
	
	type_identifier = property(__get_type_identifier)
	name = property(__get_name)

# read the declarations first
declarations = dict();
if options.declarations != None:
	declarations_document = parse(options.declarations)
	declarations_element = declarations_document.documentElement
	for declaration_element in declarations_element.childNodes:
		if declaration_element.nodeType == Node.ELEMENT_NODE:
			if declarations.has_key(declaration_element.tagName) == False:
				declaration = Declaration(declaration_element.tagName)
				if declaration_element.attributes.has_key("type") == True:
					declaration.type = int(declaration_element.attributes.get("type").nodeValue)
				if declaration_element.attributes.has_key("sub-type") == True:
					declaration.sub_type = int(declaration_element.attributes.get("sub-type").nodeValue)
				if declaration_element.attributes.has_key("is") == True:
					declaration.is_declaration = declaration_element.attributes.get("is").nodeValue
				else:
					for part_element in declaration_element.childNodes:
						if part_element.nodeType == Node.ELEMENT_NODE:
							if part_element.attributes.has_key("identifier") == True:
								declaration.parts.append(Part(part_element.attributes.get("identifier").nodeValue, part_element.tagName))
							else:
								print LightRed + "Error" + White + ": In file " + LightYellow + options.declarations + White + " there is a declaration part without an " + LightYellow + "identifier" + White + " attribute on the " + LightYellow + declaration_element.tagName + White + " declaration."
								raise ConvertException()
				declarations[declaration.identifier] = declaration
			else:
				print LightRed + "Error" + White + ": In file " + LightYellow + options.declarations + White + " there is a duplicate declaration for " + LightYellow + declaration_element.tagName + White + "."
				raise ConvertException()

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
	elif data_type == "array-with-types":
		count = 0
		for node_part in node.childNodes:
			if node_part.nodeType == Node.ELEMENT_NODE:
				count += 1
		out_file.write(pack('I', long(count)))
		for node_part in node.childNodes:
			if node_part.nodeType == Node.ELEMENT_NODE:
				data_type = node_part.tagName
				if declarations.has_key(data_type) == False:
					print LightRed + "Error" + White + ": In file " + LightYellow + options.in_file + White + " in " + LightYellow + stack_path + White + " I could not find any declaration for " + DarkYellow + data_type + White + "."
					raise ConvertException()
				declaration = declarations[data_type]
				if declaration.type == None:
					print LightRed + "Error" + White + ": In file " + LightYellow + options.in_file + White + " for the array with types " + LightYellow + stack_path + White + " no type was defined in the declaration of " + DarkYellow + data_type + White + "."
					raise ConvertException()
				if declaration.sub_type == None:
					print LightRed + "Error" + White + ": In file " + LightYellow + options.in_file + White + " for the array with types " + LightYellow + stack_path + White + " no sub-type was defined in the declaration of " + DarkYellow + data_type + White + "."
					raise ConvertException()
				out_file.write(pack('II', long(declaration.type), long(declaration.sub_type)))
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
		if declarations[data_type].is_declaration != None:
			out(declarations[data_type].is_declaration, node)
		else:
			for child_node in node.childNodes:
				if child_node.nodeType != Node.ELEMENT_NODE:
					node.removeChild(child_node)
			# this fills the parts list with pairs of values from the first and second list extending with None if necessary
			parts = map(None, declarations[data_type].parts, node.childNodes)
			for declaration_part, definition_node in parts:
				# safe-guard
				assert declaration_part != None or definition_node != None
				if declaration_part == None:
					print LightRed + "Error" + White + ": In file " + LightYellow + options.in_file + White + " for " + LightYellow + stack_path + "/" + definition_node.tagName + White + " I could not find any declaration."
					raise ConvertException()
				if definition_node == None:
					print LightRed + "Error" + White + ": In file " + LightYellow + options.in_file + White + " for " + LightYellow + stack_path + "/" + declaration_part.name + White + " of type " + DarkYellow + declaration_part.type_identifier + White + " I could not find any definition."
					raise ConvertException()
				if declaration_part.name != definition_node.tagName:
					print LightRed + "Error" + White + ": In file " + LightYellow + options.in_file + White + " the definition for " + LightYellow + stack_path + "/" + definition_node.tagName + White + " does not belong there."
					print "       Expected to find a definition for " + LightYellow + stack_path + "/" + declaration_part.name + White + " of type " + LightBlue + declaration_part.type_identifier + White + "."
					raise ConvertException()
				else:
					out(declaration_part.type_identifier, definition_node)
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
