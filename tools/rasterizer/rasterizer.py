from math import sqrt
from optparse import OptionParser
from struct import pack
import sys
from xml.dom.minidom import Node, parse

class Error(Exception):
    pass

class PremultipliedAlphaError(Error):
    def __init__(self, expression, message):
        self.expression = expression
        self.message = message

parser = OptionParser()
parser.add_option("-i", "--in", dest="in_file", help="The file containing the xml description of the generated picture.")
parser.add_option("-o", "--out", dest="out_file", help="The file to put the image into.")
parser.add_option("-s", "--sub-pixels", dest="sub_pixels", help="How many sub-pixels are to be used in both directions.")
parser.add_option("-d", "--dimensions", dest="dimensions", help="The size of the resulting image, specified in 10x10.")

# read the options and validate
(options, args) = parser.parse_args()
if options.in_file == None:
	print("Set the file containing the xml description with '--in'.")
	sys.exit(1)
if options.out_file == None:
	print("Set the file to write the image to with '--out'.")
	sys.exit(1)
if options.sub_pixels == None:
	print("Using default sub-pixels option '3' for 9 sub-pixels.\n")
	sub_pixels = 3
else:
	sub_pixels = int(options.sub_pixels)
if options.dimensions == None:
	print("Set the size of the resulting image with '--dimensions'.")
	sys.exit(1)
else:
	pixels_x, pixels_y = options.dimensions.split('x')
	pixels_x = int(pixels_x)
	pixels_y = int(pixels_y)

def pad_left(string, padding):
	pad = ""
	for i in range(padding - len(string)):
		pad += " "
	return pad + string

def average_float(list_of_floats):
	return sum(list_of_floats) / float(len(list_of_floats))

def mix_float(factor, from_float, to_float):
	return (1.0 - factor) * from_float + factor * to_float

def mix_color(factor, from_color, to_color):
	return Color(mix_float(factor, from_color.red, to_color.red), mix_float(factor, from_color.green, to_color.green), mix_float(factor, from_color.blue, to_color.blue), mix_float(factor, from_color.opacity, to_color.opacity))

def blend_color(destination_color, source_color):
	return Color((1.0 - source_color.opacity) * destination_color.red + source_color.red, (1.0 - source_color.opacity) * destination_color.green + source_color.green, (1.0 - source_color.opacity) * destination_color.blue + source_color.blue, (1.0 - source_color.opacity) * destination_color.opacity + source_color.opacity)

def get_average_color(list_of_colors):
	red = average_float([color.red for color in list_of_colors])
	green = average_float([color.green for color in list_of_colors])
	blue = average_float([color.blue for color in list_of_colors])
	opacity = average_float([color.opacity for color in list_of_colors])
	return Color(red, green, blue, opacity)

class Color:
	def __init__(self, red, green, blue, opacity):
		self.red = red
		self.green = green
		self.blue = blue
		self.opacity = opacity
		self.__verify()
	
	def __verify(self):
		if self.opacity > 0.0:
			if self.red / self.opacity > 1.0 or self.green / self.opacity > 1.0 or self.blue / self.opacity > 1.0:
				raise PremultipliedAlphaError("The color components don't make up a valid premultiplied alpha color.")
		else:
			if self.red > 0.0 or self.green > 0.0 or self.blue > 0.0:
				raise PremultipliedAlphaError("The color components don't make up a valid premultiplied alpha color.")

class Object:
	def __init__(self):
		self.color = Color(0.0, 0.0, 0.0, 0.0)
	
	def overlaps(self, x, y):
		return false
	
	def get_color(self, x, y):
		return self.color

class Rectangle(Object):
	def __init__(self):
		Object.__init__(self)
		self.left = 0.0
		self.top = 0.0
		self.right = 0.0
		self.bottom = 0.0
	
	def overlaps(self, x, y):
		return (x >= self.left) and (x <= self.right) and (y >= self.top) and (y <= self.bottom)

class Circle(Object):
	def __init__(self):
		Object.__init__(self)
		self.x = 0.0
		self.y = 0.0
		self.radius = 0.0
	
	def overlaps(self, x, y):
		return ((x - self.x) * (x - self.x) + (y - self.y) * (y - self.y)) <= (self.radius * self.radius)

