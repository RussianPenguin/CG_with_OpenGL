from OpenGL.GLUT import *
from OpenGL.GLU import *
from OpenGL.GL import *
import sys

winWidth = 500
winHeight = 500


def init():
	glClearColor(1.0, 1.0, 1.0, 0.0)

def wireQuadSurfs():
	glClear(GL_COLOR_BUFFER_BIT)
	glColor3f(0, 0, 1)
	gluLookAt(2, 2, 2, 0, 0, 0, 0, 0, 1)
	
	glPushMatrix()
	glTranslatef(1, 1, 0)
	glutWireSphere(0.75, 8, 6)
	glPopMatrix()

	glPushMatrix()
	glTranslatef(1, -0.5, 0.5)
	glutWireCone(0.7, 2.0, 7, 6)
	glPopMatrix()

	glPushMatrix()
	glTranslatef(0, 1.2, 0.8)
	cylinder = gluNewQuadric()
	gluQuadricDrawStyle(cylinder, GLU_LINE)
	gluCylinder(cylinder, 0.6, 0.6, 1.5, 6, 4)
	glPopMatrix()

	glFlush()

def winReshapeFcn(newWidth, newHeight):
	glViewport(0, 0, newWidth, newHeight)
	glMatrixMode(GL_PROJECTION)
	glOrtho(-2, 2, -2, 2, 0, 5)
	glMatrixMode(GL_MODELVIEW)
	glClear(GL_COLOR_BUFFER_BIT)

def main():
	glutInit(sys.argv)
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB)
	glutInitWindowPosition(100, 100)	
	glutInitWindowSize(winWidth, winHeight)
	glutCreateWindow("Wired shapes")
	init()
	glutDisplayFunc(wireQuadSurfs)
	glutReshapeFunc(winReshapeFcn)
	glutMainLoop()

if __name__ == '__main__':
	main()
