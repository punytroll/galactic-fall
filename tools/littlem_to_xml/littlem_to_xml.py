from optparse import OptionParser
from sys import exit
from xml.dom.minidom import Node, parse
from xml_stream import attribute, element, end, text, value, XMLStream

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

parser = OptionParser()
parser.add_option("-i", "--in", dest="in_file", help="The littlem input file.")
parser.add_option("-o", "--out", dest="out_file", help="The xml output file.")

# read the options and validate
(options, args) = parser.parse_args()
if options.in_file == None:
	print(LightRed + "Usage" + White + ": Set the littlem input file with " + LightYellow + "--in" + White + ".")
	exit(1)
if options.out_file == None:
	print(LightRed + "Usage" + White + ": Set the xml file output file with " + LightYellow + "--out" + White + ".")
	exit(1)

class Position:
	def __init__(self):
		self.x = None
		self.y = None
		self.z = None

class Orientation:
	def __init__(self):
		self.w = None
		self.x = None
		self.y = None
		self.z = None

class Marker:
	def __init__(self):
		self.identifier = None
		self.orientation = None
		self.position = None
		self.length = None

class Point:
	def __init__(self):
		self.identifier = None
		self.name = None
		self.position_x = None
		self.position_y = None
		self.position_z = None

class TrianglePoint:
	def __init__(self):
		self.identifier = None
		self.normal_x = None
		self.normal_y = None
		self.normal_z = None
		self.point_identifier = None

class Triangle:
	def __init__(self):
		self.identifier = None
		self.name = None
		self.triangle_point_1_identifier = None
		self.triangle_point_2_identifier = None
		self.triangle_point_3_identifier = None

