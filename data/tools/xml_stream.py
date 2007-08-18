# coding: utf-8

'''
' Copyright (C) 2007  Hagen Möbius
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
' This is version 1.0.1 of the xml stream python component.
'''

class XMLStream:
	def __init__(self, stream):
		self.__stream = stream
		self.__in_element_header = False
		self.__in_element_name = False
		self.__element_stack = list()
		self.__element_name = ""
		self.__in_attribute = False
		self.__in_attribute_value = False
	
	def end_document(self):
		self.__end_element_name()
		while len(self.__element_stack) > 0:
			self.__end_element()
	
	def __lshift__(self, object):
		if object == element:
			self.__start_element()
		elif object == attribute:
			self.__start_attribute()
		elif object == value:
			self.__start_attribute_value()
		elif object == text:
			self.__start_text()
		elif object == end:
			self.__end_element()
		else:
			if self.__in_element_name == True:
				self.__element_name += str(object)
			self.__stream.write(str(object))
		return self
	
	def __start_element(self):
		if self.__in_element_header == True:
			self.__end_element_header()
		self.__in_element_name = True
		self.__in_element_header = True
		self.__stream.write("<")
	
	def __start_attribute(self):
		if self.__in_element_header == True:
			self.__end_element_name()
			self.__end_attribute()
			self.__stream.write(" ")
			self.__in_attribute = True
	
	def __start_attribute_value(self):
		if self.__in_attribute == True:
			self.__stream.write("=\"")
			self.__in_attribute_value = True
	
	def __start_text(self):
		if self.__in_element_header == True:
			self.__end_element_header()
	
	def __end_element(self):
		self.__end_element_name()
		if self.__in_element_header == True:
			self.__end_attribute()
			self.__stream.write("/>")
			self.__in_element_header = False
		else:
			self.__stream.write("</" + self.__element_stack[-1] + ">")
		self.__element_stack = self.__element_stack[:-1]
	
	def __end_element_name(self):
		if self.__in_element_name == True and len(self.__element_name) > 0:
			self.__element_stack.append(self.__element_name)
			self.__element_name = ""
			self.__in_element_name = False
	
	def __end_element_header(self):
		self.__end_element_name()
		self.__end_attribute()
		if self.__in_element_header == True:
			self.__stream.write(">")
			self.__in_element_header = False
	
	def __end_attribute(self):
		self.__end_attribute_value()
		if self.__in_attribute == True:
			self.__stream.write("=\"\"")
			self.__in_attribute = False
	
	def __end_attribute_value(self):
		if self.__in_attribute_value == True:
			self.__stream.write("\"")
			self.__in_attribute_value = False
			self.__in_attribute = False

def element():
	pass

def attribute():
	pass

def value():
	pass

def text():
	pass

def end():
	pass

if __name__ == "__main__":
	from StringIO import StringIO
	
	string_stream = StringIO()
	xml_stream = XMLStream(string_stream)
	xml_stream << element << "name" << end
	assert string_stream.getvalue() == "<name/>"
	
	string_stream = StringIO()
	xml_stream = XMLStream(string_stream)
	xml_stream << element << "name" << text << "TEXT" << end
	assert string_stream.getvalue() == "<name>TEXT</name>"
	
	string_stream = StringIO()
	xml_stream = XMLStream(string_stream)
	xml_stream << element << "name" << attribute << "TEXT" << end
	assert string_stream.getvalue() == "<name TEXT=\"\"/>"
	
	string_stream = StringIO()
	xml_stream = XMLStream(string_stream)
	xml_stream << element << "name" << attribute << "TEXT" << value << 1234 << end
	assert string_stream.getvalue() == "<name TEXT=\"1234\"/>"
	
	string_stream = StringIO()
	xml_stream = XMLStream(string_stream)
	xml_stream << element << "name" << attribute << "TEXT" << value << 1234 << text << 1.45 << end
	assert string_stream.getvalue() == "<name TEXT=\"1234\">1.45</name>"
	
	string_stream = StringIO()
	xml_stream = XMLStream(string_stream)
	xml_stream << element << "name" << attribute << "TEXT" << text << 1.45 << end
	assert string_stream.getvalue() == "<name TEXT=\"\">1.45</name>"
