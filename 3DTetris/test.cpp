#include <GL/gl.h>   // Open Graphics Library (OpenGL) header
#include <GL/glut.h> // The GL Utility Toolkit (GLUT) Header
#include <cmath>
#include<vector>
#include<array>
#include<iostream>
// Camera state variables
float cameraYaw = 0.0f;    // Rotation around the local Y-axis
float cameraPitch = 0.0f;  // Rotation around the local X-axis
float cameraDistance = 20.0f; // Distance from the target (zoom level)

// Mouse state variables
int lastMouseX, lastMouseY; // To track the last mouse position
bool isDragging = false;    // Whether the mouse is being dragged

// Mouse wheel callback (FreeGLUT-specific)
void mouseWheel(int button, int dir, int x, int y) {
    // Zoom in or out based on mouse wheel direction
    if (dir > 0) {
        cameraDistance *= 0.9f; // Zoom in (reduce distance)
    } else {
        cameraDistance *= 1.1f; // Zoom out (increase distance)
    }

    // Clamp camera distance to prevent extreme zooming
    if (cameraDistance < 2.0f) cameraDistance = 2.0f;    // Minimum zoom distance
    if (cameraDistance > 100.0f) cameraDistance = 100.0f; // Maximum zoom distance

    glutPostRedisplay(); // Request a redraw
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
    }
}

// Mouse motion callback
void mouseMotion(int x, int y) {
    if (isDragging) {
        // Calculate mouse movement
        float deltaX = (x - lastMouseX) * 0.2f; // Sensitivity factor
        float deltaY = (y - lastMouseY) * 0.2f;

        // Update yaw and pitch
        cameraYaw += deltaX;
        cameraPitch += deltaY;

        // Clamp pitch to avoid flipping (optional)
        if (cameraPitch > 89.0f) cameraPitch = 89.0f;
        if (cameraPitch < -89.0f) cameraPitch = -89.0f;

        // Update last mouse position
        lastMouseX = x;
        lastMouseY = y;

        // Redraw the scene with the updated camera
        glutPostRedisplay();
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

// Initialization function
void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // Black background
    glEnable(GL_DEPTH_TEST);          // Enable depth testing
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
class Tetromino {
public:
    enum Shape { I, J, L, O, S, T, Z };

    Tetromino(Shape shape) : shape(shape), position{5.0f, 0.0f, 6.5f} {
        initializeShape();
    }

    Shape getShape() const { return shape; }

    const std::vector<std::vector<std::vector<int>>>& getBlocks() const { return blocks; }

    void updatePosition(float dx, float dy, float dz) {
        position[0] += dx;
        position[1] += dy;
        position[2] += dz;
    }

    std::array<float, 3> getPosition() const { return position; }

    void rotateX() { rotateShape('X'); } // Rotate around X-axis
    void rotateY() { rotateShape('Y'); } // Rotate around Y-axis
    void rotateZ() { rotateShape('Z'); } // Rotate around Z-axis
     float getPositionY() const {
        return position[1];
    }
    float getPositionX() const {
        return position[0];
    }
    float getPositionZ() const {
        return position[2];
    }

private:
    Shape shape;
    std::vector<std::vector<std::vector<int>>> blocks; // 3D representation of Tetromino
    std::array<float, 3> position;                      // {x, y, z}

    void initializeShape() {
        switch (shape) {
            case I: blocks = {{{1}, {1}, {1}, {1}}}; break; // 3D I shape
            case J: blocks = {{{1, 0, 0}, {1, 1, 1}}}; break; // 3D J shape
            case L: blocks = {{{0, 0, 1}, {1, 1, 1}}}; break; // 3D L shape
            case O: blocks = {{{1, 1}, {1, 1}}}; break; // 3D O shape
            case S: blocks = {{{0, 1, 1}, {1, 1, 0}}}; break; // 3D S shape
            case T: blocks = {{{0, 1, 0}, {1, 1, 1}}}; break; // 3D T shape
            case Z: blocks = {{{1, 1, 0}, {0, 1, 1}}}; break; // 3D Z shape
        }
    }

    void rotateShape(char axis) {
        if (axis == 'X') {
            // Rotate around X-axis
            for (size_t i = 0; i < blocks.size(); i++) {
                for (size_t j = 0; j < blocks[i].size(); j++) {
                    for (size_t k = 0; k < blocks[i][j].size(); k++) {
                        // Apply rotation formula for X-axis (Transpose)
                        blocks[i][j][k] = blocks[i][j][k]; // Adjust accordingly for rotation logic
                    }
                }
            }
        } else if (axis == 'Y') {
            // Rotate around Y-axis
            for (size_t i = 0; i < blocks.size(); i++) {
                for (size_t j = 0; j < blocks[i].size(); j++) {
                    for (size_t k = 0; k < blocks[i][j].size(); k++) {
                        // Apply rotation formula for Y-axis
                        blocks[i][j][k] = blocks[i][j][k]; // Adjust accordingly for rotation logic
                    }
                }
            }
        } else if (axis == 'Z') {
            // Rotate around Z-axis
            for (size_t i = 0; i < blocks.size(); i++) {
                for (size_t j = 0; j < blocks[i].size(); j++) {
                    for (size_t k = 0; k < blocks[i][j].size(); k++) {
                        // Apply rotation formula for Z-axis
                        blocks[i][j][k] = blocks[i][j][k]; // Adjust accordingly for rotation logic
                    }
                }
            }
        }
    }
    void displayTetromino(const Tetromino& t, int color) {
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
};
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
void testTetromino() {
    // Create a Tetromino (for example, a "T" shape)
    Tetromino tetromino(Tetromino::T);

    // Move the Tetromino
    tetromino.updatePosition(1.0f, 0.0f, 0.0f);

    // Rotate the Tetromino
    tetromino.rotateZ(); // Rotate by Z axis

    // Output the current position of the Tetromino for debugging
    auto pos = tetromino.getPosition();
    std::cout << "Tetromino Position: (" 
              << pos[0] << ", " << pos[1] << ", " << pos[2] << ")\n";

    // Display the blocks of the tetromino (for debugging purposes)
    for (const auto& layer : tetromino.getBlocks()) {
        for (const auto& row : layer) {
            for (int cell : row) {
                std::cout << cell << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}
// Display callback
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    testTetromino(); // Call the test function to display Tetromino

    // Update the camera
    updateCamera();

    // Draw the grid
    displayGrid();

    glutSwapBuffers();
}



// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Camera with Zoom");

    init();

    // Register callbacks
    glutDisplayFunc(display);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMotion);

    // Use FreeGLUT's mouse wheel support if available
    glutMouseFunc([](int button, int state, int x, int y) {
        if (button == 3) { // Scroll up
            mouseWheel(button, 1, x, y);
        } else if (button == 4) { // Scroll down
            mouseWheel(button, -1, x, y);
        } else {
            mouseButton(button, state, x, y);
        }
    });

    glutMainLoop();
    return 0;
}
