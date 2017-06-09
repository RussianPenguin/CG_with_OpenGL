#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

using namespace std;

const int WIDTH = 1280;
const int HEIGHT = 960;

void init()
{
    glClearColor(1, 0, 0, 0);
    cout << "Initialization successfull" << endl;
}

void shutdown()
{
}

void OnShutdown()
{
    cout << "Shutdown successfull" << endl;
}

void OnResize(int nw, int nh)
{
}

void OnRender()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
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