class CircleGradient(Object):
	def __init__(self):
		Object.__init__(self)
		self.x = 0.0
		self.y = 0.0
		self.radius = 0.0
		self.border_color = Color(0.0, 0.0, 0.0, 0.0)
	
	def overlaps(self, x, y):
		return ((x - self.x) * (x - self.x) + (y - self.y) * (y - self.y)) <= (self.radius * self.radius)
	
	def get_color(self, x, y):
		distance = sqrt((x - self.x) * (x - self.x) + (y - self.y) * (y - self.y)) / self.radius
		return mix_color(distance, self.color, self.border_color)

# Create the object model for the vector input file.
objects = list()
vector_graphics_document = parse(options.in_file)
vector_graphics_element = vector_graphics_document.documentElement
dimension_x = float(vector_graphics_element.attributes.get("dimension-x").nodeValue)
dimension_y = float(vector_graphics_element.attributes.get("dimension-y").nodeValue)
for object_element in vector_graphics_element.childNodes:
	if object_element.nodeType == Node.ELEMENT_NODE:
		try:
			if object_element.tagName == "rectangle":
				object = Rectangle()
				object.left = float(object_element.attributes.get("left").nodeValue)
				object.top = float(object_element.attributes.get("top").nodeValue)
				object.right = float(object_element.attributes.get("right").nodeValue)
				object.bottom = float(object_element.attributes.get("bottom").nodeValue)
				object.color = Color(float(object_element.attributes.get("red").nodeValue), float(object_element.attributes.get("green").nodeValue), float(object_element.attributes.get("blue").nodeValue), float(object_element.attributes.get("opacity").nodeValue))
				objects.append(object)
			if object_element.tagName == "circle":
				object = Circle()
				object.x = float(object_element.attributes.get("x").nodeValue)
				object.y = float(object_element.attributes.get("y").nodeValue)
				object.radius = float(object_element.attributes.get("radius").nodeValue)
				object.color = Color(float(object_element.attributes.get("red").nodeValue), float(object_element.attributes.get("green").nodeValue), float(object_element.attributes.get("blue").nodeValue), float(object_element.attributes.get("opacity").nodeValue))
				objects.append(object)
			if object_element.tagName == "circle-gradient":
				object = CircleGradient()
				object.x = float(object_element.attributes.get("x").nodeValue)
				object.y = float(object_element.attributes.get("y").nodeValue)
				object.radius = float(object_element.attributes.get("radius").nodeValue)
				object.color = Color(float(object_element.attributes.get("center-red").nodeValue), float(object_element.attributes.get("center-green").nodeValue), float(object_element.attributes.get("center-blue").nodeValue), float(object_element.attributes.get("center-opacity").nodeValue))
				object.border_color = Color(float(object_element.attributes.get("border-red").nodeValue), float(object_element.attributes.get("border-green").nodeValue), float(object_element.attributes.get("border-blue").nodeValue), float(object_element.attributes.get("border-opacity").nodeValue))
				objects.append(object)
		except PremultipliedAlphaError:
			print("The input color components are not specified as valid premultiplied alpha.")

# Calculate the samples of the image.
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
		sample_color = Color(0.0, 0.0, 0.0, 0.0)
		for object in objects:
			if object.overlaps(sample_x, sample_y) == True:
				sample_color = blend_color(sample_color, object.get_color(sample_x, sample_y))
		sample_row.append(sample_color)
	samples.append(sample_row)

# Write the output file.
out_file = open(options.out_file, "wb")
out_file.write(pack('!III', pixels_x, pixels_y, 3)) # format 3 is 'premultiplied alpha RGBO one byte each'
sample_factor_per_pixel = 1.0 / (sub_pixels * sub_pixels)
for row in range(pixels_y):
	for column in range(pixels_x):
		sample_colors = list()
		for sample_row in range(sub_pixels):
			for sample_column in range(sub_pixels):
				sample_colors.append(samples[row * sub_pixels + sample_row][column * sub_pixels + sample_column])
		sample_color = get_average_color(sample_colors)
		out_file.write(pack('BBBB', int(sample_color.red * 255), int(sample_color.green * 255), int(sample_color.blue * 255), int(sample_color.opacity * 255)))
out_file.close()
