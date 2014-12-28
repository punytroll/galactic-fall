from optparse import OptionParser
import sys
from xml.dom.minidom import Node, parse

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

class Declaration(object):
	def __init__(self, identifier):
		self.__identifier = identifier
		self.__is_declaration = None
		self.__parts = list()
		self.__sub_type = None
		self.__type = None
	
	def get_identifier(self):
		return self.__identifier
	
	def get_is_declaration(self):
		return self.__is_declaration
	
	def get_parts(self):
		return self.__parts
	
	def get_sub_type(self):
		return self.__sub_type
	
	def get_type(self):
		return self.__type
	
	def set_is_declaration(self, is_declaration):
		self.__is_declaration = is_declaration
	
	def set_sub_type(self, sub_type):
		self.__sub_type = sub_type
	
	def set_type(self, type):
		self.__type = type

class Part(object):
	def __init__(self, name, type_identifier):
		self.__type_identifier = type_identifier
		self.__name = name
	
	def get_type_identifier(self):
		return self.__type_identifier
	
	def get_name(self):
		return self.__name

parser = OptionParser()
parser.add_option("-d", "--declarations", dest="declarations", help="Load the declarations from the file.")

# read the options and validate
(options, args) = parser.parse_args()

# read the declarations first
by_identifier = dict()
by_type = dict()
if options.declarations != None:
	declarations_document = parse(options.declarations)
	declarations_element = declarations_document.documentElement
	for declaration_element in declarations_element.childNodes:
		if declaration_element.nodeType == Node.ELEMENT_NODE:
			if declaration_element.tagName not in by_identifier:
				declaration = Declaration(declaration_element.tagName)
				if "type" in declaration_element.attributes:
					declaration.set_type(int(declaration_element.attributes.get("type").nodeValue))
				if "sub-type" in declaration_element.attributes:
					declaration.set_sub_type(int(declaration_element.attributes.get("sub-type").nodeValue))
				if "is" in declaration_element.attributes:
					declaration.set_is_declaration(declaration_element.attributes.get("is").nodeValue)
				else:
					for part_element in declaration_element.childNodes:
						if part_element.nodeType == Node.ELEMENT_NODE:
							if "identifier" in part_element.attributes:
								declaration.get_parts().append(Part(part_element.attributes.get("identifier").nodeValue, part_element.tagName))
							else:
								print(LightRed + "Error" + White + ": In file " + LightYellow + options.declarations + White + " there is a declaration part without an " + LightYellow + "identifier" + White + " attribute on the " + LightYellow + declaration_element.tagName + White + " declaration.", file = sys.stderr)
								raise ConvertException()
				by_identifier[declaration.get_identifier()] = declaration
				if declaration.get_type() != None:
					by_sub_type = None
					if declaration.get_type() in by_type:
						by_sub_type = by_type[declaration.get_type()]
					else:
						by_sub_type = dict()
						by_type[declaration.get_type()] = by_sub_type
					if declaration.get_sub_type() not in by_sub_type:						
						by_sub_type[declaration.get_sub_type()] = declaration
					else:
						print(LightRed + "Error" + White + ": In file " + LightYellow + options.declarations + White + " the declaration " + LightYellow + declaration.get_identifier() + White + " is a duplicate for the sub-type " + LightYellow + str(declaration.get_sub_type()) + White + " inside the type " + LightYellow + str(declaration.get_type()) + White + ".", file = sys.stderr)
						raise ConvertException()
				elif declaration.get_sub_type() != None:
					print(LightRed + "Error" + White + ": In file " + LightYellow + options.declarations + White + " the declaration " + LightYellow + declaration.get_identifier() + White + " has a sub-type " + LightYellow + str(declaration.get_sub_type()) + White + " but no type.", file = sys.stderr)
					raise ConvertException()
			else:
				print(LightRed + "Error" + White + ": In file " + LightYellow + options.declarations + White + " there is a duplicate declaration for " + LightYellow + declaration_element.tagName + White + ".", file = sys.stderr)
				raise ConvertException()

definitions = dict()
for type in by_type:
	by_sub_type = by_type[type]
	if None in by_sub_type:
		type_name = by_sub_type[None].get_identifier().replace("-", "_").upper()
	elif 0 in by_sub_type:
		type_name = by_sub_type[0].get_identifier().replace("-", "_").upper()
	else:
		print(LightRed + "Error" + White + ": In file " + LightYellow + options.declarations + White + " the type " + LightYellow + type + White + " has no name defining declaration (either sub-type 0 or no sub-type).", file = sys.stderr)
		raise ConvertException()
	if len(by_sub_type) > 1:
		for sub_type in by_sub_type:
			declaration = by_sub_type[sub_type]
			sub_type_key = "DATA_TYPE_" + type_name + "_SUB_TYPE_" + declaration.get_identifier().replace("-", "_").upper()
			sub_type_value = sub_type
			definitions[sub_type_key] = sub_type_value
	type_key = "DATA_TYPE_" + type_name
	type_value = type
	definitions[type_key] = type_value

print("/**")
print(" * ATTENTION: generated source code.")
print(" * Do not modify - your changes won't be permanent.")
print(" **/")
print()
for key in sorted(definitions.keys()):
	if definitions[key] != None:
		print("const uint32_t " + key + " = " + str(definitions[key]) + ";")
