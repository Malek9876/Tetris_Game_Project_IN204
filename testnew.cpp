#include <GL/gl.h>   // Open Graphics Library (OpenGL) header
#include <GL/glut.h> // The GL Utility Toolkit (GLUT) Header
#include <cmath>
#include <vector>
#include <iostream>
#include <cfloat>

// Camera state variables
float cameraYaw = 0.0f;    // Rotation around the local Y-axis
float cameraPitch = 0.0f;  // Rotation around the local X-axis
// float cameraDistance = 20.0f; // Distance from the target (zoom level)
float initialCameraYaw = 0.0f;
float initialCameraPitch = 0.0f;
float initialCameraDistance = 20.0f;
size_t selectedTetromino = 0; // Index of the currently selected Tetromino

// Tetromino class definition
class tetromino {
public:
    // Tetromino shapes
    enum Shape { I, J, O, S, T };
   
    // Constructor
    tetromino(Shape shape) : shape(shape),rotationX(0.0f), rotationY(0.0f), rotationZ(0.0f) {
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
        for (auto& pos : blockPositions) {
            std::get<1>(pos) -= fallSpeed;
            // if (std::get<1>(pos) <= 1.0f) {
            //     std::get<1>(pos) = 1.0f; // Reset position if it goes below the grid
            // }
        }
    }
    const std::vector<std::tuple<float, float, float>>& getBlockPositions() const {
        return blockPositions;
    }
    // Update move function to work with blockPositions
    void move(float dx, float dy, float dz) {
        // Check bounds before moving
        bool canMove = true;
        for(auto& pos : blockPositions) {
            float newX = std::get<0>(pos) + dx;
            float newY = std::get<1>(pos) + dy;
            float newZ = std::get<2>(pos) + dz;
            
            if (newX < 0 || newX >= 15 ||
                newY < 0 || newY >= 15 ||
                newZ < 0 || newZ >= 15) {
                canMove = false;
                break;
            }
        }

        if (canMove) {
            for(auto& pos : blockPositions) {
                std::get<0>(pos) += dx;
                std::get<1>(pos) += dy;
                std::get<2>(pos) += dz;
            }
        }
    }

    // Get the current Y position of the tetromino
    float getPositionY() const {
        return std::get<1>(blockPositions[0]);
    }
    float getPositionX() const {
        return std::get<0>(blockPositions[0]);
    }
    float getPositionZ() const {
        return std::get<2>(blockPositions[0]);
    }
    float getRotationX() const {
        return rotationX;
    }
    void setRotationX(float value) {
        rotationX = value;
    }
    float getRotationY() const {
        return rotationY;
    }
    void setRotationY(float value) {
        rotationY = value;
    }
    float getRotationZ() const {
        return rotationZ;
    }
    void setRotationZ(float value) {
        rotationZ = value;
    }

    // Update rotateTetromino method 
    void rotate(int axis, float angle) {
        // Store original positions in case rotation is invalid
        auto originalPositions = blockPositions;
        
        // Update rotation angles
        switch(axis) {
            case 0: rotationX += angle; break;
            case 1: rotationY += angle; break;
            case 2: rotationZ += angle; break;
        }

        // Update actual block positions
        updateBlockPositionsAfterRotation(axis, angle);

        // Check if rotation caused invalid position
        bool isValid = true;
        for(const auto& pos : blockPositions) {
            if(std::get<0>(pos) < 0 || std::get<0>(pos) >= 15 ||
               std::get<1>(pos) < 0 || std::get<1>(pos) >= 15 ||
               std::get<2>(pos) < 0 || std::get<2>(pos) >= 15) {
                isValid = false;
                break;
            }
        }

        // Revert if invalid
        if(!isValid) {
            blockPositions = originalPositions;
            switch(axis) {
                case 0: rotationX -= angle; break;
                case 1: rotationY -= angle; break;
                case 2: rotationZ -= angle; break;
            }
        }
    }

private:
    Shape shape;
    std::vector<std::vector<std::vector<int>>> blocks;
    std::vector<std::tuple<float, float, float>> blockPositions;
    float rotationX ;
    float rotationY ;
    float rotationZ ;


