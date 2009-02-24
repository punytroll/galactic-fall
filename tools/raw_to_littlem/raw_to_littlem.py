#! /usr/bin/env python

from math import sqrt
from sys import exit
from optparse import OptionParser
from xml_stream import attribute, element, end, value, XMLStream

parser = OptionParser()
parser.add_option("-n", "--identifier", dest = "identifier", help = "The identifier of the new mesh.")
parser.add_option("-i", "--in", dest = "in_file", help = "The file to convert from raw to littlem form.")
parser.add_option("-o", "--out", dest = "out_file", help = "The file to put the littlem form into.")
parser.add_option("-s", "--smooth", action = "store_true", default = False, dest = "smooth", help = "Set this option if you want the normals to smooth the surface.")

# read the options and validate
(options, args) = parser.parse_args()
if options.in_file == None:
	print "Set the raw file to convert with '--in'."
	exit(1)
if options.out_file == None:
	print "Set the file to write the littlem form to with '--out'."
	exit(1)

class Point:
	def __init__(self, identifier, x, y, z):
		self.identifier = identifier
		self.x = x
		self.y = y
		self.z = z
		self.triangle_points = list()
	
	def add_triangle_point(self, triangle_point):
		self.triangle_points.append(triangle_point)

class Vector:
	@staticmethod
	def create_from_to(point_1, point_2):
		return Vector(point_2.x - point_1.x, point_2.y - point_1.y, point_2.z - point_1.z)
	
	def __init__(self, x, y, z):
		self.x = x
		self.y = y
		self.z = z
	
	def cross(self, other):
		return Vector(self.y * other.z - self.z * other.y, self.z * other.x - self.x * other.z, self.x * other.y - self.y * other.x)
	
	def length(self):
		return sqrt(self.x * self.x + self.y * self.y + self.z * self.z)
	
	def normalize(self):
		length = self.length()
		self.x /= length
		self.y /= length
		self.z /= length
		return self
	
	def __iadd__(self, other):
		self.x += other.x
		self.y += other.y
		self.z += other.z
		return self

class TrianglePoint:
	def __init__(self, identifier, point, normal):
		self.identifier = identifier
		self.point = point
		self.point.add_triangle_point(self)
		self.normal = normal
		self.triangles = list()
	
	def add_triangle(self, triangle):
		self.triangles.append(triangle)
	
	def remove_triangle(self, triangle):
		self.triangles.remove(triangle)

class Triangle:
	def __init__(self, identifier, triangle_point_1, triangle_point_2, triangle_point_3):
		self.identifier = identifier
		self.triangle_point_1 = triangle_point_1
		self.triangle_point_1.add_triangle(self)
		self.triangle_point_2 = triangle_point_2
		self.triangle_point_2.add_triangle(self)
		self.triangle_point_3 = triangle_point_3
		self.triangle_point_3.add_triangle(self)
	
	def exchange_triangle_point(self, old_triangle_point, new_triangle_point):
		if self.triangle_point_1 == old_triangle_point:
			self.triangle_point_1.remove_triangle(self)
			self.triangle_point_1 = new_triangle_point
			self.triangle_point_1.add_triangle(self)
		elif self.triangle_point_2 == old_triangle_point:
			self.triangle_point_2.remove_triangle(self)
			self.triangle_point_2 = new_triangle_point
			self.triangle_point_2.add_triangle(self)
		elif self.triangle_point_3 == old_triangle_point:
			self.triangle_point_3.remove_triangle(self)
			self.triangle_point_3 = new_triangle_point
			self.triangle_point_3.add_triangle(self)
		else:
			raise Exception()

class PointManager:
	def __init__(self):
		self.__points = dict()
	
	def add(self, x, y, z):
		for point in self.__points.values():
			if point.x == x and point.y == y and point.z == z:
				return point
		point = Point(len(self.__points), x, y, z)
		self.__points[point.identifier] = point
		return point
	
	def remove(self, identifier):
		del self.__points[identifier]
	
	def get_points(self):
		return self.__points.values()

class TrianglePointManager:
	def __init__(self):
		self.__triangle_points = dict()
	
	def add(self, point, normal):
		triangle_point = TrianglePoint(len(self.__triangle_points), point, normal)
		self.__triangle_points[triangle_point.identifier] = triangle_point
		return triangle_point
	
	def remove(self, identifier):
		del self.__triangle_points[identifier]
	
	def get_triangle_points(self):
		return self.__triangle_points.values()

class TriangleManager:
	def __init__(self):
		self.__triangles = dict()
	
	def add(self, triangle_point_1, triangle_point_2, triangle_point_3):
		triangle = Triangle(len(self.__triangles), triangle_point_1, triangle_point_2, triangle_point_3)
		self.__triangles[triangle.identifier] = triangle
		return triangle
	
	def remove(self, identifier):
		del self.__triangles[identifier]
	
	def get_triangles(self):
		return self.__triangles.values()

