from OpenGL.GLUT import *
from OpenGL.GLU import *
from OpenGL.GL import *
import sys

def init():
	glClearColor(1.0, 1.0, 1.0, 0.0)
	glMatrixMode(GL_PROJECTION)
	gluOrtho2D(0.0, 200.0, 0.0, 150.0)

def lineSegment():
	glClear(GL_COLOR_BUFFER_BIT)
	glColor3f(1.0, 0.0, 0.0)
	
	glBegin(GL_LINES)
	glVertex2i(180, 15)
	glVertex2i(10, 145)
	glEnd()

	glFlush()

def main():
	glutInit(sys.argv)
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB)
	glutInitWindowSize(400, 300)
	glutCreateWindow("An example OpenGL in Python")
	init()
	glutDisplayFunc(lineSegment)
	glutMainLoop()

if __name__ == '__main__':
	main()
