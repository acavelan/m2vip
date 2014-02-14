#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xdrfile.h"
#include "lecteur.h"

using namespace std;

int currentTime, previousTime,nbFrames, nbFramesMax;

XDRFILE * xdrFile;

int totalFrameNumber;
int atomNumber;
int totalAtomNumber;
int step;
float tps;
float box[9];

GLuint vbo[3];

// Les tableaux d'indices, de positions et de couleurs
GLuint *IndiceArray;
GLfloat *DataArray;
GLfloat *ColorArray;

float minx, maxx;
float miny, maxy;
float minz, maxz;


    
int xdrfile_getframe_positions(int natoms, float *ap, XDRFILE* xfp)
{
    float precision;
    if (xdrfile_read_compr_coord_float(ap, &natoms, &precision, xfp) != natoms) {
        std::cout << "Wrong atom count (not " << natoms << ')' << std::endl;
        return 1;
    }
    return 0;
}  

// xdrfile_getframe - get a complete frame
// returns 1 at the end of file
int xdrfile_getframe_header(int *natoms, int *step, float *tps, float *box, XDRFILE*   xfp)
{
    int magic;

    // Reading magic number
    if (xdrfile_read_int(&magic, 1, xfp) != 1) {
        std::cout << "Problem : magic or end of file" << std::endl;
        return 1; // EOF !
    }

    // Reading the number of atoms
    if (xdrfile_read_int(natoms, 1, xfp) != 1) {
        std::cout << "Problem : atoms" << std::endl;
        return 1; // EOF !
    }

    // Reading the step number
    if (xdrfile_read_int(step, 1, xfp) != 1) {
        std::cout << "Problem : step" << std::endl;
        return 1; // EOF !
    }

    // Reading the time
    if (xdrfile_read_float(tps, 1, xfp) != 1) {
        std::cout << "Problem : time" << std::endl;
        return 1; // EOF !
    }

    // Reading the bounding box of the simulation
    if (xdrfile_read_float(box, 9, xfp) != 9) {
        std::cout << "Problem reading 9 floats from the file" << std::endl;
        return 1;
    }

    return 0;
}

void choix_couleur(const char s, float *rgb) {
  switch(s){
	case 'H': rgb[0]=1.000;rgb[1]=1.000;rgb[2]=1.000; break;
	case 'C': rgb[0]=0.250;rgb[1]=0.750;rgb[2]=0.750; break;
	case 'N': rgb[0]=0.000;rgb[1]=0.000;rgb[2]=1.000; break;
	case 'O': rgb[0]=1.000;rgb[1]=0.000;rgb[2]=0.000; break;
	case 'S': rgb[0]=1.000;rgb[1]=1.000;rgb[2]=0.000; break;
	default:  rgb[0]=0.500;rgb[1]=0.300;rgb[2]=0.000; break;
	}
}

GLfloat *allVertex;
GLfloat *allColors;
GLuint *allIndices;

