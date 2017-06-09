#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include "GLSLShader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GL_CHECK_ERRORS assert(glGetError() == GL_NO_ERROR);

using namespace std;

const int WIDTH = 1280;
const int HEIGHT = 960;

typedef struct _vertex {
    glm::vec3 position;
    glm::vec3 color;
} Vertex;

GLSLShader *shader;

GLuint vaoID;
GLuint vboVerticesID;
GLuint vboIndicesID;

// triangle and indices
Vertex vertices[3];
GLushort indices[3];

// projection and modelview matrices
glm::mat4  P = glm::mat4(1);
glm::mat4 MV = glm::mat4(1);

void init()
{
    shader = new GLSLShader();
    GL_CHECK_ERRORS
    shader->LoadFromFile(GL_VERTEX_SHADER, "../shaders/shader.vert");
    shader->LoadFromFile(GL_FRAGMENT_SHADER, "../shaders/shader.frag");
    shader->CreateAndLinkProgram();
    shader->Use();
    shader->AddAttribute("vVertex");
    shader->AddAttribute("vColor");
    shader->AddUniform("MVP");
    shader->UnUse();
    GL_CHECK_ERRORS
    // triangle geometry and vertices
    vertices[0].color = glm::vec3(1, 0, 0);
    vertices[1].color = glm::vec3(0, 1, 0);
    vertices[2].color = glm::vec3(0, 0, 1);

    vertices[0].position = glm::vec3(-1, -1, 0);
    vertices[1].position = glm::vec3( 0,  1, 0);
    vertices[2].position = glm::vec3( 1, -1, 0);

    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;

    GLsizei stride = sizeof(Vertex);

    glGenVertexArrays(1, &vaoID);
    glGenBuffers(1, &vboVerticesID);
    glGenBuffers(1, &vboIndicesID);

    glBindVertexArray(vaoID);
    glBindBuffer(GL_ARRAY_BUFFER, vboVerticesID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0],
            GL_STATIC_DRAW);
    glEnableVertexAttribArray((*shader)["vVertex"]);
    glVertexAttribPointer((*shader)["vVertex"], 3, GL_FLOAT, GL_FALSE, stride, 
            (const GLvoid*)offsetof(Vertex, position));
    glEnableVertexAttribArray((*shader)["vColor"]);
    glVertexAttribPointer((*shader)["vColor"], 3, GL_FLOAT, GL_FALSE, stride,
            (const GLvoid*)offsetof(Vertex, color));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0],
            GL_STATIC_DRAW);

    cout << "Initialization successfull" << endl;
}

void shutdown()
{
    delete shader;
}

void OnShutdown()
{
    shader->DeleteShaderProgram();
    glDeleteBuffers(1, &vboVerticesID);
    glDeleteBuffers(1, &vboIndicesID);
    glDeleteVertexArrays(1, &vaoID);
    cout << "Shutdown successfull" << endl;
}

void OnResize(int nw, int nh)
{
    glViewport(0, 0, (GLsizei) nw, (GLsizei) nh);
    P = glm::ortho(-1,1,-1,1);
}

void OnRender()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    shader->Use();
    glUniformMatrix4fv((*shader)("MVP"), 1, GL_FALSE, glm::value_ptr(P*MV));
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);
    shader->UnUse();
    glutSwapBuffers();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitContextVersion(3, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitContextFlags(GLUT_DEBUG);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Simple triangle with texture and fragment shader");

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err){
        cerr << "Error: " << glewGetErrorString(err) << endl;
        return 1;
    } else {
        if (GLEW_VERSION_3_3){
            cout << "Driver supports OpenGL 3.3\nDetails: " << endl;
        }
    }

    err = glGetError();
    GL_CHECK_ERRORS
    cout << "\tUsing glew: "<< glewGetString(GLEW_VERSION) << endl;
    cout << "\tVendor: " << glGetString(GL_VENDOR) << endl;
    cout << "\tRenderer: " << glGetString(GL_RENDERER) << endl;
    cout << "\tVersion: " << glGetString(GL_VERSION) << endl;
    cout << "\tGLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

    init();
    glutCloseFunc(OnShutdown);
    glutDisplayFunc(OnRender);
    glutReshapeFunc(OnResize);
    glutMainLoop();
    shutdown();
    return 0;
}
