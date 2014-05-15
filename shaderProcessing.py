from triangles import LoadShaders

from OpenGL.GL import *
from OpenGL.GL import shaders
from OpenGL.GLU import *
from OpenGL.GLUT import *

import numpy as np
import math
import ctypes

import sys

vertex_positions = [
	[-1, -1, 0, 1],
	[1, -1, 0, 1],
	[-1, 1, 0, 1],
	[-1, -1, 0, 1],
]

vertex_positions = np.matrix(vertex_positions, dtype="f")

vertex_indices = [
	0, 1, 2,
]

vertex_indices = np.array(vertex_indices, dtype=np.int32)

shaderInfo = [
	(GL_VERTEX_SHADER, './triangles.vert'),
	#(GL_VERTEX_SHADER, './shaders/restart.vert'),
	#(GL_FRAGMENT_SHADER, './shaders/restart.frag'),
	(GL_FRAGMENT_SHADER, './triangles.frag'),
]

vao = None
ebo = None
vbo = None

render_prog = None

def init():
	global render_prog

	global vao
	global ebo
	global vbo

	render_prog = LoadShaders(shaderInfo)
	glUseProgram(render_prog)

	vao = glGenVertexArrays(1)
	glBindVertexArray(vao)

	ebo = glGenBuffers(1)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo)
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertex_indices, GL_STATIC_DRAW)

	vbo = glGenBuffers(1)
	glBindBuffer(GL_ARRAY_BUFFER, vbo)

	glBufferData(GL_ARRAY_BUFFER, vertex_positions, GL_STATIC_DRAW)

	glEnableVertexAttribArray(0)
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, None)


def display():
	global vao
	global render_prog
	
	glClearColor(0, 0, 0, 1)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

	glUseProgram(render_prog)

	glBindVertexArray(vao)
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo)
	glEnableVertexAttribArray(0)

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glutSwapBuffers()

if __name__ == '__main__':

	glutInit(sys.argv)
	glutInitDisplayMode(GLUT_RGB)
	glutInitWindowSize(512, 512)
	glutInitContextVersion(3, 3)
	glutInitContextProfile(GLUT_CORE_PROFILE)
	glutCreateWindow(sys.argv[0])

	init()
	
	glutDisplayFunc(display)
	glutMainLoop()