points = list()
triangle_points = list()
triangles = list()
markers = list()
in_document = parse(options.in_file)
mesh_element = in_document.documentElement
# now open the out file for writing
out_file = open(options.out_file, "wt")
xml_stream = XMLStream(out_file)
if mesh_element.nodeType == Node.ELEMENT_NODE:
	# input
	for mesh_child_element in mesh_element.childNodes:
		if mesh_child_element.nodeType == Node.ELEMENT_NODE:
			if mesh_child_element.tagName == "point":
				point = Point()
				point.identifier = mesh_child_element.attributes.get("identifier").nodeValue
				if mesh_child_element.attributes.get("name") != None:
					point.name = mesh_child_element.attributes.get("name").nodeValue
				point.position_x = mesh_child_element.attributes.get("position-x").nodeValue
				point.position_y = mesh_child_element.attributes.get("position-y").nodeValue
				point.position_z = mesh_child_element.attributes.get("position-z").nodeValue
				points.append(point)
			elif mesh_child_element.tagName == "triangle-point":
				triangle_point = TrianglePoint()
				triangle_point.identifier = mesh_child_element.attributes.get("identifier").nodeValue
				triangle_point.normal_x = mesh_child_element.attributes.get("normal-x").nodeValue
				triangle_point.normal_y = mesh_child_element.attributes.get("normal-y").nodeValue
				triangle_point.normal_z = mesh_child_element.attributes.get("normal-z").nodeValue
				for triangle_point_child_element in mesh_child_element.childNodes:
					if triangle_point_child_element.nodeType == Node.ELEMENT_NODE:
						if triangle_point_child_element.tagName == "point":
							if triangle_point.point_identifier == None:
								triangle_point.point_identifier = triangle_point_child_element.attributes.get("point-identifier").nodeValue
				triangle_points.append(triangle_point)
			elif mesh_child_element.tagName == "triangle":
				triangle = Triangle()
				triangle.identifier = mesh_child_element.attributes.get("identifier").nodeValue
				if mesh_child_element.attributes.get("name") != None:
					triangle.name = mesh_child_element.attributes.get("name").nodeValue
				for triangle_child_element in mesh_child_element.childNodes:
					if triangle_child_element.nodeType == Node.ELEMENT_NODE:
						if triangle_child_element.tagName == "triangle-point":
							if triangle.triangle_point_1_identifier == None:
								triangle.triangle_point_1_identifier = triangle_child_element.attributes.get("triangle-point-identifier").nodeValue
							elif triangle.triangle_point_2_identifier == None:
								triangle.triangle_point_2_identifier = triangle_child_element.attributes.get("triangle-point-identifier").nodeValue
							elif triangle.triangle_point_3_identifier == None:
								triangle.triangle_point_3_identifier = triangle_child_element.attributes.get("triangle-point-identifier").nodeValue
				triangles.append(triangle)
			elif mesh_child_element.tagName == "marker":
				marker = Marker()
				marker.identifier = mesh_child_element.attributes.get("identifier").nodeValue
				for marker_child_element in mesh_child_element.childNodes:
					if marker_child_element.nodeType == Node.ELEMENT_NODE:
						if marker_child_element.tagName == "position":
							assert(marker.position == None)
							position = Position()
							position.x = marker_child_element.attributes.get("x").nodeValue
							position.y = marker_child_element.attributes.get("y").nodeValue
							position.z = marker_child_element.attributes.get("z").nodeValue
							marker.position = position
						elif marker_child_element.tagName == "orientation":
							assert(marker.orientation == None)
							orientation = Orientation()
							orientation.w = marker_child_element.attributes.get("w").nodeValue
							orientation.x = marker_child_element.attributes.get("x").nodeValue
							orientation.y = marker_child_element.attributes.get("y").nodeValue
							orientation.z = marker_child_element.attributes.get("z").nodeValue
							marker.orientation = orientation
						elif marker_child_element.tagName == "length":
							assert(marker.length == None)
							marker.length = marker_child_element.attributes.get("value").nodeValue
				markers.append(marker)
	# output
	xml_stream << element << "mesh"
	xml_stream << element << "identifier" << text << mesh_element.attributes.get("identifier").nodeValue << end
	xml_stream << element << "points"
	for point in points:
		xml_stream << element << "point" << element << "identifier" << text << point.identifier << end << element << "name"
		if point.name != None:
			xml_stream << text << point.name
		xml_stream << end << element << "position" << element << "x" << text << point.position_x << end << element << "y" << text << point.position_y << end << element << "z" << text << point.position_z << end << end << end
	xml_stream << end
	xml_stream << element << "triangle-points"
	for triangle_point in triangle_points:
		xml_stream << element << "triangle-point" << element << "identifier" << text << triangle_point.identifier << end << element << "normal" << element << "x" << text << triangle_point.normal_x << end << element << "y" << text << triangle_point.normal_y << end << element << "z" << text << triangle_point.normal_z << end << end << element << "point-identifier" << text << triangle_point.point_identifier << end << end
	xml_stream << end
	xml_stream << element << "triangles"
	for triangle in triangles:
		xml_stream << element << "triangle" << element << "identifier" << text << triangle.identifier << end << element << "name"
		if triangle.name != None:
			xml_stream << text << triangle.name
		xml_stream << end << element << "triangle-point-1-identifier" << text << triangle.triangle_point_1_identifier << end << element << "triangle-point-2-identifier" << text << triangle.triangle_point_2_identifier << end << element << "triangle-point-3-identifier" << text << triangle.triangle_point_3_identifier << end << end
	xml_stream << end
	xml_stream << element << "markers"
	for marker in markers:
		xml_stream << element << "marker" << element << "identifier" << text << marker.identifier << end
		xml_stream << element << "length"
		if marker.length != None:
			xml_stream << element << "valid" << text << "true" << end << element << "value" << text << marker.length << end
		else:
			xml_stream << element << "valid" << text << "false" << end << element << "value" << text << "0.0" << end
		xml_stream << end
		xml_stream << element << "position"
		if marker.position != None:
			xml_stream << element << "valid" << text << "true" << end << element << "value" << element << "x" << text << marker.position.x << end << element << "y" << text << marker.position.y << end << element << "z" << text << marker.position.z << end << end
		else:
			xml_stream << element << "valid" << text << "false" << end << element << "value" << element << "x" << text << "0.0" << end << element << "y" << text << "0.0" << end << element << "z" << text << "0.0" << end << end
		xml_stream << end
		xml_stream << element << "orientation"
		if marker.orientation != None:
			xml_stream << element << "valid" << text << "true" << end << element << "value" << element << "w" << text << marker.orientation.w << end << element << "x" << text << marker.orientation.x << end << element << "y" << text << marker.orientation.y << end << element << "z" << text << marker.orientation.z << end << end
		else:
			xml_stream << element << "valid" << text << "false" << end << element << "value" << element << "w" << text << "1.0" << end << element << "x" << text << "0.0" << end << element << "y" << text << "0.0" << end << element << "z" << text << "0.0" << end << end
		xml_stream << end
		xml_stream << end
	xml_stream << end
	xml_stream << end
	out_file.close()