    // Initialize the blocks of the tetromino based on its shape
    void initializeShape() {
        float initial_lower_Y = 11.5f;
        float initial_lower_X = 4.5f;
        float initial_lower_Z = 4.5f;
        switch (shape) {
            case I:
                blocks = {{{1}, {1}, {1}, {1}}};
                blockPositions = {std::make_tuple(initial_lower_X, initial_lower_Y, initial_lower_Z), std::make_tuple(initial_lower_X, initial_lower_Y + 1, initial_lower_Z), std::make_tuple(initial_lower_X, initial_lower_Y + 2, initial_lower_Z), std::make_tuple(initial_lower_X, initial_lower_Y + 3, initial_lower_Z)};
                break;
            case J:
                blocks = {{{1, 0, 0}, {1, 1, 1}}};
                blockPositions = {std::make_tuple(initial_lower_X, initial_lower_Y, initial_lower_Z), std::make_tuple(initial_lower_X, initial_lower_Y + 1, initial_lower_Z), std::make_tuple(initial_lower_X, initial_lower_Y + 1, initial_lower_Z + 1), std::make_tuple(initial_lower_X, initial_lower_Y + 1, initial_lower_Z + 2)};
                break;
            case O:
                blocks = {{{1, 1}, {1, 1}}};
                blockPositions = {std::make_tuple(initial_lower_X, initial_lower_Y, initial_lower_Z), std::make_tuple(initial_lower_X, initial_lower_Y, initial_lower_Z + 1), std::make_tuple(initial_lower_X, initial_lower_Y + 1, initial_lower_Z), std::make_tuple(initial_lower_X, initial_lower_Y + 1, initial_lower_Z + 1)};
                break;
            case S:
                blocks = {{{0, 1, 1}, {1, 1, 0}}};
                blockPositions = {std::make_tuple(initial_lower_X, initial_lower_Y, initial_lower_Z), std::make_tuple(initial_lower_X, initial_lower_Y, initial_lower_Z + 1), std::make_tuple(initial_lower_X, initial_lower_Y + 1, initial_lower_Z - 1), std::make_tuple(initial_lower_X, initial_lower_Y + 1, initial_lower_Z)};
                break;
            case T:
                blocks = {{{0, 1, 0}, {1, 1, 1}}};
                blockPositions = {std::make_tuple(initial_lower_X, initial_lower_Y, initial_lower_Z), std::make_tuple(initial_lower_X, initial_lower_Y + 1, initial_lower_Z - 1), std::make_tuple(initial_lower_X, initial_lower_Y + 1, initial_lower_Z), std::make_tuple(initial_lower_X, initial_lower_Y + 1, initial_lower_Z + 1)};
                break;
        }
    }

    void rotatePoint(float& x, float& y, float& z, float angle, int axis) {
        const float PI = 3.14159265359f;
        float rad = angle * PI / 180.0f;
        float cosA = cos(rad);
        float sinA = sin(rad);
        float tempX, tempY, tempZ;

        switch(axis) {
            case 0: // X axis
                tempY = y;
                tempZ = z;
                y = tempY * cosA - tempZ * sinA;
                z = tempY * sinA + tempZ * cosA;
                break;
            case 1: // Y axis
                tempX = x;
                tempZ = z;
                x = tempX * cosA + tempZ * sinA;
                z = -tempX * sinA + tempZ * cosA;
                break;
            case 2: // Z axis
                tempX = x;
                tempY = y;
                x = tempX * cosA - tempY * sinA;
                y = tempX * sinA + tempY * cosA;
                break;
        }
    }

