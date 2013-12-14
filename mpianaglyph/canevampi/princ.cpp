#ifndef config_h
#define config_h

#include "os.h"

#ifdef MAC
#include <GLUT/freeglut.h>
#include <OpenGL/gl.h>
#endif

#ifdef LINUX
#include <GL/freeglut.h>
#endif

#endif

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <mpi.h>

static GLfloat light_position[] = { 2.0,2.0,2.0,1.0 };

int move = 1;
int width = 1280/2, height = 600/2;
int rX = 0.0f, rY = 0.0f, mX = 400.0f, mY = 300.0f;

// Camera
float camR = 4.0f;
float phi = 0.0f, theta = 0.0f;

// MPI
int state = 1;
int rank = -1;
int size = -1;

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
    
    glutWireTeapot(0.5f);
    
    // Right eye
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glClear(GL_DEPTH_BUFFER_BIT);
    glColorMask(GL_FALSE, GL_FALSE, GL_TRUE, GL_TRUE);
    
    gluLookAt(0.1f + camX, camY, camZ, 0.0f, 0.0f, 0.0f, upX, upY, upZ);
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    glutWireTeapot(0.5f);
    
    MPI_Barrier(MPI_COMM_WORLD);
    
    glutSwapBuffers();
}

void onResize(GLint w, GLint h)
{
    printf("onResize[%d](%d, %d)\n", rank, w, h);
    
    width = w;
    height = h;
    
    glViewport(0, 0, width, height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    float aspect_ratio = ((float)height) / ((float)width);

    if(rank == 0) // upper left
    {
        glFrustum(.5, 0.0, 0.0, .5 * aspect_ratio, 1, 200);
    }
    else if(rank == 1) // upper right
    {
        glFrustum(0.0, -.5, 0.0, .5 * aspect_ratio, 1, 200);
    }
    else if(rank == 2) // lower left
    {
        glFrustum(.5, 0.0, -.5 * aspect_ratio, 0.0, 1, 200);
    }
    else if(rank == 3) // lower right
    {
        glFrustum(0.0, -.5, -.5 * aspect_ratio, 0.0, 1, 200);
    }
    
    glMatrixMode(GL_MODELVIEW);
}

void onUpdate(void)
{
    MPI_Bcast(&state, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    if(state == 0)
        exit(0);
    
    MPI_Bcast(&mX, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&mY, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&rX, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&rY, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&move, 1, MPI_INT, 0, MPI_COMM_WORLD);
    //MPI_Barrier(MPI_COMM_WORLD);
    
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
        move = 1;
    else
        move = 0;
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
    printf("onClose[%d]()\n", rank);
    
    state = 0;
    
    if(rank == 0)
    {
        MPI_Bcast(&state, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
    }
    
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}

void onInit(void)
{	
    printf("onInit[%d]()\n", rank);
    
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
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    glutInit(&argc, argv); 
    glutInitWindowSize(width, height);
    
    if(rank < 2)
        glutInitWindowPosition(rank * width, 0);
    else                       
        glutInitWindowPosition((rank - 2) * width, height + 27);
        
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_BORDERLESS);
    
    glutCreateWindow("Demo");
    
    glutDisplayFunc(onDraw);
    glutReshapeFunc(onResize);
    
    if(rank == 0)
    {
        glutKeyboardFunc(onKeyboardKeyPressed);
        glutMouseFunc(onMouseButtonPressed);
        glutMotionFunc(onMouseMove);
    }
    
    glutIdleFunc(onUpdate);
    
    atexit(onClose);
    
    onInit();
    
    glutMainLoop(); 
}

