#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "display.hpp"

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create an OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "3D Tetris", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    Display Tetris;
    Tetris.Run(window);

    glfwTerminate();
    return 0;
}