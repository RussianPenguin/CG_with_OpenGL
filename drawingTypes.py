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

vertex_indices = np.array(vertex_indices, dtype="ushort")

vertex_colors = [
	[1, 1, 1, 1],
	[1, 1, 0, 1],
	[1, 0, 1, 1],
	[0, 1, 1, 1],
]

vertex_colors = np.matrix(vertex_colors, dtype="f")

shaderInfo = [
	(GL_VERTEX_SHADER, './shaders/restart.vert'),
	(GL_FRAGMENT_SHADER, './shaders/restart.frag'),
]

vao = None
ebo = None
vbo = None

render_prog = None

render_model_matrix_loc = None
render_projection_matrix_loc = None

aspect = 0

def frustum(left, right, bottom, top, n, f):
	m = [
		[(2.0 * n) / (right - left), 0.0, 0.0, 0.0],
		[0.0, (2.0 * n) / (top - bottom), 0.0, 0.0],
		[(right + left)/(right - left), (top + bottom)/(top - bottom), -(f+n)/(f-n), -1.0],
		[0.0, 0.0, -(2.0 * f * n)/(f-n), 0.0],
	]
	m = np.matrix(m, dtype="f")
	return m
	

def translation(vector):
	m = [
		[1, 0, 0, 0],
		[0, 1, 0, 0],
		[0, 0, 1, 0],
		[vector[0], vector[1], vector[2], 1],
	]
	
	m = np.matrix(m, dtype="f")
	return m


def init():
	global render_prog

	global vao
	global ebo
	global vbo

	global render_model_matrix_loc
	global render_projection_matrix_loc

	render_prog = LoadShaders(shaderInfo)
	glUseProgram(render_prog)

	render_model_matrix_loc = glGetUniformLocation(render_prog, "model_matrix")
	render_projection_matrix_loc = glGetUniformLocation(render_prog, "projection_matrix")

	vao = glGenVertexArrays(1)
	glBindVertexArray(vao)

	ebo = glGenBuffers(1)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo)
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertex_indices, GL_STATIC_DRAW)

	vbo = glGenBuffers(1)
	glBindBuffer(GL_ARRAY_BUFFER, vbo)

	glBufferData(GL_ARRAY_BUFFER, vertex_positions.nbytes + vertex_colors.nbytes, None, GL_STATIC_DRAW)
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertex_positions.nbytes, vertex_positions)
	glBufferSubData(GL_ARRAY_BUFFER, vertex_positions.nbytes, vertex_colors.nbytes, vertex_colors)

	glEnableVertexAttribArray(0)
	glEnableVertexAttribArray(1)
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, None)
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, ctypes.c_void_p(vertex_positions.nbytes))


def display():
	global vao
	global render_prog
	
	glClearColor(0, 0, 0, 1)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

	projection_matrix = frustum(-1.0, 1.0, -aspect, aspect, 1.0, 500.0)

	glUniformMatrix4fv(render_projection_matrix_loc, 1, GL_FALSE, projection_matrix)

	glBindVertexArray(vao)
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo)
	glEnableVertexAttribArray(0)

	# DrawArrays
	model_matrix = translation(np.array([-3.0, 0.0, -5.0], dtype="f"));
	glUniformMatrix4fv(render_model_matrix_loc, 1, GL_FALSE, model_matrix);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	
	# DrawElements
	model_matrix = translation(np.array([-1.0, 0.0, -5.0], dtype="f"));
	glUniformMatrix4fv(render_model_matrix_loc, 1, GL_FALSE, model_matrix);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, None);
	
	# DrawElementsBaseVertex
	model_matrix = translation(np.array([1.0, 0.0, -5.0], dtype="f"));
	glUniformMatrix4fv(render_model_matrix_loc, 1, GL_FALSE, model_matrix);
	glDrawElementsBaseVertex(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, None, 1);

	model_matrix = translation(np.array([3.0, 0.0, -5.0], dtype="f"));
	glUniformMatrix4fv(render_model_matrix_loc, 1, GL_FALSE, model_matrix);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 3, 1);	

	glutSwapBuffers()

def reshape(w, h):
	global aspect 
	aspect= float(h)/float(w)
	glViewport(0, 0, w, h)


if __name__ == '__main__':

	glutInit(sys.argv)
	glutInitDisplayMode(GLUT_RGB)
	glutInitWindowSize(512, 512)
	glutInitContextVersion(3, 3)
	glutInitContextProfile(GLUT_CORE_PROFILE)
	glutCreateWindow(sys.argv[0])

	init()
	
	glutDisplayFunc(display)
	glutReshapeFunc(reshape)
	glutMainLoop()
