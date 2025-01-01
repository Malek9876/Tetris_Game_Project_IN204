#include "display.hpp"

Display::Display(): 
   xSize(width),
   ySize(height + amount_of_pixels),
   pixel_factor(px_factor),
   window_width(screen_width),
   window_height(screen_height),
   game(xSize, ySize)
{
   level = game.getLevel();
   sleepCont = 0;
   colorChanging = 0;
   colorAug = true;
   counterGravity = 0;
   
   player_Name = "";
   server_choosen = 0;
   reload = false;
   display_ready = false;
   my_status = changednt;
   networking = nullptr;
   thread_runing = false;
   namecheck = false;
   my_pos = 0;
}

void Display::InitializeOpenGL() {
    // Define vertices for a cube (tetromino block)
    float cubeVertices[] = {
        // Positions         // Colors
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
        // (repeat for all cube faces)
    };

    // Create VAO and VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Display::Run(GLFWwindow* window) {
    InitializeOpenGL();

    // Game Loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render the game
        Render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Display::Render() {
    // Bind VAO and draw the cube
    glBindVertexArray(VAO);
    glDrawArrays(GL_QUADS, 0, 24);
}

Display::~Display() {
    // Cleanup OpenGL resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}