#include <GL/gl.h>   // Open Graphics Library (OpenGL) header
#include <GL/glut.h> // The GL Utility Toolkit (GLUT) Header
#include <cmath>
#include <vector>
#include <iostream>
#include <cfloat>
#include <cctype>
#include <tuple>

int n = 10; // Grid length
float floor_pos = 0.0f;
float wall_pos=n;
bool isPaused = false;
// Camera state variables
float cameraYaw = 0.0f;    // Rotation around the local Y-axis
float cameraPitch = 0.0f;  // Rotation around the local X-axis
// float cameraDistance = 20.0f; // Distance from the target (zoom level)
float initialCameraYaw = 0.0f;
float initialCameraPitch = 0.0f;
float initialCameraDistance = 20.0f;
size_t selectedTetromino = 0; // Index of the currently selected Tetromino
float fallSpeed = 1.0f; // Speed at which the Tetrominoes fall
int fallInterval = 2500; // Time interval in milliseconds
// Global variables for mouse control
float angleX = 0.0f;
float angleY = 0.0f;
float zoom = 0.5f;
float panX = 0.0f;
float panY = 0.0f;
int lastMouseX = 0;
int lastMouseY = 0;
bool isDragging = false;
bool isPanning = false;

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
            
            if (newX < floor_pos || newX >= wall_pos ||
                newY < floor_pos || newY >= 2*wall_pos ||
                newZ < floor_pos || newZ >= wall_pos) {
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
            if(std::get<0>(pos) < floor_pos || std::get<0>(pos) >= wall_pos ||
               std::get<1>(pos) < floor_pos || std::get<1>(pos) >= 2*wall_pos ||
               std::get<2>(pos) < floor_pos || std::get<2>(pos) >= wall_pos) {
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
        //glutPostRedisplay();
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
        float initial_lower_Y = floor(4*n/3)+0.5 ;
        float initial_lower_X = floor(n/2) + 0.5;
        float initial_lower_Z =floor(n/2) + 0.5;
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


    void updateBlockPositionsAfterRotation(int axis, float angle) {
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

    // Convert angle to radians
    const float PI = 3.14159265359f;
    float rad = angle * PI / 180.0f;
    float cosA = cos(rad);
    float sinA = sin(rad);

    // Rotate each block around the center
    for(auto& pos : blockPositions) {
        // Get relative coordinates
        float x = std::get<0>(pos) - centerX;
        float y = std::get<1>(pos) - centerY;
        float z = std::get<2>(pos) - centerZ;

        // Apply rotation based on axis
        float newX = x, newY = y, newZ = z;
        
        switch(axis) {
            case 0: // X-axis rotation
                newY = y * cosA - z * sinA;
                newZ = y * sinA + z * cosA;
                std::get<1>(pos) = newY + centerY;
                std::get<2>(pos) = newZ + centerZ;
                break;

            case 1: // Y-axis rotation
                newX = x * cosA + z * sinA; 
                newZ = -x * sinA + z * cosA;
                std::get<0>(pos) = newX + centerX;
                std::get<2>(pos) = newZ + centerZ;
                break;

            case 2: // Z-axis rotation
                newX = x * cosA - y * sinA;
                newY = x * sinA + y * cosA;
                std::get<0>(pos) = newX + centerX;
                std::get<1>(pos) = newY + centerY;
                
                break;
        }
        std::cout<<"new rotation  x:"<<std::get<0>(pos)<<" y:"<<std::get<1>(pos)<<" z:"<<std::get<2>(pos)<<std::endl;
        //glutPostRedisplay();
    }
    std::cout<<"--------------------------"<<std::endl;
}
};


// Global variables for Tetrominoes and fall speed
std::vector<tetromino> tetrominoes;



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
        glutPostRedisplay();

    }
    
    glPopMatrix();
}






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
    glOrtho(-10, 10, -10, 10, 1, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(10.0, 10.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

// Function to display the grid
void displayGrid() {
    
    glBegin(GL_LINES);
    for (int i = 0; i <= n; i++) /*{
        // Set color (X-Z plane in red)
        glColor3f(1, 0, 0);
        glVertex3f(i, 0, 0);
        glVertex3f(i, 0, n);

        glVertex3f(0, 0, i);
        glVertex3f(n, 0, i);

        // Set color (Y-Z plane in green)
        glColor3f(0, 1, 0);
        glVertex3f(0, 2*i, 0);
        glVertex3f(0, 2*i, n);

        glVertex3f(0, 0, i);
        glVertex3f(0, 2*n, i);

        // Set color (X-Y plane in cyan)
        glColor3f(0, 1, 1);
        glVertex3f(i, 0, 0);
        glVertex3f(i, 2*n, 0);

        glVertex3f(0, 2*i, 0);
        glVertex3f(n, 2*i, 0);
    }*/
        {
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

        glVertex3f(0, n+i, 0);
        glVertex3f(0, n+i, n);

        glVertex3f(0, 0, i);
        glVertex3f(0, 2*n, i);

        glColor3f(0, 1, 1);
        // x-y axis
        glVertex3f(i, 0, 0);
        glVertex3f(i, 2*n, 0);

        glVertex3f(0, i, 0);
        glVertex3f(n, i, 0);

        glVertex3f(0, n+i, 0);
        glVertex3f(n, n+i, 0);
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
                    if(std::abs(std::get<0>(pos) - std::get<0>(otherPos)) < 0.5f &&
                        std::abs(std::get<1>(pos) - std::get<1>(otherPos)) <1.5f  &&
                       std::abs(std::get<2>(pos) - std::get<2>(otherPos)) < 0.5f) {
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
    if (!isPaused) {
    if (selectedTetromino < tetrominoes.size()) {
        tetromino& current = tetrominoes[selectedTetromino];
        //float oldY = current.getPositionY();
        
        for (auto& pos : current.getBlockPositions()) {
            std::cout << "X: " << std::get<0>(pos) << " Y: " << std::get<1>(pos) << " Z: " << std::get<2>(pos) << std::endl;
        }
        std::cout<<"==============================="<<std::endl;
        // Preview movement
        // //current.move(0, -0.1f, 0);
        
        if (checkCollision(current)) {
            // current.move(0, 0.1f, 0);  // Restore position
            selectedTetromino++;
            tetrominoes.push_back(tetromino(static_cast<tetromino::Shape>(rand() % 5)));
        }
        else {
            current.move(0, -1.0f, 0);  // Complete the movement
        }
    }
    
    //glutPostRedisplay();
    glutTimerFunc(fallInterval, timer, 0);
    }
    else {
    glutTimerFunc(fallInterval, timer, 0);
    }
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
        //if (cameraPitch > 89.0f) cameraPitch = 89.0f;
        //if (cameraPitch < -89.0f) cameraPitch = -89.0f;

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
    key = toupper(key); // Convert key to uppercase for uniform handling

    switch (key) {
        case 'P':
            // Pause or resume the game
            isPaused = !isPaused;
            if (isPaused) {
                std::cout << "Game paused." << std::endl;
            } else {
                std::cout << "Game resumed." << std::endl;
            }
            break;    
        case 'X':
            // Reset camera to initial view
            cameraPitch = initialCameraPitch;
            cameraYaw = initialCameraYaw;
            zoom = 0.35f;
            panX = 0.0f;
            panY = 0.0f;
            //glutPostRedisplay();
            break;

        case 'A':
            // Move selected Tetromino left
            tetrominoes[selectedTetromino].move(-1.0f, 0.0f, 0.0f);
            //glutPostRedisplay();
            break;

        case 'D':
            // Move selected Tetromino right
            tetrominoes[selectedTetromino].move(1.0f, 0.0f, 0.0f);
            //glutPostRedisplay();
            break;

        case 'W':
            // Move selected Tetromino up
            tetrominoes[selectedTetromino].move(0.0f, 0.0f, -1.0f);
            //glutPostRedisplay();
            break;

        case 'S':
            // Move selected Tetromino backward
            tetrominoes[selectedTetromino].move(0.0f, 0.0f, 1.0f);
            //glutPostRedisplay();
            break;

        case 'Q':
            // Move selected Tetromino down
            if (!checkCollision(tetrominoes[selectedTetromino])) {
                tetrominoes[selectedTetromino].move(0.0f, -1.0f, 0.0f);
                //glutPostRedisplay();
            }
            break;

        case 'E':
            // Rotate the selected Tetromino around the X-axis
            rotateTetromino(0);
            //glutPostRedisplay();
            break;

        case 'R':
            // Rotate the selected Tetromino around the Y-axis
            rotateTetromino(1);
            //glutPostRedisplay();
            break;

        case 'F':
            // Rotate the selected Tetromino around the Z-axis
            rotateTetromino(2);
            //glutPostRedisplay();
            break;

        default:
            // No action for other keys
            break;
    }
    glutPostRedisplay();
    
   
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