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
' This is version 0.8.2 of the code-generator python application.
'''

import types

import Context
import Functors

class CodeModelException(Exception):
	def __init__(self, message):
		Exception.__init__(self, message)
	
	def __str__(self):
		return Exception.__str__(self)

class BuildModelException(Exception):
	def __init__(self, code_part, message):
		Exception.__init__(self, message)
		self.__code_part = code_part
	
	def __get_code_part(self):
		return self.__code_part
	
	code_part = property(__get_code_part)

class CodePart(object):
	def __init__(self):
		self.__tag = None
		self.__file = None
		self.__line = None
		self.__column = None
		self.__parent = None
	
	def get(self, context):
		raise CodeModelException("Abstract get() function called on CodePart object.")
	
	def __get_tag(self):
		return self.__tag
	
	def __set_tag(self, tag):
		if tag == None:
			raise BuildModelException("Setting the tag attribute of a CodePart object to None is not allowed.")
		if self.__tag != None:
			raise BuildModelException("Changing the tag attribute on a CodePart object is only allowed once.")
		self.__tag = tag
	
	def __get_file(self):
		return self.__file
	
	def __set_file(self, file):
		if file == None:
			raise BuildModelException("Setting the file attribute of a CodePart object to None is not allowed.")
		if self.__file != None:
			raise BuildModelException("Changing the file attribute on a CodePart object is only allowed once.")
		self.__file = file
	
	def __get_line(self):
		return self.__line
	
	def __set_line(self, line):
		if line == None:
			raise BuildModelException("Setting the line attribute of a CodePart object to None is not allowed.")
		if self.__line != None:
			raise BuildModelException("Changing the line attribute on a CodePart object is only allowed once.")
		self.__line = line
	
	def __get_column(self):
		return self.__column
	
	def __set_column(self, column):
		if column == None:
			raise BuildModelException("Setting the column attribute of a CodePart object to None is not allowed.")
		if self.__column != None:
			raise BuildModelException("Changing the column attribute on a CodePart object is only allowed once.")
		self.__column = column
	
	def __get_parent(self):
		return self.__parent
	
	def __set_parent(self, parent):
		if parent == None:
			raise BuildModelException("Setting the parent attribute of a CodePart object to None is not allowed.")
		if self.__parent != None:
			raise BuildModelException("Changing the parent attribute on a CodePart object is only allowed once.")
		self.__parent = parent
	
	tag = property(__get_tag, __set_tag)
	file = property(__get_file, __set_file)
	line = property(__get_line, __set_line)
	column = property(__get_column, __set_column)
	parent = property(__get_parent, __set_parent)

class CodeContainer(CodePart):
	def __init__(self):
		CodePart.__init__(self)
		self.__code_part = None
	
	def get(self, context):
		if self.code_part == None:
			raise CodeModelException("The code is errornous because a \"" + self.tag + "\" code element does not contain any code elements.")
		return context.get(self.code_part)
	
	def add(self, code_part):
		if self.code_part != None:
			raise CodeModelException("The code is errornous because a \"" + self.tag + "\" code element contains multiple unconnected code elements.")
		if code_part.parent != None:
			raise BuildModelException("Adding a parented CodePart to a CodeContainer is not allowed.")
		code_part.parent = self
		self.__code_part = code_part
	
	def __get_code_part(self):
		return self.__code_part
	
	code_part = property(__get_code_part)

class CodeList(CodePart):
	def __init__(self):
		CodePart.__init__(self)
		self.__code_parts = list()
	
	def get(self, context):
		result = u""
		for code_part in self.__code_parts:
			code = context.get(code_part)
			if type(code) is types.UnicodeType:
				result += code
			else:
				raise CodeModelException("The code is errornous because a \"" + code_part.tag + "\" code element does not evaluate to a unicode string.")
		return result
	
	def add(self, code_part):
		if code_part.parent != None:
			raise BuildModelException("Adding a parented CodePart to a CodeList is not allowed.")
		code_part.parent = self
		self.__code_parts.append(code_part)
	
	def __get_code_parts(self):
		return self.__code_parts
	
	code_parts = property(__get_code_parts)

class If(CodePart):
	def __init__(self):
		CodePart.__init__(self)
		self.__condition_code_part = None
		self.__then_code_part = None
		self.__else_code_part = None
	
	def get(self, context):
		if self.condition_code_part != None:
			if context.get(self.condition_code_part) == True:
				if self.then_code_part != None:
					return context.get(self.then_code_part)
				else:
					return u""
			else:
				if self.else_code_part != None:
					return context.get(self.else_code_part)
				else:
					return u""
		else:
			raise BuildModelException(self, "The \"if\" code element is not evaluable because it does not contain a \"condition\" code element.")
	
	def add(self, code_part):
		if code_part.tag == "condition":
			if self.condition_code_part == None:
				code_part.parent = self
				self.__condition_code_part = code_part
			else:
				raise BuildModelException(code_part, "Adding more than one \"condition\" code element to an \"if\" code element is not allowed.")
		elif code_part.tag == "then":
			if self.then_code_part == None:
				code_part.parent = self
				self.__then_code_part = code_part
			else:
				raise BuildModelException(code_part, "Adding more than one \"then\" code element to an \"if\" code element is not allowed.")
		elif code_part.tag == "else":
			if self.else_code_part == None:
				code_part.parent = self
				self.__else_code_part = code_part
			else:
				raise BuildModelException(code_part, "Adding more than one \"else\" code element to an \"if\" code element is not allowed.")
		else:
			raise BuildModelException(code_part, "Adding a \"" + code_part.tag + "\" code element to an \"if\" code element is not allowed.")
	
	def __get_condition_code_part(self):
		return self.__condition_code_part
	
	def __get_then_code_part(self):
		return self.__then_code_part
	
	def __get_else_code_part(self):
		return self.__else_code_part
	
	condition_code_part = property(__get_condition_code_part)
	then_code_part = property(__get_then_code_part)
	else_code_part = property(__get_else_code_part)

class Foreach(CodePart):
	def __init__(self):
		CodePart.__init__(self)
		self.__name = None
		self.__over_code_part = None
		self.__do_code_part = None
	
	def get(self, context):
		if self.over_code_part != None:
			over_list = context.get(self.over_code_part)
			if len(over_list) > 0:
				if self.do_code_part != None:
					result = u""
					for element in over_list:
						element_variable = Context.Variable()
						element_variable.name = self.name
						element_variable.value = element
						context.push_scope()
						context.current_scope.add_variable(element_variable)
						result += context.get(self.do_code_part)
						context.pop_scope()
					return result
				else:
					raise BuildModelException(code_part, "The \"foreach\" code element is not evaluable because it does not contain a \"do\" code element and the over list is not empty.")
			else:
				return u""
		else:
			raise BuildModelException(code_part, "The \"foreach\" code element is not evaluable because it does not contain an \"over\" code element.")
	
	def add(self, code_part):
		if code_part.tag == "over":
			if self.over_code_part == None:
				code_part.parent = self
				self.__over_code_part = code_part
			else:
				raise BuildModelException(code_part, "Adding more than one \"over\" code element to a \"foreach\" code element is not allowed.")
		elif code_part.tag == "do":
			if self.do_code_part == None:
				code_part.parent = self
				self.__do_code_part = code_part
			else:
				raise BuildModelException(code_part, "Adding more than one \"do\" code element to a \"foreach\" code element is not allowed.")
		else:
			raise BuildModelException(code_part, "Adding a \"" + code_part.tag + "\" code element to an \"if\" code element is not allowed.")
	
	def __get_over_code_part(self):
		return self.__over_code_part
	
	def __get_do_code_part(self):
		return self.__do_code_part
	
	def __get_name(self):
		return self.__name
	
	def __set_name(self, name):
		self.__name = name
	
	over_code_part = property(__get_over_code_part)
	do_code_part = property(__get_do_code_part)
	name = property(__get_name, __set_name)

class Functor(CodeList):
	def __init__(self):
		CodeList.__init__(self)
		self.__functor = None
	
	def get(self, context):
		if self.functor == None:
			raise CodeModelException("A \"" + self.tag + "\" code element does not contain a valid function.")
		else:
			return self.functor(context, *self.code_parts)
	
	def __get_functor(self):
		return self.__functor
	
	def __set_functor(self, functor):
		self.__functor = functor
	
	functor = property(__get_functor, __set_functor)

class Literal(CodePart):
	def __init__(self):
		CodePart.__init__(self)
		self.__content = None
	
	def get(self, context):
		return self.content
	
	def __get_content(self):
		return self.__content
	
	def __set_content(self, content):
		self.__content = content
	
	content = property(__get_content, __set_content)

class MissingFunctor(CodeList):
	def __init__(self):
		CodeList.__init__(self)
	
	def get(self, context):
		raise Functors.ExecuteException(self, "Execution failed due to unknown \"" + self.tag + "\" functor.")
