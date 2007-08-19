#! /usr/bin/python

from optparse import OptionParser
from PyQt4 import QtCore, QtGui, QtOpenGL
from struct import unpack
import sys

parser = OptionParser()
parser.add_option("-i", "--in", dest="in_file", help="The raw image file with the texture information.")
(options, args) = parser.parse_args()
if options.in_file == None:
	print "Set the file containing the texture with '--in'."
	sys.exit(1)

# now reading the file content
in_file = open(options.in_file, "rb")
(dimension_x, dimension_y, format) = unpack("!III", in_file.read(12))
print "Reading texture with width='" + str(dimension_x) + "' and height='" + str(dimension_y) + "'."
if format == 1: # RGBA one byte each
	print "Reading in format='RGBA one byte each'."
	pixels = in_file.read(4 * dimension_x * dimension_y)
elif format == 2: # RGB one byte each
	print "Reading in format='RGB one byte each'."
	pixels = in_file.read(3 * dimension_x * dimension_y)
else:
	print "Unknown format '" + str(format) + "'."
	sys.exit(1)
aspect = float(dimension_x) / float(dimension_y)
if aspect > 1:
	x = 1.0
	y = 1.0 / aspect
else:
	x = 1.0 * aspect
	y = 1.0

try:
    from OpenGL import GL
except ImportError:
	print"PyOpenGL must be installed to run the texture viewer."
	sys.exit(1)

class GLWidget(QtOpenGL.QGLWidget):
	def __init__(self, parent=None):
		QtOpenGL.QGLWidget.__init__(self, parent)
		self.setMinimumSize(400, 400)

	def initializeGL(self):
		GL.glClearColor(0.0, 0.0, 0.0, 0.0)
		GL.glShadeModel(GL.GL_FLAT)
		GL.glEnable(GL.GL_DEPTH_TEST)
		GL.glEnable(GL.GL_CULL_FACE)
		GL.glEnable(GL.GL_TEXTURE_2D)
		#~ GL.glEnable(GL.GL_BLEND)
		GL.glBlendFunc(GL.GL_SRC_ALPHA, GL.GL_ONE);
		self.__texture = GL.glGenTextures(1)
		GL.glBindTexture(GL.GL_TEXTURE_2D, self.__texture)
		if format == 1:
			GL.glTexImage2D(GL.GL_TEXTURE_2D, 0, GL.GL_RGBA, dimension_x, dimension_y, 0, GL.GL_RGBA, GL.GL_UNSIGNED_BYTE, pixels)
		elif format == 2:
			GL.glTexImage2D(GL.GL_TEXTURE_2D, 0, GL.GL_RGBA, dimension_x, dimension_y, 0, GL.GL_RGB, GL.GL_UNSIGNED_BYTE, pixels)
		else:
			print "Don't know how to pass the image data to OpenGL."
			sys.exit(1)
		GL.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_MIN_FILTER, GL.GL_LINEAR)
		GL.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_MAG_FILTER, GL.GL_LINEAR)

	def paintGL(self):
		GL.glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT)
		GL.glLoadIdentity()
		GL.glTranslated(0.0, 0.0, -10.0)
		GL.glBindTexture(GL.GL_TEXTURE_2D, self.__texture)
		GL.glBegin(GL.GL_QUADS)
		GL.glTexCoord(0.0, 0.0)
		GL.glVertex(-x, y, 0.0)
		GL.glTexCoord(1.0, 0.0)
		GL.glVertex(x, y, 0.0)
		GL.glTexCoord(1.0, 1.0)
		GL.glVertex(x, -y, 0.0)
		GL.glTexCoord(0.0, 1.0)
		GL.glVertex(-x, -y, 0.0)
		GL.glEnd()

	def resizeGL(self, width, height):
		side = min(width, height)
		GL.glViewport((width - side) / 2, (height - side) / 2, side, side)
		GL.glMatrixMode(GL.GL_PROJECTION)
		GL.glLoadIdentity()
		GL.glOrtho(-1.2, +1.2, +1.2, -1.2, 4.0, 15.0)
		GL.glMatrixMode(GL.GL_MODELVIEW)

class Window(QtGui.QWidget):
	def __init__(self, parent = None):
		QtGui.QWidget.__init__(self, parent)
		self.__opengl_widget = GLWidget()
		main_layout = QtGui.QHBoxLayout()
		main_layout.addWidget(self.__opengl_widget)
		self.setLayout(main_layout)
		self.setWindowTitle("Texture Viewer")

if __name__ == '__main__':
	application = QtGui.QApplication(sys.argv)
	window = Window()
	window.showMaximized()
	sys.exit(application.exec_())
