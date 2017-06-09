#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLSLShader.h>

using namespace std;

const int NUM_Z = 20;
const int NUM_X = 20;
const float SIZE_X = 4;
const float SIZE_Z = 4;
const float HALF_SIZE_X = SIZE_X/2.f;
const float HALF_SIZE_Z = SIZE_Z/2.f;
const int TOTAL_VERTICES = (NUM_X+1)*(NUM_Z+1);
const int TOTAL_TRIANGLES = NUM_X*NUM_Z*2;
const int TOTAL_INDICES = TOTAL_TRIANGLES*3;
const float SPEED = 1.f;

const int WIDTH = 1280;
const int HEIGHT = 960;

GLSLShader *shader;
float dist = -7.f;
float rX = 0.4;
float rY = 0.2;

GLuint vaoID;
GLuint vboVerticesID;
GLuint vboIndicesID;

glm::vec3 vertices[TOTAL_VERTICES];
GLushort indices[TOTAL_INDICES];

glm::mat4 P;

void init()
{
    shader = new GLSLShader();
    shader->LoadFromFile(GL_VERTEX_SHADER, "../shaders/wave.vert");
    shader->LoadFromFile(GL_FRAGMENT_SHADER, "../shaders/wave.frag");
    shader->CreateAndLinkProgram();
    shader->Use();
    shader->AddAttribute("vVertex");
    shader->AddAttribute("MVP");
    shader->AddAttribute("time");
    shader->UnUse();

    int count = 0;
    int i = 0, j = 0;
    //create geometry
    for (j = 0; j<=NUM_Z;j++){
        for(i = 0; i<=NUM_X; i++){
            vertices[count++] = glm::vec3(
                    ((float(i)/(NUM_X-1))*2-1)*HALF_SIZE_X, 0,
                    ((float(j)/(NUM_Z-1))*2-1)*HALF_SIZE_Z);
        }
    }
    //create topology
    GLushort* id = &indices[0];
    for(i=0; i<NUM_Z; i++){
        for(j=0; j<NUM_X; j++){
            int i0 = i*(NUM_X+1)+j;
            int i1 = i0+1;
            int i2 = i0+(NUM_X+1);
            int i3 = i2+1;
            if ((j+i)%2){
                *id++ = i0; *id++ = i2; *id++ = i1;
                *id++ = i1; *id++ = i2; *id++ = i3;
            }else{
                *id++ = i0; *id++ = i2; *id++ = i3;
                *id++ = i2; *id++ = i3; *id++ = i1;
            }
        }
    }
    //store geometry in the buffer objects
    glGenVertexArrays(1, &vaoID);
    glGenBuffers(1, &vboVerticesID);
    glGenBuffers(1, &vboIndicesID);
    glBindVertexArray(vaoID);
    glBindBuffer(GL_ARRAY_BUFFER, vboVerticesID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
            &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray((*shader)["vVertex"]);
    glVertexAttribPointer((*shader)["vVertex"], 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0],
            GL_STATIC_DRAW);
    //setup projection matrix
    P = glm::mat4(1);;
    glClearColor(0, 0, 0, 0);
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
    //setup perspective projection
    P = glm::perspective(45.0f, (GLfloat)nw/nh, 1.f, 1000.f);
}

void OnRender()
{
    //glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    float time = glutGet(GLUT_ELAPSED_TIME)/1000.f*SPEED;
    glm::mat4 T = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, dist));
    glm::mat4 Rx = glm::rotate(T, rX, glm::vec3(1.f, 0.f, 0.f));
    glm::mat4 MV = glm::rotate(Rx, rY, glm::vec3(0.f, 1.f, 0.f));
    glm::mat4 MVP = P*MV;
    shader->Use();
    glUniformMatrix4fv((*shader)("MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
    glUniform1f((*shader)("time"), time);
    glDrawElements(GL_TRIANGLES, TOTAL_INDICES, GL_UNSIGNED_SHORT, 0);
    shader->UnUse();
    glutSwapBuffers();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitContextVersion(3, 3);
    //glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitContextFlags(GLUT_DEBUG);
    //glutInitContextFlags(GLUT_FORWARD_COMPATIBLE|GLUT_DEBUG);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Getting started with OpenGL 3.3");

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
