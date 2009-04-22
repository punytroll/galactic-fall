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

class ExecuteException(Exception):
	def __init__(self, code_part, message):
		Exception.__init__(self, message)
		self.__code_part = code_part
	
	def __get_code_part(self):
		return self.__code_part
	
	code_part = property(__get_code_part)

def functor_add(context, *arguments):
	if len(arguments) == 2:
		first_value = context.get(arguments[0])
		second_value = context.get(arguments[1])
		if type(first_value) is types.IntType and type(second_value) is types.IntType:
			return first_value + second_value
		else:
			raise ExecuteException(context.current_code_part, "The functor \"minus\" requires two integer arguments but got \"" + str(type(from_value)) + "\" and \"" + str(type(to_value)) + "\" arguments.")
	else:
		raise ExecuteException(context.current_code_part, "The functor \"minus\" requires exactely two arguments but " + str(len(arguments)) + " have been given.")

def functor_and(context, *arguments):
	if len(arguments) == 2:
		first_value = context.get(arguments[0])
		second_value = context.get(arguments[1])
		if type(first_value) is types.BooleanType and type(second_value) is types.BooleanType:
			return first_value and second_value
		else:
			raise ExecuteException(context.current_code_part, "The functor \"and\" requires two boolean arguments but got \"" + str(type(first_value)) + "\" and \"" + str(type(second_value)) + "\" arguments.")
	else:
		raise ExecuteException(context.current_code_part, "The functor \"and\" requires exactely two arguments but " + str(len(arguments)) + " have been given.")

def functor_concatenate(context, *arguments):
	result = u""
	for code_part in arguments:
		part_value = context.get(code_part)
		if type(part_value) is types.UnicodeType:
			result += part_value
		else:
			raise ExecuteException(context.current_code_part, "The functor \"concatenate\" requires all arguments to be of type \"unicode\" but at least one was of type \"" + str(type(part_value)) + "\".")
	return result

def functor_equal(context, *arguments):
	if len(arguments) == 2:
		first_value = context.get(arguments[0])
		second_value = context.get(arguments[1])
		if first_value == second_value:
			return True
		else:
			return False
	return ExecuteException(context.current_code_part, "The functor \"equal\" requires exactely two arguments but " + str(len(arguments)) + " have been given.")

def functor_greater(context, *arguments):
	if len(arguments) == 2:
		first_value = context.get(arguments[0])
		second_value = context.get(arguments[1])
		if type(first_value) is types.IntType and type(second_value) is types.IntType:
			return first_value > second_value
		else:
			raise ExecuteException(context.current_code_part, "The functor \"greater\" requires two integer arguments but got \"" + str(type(from_value)) + "\" and \"" + str(type(to_value)) + "\" arguments.")
	else:
		raise ExecuteException(context.current_code_part, "The functor \"greater\" requires exactely two arguments but " + str(len(arguments)) + " have been given.")

def functor_not_equal(context, *arguments):
	if len(arguments) == 2:
		first_value = context.get(arguments[0])
		second_value = context.get(arguments[1])
		if first_value == second_value:
			return False
		else:
			return True
	else:
		raise ExecuteException(context.current_code_part, "The functor \"not_equal\" requires exactely two arguments but " + str(len(arguments)) + " have been given.")

def functor_or(context, *arguments):
	if len(arguments) == 2:
		first_value = context.get(arguments[0])
		second_value = context.get(arguments[1])
		if type(first_value) is types.BooleanType and type(second_value) is types.BooleanType:
			return first_value or second_value
		else:
			raise ExecuteException(context.current_code_part, "The functor \"or\" requires two boolean arguments but got \"" + str(type(first_value)) + "\" and \"" + str(type(second_value)) + "\" arguments.")
	else:
		raise ExecuteException(context.current_code_part, "The functor \"or\" requires exactely two arguments but " + str(len(arguments)) + " have been given.")

