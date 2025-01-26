#include <GL/gl.h>   // Open Graphics Library (OpenGL) header
#include <GL/glut.h> // The GL Utility Toolkit (GLUT) Header
#include <cmath>
#include <vector>
#include <iostream>
#define BOARD_HEIGHT 15
#define BOARD_WIDTH_DEPTH 15
using BLOCKS = std::vector<std::vector<std::vector<int>>>;
// Camera state variables
float cameraYaw = 0.0f;    // Rotation around the local Y-axis
float cameraPitch = 0.0f;  // Rotation around the local X-axis
// float cameraDistance = 20.0f; // Distance from the target (zoom level)
float initialCameraYaw = 0.0f;
float initialCameraPitch = 0.0f;
float initialCameraDistance = 20.0f;
size_t selectedTetromino = 0; // Index of the currently selected Tetromino
class GameBoard {
private:
size_t GRID_SIZE =4;
    //static const int BOARD_WIDTH_DEPTH = 10;
    //static const int BOARD_HEIGHT = 20;
    bool occupiedPositions[BOARD_WIDTH_DEPTH][BOARD_HEIGHT][BOARD_WIDTH_DEPTH];

public:
    GameBoard() {
        clearBoard();
    }

    void clearBoard() {
        for(int x = 0; x < BOARD_WIDTH_DEPTH; x++) {
            for(int y = 0; y < BOARD_HEIGHT; y++) {
                for(int z = 0; z < BOARD_WIDTH_DEPTH; z++) {
                    occupiedPositions[x][y][z] = false;
                }
            }
        }
    }

    void clearLayer(int y) {
        // Move all layers above down
        for(int currentY = y; currentY < BOARD_HEIGHT - 1; currentY++) {
            for(int x = 0; x < BOARD_WIDTH_DEPTH; x++) {
                for(int z = 0; z < BOARD_WIDTH_DEPTH; z++) {
                    occupiedPositions[x][currentY][z] = occupiedPositions[x][currentY + 1][z];
                }
            }
        }
        
        // Clear top layer
        for(int x = 0; x < BOARD_WIDTH_DEPTH; x++) {
            for(int z = 0; z < BOARD_WIDTH_DEPTH; z++) {
                occupiedPositions[x][BOARD_HEIGHT - 1][z] = false;
            }
        }
    }

    bool isLayerFull(int y) {
        for(int x = 0; x < BOARD_WIDTH_DEPTH; x++) {
            for(int z = 0; z < BOARD_WIDTH_DEPTH; z++) {
                if(!occupiedPositions[x][y][z]) return false;
            }
        }
        return true;
    }

    bool isPositionOccupied(int x, int y, int z) const {
        return occupiedPositions[x][y][z];
    }

    bool isPositionValid(int x, int y, int z) const {
        return x >= 0 && x < BOARD_WIDTH_DEPTH &&
               y >= 0 && y < BOARD_HEIGHT &&
               z >= 0 && z < BOARD_WIDTH_DEPTH &&
               !occupiedPositions[x][y][z];
    }

    void occupyPosition(int x, int y, int z) {
        if(isPositionValid(x, y, z)) {
            occupiedPositions[x][y][z] = true;
        }
    }

    void releasePosition(int x, int y, int z) {
        if(x >= 0 && x < BOARD_WIDTH_DEPTH &&
           y >= 0 && y < BOARD_HEIGHT &&
           z >= 0 && z < BOARD_WIDTH_DEPTH) {
            occupiedPositions[x][y][z] = false;
        }
    }

    bool checkAndClearFullLayers() {
        bool layerCleared = false;
        for(int y = 0; y < BOARD_HEIGHT; y++) {
            if(isLayerFull(y)) {
                clearLayer(y);
                layerCleared = true;
                // Stay on same layer as everything moved down
                y--;
            }
        }
        return layerCleared;
    }

    bool isGameOver() const {
        // Check top layer for any occupied positions
        for(int x = 0; x < BOARD_WIDTH_DEPTH; x++) {
            for(int z = 0; z < BOARD_WIDTH_DEPTH; z++) {
                if(occupiedPositions[x][BOARD_HEIGHT-1][z]) {
                    return true;
                }
            }
        }
        return false;
    }

