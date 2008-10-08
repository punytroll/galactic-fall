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

class Variable(object):
	def __init__(self):
		self.__name = None
		self.__value = None
	
	def __get_name(self):
		return self.__name
	
	def __set_name(self, name):
		assert type(name) is types.UnicodeType
		self.__name = name
	
	def __get_value(self):
		return self.__value
	
	def __set_value(self, value):
		self.__value = value
	
	name = property(__get_name, __set_name)
	value = property(__get_value, __set_value)

class Scope(object):
	def __init__(self):
		self.__variables = dict()
	
	def add_variable(self, variable):
		assert variable.name not in self.__variables
		self.__variables[variable.name] = variable
	
	def get_variable(self, name):
		assert type(name) is types.UnicodeType
		if name in self.__variables.keys():
			return self.__variables[name]
		else:
			return None

class Context(object):
	def __init__(self):
		self.__stack = list()
		self.__current_code_part = None
	
	def get(self, code_part):
		save_current_code_part = self.current_code_part
		self.__current_code_part = code_part
		result = self.__current_code_part.get(self)
		self.__current_code_part = save_current_code_part
		return result
	
	def get_variable(self, name):
		assert type(name) is types.UnicodeType
		for scope in reversed(self.__stack):
			variable = scope.get_variable(name)
			if variable != None:
				return variable
		return None
	
	def push_scope(self):
		self.__stack.append(Scope())
	
	def pop_scope(self):
		self.__stack.pop()
	
	def __get_current_code_part(self):
		return self.__current_code_part
	
	def __get_current_scope(self):
		assert len(self.__stack) > 0
		return self.__stack[-1]
	
	current_code_part = property(__get_current_code_part)
	current_scope = property(__get_current_scope)
