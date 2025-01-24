#include <GL/gl.h>   // Open Graphics Library (OpenGL) header
#include <GL/glut.h> // The GL Utility Toolkit (GLUT) Header
#include <cmath>

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
