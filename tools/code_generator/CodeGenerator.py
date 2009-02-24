# coding: utf-8

'''
' Copyright (C) 2008  Hagen Möbius
' 
' This program is free software; you can redistribute it and/or
' modify it under the terms of the GNU General Public License
' as published by the Free Software Foundation; either version 2
' of the License, or (at your option) any later version.
' 
' This program is distributed in the hope that it will be useful,
' but WITHOUT ANY WARRANTY; without even the implied warranty of
' MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
' GNU General Public License for more details.
'
' You should have received a copy of the GNU General Public License
' along with this program; if not, write to the Free Software
' Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
'''

'''
' This is version 0.8.0 of the code-generator python application.
'''

import optparse
import os
import sys
import types
import xml.parsers.expat

import CodeModel
import Context
import Functors
import Template

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

def append_separated(string, append, separator):
	if len(string) == 0:
		return append
	else:
		return string + separator + append

def get_functor_name(string):
	return "functor_" + string.replace("-", "_")

def get_path_string(code_part):
	result = ""
	while code_part != None:
		result = LightYellow + "/" + White + code_part.tag + result
		code_part = code_part.parent
	return result

class TemplateBodyParser:
	def __init__(self):
		self.__body = None
		self.__in_body = False
		self.__object_stack = list()
		self.__element_path = list()
		self.__parser = None
		self.__file = None
	
	def parse(self, file):
		self.__file = file
		self.__parser = xml.parsers.expat.ParserCreate()
		self.__parser.StartElementHandler = self.__on_element_start
		self.__parser.EndElementHandler = self.__on_element_end
		self.__parser.CharacterDataHandler = self.__on_character_data
		self.__parser.ParseFile(open(file))
		self.__parser = None
		self.__file = None
		return self.__body
	
	def __on_element_start(self, name, attributes):
		self.__element_path.append(name)
		if name == "body":
			if self.__body != None:
				print LightRed + "The template is errornous because it contains multiple \"body\" elements." + White
				sys.exit(1)
			code_part = CodeModel.CodeList()
			code_part.tag = name
			code_part.file = self.__file
			code_part.line = self.__parser.CurrentLineNumber
			code_part.column = self.__parser.CurrentColumnNumber
			self.__object_stack.append(code_part);
			self.__body = self.__object_stack[-1]
			self.__in_body = True
		else:
			if self.__in_body == True:
				code_part = None
				if name == "if":
					code_part = CodeModel.If()
				elif name == "condition":
					if self.__object_stack[-1].tag == "if":
						code_part = CodeModel.CodeContainer()
					else:
						raise CodeModel.BuildModelException(self.__object_stack[-1], "A \"condition\" code element must be a sub element of an \"if\" code element.")
				elif name == "then":
					if self.__object_stack[-1].tag == "if":
						code_part = CodeModel.CodeList()
					else:
						raise CodeModel.BuildModelException(self.__object_stack[-1], "A \"then\" code element must be a sub element of an \"if\" code element.")
				elif name == "else":
					if self.__object_stack[-1].tag == "if":
						code_part = CodeModel.CodeList()
					else:
						raise CodeModel.BuildModelException(self.__object_stack[-1], "An \"else\" code element must be a sub element of an \"if\" code element.")
				elif name == "foreach":
					code_part = CodeModel.Foreach()
					if "name" in attributes:
						code_part.name = attributes["name"]
				elif name == "over":
					if self.__object_stack[-1].tag == "foreach":
						code_part = CodeModel.CodeContainer()
					else:
						raise CodeModel.BuildModelException(self.__object_stack[-1], "An \"over\" code element must be a sub element of a \"foreach\" code element.")
				elif name == "do":
					if self.__object_stack[-1].tag == "foreach":
						code_part = CodeModel.CodeList()
					else:
						raise CodeModel.BuildModelException(self.__object_stack[-1], "A \"do\" code element must be a sub element of a \"foreach\" code element.")
				else:
					if get_functor_name(name) in Functors.__dict__:
						code_part = CodeModel.Functor()
						code_part.functor = Functors.__dict__[get_functor_name(name)]
					else:
						code_part = CodeModel.MissingFunctor()
				code_part.tag = name
				code_part.file = self.__file
				code_part.line = self.__parser.CurrentLineNumber
				code_part.column = self.__parser.CurrentColumnNumber
				self.__object_stack[-1].add(code_part)
				self.__object_stack.append(code_part)
	
	def __on_element_end(self, name):
		if self.__in_body == True:
			if name == "body":
				self.__in_body = False
			del self.__object_stack[-1]
		self.__element_path = self.__element_path[:-1]
	
	def __on_character_data(self, character_data):
		if self.__in_body == True:
			# by his we combine adjacent literal code parts
			if len(self.__object_stack[-1].code_parts) > 0 and self.__object_stack[-1].code_parts[-1].tag == "literal":
				self.__object_stack[-1].code_parts[-1].content += character_data
			else:
				self.__element_path.append("literal")
				code_part = CodeModel.Literal()
				code_part.tag = "literal"
				code_part.file = self.__file
				code_part.line = self.__parser.CurrentLineNumber
				code_part.column = self.__parser.CurrentColumnNumber
				code_part.content = character_data
				self.__object_stack[-1].add(code_part)
				self.__element_path = self.__element_path[:-1]

