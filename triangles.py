from OpenGL.GLUT import *
from OpenGL.GLU import *
from OpenGL.GL import *
from OpenGL.raw.GL.ARB.vertex_array_object import glGenVertexArrays, \
                                                  glBindVertexArray
import numpy as np

import sys

NumVertices = 6
NumBuffers = 1
VAOs = GLuint(0)
NumVAOs = 2
vPosition = 1

def ReadShader(filename):
	with open(filename, rb) as file:
		return file.raw_read()
	return ''

def LoadShaders(shaders):
	program = glCreateProgram()
	shaderInfo = []

	for shaderEntry in shaders:
		shader = glCreateShader(shaderEntry[0])
		source = ReadShader(shaderEntry[1])

		glShaderSource(shader, source)
		glCompileShader(shader)

		glAttachShader(program, shader)

	glLinkProgram(program)

	return program

def init():
	glGenVertexArrays(NumVAOs, VAOs)
	glBindVertexArray(VAOs)

	vertices = [
		[-0.90, -0.90],
		[0.85, -0.90],
		[-0.90, 0.85],
		[0.90, -0.85],
		[0.90, 0.90],
		[-0.85, 0.90],
	]

	vertices = np.array(vertices, dtype="f")

	buffers = glGenBuffers(NumBuffers)
	glBindBuffer(GL_ARRAY_BUFFER, buffers)
	glBufferData(GL_ARRAY_BUFFER, vertices, GL_STATIC_DRAW)

	shaders = [
	#	(GL_VERTEX_SHADER, "triangles.vert"),
	#	(GL_FRAGMENT_SHADER, "triangles.frag"),
	]

	program = 0 #LoadShaders(shaders)
	glUseProgram(program)
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, None)
	
	glEnableVertexAttribArray(vPosition)

def display():
	glClear(GL_COLOR_BUFFER_BIT)

	glBindVertexArray(VAOs);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices)

	glFlush()

if __name__ == '__main__':
	glutInit(sys.argv)
	glutInitDisplayMode(GLUT_RGBA)
	glutInitWindowSize(512, 512)
	glutInitContextVersion(3, 3)
	glutInitContextProfile(GLUT_CORE_PROFILE)
	glutCreateWindow(sys.argv[0])


	#if glewInit():
	#	sys.strderr.write("Unable to inittialize GLEW ... exiting")
	#	sys.exit(1)

	init()
	
	glutDisplayFunc(display)
	glutMainLoop()

