#! /usr/bin/env python

from optparse import OptionParser
from sys import exit
from xml.dom.minidom import Node, parse

parser = OptionParser()
parser.add_option("-i", "--in", dest="in_file", help="The littlem file to analyze.")

# read the options and validate
(options, args) = parser.parse_args()
if options.in_file == None:
	print "Set the raw file to convert with '--in'."
	exit(1)

def analyze(mesh_element):
	print "identifier: " + mesh_element.attributes.get("identifier").nodeValue
	number_of_points = 0
	number_of_triangle_points = 0
	number_of_triangles = 0
	for element in mesh_element.childNodes:
		if element.nodeType == Node.ELEMENT_NODE:
			if element.tagName == "point":
				number_of_points += 1
			elif element.tagName == "triangle-point":
				number_of_triangle_points += 1
			elif element.tagName == "triangle":
				number_of_triangles += 1
	print "#points: " + str(number_of_points)
	print "#triangle-points: " + str(number_of_triangle_points)
	print "#triangles: " + str(number_of_triangles)

# now parse the in_file
in_document = parse(options.in_file)
mesh_element = in_document.documentElement
if mesh_element.nodeType == Node.ELEMENT_NODE:
	analyze(mesh_element)