option_parser = optparse.OptionParser()
option_parser.add_option("-l", "--list", dest = "list", help = "Lists the templates in a directory.")
option_parser.add_option("-i", "--in", dest = "in_file", help = "The code template to read.")
option_parser.add_option("-o", "--out", dest = "out_file", help = "The code file to write the result to.")
option_parser.add_option("-v", "--variable", dest = "variables", action = "append", default = list(), help = "Specify variable values.")

(options, args) = option_parser.parse_args()

if options.list != None:
	first = True
	path = os.path.abspath(sys.argv[2])
	files = os.listdir(path)
	for file in files:
		file_path = os.path.join(path, file)
		if os.path.isdir(file_path) == True:
			continue
		if first == True:
			first = False
		else:
			print
		print "File: " + LightGreen + file_path + White
		try:
			header_parser = Template.TemplateHeaderParser()
			header = header_parser.parse(file_path)
		except xml.parsers.expat.ExpatError:
			print LightRed + "  This file doesn't seem to be a valid template file." + White
			continue
		if header == None:
			print LightRed + "  This file doesn't seem to be a valid template file." + White
			continue
		print "   " + LightBlue + "Identifier" + White + ": " + LightYellow + file + White
		print "   " + LightBlue + "Name" + White + ": " + LightYellow + header.name + White
		print "   " + LightBlue + "Description" + White + ": " + LightYellow + header.description + White
		label_string = ""
		for label in header.labels:
			if label.content == None:
				label_string = append_separated(label_string, LightYellow + label.category + White, ", ")
			else:
				label_string = append_separated(label_string, LightYellow + label.category + White + "=" + DarkYellow + label.content + White, ", ")
		print "   " + LightBlue + "Labels" + White + ": " + label_string
		variable_string = ""
		for variable in header.variables.values():
			variable_string = append_separated(variable_string, DarkYellow + variable.type + White + " " + LightYellow + variable.name + White, ", ")
		print "   " + LightBlue + "Variables" + White + ": " + variable_string
else:
	if options.in_file != None:
		template_file = os.path.abspath(options.in_file)
	if os.path.exists(template_file) == False:
		raise Exception("Could not file the file \"" + template_file + "\".")
	elif os.path.isfile(template_file) == False:
		raise Exception("\"template_file\" is not a file.")
	out_file = None
	command_line_variables = dict()
	for variable in options.variables:
		name, value = variable.split("=")
		command_line_variables[unicode(name)] = unicode(value)
	try:
		body_parser = TemplateBodyParser()
		body = body_parser.parse(template_file)
		header_parser = Template.TemplateHeaderParser()
		header = header_parser.parse(template_file)
		context = Context.Context()
		context.push_scope()
		# Template.Name
		context_variable = Context.Variable()
		context_variable.name = u"Template.Name"
		context_variable.value = header.name
		context.current_scope.add_variable(context_variable)
		# Template.UUID
		context_variable = Context.Variable()
		context_variable.name = u"Template.UUID"
		context_variable.value = header.uuid
		context.current_scope.add_variable(context_variable)
		# Template.Description
		context_variable = Context.Variable()
		context_variable.name = u"Template.Description"
		context_variable.value = header.description
		context.current_scope.add_variable(context_variable)
		# Template variables
		for header_variable in header.variables.values():
			context_variable = Context.Variable()
			context_variable.name = header_variable.name
			# allow default values to be overridden on the command line
			if header_variable.name in command_line_variables:
				header_variable.value = command_line_variables[header_variable.name]
			context_variable.value = header_variable.value
			context.current_scope.add_variable(context_variable)
		result = context.get(body)
		if options.out_file != None:
			out_file = open(options.out_file, "w")
			out_file.write(result)
		else:
			print result
	except CodeModel.CodeModelException, exception:
		print LightRed + str(exception) + White
		sys.exit(1)
	except Functors.ExecuteException, exception:
		print exception.code_part.file + ":" + str(exception.code_part.line) + ":" + str(exception.code_part.column) + ": " + get_path_string(exception.code_part)
		print "  " + LightRed + "ExecuteException" + White + ": " + exception.message
		sys.exit(1)
