from math import sqrt
from optparse import OptionParser
from struct import pack
from sys import exit, stderr, stdout
from xml.dom.minidom import Node, parse

parser = OptionParser()
parser.add_option("-i", "--in", dest="in_file", help="The file containing the xml description of the generated picture.")
parser.add_option("-o", "--out", dest="out_file", help="The file to put the image into.")
parser.add_option("-s", "--sub-pixels", dest="sub_pixels", help="How many sub-pixels are to be used in both directions.")
parser.add_option("-d", "--dimensions", dest="dimensions", help="The size of the resulting image, specified in 10x10.")

# read the options and validate
(options, args) = parser.parse_args()
if options.in_file == None:
	print "Set the file containing the xml description with '--in'."
	exit(1)
if options.out_file == None:
	print "Set the file to write the image to with '--out'."
	exit(1)
if options.sub_pixels == None:
	stderr.write("Using default sub-pixels option '3' for 9 sub-pixels.\n")
	sub_pixels = 3
else:
	sub_pixels = int(options.sub_pixels)
if options.dimensions == None:
	print "Set the size of the resulting image with '--dimensions'."
	exit(1)
else:
	pixels_x, pixels_y = options.dimensions.split('x')
	pixels_x = long(pixels_x)
	pixels_y = long(pixels_y)

def pad_left(string, padding):
	pad = ""
	for i in range(padding - len(string)):
		pad += " "
	return pad + string

class Color:
	def __init__(self, red, green, blue, transparency):
		self.red = red
		self.green = green
		self.blue = blue
		self.transparency = transparency

class Object:
	def __init__(self):
		self.color = Color(0, 0, 0, 1)
	
	def overlaps(self, x, y):
		return false
	
	def get_color(self, x, y):
		return self.color

class Rectangle(Object):
	def __init__(self):
		Object.__init__(self)
		self.left = 0
		self.top = 0
		self.right = 0
		self.bottom = 0
	
	def overlaps(self, x, y):
		return (x >= self.left) and (x <= self.right) and (y >= self.top) and (y <= self.bottom)

class Circle(Object):
	def __init__(self):
		Object.__init__(self)
		self.x = 0
		self.y = 0
		self.radius = 0
	
	def overlaps(self, x, y):
		return ((x - self.x) * (x - self.x) + (y - self.y) * (y - self.y)) <= (self.radius * self.radius)

class CircleGradient(Object):
	def __init__(self):
		Object.__init__(self)
		self.x = 0
		self.y = 0
		self.radius = 0
		self.border_color = Color(0, 0, 0, 1)
	
	def overlaps(self, x, y):
		return ((x - self.x) * (x - self.x) + (y - self.y) * (y - self.y)) <= (self.radius * self.radius)
	
	def get_color(self, x, y):
		difference_color = Color(self.border_color.red - self.color.red, self.border_color.green - self.color.green, self.border_color.blue - self.color.blue, self.border_color.transparency - self.color.transparency)
		distance = sqrt((x - self.x) * (x - self.x) + (y - self.y) * (y - self.y)) / self.radius
		return Color(self.color.red + (difference_color.red * distance), self.color.green + (difference_color.green * distance), self.color.blue + (difference_color.blue * distance), self.color.transparency + (difference_color.transparency * distance))

