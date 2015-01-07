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
	def __init__(self, name):
		self.__element_type_identifier = None
		self.__is_built_in = False
		self.__is_declaration = None
		self.__name = name
		self.__parts = list()
		self.__sub_type = None
		self.__type = None
	
	def get_element_type_identifier(self):
		return self.__element_type_identifier
	
	def get_is_built_in(self):
		return self.__is_built_in
	
	def get_name(self):
		return self.__name
	
	def __get_is_declaration(self):
		return self.__is_declaration
	
	def __get_parts(self):
		return self.__parts
	
	def __get_sub_type(self):
		return self.__sub_type
	
	def __get_type(self):
		return self.__type
	
	def set_element_type_identifier(self, element_type_identifier):
		self.__element_type_identifier = element_type_identifier
	
	def set_is_built_in(self, is_built_in):
		self.__is_built_in = is_built_in
	
	def __set_is_declaration(self, is_declaration):
		self.__is_declaration = is_declaration
	
	def __set_sub_type(self, sub_type):
		self.__sub_type = sub_type
	
	def __set_type(self, type):
		self.__type = type
	
	is_declaration = property(__get_is_declaration, __set_is_declaration)
	parts = property(__get_parts)
	sub_type = property(__get_sub_type, __set_sub_type)
	type = property(__get_type, __set_type)

class Part(object):
	def __init__(self, name, type_identifier):
		self.__element_type_identifier = None
		self.__type_identifier = type_identifier
		self.__name = name
	
	def get_element_type_identifier(self):
		return self.__element_type_identifier
	
	def get_type_identifier(self):
		return self.__type_identifier
	
	def get_name(self):
		return self.__name
	
	def set_element_type_identifier(self, element_type_identifier):
		self.__element_type_identifier = element_type_identifier

def add_built_in_type(name):
	declaration = Declaration(name)
	declaration.set_is_built_in(True)
	declarations[declaration.get_name()] = declaration

# read the declarations first
declarations = dict()
add_built_in_type("boolean")
add_built_in_type("collection")
add_built_in_type("file")
add_built_in_type("float")
add_built_in_type("string")
add_built_in_type("u1byte")
add_built_in_type("u4byte")
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
					if declaration.is_declaration == "collection":
						if declaration_element.attributes.has_key("element-type") == True:
							declaration.set_element_type_identifier(declaration_element.attributes.get("element-type").nodeValue)
						else:
							print LightRed + "Error" + White + ": In file " + LightYellow + options.declarations + White + " the " + LightBlue + declaration_element.tagName + White + " declaration is a " + LightBlue + "collection" + White + " alias but missing the " + LightYellow + "element-type" + White + " attribute."
							raise ConvertException()
				else:
					for part_element in declaration_element.childNodes:
						if part_element.nodeType == Node.ELEMENT_NODE:
							part = None
							if part_element.attributes.has_key("identifier") == True:
								part = Part(part_element.attributes.get("identifier").nodeValue, part_element.tagName)
							else:
								print LightRed + "Error" + White + ": In file " + LightYellow + options.declarations + White + " there is a declaration part without an " + LightYellow + "identifier" + White + " attribute on the " + LightYellow + declaration_element.tagName + White + " declaration."
								raise ConvertException()
							if part_element.tagName == "collection":
								if part_element.attributes.has_key("element-type") == True:
									part.set_element_type_identifier(part_element.attributes.get("element-type").nodeValue)
								else:
									print LightRed + "Error" + White + ": In file " + LightYellow + options.declarations + White + " on the " + LightBlue + declaration_element.tagName + White + " declaration, there is a collection " + LightYellow + part.get_name() + White + " declared without an " + LightYellow + "element-type" + White + " attribute."
									raise ConvertException()
							declaration.parts.append(part)
				declarations[declaration.get_name()] = declaration
			else:
				if declarations[declaration_element.tagName].get_is_built_in() == True:
					print LightRed + "Error" + White + ": In file " + LightYellow + options.declarations + White + " there is an invalid re-declaration for the built-in type " + LightBlue + declaration_element.tagName + White + "."
					raise ConvertException()
				else:
					print LightRed + "Error" + White + ": In file " + LightYellow + options.declarations + White + " there is a duplicate declaration for " + LightYellow + declaration_element.tagName + White + "."
					raise ConvertException()

# now open the out file for writing binary
out_file = open(options.out_file, "wb")

# the out() call stack
out_call_stack = list()

# the recursive output function
def out(data_type, node, element_type = None):
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
		elif node.firstChild.nodeValue == "false":
			out_file.write(pack('B', 0))
		else:
			print LightRed + "Error" + White + ": In file " + LightYellow + options.in_file + White + " for " + LightYellow + stack_path + White + " of type " + DarkYellow + data_type + White + " I found an invalid value: " + LightRed + node.firstChild.nodeValue + White
			raise ConvertException()
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
	elif data_type == "collection":
		count = 0
		for node_part in node.childNodes:
			if node_part.nodeType == Node.ELEMENT_NODE:
				count += 1
		out_file.write(pack('I', long(count)))
		for node_part in node.childNodes:
			if node_part.nodeType == Node.ELEMENT_NODE:
				if element_type != None:
					if node_part.tagName == element_type:
						out(node_part.tagName, node_part)
					else:
						print LightRed + "Error" + White + ": In file " + LightYellow + options.in_file + White + " for the collection " + LightYellow + stack_path + White + " the element type " + LightBlue + element_type + White + " was declared but not followed by " + LightYellow + node_part.tagName + White + "."
						raise ConvertException()
				else:
					print LightRed + "Error" + White + ": In file " + LightYellow + options.in_file + White + " for the collection " + LightYellow + stack_path + White + " no element type was declared."
					raise ConvertException()
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
			out_call_stack.pop()
			out(declarations[data_type].is_declaration, node, declarations[data_type].get_element_type_identifier())
			out_call_stack.append(node.tagName)
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
					print LightRed + "Error" + White + ": In file " + LightYellow + options.in_file + White + " for " + LightYellow + stack_path + "/" + declaration_part.get_name() + White + " of type " + DarkYellow + declaration_part.get_type_identifier() + White + " I could not find any definition."
					raise ConvertException()
				if declaration_part.get_name() != definition_node.tagName:
					print LightRed + "Error" + White + ": In file " + LightYellow + options.in_file + White + " the definition for " + LightYellow + stack_path + "/" + definition_node.tagName + White + " does not belong there."
					print "       Expected to find a definition for " + LightYellow + stack_path + "/" + declaration_part.name + White + " of type " + LightBlue + declaration_part.type_identifier + White + "."
					raise ConvertException()
				else:
					out(declaration_part.get_type_identifier(), definition_node, declaration_part.get_element_type_identifier())
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
