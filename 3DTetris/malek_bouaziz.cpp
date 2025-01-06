#include <GL/gl.h>		   // Open Graphics Library (OpenGL) header
#include <GL/glut.h>	   // The GL Utility Toolkit (GLUT) Header
#include <GL/glu.h>

#include <iostream>
#include <list>
#include <queue>

using namespace std;
int eyeX = 30;
int eyeY = 30;
int eyeZ = 30;

int CamFlag = 0;

/* Current cube positions */
int indexX=0;
int indexY=0;
int indexZ=15;

/* Camera angles */
float angleX = 0.0;
float angleY = 0.0;
float angleZ = 0.0;

/* selected cube coordinates */
int selectX = 0;
int selectY = 0;
int selectZ = 0;

/* random number from 1 to 3 */
int randNum=1;

/* lighting */
GLfloat lightPos[] = { 16, 16, 16, 1.0f };
GLfloat whiteLight[] = { 0.2f, 0.2f, 0.2f, 0.2f };
GLfloat sourceLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };

int alreadyMoved=0;
int listDisplayed =0;
int displayCount = 0;

/* pause */
int PAUSE = 1;
int gg=0;
/* later */
int START=1;

int flag=0;

int camFlag=0;

/* Overall objects */
int cubeNum=0;

int SCORE=0;
void displayGrid() {
	int n = 15;		// grid length

		/* white grid */
		glBegin(GL_LINES);
			for(int i=0; i<=n; i++) {
				// set colour
				glColor3f(1, 0, 0);

				// x-z axis
				glVertex3f(i, 0, 0);
				glVertex3f(i, 0, n);

				glVertex3f(0, 0, i);
				glVertex3f(n, 0, i);
				
				glColor3f(0, 1, 0);
				// y-z axis
				glVertex3f(0, i, 0);
				glVertex3f(0, i, n);

				glVertex3f(0, 0, i);
				glVertex3f(0, n, i);

				glColor3f(0, 1, 1);
				// x-y axis
				glVertex3f(i, 0, 0);
				glVertex3f(i, n, 0);

				glVertex3f(0, i, 0);
				glVertex3f(n, i, 0);
			}
		glEnd();
}

void init() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1.0, 1, 1000);
	
	gluLookAt(30, 30, 30, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 
	
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLight);
	glLightfv(GL_LIGHT0,GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0,GL_SPECULAR, sourceLight);											

	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	
	glShadeModel( GL_SMOOTH );
}
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	displayGrid();
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	//displayCube(indexX, indexY, indexZ, randNum);
	//displayAll();
	
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	
	/*if( (PAUSE==1) && (START==0) ) {
		func();
		displaySelectedCube();
	}*/
		
	glutSwapBuffers();
	glFlush();
}



int main(int argc, char **argv) {
	glutInit(&argc, argv);                  	                			    // GLUT initialization
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );					// Display Mode
	glutInitWindowPosition (100, 250);
	glutInitWindowSize(1920,1080);												// set window size
	glutCreateWindow("TETRIS-3D");									// create Window

	init();
	glutDisplayFunc(display);													// register Display Function
	//glutKeyboardFunc(keyboardFunc);
	//glutSpecialFunc(specialFunc);
	glutMainLoop();																// run GLUT mainloop
return 0;
}