    void lockTetromino(const float posX,const float posY,const float posZ,BLOCKS blocks) {
        for(size_t i = 0; i < blocks.size(); ++i) {
            for(size_t j = 0; j < blocks[i].size(); ++j) {
                for(size_t k = 0; k < blocks[i][j].size(); ++k) {
                    if(blocks[i][j][k] != 0) {
                        int x = static_cast<int>(posX + i);
                        int y = static_cast<int>(posY + j);
                        int z = static_cast<int>(posZ + k);
                        if(isPositionValid(x, y, z)) {
                            occupyPosition(x, y, z);
                        }
                    }
                }
            }
        }
        checkAndClearFullLayers();
    }

    // bool isValidPosition(int posX,int posY,int posZ,BLOCKS blocks) const {
        

    //     for(int i = 0; i < GRID_SIZE; i++) {
    //         for(int j = 0; j < GRID_SIZE; j++) {
    //             for(int k = 0; k < GRID_SIZE; k++) {
    //                 if(blocks[i][j][k] != 0) {
    //                     int worldX = posX + i;
    //                     int worldY = posY + j;
    //                     int worldZ = posZ + k;
                        
    //                     // Check bounds and collisions
    //                     if(worldX < 0 || worldX >= BOARD_WIDTH_DEPTH ||
    //                        worldY < 0 || worldY >= BOARD_HEIGHT ||
    //                        worldZ < 0 || worldZ >= BOARD_WIDTH_DEPTH ||
    //                        occupiedPositions[worldX][worldY][worldZ]) {
    //                         return false;
    //                     }
    //                 }
    //             }
    //         }
    //     }
    //     return true;
    // }
};

GameBoard gameboard;


class Tetromino {
public:
    // Tetromino shapes
    enum Shape { I, J, L, O, S, T, Z };
    
    Tetromino(Shape shape) : 
        shape(shape),posX(0), posY(BOARD_HEIGHT-1), posZ(0),
        dx(0), dy(0), dz(0) {
        blocks = std::vector<std::vector<std::vector<int>>>(
            GRID_SIZE, 
            std::vector<std::vector<int>>(
                GRID_SIZE, 
                std::vector<int>(GRID_SIZE, 0)
            )
        );
        initializeShape();
    }
    // Get the shape of the Tetromino
    Shape getShape() const {
        return shape;
    }
    
    // Get the blocks of the Tetromino
    const std::vector<std::vector<std::vector<int>>>& getBlocks() const {
        return blocks;
    }

    // Update the position of the Tetromino
    void updatePosition(float fallSpeed) {
        posY -= fallSpeed;
        if (posY <= 0.25f) posY = 0.25f; // Reset position if it goes below the grid
    }

    // Move the Tetromino
    void move(float dx, float dy, float dz) {
        float newX = posX + dx;
        float newY = posY + dy;
        float newZ = posZ + dz;
       
        // Add bounds checking
        if (newX >= 0 && newX < BOARD_WIDTH_DEPTH - blocks.size() &&
            newY >= 0 && newY < BOARD_HEIGHT - blocks[0].size() &&
            newZ >= 0 && newZ < BOARD_WIDTH_DEPTH - blocks[0][0].size()) {
            posX = newX;
            posY = newY;
            posZ = newZ;
        }
    }