def functor_range(context, *arguments):
	if len(arguments) == 2:
		from_value = context.get(arguments[0])
		to_value = context.get(arguments[1])
		if type(from_value) is types.IntType and type(to_value) is types.IntType:
			return range(from_value, to_value + 1)
		else:
			raise ExecuteException(context.current_code_part, "The functor \"range\" requires two integer arguments but got \"" + str(type(from_value)) + "\" and \"" + str(type(to_value)) + "\" arguments.")
	else:
		raise ExecuteException(context.current_code_part, "The functor \"range\" requires exactely two arguments but " + str(len(arguments)) + " have been given.")

def functor_subtract(context, *arguments):
	if len(arguments) == 2:
		first_value = context.get(arguments[0])
		second_value = context.get(arguments[1])
		if type(first_value) is types.IntType and type(second_value) is types.IntType:
			return first_value - second_value
		else:
			raise ExecuteException(context.current_code_part, "The functor \"subtract\" requires two integer arguments but got \"" + str(type(first_value)) + "\" and \"" + str(type(second_value)) + "\" arguments.")
	else:
		raise ExecuteException(context.current_code_part, "The functor \"subtract\" requires exactely two arguments but " + str(len(arguments)) + " have been given.")

def functor_to_boolean(context, *arguments):
	if len(arguments) == 1:
		value = context.get(arguments[0])
		if type(value) is types.UnicodeType:
			if value == u"true":
				return True
			elif value == u"false":
				return False
			else:
				raise ExecuteException(context.current_code_part, "The functor \"" + to_boolean + "\" refused to convert the string value \"" + value + "\" to a boolean value.")
		elif type(value) is types.BooleanType:
			return value
		else:
			raise ExecuteException(context.current_code_part, "The functor \"to_boolean\" does not know how to handle \"" + str(type(value)) + "\" arguments.")
	else:
		raise ExecuteException(context.current_code_part, "The functor \"to_boolean\" requires exactely one argument but " + str(len(arguments)) + " have been given.")

def functor_to_integer(context, *arguments):
	if len(arguments) == 1:
		value = context.get(arguments[0])
		if type(value) is types.UnicodeType:
			return int(value)
		else:
			raise ExecuteException(context.current_code_part, "The functor \"to_integer\" does not know how to handle \"" + str(type(value)) + "\" arguments.")
	else:
		raise ExecuteException(context.current_code_part, "The functor \"to_integer\" requires exactely one argument but " + str(len(arguments)) + " have been given.")

def functor_to_list(context, *arguments):
	result = list()
	for code_part in arguments:
		result.append(context.get(code_part))
	return result

def functor_to_string(context, *arguments):
	if len(arguments) == 1:
		value = context.get(arguments[0])
		if type(value) is types.UnicodeType:
			return value
		if type(value) is types.IntType:
			return unicode(value)
		elif type(value) is types.BooleanType:
			if value == True:
				return u"true"
			else:
				return u"false"
		else:
			raise ExecuteException(context.current_code_part, "The functor \"to_string\" does not know how to handle \"" + str(type(value)) + "\" arguments.")
	else:
		raise ExecuteException(context.current_code_part, "The functor \"to_string\" requires exactely one argument but " + str(len(arguments)) + " have been given.")

def functor_variable(context, *arguments):
	if len(arguments) == 1:
		variable_name = context.get(arguments[0])
		if type(variable_name) is types.UnicodeType:
			variable = context.get_variable(variable_name)
			if variable != None:
				variable_value = variable.value
				if variable_value != None:
					return variable_value
				else:
					raise ExecuteException(context.current_code_part, "The code is errornous because the referenced variable \"" + variable_name + "\" has no value.")
			else:
				raise ExecuteException(context.current_code_part, "The code is errornous because a variable code element references an undefined variable \"" + variable_name + "\".")
		else:
			raise ExecuteException(context.current_code_part, "The code is errornous because a variable name is not of type \"unicode\" but \"" + str(type(variable_name)) + "\".")
	else:
		raise ExecuteException(context.current_code_part, "The functor \"variable\" requires exactely one argument but " + str(len(arguments)) + " have been given.")
