from OpenGL.GLUT import *
from OpenGL.GLU import *
from OpenGL.GL import *
import sys

winWidth = 600
winHeight = 600

x0 = 100
y0 = 50
z0 = 50

xref = 50
yref = 50
zref = 0

Vx = 0
Vy = 1
Vz = 0

xwMin = -40
ywMin = -60
xwMax = 40
ywMax = 60

dnear = 25
dfar = 125

def init():
	glClearColor(1.0, 1.0, 1.0, 0.0)
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(x0, y0, z0, xref, yref, zref, Vx, Vy, Vz)
	glMatrixMode(GL_PROJECTION)
	glFrustum(xwMin, xwMax, ywMin, ywMax, dnear, dfar)

def displayFcn():
	glClear(GL_COLOR_BUFFER_BIT)
	glColor3f(0.0, 1.0, 0.0)
	
	glPolygonMode(GL_FRONT, GL_FILL)
	glPolygonMode(GL_BACK, GL_LINE)

	glBegin(GL_QUADS)
	glVertex3f(0, 0, 0)
	glVertex3f(100, 0, 0)
	glVertex3f(100, 100, 0)
	glVertex3f(0, 100, 0)
	glEnd()

	glFlush()

def reshapeFcn(newWidth, newHeight):
	glViewport(0, 0, newWidth, newHeight)
	winWidth = newWidth
	winHeight = newHeight

def main():
	glutInit(sys.argv)
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB)
	glutInitWindowPosition(50, 50)
	glutInitWindowSize(winWidth, winHeight)
	glutCreateWindow("Draw a quad")
	init()
	glutDisplayFunc(displayFcn)
	glutReshapeFunc(reshapeFcn)
	glutMainLoop()

if __name__ == '__main__':
	main()
