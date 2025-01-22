#include <GL/gl.h>		   // Open Graphics Library (OpenGL) header
#include <GL/glut.h>	   // The GL Utility Toolkit (GLUT) Header
#include <GL/glu.h>

#include <iostream>
#include <list>
#include <queue>
#include <memory>

class tetromino {
	public:
		// Tetromino shapes
		enum Shape { I, J, L, O, S, T, Z };

		// Constructor
		tetromino(Shape shape) : shape(shape) {
			initializeShape();
		}

		// Get the shape of the tetromino
		Shape getShape() const {
			return shape;
		}

		// Get the blocks of the tetromino
		const std::vector<std::vector<std::vector<int>>>& getBlocks() const {
			return blocks;
		}

	private:
		Shape shape;
		std::vector<std::vector<std::vector<int>>> blocks;

		// Initialize the blocks of the tetromino based on its shape
		void initializeShape() {
			switch (shape) {
				case I:
					blocks = {{{1, 1, 1, 1}}};
					break;
				case J:
					blocks = {{{1, 0, 0}, {1, 1, 1}}};
					break;
				case L:
					blocks = {{{0, 0, 1}, {1, 1, 1}}};
					break;
				case O:
					blocks = {{{1, 1}, {1, 1}}};
					break;
				case S:
					blocks = {{{0, 1, 1}, {1, 1, 0}}};
					break;
				case T:
					blocks = {{{0, 1, 0}, {1, 1, 1}}};
					break;
				case Z:
					blocks = {{{1, 1, 0}, {0, 1, 1}}};
					break;
			}
		}
	};
	void rotateX(tetromino& t) {
		auto blocks = std::make_shared<std::vector<std::vector<std::vector<int>>>>(t.getBlocks());
		int n = blocks->size();
		std::vector<std::vector<std::vector<int>>> rotated(n, std::vector<std::vector<int>>(n, std::vector<int>(n)));

		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				for (int k = 0; k < n; ++k) {
					rotated[i][j][k] = (*blocks)[i][n - 1 - k][j];
				}
			}
		}
		*blocks = rotated;
	}

	void rotateY(tetromino& t) {
		auto blocks = std::make_shared<std::vector<std::vector<std::vector<int>>>>(t.getBlocks());
		int n = blocks->size();
		std::vector<std::vector<std::vector<int>>> rotated(n, std::vector<std::vector<int>>(n, std::vector<int>(n)));

		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				for (int k = 0; k < n; ++k) {
					rotated[i][j][k] = (*blocks)[n - 1 - k][j][i];
				}
			}
		}
		*blocks = rotated;
	}
	void rotateZ(tetromino& t) {
		auto blocks = std::make_shared<std::vector<std::vector<std::vector<int>>>>(t.getBlocks());
		int n = blocks->size();
		std::vector<std::vector<std::vector<int>>> rotated(n, std::vector<std::vector<int>>(n, std::vector<int>(n)));

		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				for (int k = 0; k < n; ++k) {
					rotated[i][j][k] = (*blocks)[j][n - 1 - i][k];
				}
			}
		}
		*blocks = rotated;
	}
	void displayCube(int x, int y, int z, int color) {
		glPushMatrix();
		glTranslatef(x, y, z);
		glColor3f(1, 0, 0);
		glutSolidCube(1);
		glPopMatrix();
	}


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
		// Enable lighting
	glEnable(GL_LIGHTING);

	// Set up light 0
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, whiteLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sourceLight);

	// Enable light 0
	glEnable(GL_LIGHT0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2, 0.2, 0.2, 1.0); //black background
	//glClearColor(1.0, 1.0, 1.0, 1.0); //white background
	
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