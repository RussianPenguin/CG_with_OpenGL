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

GLSLShader *shader;

GLuint vaoID;
GLuint vboVerticesID;
GLuint vboIndicesID;

// triangle and indices
glm::vec3 vertices[4];
GLushort indices[3];
GLint sub_division = 1;
float dist = -35;
float rX = 0.4;
float rY = 0.2;
int oldX = 0, oldY = 0;

bool state = false;

// projection and modelview matrices
glm::mat4  P = glm::mat4(1);
glm::mat4 MV = glm::mat4(1);
glm::mat4 M[4];

void init()
{
    shader = new GLSLShader();
    GL_CHECK_ERRORS
    M[0] = glm::translate(glm::mat4(1), glm::vec3(-5, 0, -5));
    M[1] = glm::translate(M[0], glm::vec3( 10, 0,  0));
    M[2] = glm::translate(M[1], glm::vec3(  0, 0, 10));
    M[3] = glm::translate(M[2], glm::vec3(-10, 0,  0));

    shader->LoadFromFile(GL_VERTEX_SHADER, "../shaders/instanced_divide.vert");
    shader->LoadFromFile(GL_GEOMETRY_SHADER,
            "../shaders/instanced_divide.geom");
    shader->LoadFromFile(GL_FRAGMENT_SHADER,
            "../shaders/instanced_divide.frag");
    shader->CreateAndLinkProgram();
    shader->Use();
    shader->AddAttribute("vVertex");
    shader->AddUniform("M");
    shader->AddUniform("PV");
    shader->AddUniform("sub_division");
    glUniform1i((*shader)("sub_division"), sub_division);
    glUniformMatrix4fv((*shader)("M"), 4, GL_FALSE, glm::value_ptr(M[0]));
    shader->UnUse();
    GL_CHECK_ERRORS
    // triangle geometry and vertices
    vertices[0] = glm::vec3(-5 ,0 , -5);
    vertices[1] = glm::vec3(-5 ,0 ,  5);
    vertices[2] = glm::vec3( 5 ,0 ,  5);
    vertices[3] = glm::vec3( 5 ,0 , -5);

    GLushort* id = &indices[0];
    *id++ = 0;
    *id++ = 1;
    *id++ = 2;
    *id++ = 0;
    *id++ = 2;
    *id++ = 3;
    
    glGenVertexArrays(1, &vaoID);
    glGenBuffers(1, &vboVerticesID);
    glGenBuffers(1, &vboIndicesID);

    glBindVertexArray(vaoID);
    glBindBuffer(GL_ARRAY_BUFFER, vboVerticesID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0],
            GL_STATIC_DRAW);
    glEnableVertexAttribArray((*shader)["vVertex"]);
    glVertexAttribPointer((*shader)["vVertex"], 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0],
            GL_STATIC_DRAW);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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

void OnMouseMove(int x, int y)
{
    if (!state){
        dist *= (1+(y-oldY)/60.f);
    }else{
        rY += (x-oldX)/5.f;
        rX += (y-oldY)/5.f;
    }
    oldX = x;
    oldY = y;

    glutPostRedisplay();
}

void OnMouseDown(int button, int s, int x, int y)
{
    if (s == GLUT_DOWN){
        oldX = x;
        oldY = y;
    }
    if (button == GLUT_RIGHT_BUTTON)
        state = false;
    else
        state = true;
}

void OnKey(unsigned char key, int x, int y)
{
    switch(key){
        case '.': sub_division--; break;
        case ',': sub_division++; break;
    }
    sub_division = max(1, min(8, sub_division));
    glutPostRedisplay();
}

void OnResize(int nw, int nh)
{
    glViewport(0, 0, (GLsizei) nw, (GLsizei) nh);
    P = glm::perspective(45.f, (GLfloat)nw/nh, 1.f, 1000.f);
}

void OnRender()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glm::mat4 T = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, dist));
    glm::mat4 Rx = glm::rotate(T, rX, glm::vec3(1.f, 0.f, 0.f));
    glm::mat4 V = glm::rotate(Rx, rY, glm::vec3(0.f, 1.f, 0.f));
    glm::mat4 PV = P*V;
    shader->Use();
    glUniform1i((*shader)("sub_division"), sub_division);
    glUniformMatrix4fv((*shader)("PV"), 1, GL_FALSE, glm::value_ptr(PV));
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0, 4);
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
    glutCreateWindow("Geometry shader");

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
    glutMotionFunc(OnMouseMove);
    glutMouseFunc(OnMouseDown);
    glutKeyboardFunc(OnKey);
    glutMainLoop();
    shutdown();
    return 0;
}
