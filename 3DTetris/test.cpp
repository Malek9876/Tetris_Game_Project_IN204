#include <GL/gl.h>   // Open Graphics Library (OpenGL) header
#include <GL/glut.h> // The GL Utility Toolkit (GLUT) Header
#include <cmath>
#include <vector>
#include <iostream>

// Camera state variables
float cameraYaw = 0.0f;    // Rotation around the local Y-axis
float cameraPitch = 0.0f;  // Rotation around the local X-axis
float cameraDistance = 20.0f; // Distance from the target (zoom level)
float initialCameraYaw = 0.0f;
float initialCameraPitch = 0.0f;
float initialCameraDistance = 20.0f;

// Tetromino class definition
class tetromino {
public:
    // Tetromino shapes
    enum Shape { I, J, L, O, S, T, Z };

    // Constructor
    tetromino(Shape shape) : shape(shape), posY(15.5f), posX(0.5f), posZ(0.5f) {
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

    // Update the position of the tetromino
    void updatePosition(float fallSpeed) {
        posY -= fallSpeed;
        if (posY < 0.5f) posY = 0.5f; // Reset position if it goes below the grid
    }

    // Move the tetromino
    void move(float dx, float dy, float dz) {
        posX += dx;
        posY += dy;
        posZ += dz;
    }

    // Get the current Y position of the tetromino
    float getPositionY() const {
        return posY;
    }
    float getPositionX() const {
        return posX;
    }
    float getPositionZ() const {
        return posZ;
    }

private:
    Shape shape;
    std::vector<std::vector<std::vector<int>>> blocks;
    float posY;
    float posX;
    float posZ;
    // Initialize the blocks of the tetromino based on its shape
    void initializeShape() {
        switch (shape) {
            case I:
                blocks = {{{1}, {1}, {1}, {1}}};
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

// Function to display a Tetromino
void displayTetromino(const tetromino& t, int color) {
    const auto& blocks = t.getBlocks();
    float y = t.getPositionY();
    float x = t.getPositionX();
    float z = t.getPositionZ();
    // Set color based on the color parameter
    GLfloat r, g, b;
    switch (color) {
        case 0: r = 0xBA / 255.0f; g = 0x20 / 255.0f; b = 0x20 / 255.0f; break; // Red BA2020
        case 1: r = 0x62 / 255.0f; g = 0xC4 / 255.0f; b = 0x00 / 255.0f; break; // Green 62C400
        case 2: r = 0x12 / 255.0f; g = 0xAD / 255.0f; b = 0xD4 / 255.0f; break; // Blue 12ADD4
        case 3: r = 0xDE / 255.0f; g = 0xE9 / 255.0f; b = 0x3C / 255.0f; break; // Yellow DEE93C
        case 4: r = 0x9E / 255.0f; g = 0x13 / 255.0f; b = 0xE4 / 255.0f; break; // Purple 9E13E4
        case 5: r = 0x08 / 255.0f; g = 0x47 / 255.0f; b = 0xF3 / 255.0f; break; // Blue 0847F3
        default: r = 1; g = 1; b = 1; break; // White
    }

    // Draw each block of the Tetromino
    for (size_t i = 0; i < blocks.size(); ++i) {
        for (size_t j = 0; j < blocks[i].size(); ++j) {
            for (size_t k = 0; k < blocks[i][j].size(); ++k) {
                if (blocks[i][j][k] != 0) {
                    glPushMatrix();
                    glTranslatef(x + i, y + j, z + k);

                    // Set color for the solid cube
                    glColor3f(r, g, b);
                    glutSolidCube(1);

                    // Calculate darker shade for the wireframe
                    GLfloat darkR = r * 0.5f;
                    GLfloat darkG = g * 0.5f;
                    GLfloat darkB = b * 0.5f;

                    // Draw wireframe edges
                    glLineWidth(2.0f); // Set line width to 2.0
                    glColor3f(darkR, darkG, darkB); // Set edge color to darker shade
                    glutWireCube(1.01); // Slightly larger to avoid z-fighting

                    glPopMatrix();
                }
            }
        }
    }
}

// Global variables for mouse control
float angleX = 0.0f;
float angleY = 0.0f;
float zoom = 0.35f;
float panX = 0.0f;
float panY = 0.0f;
int lastMouseX = 0;
int lastMouseY = 0;
bool isDragging = false;
bool isPanning = false;
float accumulatedAngleX = 0.0f;
float accumulatedAngleY = 0.0f;

// Global variables for Tetrominoes and fall speed
std::vector<tetromino> tetrominoes;
float fallSpeed = 0.5f; // Speed at which the Tetrominoes fall
int fallInterval = 1000; // Time interval in milliseconds
size_t selectedTetromino = 0; // Index of the currently selected Tetromino

// Function to initialize OpenGL settings
void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // Black background
    glEnable(GL_DEPTH_TEST);          // Enable depth testing
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(10.0, 10.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

// Function to display the grid
void displayGrid() {
    int n = 15; // Grid length
    glBegin(GL_LINES);
    for (int i = 0; i <= n; i++) {
        // Set color (X-Z plane in red)
        glColor3f(1, 0, 0);
        glVertex3f(i, 0, 0);
        glVertex3f(i, 0, n);

        glVertex3f(0, 0, i);
        glVertex3f(n, 0, i);

        // Set color (Y-Z plane in green)
        glColor3f(0, 1, 0);
        glVertex3f(0, i, 0);
        glVertex3f(0, i, n);

        glVertex3f(0, 0, i);
        glVertex3f(0, n, i);

        // Set color (X-Y plane in cyan)
        glColor3f(0, 1, 1);
        glVertex3f(i, 0, 0);
        glVertex3f(i, n, 0);

        glVertex3f(0, i, 0);
        glVertex3f(n, i, 0);
    }
    glEnd();
}

// Display callback
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    glTranslatef(panX, panY, 0.0f); // Apply panning
    glScalef(zoom, zoom, zoom); // Apply zoom
    glRotatef(accumulatedAngleX, 1.0, 0.0, 0.0);
    glRotatef(accumulatedAngleY, 0.0, 1.0, 0.0);

    displayGrid();

    // Display all Tetrominoes
    for (size_t i = 0; i < tetrominoes.size(); ++i) {
        displayTetromino(tetrominoes[i], i % 6); // Display each Tetromino with a different color
    }

    glPopMatrix();

    glutSwapBuffers();
    glFlush();
}

// Function to check if the Tetromino is above the grid or another Tetromino
bool checkCollision(const tetromino& t) {
    if (t.getPositionY() <= 0.5f) {
        return true;
    }
    // Check collision with other Tetrominoes
    for (size_t i = 0; i < tetrominoes.size(); ++i) {
        if (i != selectedTetromino) {
            const auto& blocks = tetrominoes[i].getBlocks();
            float y = tetrominoes[i].getPositionY();
            float x = tetrominoes[i].getPositionX();
            float z = tetrominoes[i].getPositionZ();

            for (size_t bi = 0; bi < blocks.size(); ++bi) {
                for (size_t bj = 0; bj < blocks[bi].size(); ++bj) {
                    for (size_t bk = 0; bk < blocks[bi][bj].size(); ++bk) {
                        if (blocks[bi][bj][bk] != 0) {
                            if (std::abs(t.getPositionX() - (x + bi)) < 0.5f &&
                                std::abs(t.getPositionY() - (y + bj)) < 0.5f &&
                                std::abs(t.getPositionZ() - (z + bk)) < 0.5f) {
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

// Timer function to update the Tetrominoes' positions
void timer(int value) {
    if (selectedTetromino < tetrominoes.size()) {
        tetrominoes[selectedTetromino].updatePosition(fallSpeed); // Update the position of the selected Tetromino

        // Check if the Tetromino has reached the ground or landed on another Tetromino
        if (checkCollision(tetrominoes[selectedTetromino])) {
            // Freeze the Tetromino and create a new one
            selectedTetromino++;
            if (selectedTetromino < tetrominoes.size()) {
                tetrominoes.push_back(tetromino(static_cast<tetromino::Shape>(rand() % 7)));
            }
        }
    }

    glutPostRedisplay(); // Request a redraw
    glutTimerFunc(fallInterval, timer, 0); // Set the timer again
}

// Mouse button callback
void mouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isDragging = true;
            lastMouseX = x;
            lastMouseY = y;
        } else if (state == GLUT_UP) {
            isDragging = false;
        }
    } else if (button == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN) {
            isPanning = true;
            lastMouseX = x;
            lastMouseY = y;
        } else if (state == GLUT_UP) {
            isPanning = false;
        }
    }
}

// Mouse motion callback
void mouseMotion(int x, int y) {
    if (isDragging) {
        // Calculate mouse movement
        float deltaX = (x - lastMouseX) * 0.2f; // Sensitivity factor
        float deltaY = (y - lastMouseY) * 0.2f;

        // Update yaw and pitch
        accumulatedAngleY += deltaX;
        accumulatedAngleX += deltaY;

        // Clamp pitch to avoid flipping (optional)
        if (accumulatedAngleX > 89.0f) accumulatedAngleX = 89.0f;
        if (accumulatedAngleX < -89.0f) accumulatedAngleX = -89.0f;

        // Update last mouse position
        lastMouseX = x;
        lastMouseY = y;

        // Redraw the scene with the updated camera
        glutPostRedisplay();
    } else if (isPanning) {
        int dx = x - lastMouseX;
        int dy = y - lastMouseY;

        panX += dx * 0.05f;
        panY -= dy * 0.05f;

        lastMouseX = x;
        lastMouseY = y;

        glutPostRedisplay();
    }
}

// Mouse wheel callback (FreeGLUT-specific)
void mouseWheel(int button, int dir, int x, int y) {
    // Zoom in or out based on mouse wheel direction
    if (dir > 0) {
        zoom *= 1.1f; // Zoom in
    } else {
        zoom /= 1.1f; // Zoom out
    }
    glutPostRedisplay();
}

// Mouse function to handle both button and wheel events
void mouseFunc(int button, int state, int x, int y) {
    if (button == 3 || button == 4) { // Scroll up or down
        mouseWheel(button, (button == 3) ? 1 : -1, x, y);
    } else {
        mouseButton(button, state, x, y);
    }
}

// Keyboard callback to reset the camera view and control Tetromino
void keyboard(unsigned char key, int x, int y) {
    if (key == 'r' || key == 'R') {
        // Reset camera to initial view
        accumulatedAngleX = initialCameraPitch;
        accumulatedAngleY = initialCameraYaw;
        zoom = 0.35f;
        panX = 0.0f;
        panY = 0.0f;
        glutPostRedisplay();
    } else if (key == 'a' || key == 'A') {
        // Move selected Tetromino left
        tetrominoes[selectedTetromino].move(-1.0f, 0.0f, 0.0f);
        glutPostRedisplay();
    } else if (key == 'd' || key == 'D') {
        // Move selected Tetromino right
        tetrominoes[selectedTetromino].move(1.0f, 0.0f, 0.0f);
        glutPostRedisplay();
    } else if (key == 'w' || key == 'W') {
        // Move selected Tetromino up
        tetrominoes[selectedTetromino].move(0.0f, 0.0f, -1.0f);
        glutPostRedisplay();
    } else if (key == 's' || key == 'S') {
        // Move selected Tetromino backward
        tetrominoes[selectedTetromino].move(0.0f, 0.0f, 1.0f);
        glutPostRedisplay();
    } else if (key == 'q' || key == 'Q') {
        // Move selected Tetromino forward
        tetrominoes[selectedTetromino].move(0.0f, 0.0f, -1.0f);
        glutPostRedisplay();
    } else if (key == 'e' || key == 'E') {
        // Move selected Tetromino down
        if (!checkCollision(tetrominoes[selectedTetromino])) {
            tetrominoes[selectedTetromino].move(0.0f, -1.0f, 0.0f);
            glutPostRedisplay();
        }
    }
}

// Function to set up the camera view
void updateCamera() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Translate the camera back (zoom)
    glTranslatef(0.0f, 0.0f, -cameraDistance);

    // Apply pitch and yaw (dynamic rotation)
    glRotatef(cameraPitch, 1.0f, 0.0f, 0.0f); // Rotate around camera's X-axis
    glRotatef(cameraYaw, 0.0f, 1.0f, 0.0f);   // Rotate around camera's Y-axis
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Tetrominoes");

    init();

    // Register callbacks
    glutDisplayFunc(display);
    glutMouseFunc(mouseFunc);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(keyboard); // Register keyboard callback
    glutTimerFunc(fallInterval, timer, 0); // Register Timer Function

    // Initialize Tetrominoes
    tetrominoes.push_back(tetromino(tetromino::I));
    tetrominoes.push_back(tetromino(tetromino::J));
    tetrominoes.push_back(tetromino(tetromino::L));
    tetrominoes.push_back(tetromino(tetromino::O));
    tetrominoes.push_back(tetromino(tetromino::S));
    tetrominoes.push_back(tetromino(tetromino::T));
    tetrominoes.push_back(tetromino(tetromino::Z));

    glutMainLoop();
    return 0;
}