    // Get the current Y position of the Tetromino
    float getPositionY() const {
        return posY;
    }
    float getPositionX() const {
        return posX;
    }
    float getPositionZ() const {
        return posZ;
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
    

    void rotateLocal(int axis) {
        // Store original orientation
        int oldDx = dx;
        int oldDy = dy;
        int oldDz = dz;
        std::cout << "dx: " << dx << " dy: " << dy << " dz: " << dz << std::endl;

        switch(axis) {
            case 0: // X axis rotation
                dy = (dy + 1) % 4;
                dz = (dz + 3) % 4;
                break;
            case 1: // Y axis rotation
                dx = (dx + 1) % 4;
                dz = (dz + 3) % 4;
                break;
            case 2: // Z axis rotation
                dx = (dx + 1) % 4;
                dy = (dy + 3) % 4;
                break;
        }
        std::cout << "dx: " << dx << " dy: " << dy << " dz: " << dz << std::endl;
        
        // Rotate blocks in 4x4x4 grid
        BLOCKS temp = blocks;
        
        try {
        // Iterate through 3D grid
        for(int i = 0; i < GRID_SIZE; i++) {
            for(int j = 0; j < GRID_SIZE; j++) {
                for(int k = 0; k < GRID_SIZE; k++) {
                    // Bounds checking
                    if (i >= GRID_SIZE || j >= GRID_SIZE || k >= GRID_SIZE) {
                        continue;
                    }
                    
                    switch(axis) {
                        case 0: // X-axis rotation
                            // (x,y,z) -> (x,-z,y)
                            if (j < GRID_SIZE && k < GRID_SIZE) {
                                blocks[i][GRID_SIZE-1-k][j] = temp[i][j][k];
                            }
                            break;
                            
                        case 1: // Y-axis rotation
                            // (x,y,z) -> (z,y,-x)
                            if (i < GRID_SIZE && k < GRID_SIZE) {
                                blocks[k][j][GRID_SIZE-1-i] = temp[i][j][k];
                            }
                            break;
                            
                        case 2: // Z-axis rotation
                            // (x,y,z) -> (-y,x,z)
                            if (i < GRID_SIZE && j < GRID_SIZE) {
                                blocks[GRID_SIZE-1-j][i][k] = temp[i][j][k];
                            }
                            break;
                    }
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Rotation error: " << e.what() << std::endl;
    }

        // Check if rotation is valid using gameboard's isValidPosition
        /*if (!gameboard.isValidPosition(*this)) {
            blocks = temp;
            dx = oldDx;
            dy = oldDy; 
            dz = oldDz;
        }*/
    }

    bool isValidPosition(const GameBoard& gameboard) const {
        for(int i = 0; i < GRID_SIZE; i++) {
            for(int j = 0; j < GRID_SIZE; j++) {
                for(int k = 0; k < GRID_SIZE; k++) {
                    if(blocks[i][j][k] != 0) {
                        int worldX = posX + i;
                        int worldY = posY + j;
                        int worldZ = posZ + k;
                        
                        // Check bounds and collisions
                        if(worldX < 0 || worldX >= BOARD_WIDTH_DEPTH ||
                           worldY < 0 || worldY >= BOARD_HEIGHT ||
                           worldZ < 0 || worldZ >= BOARD_WIDTH_DEPTH ||
                           gameboard.isPositionOccupied(worldX,worldY,worldZ)) {
                            return false;
                        }
                    }
                }
            }
        }
        return true;
    }

    // Getter methods for orientation
    int getOrientationX() const { return dx; }
    int getOrientationY() const { return dy; }
    int getOrientationZ() const { return dz; }

private:
    Shape shape;
    std::vector<std::vector<std::vector<int>>> blocks;
    float posY;
    float posX;
    float posZ;
    float rotationX ;
    float rotationY ;
    float rotationZ ;


    // Initialize the blocks of the Tetromino based on its shape
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
    static const int GRID_SIZE = 4;
    int dx, dy, dz;          // Local orientation (0-3)

    
};



// Function to display a Tetromino
void displayTetromino(const Tetromino& t, int color) {
    const auto& blocks = t.getBlocks();
    float y = t.getPositionY();
    float x = t.getPositionX();
    float z = t.getPositionZ();
    // Calculate center of the Tetromino
    // float centerX = blocks.size() / 2.0f;
    // float centerY = blocks[0].size() / 2.0f;
    // float centerZ = blocks[0][0].size() / 2.0f;
   /* switch (t.getShape())
    {
    case Tetromino::I:
        centerX = 0.5f;
        centerY = 1.5f;
        centerZ = 0.0f;
        break;
    case Tetromino::J:
        centerX = 1.0f;
        centerY = 0.5f;
        centerZ = 0.0f;
        break;
    case Tetromino::L:
        centerX = 1.0f;
        centerY = 0.5f;
        centerZ = 0.0f;
        break;
    case Tetromino::O:
        centerX = 0.5f;
        centerY = 0.5f;
        centerZ = 0.0f;
        break;
    case Tetromino::S:
        centerX = 0.5f;
        centerY = 0.5f;
        centerZ = 0.0f;
        break;
    case Tetromino::T:
        centerX = 1.0f;
        centerY = 0.5f;
        centerZ = 0.0f;
        break;
    case Tetromino::Z:  
        centerX = 0.5f;
        centerY = 0.5f;
        centerZ = 0.0f;
        break;
    default:
        break;
    }*/

    glPushMatrix();
   
    // Move to Tetromino position
    glTranslatef(x, y, z);
   
    // Then translate to center, rotate, and translate back
    //glTranslatef(centerX, centerY, centerZ);
    glRotatef(t.getRotationX(), t.getPositionX(), 0.0f, 0.0f);
    glRotatef(t.getRotationY(), 0.0f, t.getPositionY(), 0.0f);
    glRotatef(t.getRotationZ(), 0.0f, 0.0f, t.getPositionZ());
    //glTranslatef(-centerX, -centerY, -centerZ);

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
                    //std::cout << "x: " << x + i << " y: " << y + j << " z: " << z + k << std::endl;
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
std::vector<Tetromino> Tetrominoes;
float fallSpeed = 0.5f; // Speed at which the Tetrominoes fall
int fallInterval = 1000; // Time interval in milliseconds

void rotateTetromino(int axis) {
    if (selectedTetromino >= Tetrominoes.size()) return;
   
    // Rotate 90 degrees each time
    const float ROTATION_ANGLE = 90.0f;
   
    switch(axis) {
        case 0: // X axis
            Tetrominoes[selectedTetromino].setRotationX(Tetrominoes[selectedTetromino].getRotationX() + ROTATION_ANGLE);
            break;
        case 1: // Y axis
            Tetrominoes[selectedTetromino].setRotationY(Tetrominoes[selectedTetromino].getRotationY() + ROTATION_ANGLE);
            break;  
        case 2: // Z axis    
            Tetrominoes[selectedTetromino].setRotationZ(Tetrominoes[selectedTetromino].getRotationZ() + ROTATION_ANGLE);
            break;    

    }
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
    int n = BOARD_WIDTH_DEPTH; // Grid length
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

    displayGrid(); // Display the grid
    // Display all Tetrominoes
    for (size_t i = 0; i < Tetrominoes.size(); ++i) {
        displayTetromino(Tetrominoes[i], i % 6); // Display each Tetromino with a different color
    }
    glPopMatrix();

    glutSwapBuffers();
    glFlush();
}

// Function to check if the Tetromino is above the grid or another Tetromino
bool checkCollision(const Tetromino& t) {
    if (t.getPositionY() <= 0.25f) {

        return true;
    }
    // Check collision with other Tetrominoes
    for (size_t i = 0; i < Tetrominoes.size(); ++i) {
        if (i != selectedTetromino) {
            const auto& blocks = Tetrominoes[i].getBlocks();
            float y = Tetrominoes[i].getPositionY();
            float x = Tetrominoes[i].getPositionX();
            float z = Tetrominoes[i].getPositionZ();

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
    if (selectedTetromino < Tetrominoes.size()) {
        Tetrominoes[selectedTetromino].updatePosition(fallSpeed); // Update the position of the selected Tetromino

        // Check if the Tetromino has reached the ground or landed on another Tetromino
        if (checkCollision(Tetrominoes[selectedTetromino])) {
            // Freeze the Tetromino and create a new one
            selectedTetromino++;
            if (selectedTetromino < Tetrominoes.size()) {
                Tetrominoes.push_back(Tetromino(static_cast<Tetromino::Shape>(rand() % 7)));
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
        Tetrominoes[selectedTetromino].move(-0.5f, 0.0f, 0.0f);
        glutPostRedisplay();
    } else if (key == 'd' || key == 'D') {
        // Move selected Tetromino right
        Tetrominoes[selectedTetromino].move(0.5f, 0.0f, 0.0f);
        glutPostRedisplay();
    } else if (key == 'w' || key == 'W') {
        // Move selected Tetromino up
        Tetrominoes[selectedTetromino].move(0.0f, 0.0f, -0.5f);
        glutPostRedisplay();
    } else if (key == 's' || key == 'S') {
        // Move selected Tetromino backward
        Tetrominoes[selectedTetromino].move(0.0f, 0.0f, 0.5f);
        glutPostRedisplay();
    } else if (key == 'q' || key == 'Q') {
        // Move selected Tetromino down
        if (!checkCollision(Tetrominoes[selectedTetromino])) {
            Tetrominoes[selectedTetromino].move(0.0f, -0.5f, 0.0f);
            glutPostRedisplay();
        }
    } else if (key == 'e' || key == 'E') {
        std::cout << "t" << std::endl;
        // Rotate the selected Tetromino around the X-axis
        Tetrominoes[selectedTetromino].rotateLocal(0);
        glutPostRedisplay();
    } else if (key == 'r' || key == 'R') {
        // Rotate the selected Tetromino around the Y-axis
        Tetrominoes[selectedTetromino].rotateLocal(1);
        glutPostRedisplay();
    } else if (key == 'f' || key == 'F') {
        // Rotate the selected Tetromino around the Z-axis
        Tetrominoes[selectedTetromino].rotateLocal(2);

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
    glutTimerFunc(fallInterval, timer, 0); // R    glutKeyboardFunc(ke
    // Initialize Tetrominoes
    Tetrominoes.push_back(Tetromino(Tetromino::I));
    std::cout << "Tetrominoes size: " << Tetrominoes.size() << std::endl;
 
    glutMainLoop();
    return 0;
}