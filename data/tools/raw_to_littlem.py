#! /usr/bin/env python

from math import sqrt
from sys import exit
from optparse import OptionParser
from xml_stream import attribute, element, end, value, XMLStream

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

class PointManager:
	class Point:
		def __init__(self, x, y, z):
			self.x = x
			self.y = y
			self.z = z
	
	def __init__(self):
		self.__points = list()
	
	def add(self, x, y, z):
		for (index, point) in enumerate(self.__points):
			if point.x == x and point.y == y and point.z == z:
				return index
		self.__points.append(PointManager.Point(x, y, z))
		return len(self.__points) - 1
	
	def get_points(self):
		return self.__points

points = PointManager()
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
		normal_length = sqrt(normal_x * normal_x + normal_y * normal_y + normal_z * normal_z)
		if normal_length == 0.0:
			continue
		normal_x /= normal_length
		normal_y /= normal_length
		normal_z /= normal_length
		point_1_index = points.add(point_1_x, point_1_y, point_1_z)
		point_2_index = points.add(point_2_x, point_2_y, point_2_z)
		point_3_index = points.add(point_3_x, point_3_y, point_3_z)
		triangle_point_1_index, triangle_point_2_index, triangle_point_3_index = len(triangle_points), len(triangle_points) + 1, len(triangle_points) + 2
		triangle_index = len(triangles)
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
		normal_length = sqrt(normal_x * normal_x + normal_y * normal_y + normal_z * normal_z)
		normal_x /= normal_length
		normal_y /= normal_length
		normal_z /= normal_length
		if normal_length == 0.0:
			continue
		point_1_index = points.add(point_1_x, point_1_y, point_1_z)
		point_2_index = points.add(point_2_x, point_2_y, point_2_z)
		point_3_index = points.add(point_3_x, point_3_y, point_3_z)
		point_4_index = points.add(point_4_x, point_4_y, point_4_z)
		triangle_point_1_index, triangle_point_2_index, triangle_point_3_index, triangle_point_4_index = len(triangle_points), len(triangle_points) + 1, len(triangle_points) + 2, len(triangle_points) + 3
		triangle_1_index, triangle_2_index = len(triangles), len(triangles) + 1
		triangle_points.append((point_1_index, normal_x, normal_y, normal_z))
		triangle_points.append((point_2_index, normal_x, normal_y, normal_z))
		triangle_points.append((point_3_index, normal_x, normal_y, normal_z))
		triangle_points.append((point_4_index, normal_x, normal_y, normal_z))
		triangles.append((triangle_point_1_index, triangle_point_2_index, triangle_point_3_index))
		triangles.append((triangle_point_1_index, triangle_point_3_index, triangle_point_4_index))

# now write the output
out_file = open(options.out_file, "w")
xml_stream = XMLStream(out_file)
xml_stream << element << "model"
if options.identifier != None and options.identifier != "":
	xml_stream << attribute << "identifier" << value << options.identifier
for identifier, point in enumerate(points.get_points()):
	xml_stream << element << "point" << attribute << "identifier" << value << str(identifier) << attribute << "position-x" << value << str(point.x) << attribute << "position-y" << value << str(point.y) << attribute << "position-z" << value << str(point.z) << end
for identifier, triangle_point in enumerate(triangle_points):
	xml_stream << element << "triangle-point" << attribute << "identifier" << value << str(identifier) << attribute << "normal-x" << value << str(triangle_point[1]) << attribute << "normal-y" << value << str(triangle_point[2]) << attribute << "normal-z" << value << str(triangle_point[3])
	xml_stream << element << "point" << attribute << "point-identifier" << value << str(triangle_point[0]) << end << end
for identifier, triangle in enumerate(triangles):
	xml_stream << element << "triangle" << attribute << "identifier" << value << str(identifier)
	xml_stream << element << "triangle-point" << attribute << "triangle-point-identifier" << value << str(triangle[0]) << end
	xml_stream << element << "triangle-point" << attribute << "triangle-point-identifier" << value << str(triangle[1]) << end
	xml_stream << element << "triangle-point" << attribute << "triangle-point-identifier" << value << str(triangle[2]) << end << end
xml_stream << end
