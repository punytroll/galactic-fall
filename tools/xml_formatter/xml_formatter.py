import sys
import xml.sax

class Writer(xml.sax.handler.ContentHandler):
	def __init__(self):
		xml.sax.handler.ContentHandler.__init__(self)
		self.__indentation = 0
		self.__in_element = False
		self.__text = ""
	
	def __get_indentation_string(self):
		return "\t" * self.__indentation
	
	def startElement(self, name, attributes):
		if self.__in_element == True:
			print(">")
		if self.__text != "":
			print(self.__get_indentation_string() + self.__text)
			self.__text = ""
		self.__in_element = True
		attribute_string = ""
		for attribute_name in sorted(attributes.keys()):
			attribute_string += " " + attribute_name + "=\"" + attributes[attribute_name] + "\""
		print(self.__get_indentation_string() + "<" + name + attribute_string, end = "")
		self.__indentation += 1
	
	def endElement(self, name):
		self.__indentation -= 1
		if self.__in_element == True:
			if self.__text != "":
				print(">" + self.__text + "</" + name + ">")
				self.__text = ""
			else:
				print("/>")
		else:
			print(self.__get_indentation_string() + "</" + name + ">")
		self.__in_element = False
	
	def characters(self, content):
		self.__text += content.strip()

xml.sax.parse(sys.argv[1], Writer())