void init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    gluPerspective(70,1,0.001,50);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
       
    xdrFile = xdrfile_open("00_peptide.200_traj1.xtc","r"); //Ouverture du fichier en lecture
   
    if (xdrfile_getframe_header(&atomNumber, &step, &tps, box, xdrFile)!=1) {	
	    DataArray = new GLfloat[3*atomNumber];
	    IndiceArray = new GLuint[atomNumber];
	    ColorArray = new GLfloat[3*atomNumber];
	    
	    char c;
	    ifstream f;
	    float rgb[3];
	    f.open("gestioncouleur.txt",ios::in);
	    int index = 0;
	    f>>c;

	    while (!f.eof()){
	      choix_couleur(c,rgb);
	      ColorArray[index] = rgb[0];
	      ColorArray[index+1] = rgb[1];
	      ColorArray[index+2] = rgb[2];
	      index+=3;
	      f>>c;
	    }
	    f.close();
	    
	    for (int i =0; i<atomNumber; i++)
	      IndiceArray[i]=i;
	    
	    xdrfile_getframe_positions(atomNumber, DataArray, xdrFile);
	
	    for (int i=0; i<3*atomNumber; i++)
	      DataArray[i] = DataArray[i];	
	    minx = maxx = DataArray[0];
	    miny = maxy = DataArray[1];
	    minz = maxz = DataArray[2];
	  
	    for (int i=1; i<atomNumber; i++) {
	     if (minx>DataArray[3*i])
		minx = DataArray[3*i];
	     if (miny>DataArray[3*i+1])
		miny = DataArray[3*i+1];
	     if (minz>DataArray[3*i+2])
		minz = DataArray[3*i+2];
	     if (maxx<DataArray[3*i])
		maxx = DataArray[3*i];
	     if (maxy<DataArray[3*i+1])
		maxy = DataArray[3*i+1];
	     if (maxz<DataArray[3*i+2])
		maxz = DataArray[3*i+2];
	    }
	  

   }

    // Init VBOs
    printf("atoms: %d\n", atomNumber);

    /*glGenBuffers(3, vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, 3 * atomNumber * sizeof(float), DataArray, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, 3 * atomNumber * sizeof(float), ColorArray, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, atomNumber * sizeof(int), IndiceArray, GL_STATIC_DRAW);
    
    glPointSize(5.0f);
    */

    allVertex = new GLfloat[24*atomNumber];
    allColors = new GLfloat[24*atomNumber];
    allIndices = new GLuint[36*atomNumber];

    for(int i=0; i<atomNumber; i++)
    {
        float x = DataArray[i*3];
        float y = DataArray[i*3+1];
        float z = DataArray[i*3+2];

        printf("%d\n", i);

        GLfloat cube[24] = {
            // front
            x+-0.5, y+-0.5,  z+0.5,
            x+ 0.5, y+-0.5,  z+0.5,
            x+ 0.5, y+ 0.5,  z+0.5,
            x+-0.5, y+ 0.5,  z+0.5,
            // back
            x+-0.5, y+-0.5, z+-0.5,
            x+ 0.5, y+-0.5, z+-0.5,
            x+ 0.5, y+ 0.5, z+-0.5,
            x+-0.5, y+ 0.5, z+-0.5,
        };

        memcpy(&allVertex[i*24], cube, 24*sizeof(GLfloat));

        GLfloat cubeColor[24];
        for(int j=0; j<8; j++)
        {
            allColors[i*24+j*3] = ColorArray[i*3];
            allColors[i*24+j*3+1] = ColorArray[i*3+1];
            allColors[i*24+j*3+2] = ColorArray[i*3+2];
        }

        GLuint cubeIndices[36] = {
            // front
            0, 1, 2,
            2, 3, 0,
            // top
            3, 2, 6,
            6, 7, 3,
            // back
            7, 6, 5,
            5, 4, 7,
            // bottom
            4, 5, 1,
            1, 0, 4,
            // left
            4, 0, 3,
            3, 7, 4,
            // right
            1, 5, 6,
            6, 2, 1
        };

        for(int j=0; j<36; j++)
            allIndices[i*36+j] = i*24+cubeIndices[j];
    }

    glGenBuffers( 3, vbo );

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, 24 * atomNumber * sizeof(GLfloat), allVertex, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, 24 * atomNumber * sizeof(GLfloat), allColors, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * atomNumber * sizeof(GLuint), allIndices, GL_STATIC_DRAW);

    printf("Vbo created\n");
}


void PreDisplay()
{
    
    // for the frame rate
    if (nbFrames++>nbFramesMax){
	previousTime=currentTime;
	currentTime=glutGet(GLUT_ELAPSED_TIME);
	char title[60];
	float fps=1000.0/((currentTime-previousTime)/(float)nbFrames);
	sprintf(title,"Lecteur de trajectoire frame rate: %6.0f fps",fps);
	nbFramesMax=((int)fps/10)*10;
	glutSetWindowTitle(title);
	nbFrames=0;
    }
}

