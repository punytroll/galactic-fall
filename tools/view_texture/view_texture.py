from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *
import struct

window = None
texture = None
x = None
y = None

def load_texture(file_name):
	global texture, x, y
	texture = glGenTextures(1)
	glBindTexture(GL_TEXTURE_2D, texture)
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1)
	file = open(file_name, "rb")
	(dimension_x, dimension_y, format) = struct.unpack("!III", file.read(12))
	if dimension_x > dimension_y:
		x = 0.5
		y = 0.5 * (float(dimension_y) / float(dimension_x))
	else:
		x = 0.5 * (float(dimension_x) / float(dimension_y))
		y = 0.5
	print "Reading texture with width='" + str(dimension_x) + "' and height='" + str(dimension_y) + "'."
	if format == 1: # RGBA one byte each
		print "Reading in format='RGBA one byte each'."
		pixels = file.read(4 * dimension_x * dimension_y)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dimension_x, dimension_y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels)
	elif format == 2: # RGB one byte each
		print "Reading in format='RGB one byte each'."
		pixels = file.read(3 * dimension_x * dimension_y)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, dimension_x, dimension_y, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels)
	else:
		print "Unknown format '" + str(format) + "'."
		sys.exit(1)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)

def initialize_opengl(Width, Height):
	load_texture(sys.argv[1])
	glEnable(GL_TEXTURE_2D)
	glClearColor(0.0, 0.0, 0.0, 0.0)
	glClearDepth(1.0)
	glEnable(GL_BLEND)
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA)

def resize_window(Width, Height):
	if Height == 0:
		Height = 1
	glViewport(0, 0, Width, Height)
	glMatrixMode(GL_PROJECTION)
	glLoadIdentity()
	gluPerspective(30.0, float(Width)/float(Height), 0.1, 100.0)
	glMatrixMode(GL_MODELVIEW)

def draw_scene():
	global texture, x, y
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
	glLoadIdentity()
	glTranslatef(0.0, 0.0, -1.6)
	glBindTexture(GL_TEXTURE_2D,texture)
	glBegin(GL_QUADS)
	glTexCoord2f(0.0, 0.0)
	glVertex3f(-x, -y, 0.0)
	glTexCoord2f(1.0, 0.0)
	glVertex3f(x, -y, 0.0)
	glTexCoord2f(1.0, 1.0)
	glVertex3f(x, y, 0.0)
	glTexCoord2f(0.0, 1.0)
	glVertex3f(-x, y, 0.0)
	glEnd()
	glutSwapBuffers()

def key_pressed(key, x, y):
	if key == '\033':
		glutDestroyWindow(window)
		sys.exit()

def main():
	global window
	glutInit("")
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH)
	glutInitWindowSize(640, 480)
	glutInitWindowPosition(0, 0)
	window = glutCreateWindow("Texture Viewer")
	glutDisplayFunc(draw_scene)
	glutReshapeFunc(resize_window)
	glutKeyboardFunc(key_pressed)
	initialize_opengl(640, 480)
	glutMainLoop()

main()
