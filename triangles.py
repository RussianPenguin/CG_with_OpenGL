from OpenGL.GLUT import *
from OpenGL.GLU import *
from OpenGL.GL import *
from OpenGL.raw.GL.ARB.vertex_array_object import glGenVertexArrays, \
                          glBindVertexArray
import numpy as np

import sys

NumVertices = 6
VAOs = GLuint(0)

def ReadShader(filename):
    with open(filename, 'rb') as file:
        return file.read()
    return ''

def LoadShaders(shaders):
    program = glCreateProgram()

    for shaderEntry in shaders:
        shader = glCreateShader(shaderEntry[0])
        source = ReadShader(shaderEntry[1])

        glShaderSource(shader, source)
        glCompileShader(shader)

        log = glGetShaderInfoLog(shader)
        if log:
            print("Error in shader: {0}".format(shaderEntry[1]))
            print(log)

        glAttachShader(program, shader)

    glLinkProgram(program)

    return program

def init():
    global VAOs

    glGenVertexArrays(1, VAOs)
    glBindVertexArray(VAOs)

    vertices = [
        [-0.90, -0.90],
        [ 0.85, -0.90],
        [-0.90,  0.85],
        [ 0.90, -0.85],
        [ 0.90,  0.90],
        [-0.85,  0.90],
    ]

    vertices = np.array(vertices, dtype="f")

    buffers = glGenBuffers(1)
    glBindBuffer(GL_ARRAY_BUFFER, buffers)
    glBufferData(GL_ARRAY_BUFFER, vertices, GL_STATIC_DRAW)

    shaders = [
        (GL_VERTEX_SHADER, "triangles.vert"),
        (GL_FRAGMENT_SHADER, "triangles.frag"),
    ]

    program = LoadShaders(shaders)
    glUseProgram(program)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, None)
    
    glEnableVertexAttribArray(0)

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

    init()
    
    glutDisplayFunc(display)
    glutMainLoop()

