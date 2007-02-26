#! /usr/bin/env python

from sys import exit
from xml.dom import getDOMImplementation
from optparse import OptionParser

parser = OptionParser()
parser.add_option("-n", "--identifier", dest="identifier", help="The identifier of the new model.")
parser.add_option("-i", "--in", dest="in_file", help="The file to convert from raw to littlem form.")
parser.add_option("-o", "--out", dest="out_file", help="The file to put the littlem form into.")

# read the options and validate
(options, args) = parser.parse_args()
if options.in_file == None:
	print "Set the raw file to convert with '--in'."
	exit(1)
if options.out_file == None:
	print "Set the file to write the littlem form to with '--out'."
	exit(1)

points = list()
triangle_points = list()
triangles = list()

in_file = open(options.in_file, "rb")

for line in in_file.readlines():
	splitted = map(float, line.split())
	# we may have a triangle with 3 points
	if len(splitted) == 9:
		point_1_x, point_1_y, point_1_z, point_2_x, point_2_y, point_2_z, point_3_x, point_3_y, point_3_z = splitted
		point_1_2_x, point_1_2_y, point_1_2_z = point_2_x - point_1_x, point_2_y - point_1_y, point_2_z - point_1_z
		point_1_3_x, point_1_3_y, point_1_3_z = point_3_x - point_1_x, point_3_y - point_1_y, point_3_z - point_1_z
		normal_x, normal_y, normal_z = point_1_2_y * point_1_3_z - point_1_2_z * point_1_3_y, point_1_2_z * point_1_3_x - point_1_2_x * point_1_3_z, point_1_2_x * point_1_3_y - point_1_2_y * point_1_3_x
		point_1_index, point_2_index, point_3_index = len(points), len(points) + 1, len(points) + 2
		triangle_point_1_index, triangle_point_2_index, triangle_point_3_index = len(triangle_points), len(triangle_points) + 1, len(triangle_points) + 2
		triangle_index = len(triangles)
		points.append((point_1_x, point_1_y, point_1_z))
		points.append((point_2_x, point_2_y, point_2_z))
		points.append((point_3_x, point_3_y, point_3_z))
		triangle_points.append((point_1_index, normal_x, normal_y, normal_z))
		triangle_points.append((point_2_index, normal_x, normal_y, normal_z))
		triangle_points.append((point_3_index, normal_x, normal_y, normal_z))
		triangles.append((triangle_point_1_index, triangle_point_2_index, triangle_point_3_index))
	# we may also have a quad with 4 points
	elif len(splitted) == 12:
		# point 1 and 3 are shared
		point_1_x, point_1_y, point_1_z, point_2_x, point_2_y, point_2_z, point_3_x, point_3_y, point_3_z, point_4_x, point_4_y, point_4_z = splitted
		# the normal can be calculated from three points only
		point_1_2_x, point_1_2_y, point_1_2_z = point_2_x - point_1_x, point_2_y - point_1_y, point_2_z - point_1_z
		point_1_3_x, point_1_3_y, point_1_3_z = point_3_x - point_1_x, point_3_y - point_1_y, point_3_z - point_1_z
		normal_x, normal_y, normal_z = point_1_2_y * point_1_3_z - point_1_2_z * point_1_3_y, point_1_2_z * point_1_3_x - point_1_2_x * point_1_3_z, point_1_2_x * point_1_3_y - point_1_2_y * point_1_3_x
		point_1_index, point_2_index, point_3_index, point_4_index = len(points), len(points) + 1, len(points) + 2, len(points) + 3
		triangle_point_1_index, triangle_point_2_index, triangle_point_3_index, triangle_point_4_index = len(triangle_points), len(triangle_points) + 1, len(triangle_points) + 2, len(triangle_points) + 3
		triangle_1_index, triangle_2_index = len(triangles), len(triangles) + 1
		points.append((point_1_x, point_1_y, point_1_z))
		points.append((point_2_x, point_2_y, point_2_z))
		points.append((point_3_x, point_3_y, point_3_z))
		points.append((point_4_x, point_4_y, point_4_z))
		triangle_points.append((point_1_index, normal_x, normal_y, normal_z))
		triangle_points.append((point_2_index, normal_x, normal_y, normal_z))
		triangle_points.append((point_3_index, normal_x, normal_y, normal_z))
		triangle_points.append((point_4_index, normal_x, normal_y, normal_z))
		triangles.append((triangle_point_1_index, triangle_point_2_index, triangle_point_3_index))
		triangles.append((triangle_point_1_index, triangle_point_3_index, triangle_point_4_index))

# now write the output
document = getDOMImplementation().createDocument(None, "model", None)
model_element = document.documentElement
if options.identifier != None and options.identifier != "":
	model_element.setAttribute("identifier", options.identifier)
for identifier, point in enumerate(points):
	point_element = document.createElement("point")
	point_element.setAttribute("identifier", str(identifier))
	point_element.setAttribute("position-x", str(point[0]))
	point_element.setAttribute("position-y", str(point[1]))
	point_element.setAttribute("position-z", str(point[2]))
	model_element.appendChild(point_element)
for identifier, triangle_point in enumerate(triangle_points):
	triangle_point_element = document.createElement("triangle-point")
	triangle_point_element.setAttribute("identifier", str(identifier))
	triangle_point_element.setAttribute("normal-x", str(triangle_point[1]))
	triangle_point_element.setAttribute("normal-y", str(triangle_point[2]))
	triangle_point_element.setAttribute("normal-z", str(triangle_point[3]))
	point_element = document.createElement("point")
	point_element.setAttribute("point-identifier", str(triangle_point[0]))
	triangle_point_element.appendChild(point_element)
	model_element.appendChild(triangle_point_element)
for identifier, triangle in enumerate(triangles):
	triangle_element = document.createElement("triangle")
	triangle_element.setAttribute("identifier", str(identifier))
	triangle_point_element = document.createElement("triangle-point")
	triangle_point_element.setAttribute("triangle-point-identifier", str(triangle[0]))
	triangle_element.appendChild(triangle_point_element)
	triangle_point_element = document.createElement("triangle-point")
	triangle_point_element.setAttribute("triangle-point-identifier", str(triangle[1]))
	triangle_element.appendChild(triangle_point_element)
	triangle_point_element = document.createElement("triangle-point")
	triangle_point_element.setAttribute("triangle-point-identifier", str(triangle[2]))
	triangle_element.appendChild(triangle_point_element)
	model_element.appendChild(triangle_element)

# now open the out file for writing
out_file = open(options.out_file, "w")
out_file.write(model_element.toprettyxml())