    void updateBlockPositionsAfterRotation(int axis, float angle) {
        // Calculate center of rotation
        float centerX = 0, centerY = 0, centerZ = 0;
        for(const auto& pos : blockPositions) {
            centerX += std::get<0>(pos);
            centerY += std::get<1>(pos);
            centerZ += std::get<2>(pos);
        }
        centerX /= blockPositions.size();
        centerY /= blockPositions.size();
        centerZ /= blockPositions.size();

        // Rotate each block around center
        for(auto& pos : blockPositions) {
            float x = std::get<0>(pos) - centerX;
            float y = std::get<1>(pos) - centerY; 
            float z = std::get<2>(pos) - centerZ;

            rotatePoint(x, y, z, angle, axis);

            std::get<0>(pos) = x + centerX;
            std::get<1>(pos) = y + centerY;
            std::get<2>(pos) = z + centerZ;
        }
    }
};


// Function to display a Tetromino
void displayTetromino(const tetromino& t, int color) {
    const auto& blockPositions = t.getBlockPositions();
    
    // Calculate center of the tetromino
    float centerX = 0, centerY = 0, centerZ = 0;
    for(const auto& pos : blockPositions) {
        centerX += std::get<0>(pos);
        centerY += std::get<1>(pos);
        centerZ += std::get<2>(pos);
    }
    centerX /= blockPositions.size();
    centerY /= blockPositions.size();
    centerZ /= blockPositions.size();

    // Set color
    GLfloat r, g, b;
    switch (color) {
        case 0: r = 0xBA/255.0f; g = 0x20/255.0f; b = 0x20/255.0f; break;
        case 1: r = 0x62 / 255.0f; g = 0xC4 / 255.0f; b = 0x00 / 255.0f; break; // Green 62C400
        case 2: r = 0x12 / 255.0f; g = 0xAD / 255.0f; b = 0xD4 / 255.0f; break; // Blue 12ADD4
        case 3: r = 0xDE / 255.0f; g = 0xE9 / 255.0f; b = 0x3C / 255.0f; break; // Yellow DEE93C
        case 4: r = 0x9E / 255.0f; g = 0x13 / 255.0f; b = 0xE4 / 255.0f; break; // Purple 9E13E4
        case 5: r = 0x08 / 255.0f; g = 0x47 / 255.0f; b = 0xF3 / 255.0f; break; // Blue 0847F3
        default: r = 1; g = 1; b = 1; break; // White
    }

    glPushMatrix();
    glTranslatef(centerX, centerY, centerZ);
    
    // Apply rotations
    glRotatef(t.getRotationX(), 1.0f, 0.0f, 0.0f);
    glRotatef(t.getRotationY(), 0.0f, 1.0f, 0.0f);
    glRotatef(t.getRotationZ(), 0.0f, 0.0f, 1.0f);

    // Draw blocks
    for(const auto& pos : blockPositions) {
        glPushMatrix();
        glTranslatef(std::get<0>(pos) - centerX, 
                     std::get<1>(pos) - centerY, 
                     std::get<2>(pos) - centerZ);
        
        glColor3f(r, g, b);
        glutSolidCube(1);

        GLfloat darkR = r * 0.5f;
        GLfloat darkG = g * 0.5f;
        GLfloat darkB = b * 0.5f;
        
        glLineWidth(2.0f);
        glColor3f(darkR, darkG, darkB);
        glutWireCube(1.01);
        
        glPopMatrix();
    }
    
    glPopMatrix();
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


// Global variables for Tetrominoes and fall speed
std::vector<tetromino> tetrominoes;
float fallSpeed = 1.0f; // Speed at which the Tetrominoes fall
int fallInterval = 2000; // Time interval in milliseconds

void rotateTetromino(int axis) {
    if (selectedTetromino >= tetrominoes.size()) return;
   
    // Rotate 90 degrees each time
    const float ROTATION_ANGLE = 90.0f;
   
    tetrominoes[selectedTetromino].rotate(axis, ROTATION_ANGLE);
}
// Function to initialize OpenGL settings
void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // Black background
    glEnable(GL_DEPTH_TEST);          // Enable depth testing
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-10, 10, -10, 10, 1, 100);
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