points = PointManager()
triangle_points = TrianglePointManager()
triangles = TriangleManager()

in_file = open(options.in_file, "rb")

for line in in_file.readlines():
	splitted = map(float, line.split())
	# we may have a triangle with 3 points
	if len(splitted) == 9:
		point_1_x, point_1_y, point_1_z, point_2_x, point_2_y, point_2_z, point_3_x, point_3_y, point_3_z = splitted
		point_1 = points.add(point_1_x, point_1_y, point_1_z)
		point_2 = points.add(point_2_x, point_2_y, point_2_z)
		point_3 = points.add(point_3_x, point_3_y, point_3_z)
		vector_1_2 = Vector.create_from_to(point_1, point_2)
		vector_1_3 = Vector.create_from_to(point_1, point_3)
		normal = vector_1_2.cross(vector_1_3)
		normal.normalize()
		triangle_point_1 = triangle_points.add(point_1, normal)
		triangle_point_2 = triangle_points.add(point_2, normal)
		triangle_point_3 = triangle_points.add(point_3, normal)
		triangles.add(triangle_point_1, triangle_point_2, triangle_point_3)
	# we may also have a quad with 4 points
	elif len(splitted) == 12:
		# point 1 and 3 are shared
		point_1_x, point_1_y, point_1_z, point_2_x, point_2_y, point_2_z, point_3_x, point_3_y, point_3_z, point_4_x, point_4_y, point_4_z = splitted
		point_1 = points.add(point_1_x, point_1_y, point_1_z)
		point_2 = points.add(point_2_x, point_2_y, point_2_z)
		point_3 = points.add(point_3_x, point_3_y, point_3_z)
		point_4 = points.add(point_4_x, point_4_y, point_4_z)
		# the normal can be calculated from three points only
		vector_1_2 = Vector.create_from_to(point_1, point_2)
		vector_1_3 = Vector.create_from_to(point_1, point_3)
		normal = vector_1_2.cross(vector_1_3)
		normal.normalize()
		triangle_point_1 = triangle_points.add(point_1, normal)
		triangle_point_2 = triangle_points.add(point_2, normal)
		triangle_point_3 = triangle_points.add(point_3, normal)
		triangle_point_4 = triangle_points.add(point_1, normal)
		triangle_point_5 = triangle_points.add(point_3, normal)
		triangle_point_6 = triangle_points.add(point_4, normal)
		triangles.add(triangle_point_1, triangle_point_2, triangle_point_3)
		triangles.add(triangle_point_4, triangle_point_5, triangle_point_6)

# now apply the smoothing
if options.smooth == True:
	for point in points.get_points():
		if len(point.triangle_points) > 1:
			# calculate the new normal as the average normal from all connected triangle points
			new_normal = Vector(0.0, 0.0, 0.0)
			for triangle_point in point.triangle_points:
				for i in range(len(triangle_point.triangles)):
					new_normal += triangle_point.normal
			new_normal.normalize()
			new_triangle_point = triangle_points.add(point, new_normal)
			for triangle_point in point.triangle_points:
				if triangle_point != new_triangle_point:
					for triangle in triangle_point.triangles:
						triangle.exchange_triangle_point(triangle_point, new_triangle_point)

# now write the output
out_file = open(options.out_file, "w")
xml_stream = XMLStream(out_file)
xml_stream << element << "mesh"
if options.identifier != None and options.identifier != "":
	xml_stream << attribute << "identifier" << value << options.identifier
for point in points.get_points():
	xml_stream << element << "point" << attribute << "identifier" << value << str(point.identifier) << attribute << "position-x" << value << str(point.x) << attribute << "position-y" << value << str(point.y) << attribute << "position-z" << value << str(point.z) << end
for triangle_point in triangle_points.get_triangle_points():
	if len(triangle_point.triangles) > 0:
		xml_stream << element << "triangle-point" << attribute << "identifier" << value << str(triangle_point.identifier) << attribute << "normal-x" << value << str(triangle_point.normal.x) << attribute << "normal-y" << value << str(triangle_point.normal.y) << attribute << "normal-z" << value << str(triangle_point.normal.z)
		xml_stream << element << "point" << attribute << "point-identifier" << value << str(triangle_point.point.identifier) << end << end
for triangle in triangles.get_triangles():
	xml_stream << element << "triangle" << attribute << "identifier" << value << str(triangle.identifier)
	xml_stream << element << "triangle-point" << attribute << "triangle-point-identifier" << value << str(triangle.triangle_point_1.identifier) << end
	xml_stream << element << "triangle-point" << attribute << "triangle-point-identifier" << value << str(triangle.triangle_point_2.identifier) << end
	xml_stream << element << "triangle-point" << attribute << "triangle-point-identifier" << value << str(triangle.triangle_point_3.identifier) << end << end
xml_stream << end