void PostDisplay(){
  
      if (xdrfile_getframe_header(&atomNumber, &step, &tps, box, xdrFile)!=1) {
	
	  
	xdrfile_getframe_positions(atomNumber, DataArray, xdrFile);
	
	for (int i=0; i<3*atomNumber; i++)
	  DataArray[i] = DataArray[i];	
	

      }
      else {	
	xdrfile_close(xdrFile);
	xdrFile = xdrfile_open("00_peptide.200_traj1.xtc","r");
      }
    
  
}

void Display(void)
{
    PreDisplay();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    gluLookAt((maxx-minx)/2,(maxy-miny)/2,-10,(maxx-minx)/2,(maxy-miny)/2,(maxz-minz)/2,0,1,0);

    /*glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, 3 * atomNumber * sizeof(float), DataArray, GL_STATIC_DRAW);
    glVertexPointer( 3, GL_FLOAT, 3 * sizeof(float), 0 );

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glColorPointer( 3, GL_FLOAT, 3 * sizeof(float), 0 );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[2]);
   
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_COLOR_ARRAY );
    glEnableClientState( GL_INDEX_ARRAY );

    glDrawElements(GL_TRIANGLES, atomNumber, GL_UNSIGNED_INT, 0);

    glDisableClientState( GL_INDEX_ARRAY );
    glDisableClientState( GL_COLOR_ARRAY );
    glDisableClientState( GL_VERTEX_ARRAY );*/

    for(int i=0; i<atomNumber; i++)
    {
        float x = DataArray[i*3];
        float y = DataArray[i*3+1];
        float z = DataArray[i*3+2];

        GLfloat cube[24] = {
            x+-0.5, y+-0.5,  z+0.5,
            x+ 0.5, y+-0.5,  z+0.5,
            x+ 0.5, y+ 0.5,  z+0.5,
            x+-0.5, y+ 0.5,  z+0.5,
            x+-0.5, y+-0.5,  z+-0.5,
            x+ 0.5, y+-0.5,  z+-0.5,
            x+ 0.5, y+ 0.5,  z+-0.5,
            x+-0.5, y+ 0.5,  z+-0.5,
        };

        for(int j=0; j<24; j++)
            allVertex[i*24+j] = cube[j];
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, 24 * atomNumber * sizeof(GLfloat), allVertex, GL_STATIC_DRAW);
    glVertexPointer( 3, GL_FLOAT, 0, 0 );

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glColorPointer( 3, GL_FLOAT, 0, 0 );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[2]);

    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_COLOR_ARRAY );

    glDrawElements(GL_TRIANGLES, 36*atomNumber , GL_UNSIGNED_INT, 0);

    glDisableClientState( GL_COLOR_ARRAY );
    glDisableClientState( GL_VERTEX_ARRAY );

    glutSwapBuffers();
    
    PostDisplay();
}




void Reshape(int Width, int Height)
{
    glViewport(0, 0, (GLsizei)Width, (GLsizei)Height);
}

void Idle()
{
  glutPostRedisplay();
}

void ClavierClassique(unsigned char key, int x, int y)
{
    switch(key) {	
    case 0x1B:
	exit(0);
    }
    glutPostRedisplay();
}

void ClavierSpecial(int key, int x, int y)
{

}

void Souris(int bouton, int etat, int x, int y)
{
}

void Motion (int x, int y)
{
}

void onClose()
{
    printf("The end\n");

    delete [] IndiceArray;
    delete [] DataArray;
    delete [] ColorArray;
}

int main(int argc, char** argv)
{
    atexit(onClose);

    glutInit (&argc,argv) ;
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH) ;

    glutInitWindowSize (500,500) ;
    glutInitWindowPosition (100, 100) ;
    glutCreateWindow ("Tutoriel lecteur de trajectoire") ;
    GLenum err = glewInit();
    init();
    glutReshapeFunc(Reshape);
    glutDisplayFunc(Display);
    glutIdleFunc(Idle);
    glutKeyboardFunc(ClavierClassique);
    glutSpecialFunc(ClavierSpecial);
    glutMouseFunc(Souris);
    glutMotionFunc(Motion);

    glutMainLoop () ;

    return 0 ;

}