// Function to set up the camera view
void updateCamera() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    glTranslatef(panX, panY, 0.00f); // Apply panning
    glScalef(zoom, zoom, zoom); // Apply zoom
    // glRotatef(cameraPitch, 0.0f, 0.0f, 1.0f); // Rotate around camera's X-axis
    glRotatef(cameraYaw, 0.0f, 1.0f, 0.0f);   // Rotate around camera's Y-axis

}

// Display callback
void display() {
   
    updateCamera(); // Update the camera view

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
    const auto& positions = t.getBlockPositions();
    
    // Check floor collision
    for(const auto& pos : positions) {
        if(std::get<1>(pos) <= 1.0f) return true;
    }

    // Check collision with other tetrominoes
    for(size_t i = 0; i < tetrominoes.size(); i++) {
        if(&tetrominoes[i] != &t) {
            const auto& otherPositions = tetrominoes[i].getBlockPositions();
            for(const auto& pos : positions) {
                for(const auto& otherPos : otherPositions) {
                    if(std::abs(std::get<0>(pos) - std::get<0>(otherPos)) < 0.1f &&
                       std::abs(std::get<1>(pos) - std::get<1>(otherPos)) < 0.1f &&
                       std::abs(std::get<2>(pos) - std::get<2>(otherPos)) < 0.1f) {
                        return true;
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
        tetromino& current = tetrominoes[selectedTetromino];
        float oldY = current.getPositionY();
        
        for (auto& pos : current.getBlockPositions()) {
            std::cout << "X: " << std::get<0>(pos) << " Y: " << std::get<1>(pos) << " Z: " << std::get<2>(pos) << std::endl;
            
        }
        // Preview movement
        current.move(0, -0.1f, 0);
        
        if (checkCollision(current)) {
            current.move(0, 0.1f, 0);  // Restore position
            selectedTetromino++;
            tetrominoes.push_back(tetromino(static_cast<tetromino::Shape>(rand() % 6)));
        }
        else {
            current.move(0, -0.9f, 0);  // Complete the movement
        }
    }
    
    glutPostRedisplay();
    glutTimerFunc(fallInterval, timer, 0);
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
    if (key == 'x' || key == 'X') {
        // Reset camera to initial view
        cameraPitch = initialCameraPitch;
        cameraYaw = initialCameraYaw;
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
        // Move selected Tetromino down
        if (!checkCollision(tetrominoes[selectedTetromino])) {
            tetrominoes[selectedTetromino].move(0.0f, -1.0f, 0.0f);
            glutPostRedisplay();
        }
    } else if (key == 'e' || key == 'E') {
        // Rotate the selected Tetromino around the X-axis
        rotateTetromino(0);
        glutPostRedisplay();
    } else if (key == 'r' || key == 'R') {
        // Rotate the selected Tetromino around the Y-axis
        rotateTetromino(1);
        glutPostRedisplay();
    } else if (key == 'f' || key == 'F') {
        // Rotate the selected Tetromino around the Z-axis
        rotateTetromino(2);

        glutPostRedisplay();
    }
}


// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1920, 1080);
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
    // tetrominoes.push_back(tetromino(tetromino::I));
    // tetrominoes.push_back(tetromino(tetromino::I));
    // tetrominoes.push_back(tetromino(tetromino::I));
    // tetrominoes.push_back(tetromino(tetromino::I));
    // tetrominoes.push_back(tetromino(tetromino::I));
    // tetrominoes.push_back(tetromino(tetromino::I));
    
    // tetrominoes.push_back(tetromino(tetromino::J));
    // tetrominoes.push_back(tetromino(tetromino::L));
    // tetrominoes.push_back(tetromino(tetromino::O));
    // tetrominoes.push_back(tetromino(tetromino::S));
    // tetrominoes.push_back(tetromino(tetromino::T));
    // tetrominoes.push_back(tetromino(tetromino::Z));

    glutMainLoop();
    return 0;
}