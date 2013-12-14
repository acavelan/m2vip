#ifndef config_h
#define config_h

#include "os.h"

#ifdef MAC
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#endif

#ifdef LINUX
#include <GL/glut.h>
#endif

#endif

#include <cmath>
#include <cstdio>
#include <cstdlib>

static GLfloat light_position[] = { 2.0,2.0,2.0,1.0 };

bool move = false;
int width = 800, height = 600;
float rX = 0.0f, rY = 0.0f, mX = 400.0f, mY = 300.0f;

// Camera
float camR = 2.0f;
float phi = 0.0f, theta = 0.0f;

void onDraw(void)
{
    float camX = camR * -sinf(phi * (M_PI / 180)) * cosf(theta * (M_PI / 180));
    float camY = camR * -sinf(theta * (M_PI / 180));
    float camZ = -camR * cosf(phi * (M_PI / 180)) * cosf(theta * (M_PI / 180));
    
    float camX2 = camR * -sinf(phi * (M_PI / 180)) * cosf((theta + 5.0f) * (M_PI / 180));
    float camY2 = camR * -sinf((theta + 5.0f) * (M_PI / 180));
    float camZ2 = -camR * cosf(phi * (M_PI / 180)) * cosf((theta + 5.0f) * (M_PI / 180));
    
    float upX = camX2 - camX;
    float upY = camY2 - camY;
    float upZ = camZ2 - camZ;
    
    // Left eye
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glClear(GL_COLOR_BUFFER_BIT);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE);

    gluLookAt(-0.1f + camX, camY, camZ, 0.0f, 0.0f, 0.0f, upX, upY, upZ);
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glRotatef(180, 1.0f,0.0f,0.0f);
    
    glutWireTeapot(0.5f);
    
    // Right eye
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glClear(GL_DEPTH_BUFFER_BIT);
    glColorMask(GL_FALSE, GL_TRUE, GL_TRUE, GL_TRUE);
    
    gluLookAt(0.1f + camX, camY, camZ, 0.0f, 0.0f, 0.0f, upX, upY, upZ);
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glRotatef(180, 1.0f, 0.0f, 0.0f);
    
    glutWireTeapot(0.5f);
    
    glutSwapBuffers();
}

void onResize(GLint w, GLint h)
{
    printf("onResize(%d, %d)\n", w, h);
    
    width = w;
    height = h;
    
    glViewport(0, 0, width, height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    gluPerspective(75.0, (float)width / height, 0.01f, 100.0f);
    
    glMatrixMode(GL_MODELVIEW);
}

void onUpdate(void)
{
    if(move)
    {
        phi += rX * 1.0f;
        theta += rY * 1.0f;
    }
    
    glutPostRedisplay();
}

void onKeyboardKeyPressed(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 27: exit(0); break; // ESC
        default: break;
    }
}

void onMouseButtonPressed(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        move = true;
    else
        move = false;
}

void onMouseMove(int x, int y)
{
    rX = x - mX;
    rY = y - mY;
    mX = x;
    mY = y;
}

void onClose()
{
    printf("onClose()\n");
}

void onInit(void)
{	
    printf("onInit()\n");
    
    GLfloat lmodel_ambient[] = { 0.5,0.5,0.5,1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };

    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_SMOOTH);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,lmodel_ambient);

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
}

int main(int argc, char **argv)
{        
    glutInit(&argc, argv); 
    glutInitWindowSize(width, height);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    
    glutCreateWindow("Demo"); 
    
    glutDisplayFunc(onDraw);
    glutReshapeFunc(onResize);
    glutKeyboardFunc(onKeyboardKeyPressed);
    glutMouseFunc(onMouseButtonPressed);
    glutMotionFunc(onMouseMove);
    glutIdleFunc(onUpdate);
    
    atexit(onClose);
    
    onInit();
    
    glutMainLoop(); 
    
    return 0; 
}


