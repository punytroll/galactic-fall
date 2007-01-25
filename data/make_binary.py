#! /usr/bin/env python

from pprint import pprint
from struct import pack
from sys import exit
from xml.dom.minidom import Node, parse
from optparse import OptionParser

parser = OptionParser()
parser.add_option("-d", "--definitions", dest="definitions", help="Load the definitions from the file.")
parser.add_option("-i", "--in", dest="in_file", help="The file to convert to binary form.")
parser.add_option("-o", "--out", dest="out_file", help="The file to put the binary form into.")

# read the options and validate
(options, args) = parser.parse_args()
if options.in_file == None:
	print "Set the file to convert to binary form with '--in'."
	exit(1)
if options.out_file == None:
	print "Set the file to write the binary form to with '--out'."
	exit(1)

# read the definitions first
definitions = dict();
if options.definitions != None:
	definitions_document = parse(options.definitions)
	definitions_element = definitions_document.documentElement
	for definition_element in definitions_element.childNodes:
		if definition_element.nodeType == Node.ELEMENT_NODE and definitions.has_key(definition_element.tagName) == False:
			definition = list()
			for part_element in definition_element.childNodes:
				if part_element.nodeType == Node.ELEMENT_NODE and part_element.attributes.has_key("identifier") == True:
					definition.append((part_element.attributes.get("identifier").nodeValue, part_element.tagName))
			definitions[definition_element.tagName] = definition

# now open the out file for writing binary
out_file = open(options.out_file, "wb")

# the out() call stack
out_call_stack = list()

# the recursive output function
def out(data_type, node):
	out_call_stack.append(node.tagName)
	if data_type == "string":
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
		out_file.write(pack('B', int(node.firstChild.nodeValue)))
	elif data_type == "u4byte":
		out_file.write(pack('I', long(node.firstChild.nodeValue)))
	elif definitions.has_key(data_type) == True:
		for part in definitions[data_type]:
			for node_part in node.childNodes:
				found_it = False
				if node_part.nodeType == Node.ELEMENT_NODE:
					if node_part.tagName == part[0]:
						out(part[1], node_part)
						found_it = True
						break
			if found_it == False:
				stack_path = ""
				for stack_entry in out_call_stack:
					stack_path += "/" + stack_entry
				print "In file '" + options.in_file + "' I found no value for '" + stack_path + "/" + part[0] + "' of type '" + part[1] + "'."
				exit(1)
	out_call_stack.pop()

# now parse the in file
in_document = parse(options.in_file)
in_element = in_document.documentElement
if in_element.nodeType == Node.ELEMENT_NODE:
	out(in_element.tagName, in_element)