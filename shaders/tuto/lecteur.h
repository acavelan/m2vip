#ifndef LECTEUR
#define LECTEUR


#include <iostream>

#include <GL/glut.h>    
#include <GL/gl.h>      
#include <GL/glu.h>     

#include <png.h>

using namespace std;

struct Image {
  unsigned long sizeX;
  unsigned long sizeY;
  char *data;
};

struct gl_texture_t
{
  GLuint width;           /* largeur */
  GLuint height;          /* hauteur */

  GLenum format;          /* RVB, RVBA, Luminance, Luminance Alpha */
  GLint  internalFormat;  /* composantes d'un texel */
  GLuint id;

  GLubyte *texels;         /* données de l'image */
};



int ImageLoad(char *filename, Image *image);
struct gl_texture_t *ReadPNGFromFile (const char *filename);
GLubyte* glmReadPPM(char* filename, int* width, int* height);
#endif