objects = list()
vg_document = parse(options.in_file)
vector_graphics_element = vg_document.documentElement
dimension_x = float(vector_graphics_element.attributes.get("dimension-x").nodeValue)
dimension_y = float(vector_graphics_element.attributes.get("dimension-y").nodeValue)
for object_element in vector_graphics_element.childNodes:
	if object_element.nodeType == Node.ELEMENT_NODE:
		if object_element.tagName == "rectangle":
			object = Rectangle()
			object.left = float(object_element.attributes.get("left").nodeValue)
			object.top = float(object_element.attributes.get("top").nodeValue)
			object.right = float(object_element.attributes.get("right").nodeValue)
			object.bottom = float(object_element.attributes.get("bottom").nodeValue)
			object.color.red = float(object_element.attributes.get("red").nodeValue)
			object.color.green = float(object_element.attributes.get("green").nodeValue)
			object.color.blue = float(object_element.attributes.get("blue").nodeValue)
			object.color.transparency = float(object_element.attributes.get("transparency").nodeValue)
			objects.append(object)
		if object_element.tagName == "circle":
			object = Circle()
			object.x = float(object_element.attributes.get("x").nodeValue)
			object.y = float(object_element.attributes.get("y").nodeValue)
			object.radius = float(object_element.attributes.get("radius").nodeValue)
			object.color.red = float(object_element.attributes.get("red").nodeValue)
			object.color.green = float(object_element.attributes.get("green").nodeValue)
			object.color.blue = float(object_element.attributes.get("blue").nodeValue)
			object.color.transparency = float(object_element.attributes.get("transparency").nodeValue)
			objects.append(object)
		if object_element.tagName == "circle-gradient":
			object = CircleGradient()
			object.x = float(object_element.attributes.get("x").nodeValue)
			object.y = float(object_element.attributes.get("y").nodeValue)
			object.radius = float(object_element.attributes.get("radius").nodeValue)
			object.color.red = float(object_element.attributes.get("center-red").nodeValue)
			object.color.green = float(object_element.attributes.get("center-green").nodeValue)
			object.color.blue = float(object_element.attributes.get("center-blue").nodeValue)
			object.color.transparency = float(object_element.attributes.get("center-transparency").nodeValue)
			object.border_color.red = float(object_element.attributes.get("border-red").nodeValue)
			object.border_color.green = float(object_element.attributes.get("border-green").nodeValue)
			object.border_color.blue = float(object_element.attributes.get("border-blue").nodeValue)
			object.border_color.transparency = float(object_element.attributes.get("border-transparency").nodeValue)
			objects.append(object)

samples_x = pixels_x * sub_pixels
samples_y = pixels_y * sub_pixels
sample_distance_x = dimension_x / samples_x
sample_distance_y = dimension_y / samples_y
samples = list()
for row in range(samples_y):
	sample_row = list()
	for column in range(samples_x):
		sample_x = sample_distance_x * (0.5 + column)
		sample_y = sample_distance_y * (0.5 + row)
		overlapping_objects = list()
		red = 0.0
		green = 0.0
		blue = 0.0
		transparency = 1.0
		for object in objects:
			if object.overlaps(sample_x, sample_y) == True:
				object_color = object.get_color(sample_x, sample_y)
				red = red * object_color.transparency + object_color.red * (1.0 - object_color.transparency)
				green = green * object_color.transparency + object_color.green * (1.0 - object_color.transparency)
				blue = blue * object_color.transparency + object_color.blue * (1.0 - object_color.transparency)
				transparency *= object_color.transparency
		sample_row.append(Color(red, green, blue, transparency))
	samples.append(sample_row)
	
# now open the out file for writing binary
out_file = open(options.out_file, "wb")

out_file.write(pack('!III', long(pixels_x), long(pixels_y), long(1)))
for row in range(pixels_y):
	for column in range(pixels_x):
		color = Color(0, 0, 0, 0)
		for sample_row in range(sub_pixels):
			for sample_column in range(sub_pixels):
				sample_color = samples[row * sub_pixels + sample_row][column * sub_pixels + sample_column]
				color.red += sample_color.red
				color.green += sample_color.green
				color.blue += sample_color.blue
				color.transparency += sample_color.transparency
		color.red /= sub_pixels * sub_pixels
		color.green /= sub_pixels * sub_pixels
		color.blue /= sub_pixels * sub_pixels
		color.transparency /= sub_pixels * sub_pixels
		color.red = int(color.red * 255)
		color.green = int(color.green * 255)
		color.blue = int(color.blue * 255)
		color.transparency = int(color.transparency * 255)
		out_file.write(pack('BBBB', color.red, color.green, color.blue, color.transparency))
out_file.close()
