#! /usr/bin/env python

from optparse import OptionParser
from sys import exit
from xml.dom.minidom import Node, parse

parser = OptionParser()
parser.add_option("-i", "--in", dest="in_file", help="The littlem file to analyze.")

# read the options and validate
(options, args) = parser.parse_args()
if options.in_file == None:
	print "Set the littlem file to analyze with '--in'."
	exit(1)

def analyze(mesh_element):
	print "identifier: " + mesh_element.attributes.get("identifier").nodeValue
	number_of_points = 0
	number_of_triangle_points = 0
	number_of_triangles = 0
	points_minimum_x = 0.0
	points_maximum_x = 0.0
	points_minimum_y = 0.0
	points_maximum_y = 0.0
	points_minimum_z = 0.0
	points_maximum_z = 0.0
	for element in mesh_element.childNodes:
		if element.nodeType == Node.ELEMENT_NODE:
			if element.tagName == "point":
				number_of_points += 1
				points_minimum_x = min(points_minimum_x, float(element.getAttribute("position-x")))
				points_maximum_x = max(points_maximum_x, float(element.getAttribute("position-x")))
				points_minimum_y = min(points_minimum_y, float(element.getAttribute("position-y")))
				points_maximum_y = max(points_maximum_y, float(element.getAttribute("position-y")))
				points_minimum_z = min(points_minimum_z, float(element.getAttribute("position-z")))
				points_maximum_z = max(points_maximum_z, float(element.getAttribute("position-z")))
			elif element.tagName == "triangle-point":
				number_of_triangle_points += 1
			elif element.tagName == "triangle":
				number_of_triangles += 1
	print "#points: " + str(number_of_points)
	print "#triangle-points: " + str(number_of_triangle_points)
	print "#triangles: " + str(number_of_triangles)
	print "minimum x: " + str(points_minimum_x)
	print "maximum x: " + str(points_maximum_x)
	print "minimum y: " + str(points_minimum_y)
	print "maximum y: " + str(points_maximum_y)
	print "minimum z: " + str(points_minimum_z)
	print "maximum z: " + str(points_maximum_z)

# now parse the in_file
in_document = parse(options.in_file)
mesh_element = in_document.documentElement
if mesh_element.nodeType == Node.ELEMENT_NODE:
	analyze(mesh_element)
