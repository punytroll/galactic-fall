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

import types
import xml.parsers.expat

class TemplateException(Exception):
	def __init__(self, message):
		Exception.__init__(self, message)
	
	def __str__(self):
		return Exception.__str__(self)

class Label(object):
	def __init__(self):
		self.__category = None
		self.__content = None
	
	def __get_category(self):
		return self.__category
	
	def __set_category(self, category):
		self.__category = category
	
	def __get_content(self):
		return self.__content
	
	def __set_content(self, content):
		self.__content = content
	
	category = property(__get_category, __set_category)
	content = property(__get_content, __set_content)

class Variable(object):
	def __init__(self):
		self.__type = None
		self.__name = None
		self.__value = None
		self.__description = None
	
	def __get_type(self):
		return self.__type
	
	def __set_type(self, type):
		self.__type = type
	
	def __get_name(self):
		return self.__name
	
	def __set_name(self, name):
		self.__name = name
	
	def __get_value(self):
		return self.__value
	
	def __set_value(self, value):
		assert self.type != None
		assert type(value) is types.UnicodeType
		if self.type == "string":
			self.__value = value
		elif self.type == "boolean":
			if value == u"true":
				self.__value = True
			elif value == u"false":
				self.__value = False
			else:
				raise TemplateException("The value \"" + value + "\" for the variable \"" + self.name + "\" could not be converted to the type \"" + self.type + "\".")
		elif self.type == "integer":
			self.__value = int(value)
		else:
			raise TemplateException("The value \"" + value + "\" for the variable \"" + self.name + "\" could not be converted to the type \"" + self.type + "\".")
	
	def __get_description(self):
		return self.__description
	
	def __set_description(self, description):
		self.__description = description
	
	type = property(__get_type, __set_type)
	name = property(__get_name, __set_name)
	value = property(__get_value, __set_value)
	description = property(__get_description, __set_description)

class Header(object):
	def __init__(self):
		self.__description = None
		self.__labels = list()
		self.__name = None
		self.__uuid = None
		self.__variables = dict()
	
	def __get_description(self):
		return self.__description
	
	def __set_description(self, description):
		self.__description = description
	
	def __get_labels(self):
		return self.__labels
	
	def __get_name(self):
		return self.__name
	
	def __set_name(self, name):
		self.__name = name
	
	def __get_uuid(self):
		return self.__uuid
	
	def __set_uuid(self, uuid):
		self.__uuid = uuid
	
	def __get_variables(self):
		return self.__variables
	
	description = property(__get_description, __set_description)
	labels = property(__get_labels)
	name = property(__get_name, __set_name)
	uuid = property(__get_uuid, __set_uuid)
	variables = property(__get_variables)

class TemplateHeaderParser(object):
	def __init__(self):
		self.__header = None
		self.__in_header = False
		self.__in_body = False
		self.__in_uuid = False
	
	def parse(self, file_path):
		parser = xml.parsers.expat.ParserCreate()
		parser.StartElementHandler = self.__on_element_start
		parser.EndElementHandler = self.__on_element_end
		parser.CharacterDataHandler = self.__on_character_data
		parser.ParseFile(open(file_path))
		return self.__header
	
	def __on_element_start(self, name, attributes):
		if name == "body":
			if self.__in_header == False and self.__in_body == False:
				self.__in_body = True
		if name == "header":
			if self.__in_header == False and self.__in_body == False:
				assert self.__header == None
				self.__header = Header()
				self.__in_header = True
				if "name" in attributes:
					self.__header.name = attributes["name"]
				if "description" in attributes:
					self.__header.description = attributes["description"]
		elif name == "label":
			if self.__in_header == True:
				assert self.__header != None
				label = Label()
				if "category" in attributes:
					label.category = attributes["category"]
				if "content" in attributes:
					label.content = attributes["content"]
				self.__header.labels.append(label)
		elif name == "variable":
			if self.__in_header == True:
				assert self.__header != None
				variable = Variable()
				if "type" in attributes:
					variable.type = attributes["type"]
				if "name" in attributes:
					variable.name = attributes["name"]
				if "default-value" in attributes:
					variable.value = attributes["default-value"]
				if "description" in attributes:
					variable.description = attributes["description"]
				self.__header.variables[variable.name] = variable
		elif name == "uuid":
			if self.__in_header == True:
				assert self.__header != None
				self.__in_uuid = True
	
	def __on_element_end(self, name):
		if name == "header":
			self.__in_header = False
		elif name == "body":
			self.__in_body = False
		elif name == "uuid":
			self.__in_uuid = False
	
	def __on_character_data(self, character_data):
		if self.__in_uuid == True:
			if self.__header.uuid == None:
				self.__header.uuid = character_data
			else:
				self.__header.uuid += character_data
