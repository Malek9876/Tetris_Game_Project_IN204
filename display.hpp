#ifndef DISPLAY
#define DISPLAY

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include "logic.hpp"
#include "server.hpp"
#include "client.hpp"
#include <thread>

class Display {
private:
    GLuint VAO, VBO;
    Logic game;
    Server host;
    Client player;
    // Other member variables...
    int xSize;
    int ySize;

    int counterGravity;
    int sleepCont;

    int pixel_factor;
    int window_width;
    int window_height;

    int colorChanging;
    bool colorAug;

    bool namecheck;
    short my_pos;

    bool thread_runing;
    std::thread* networking; // Use std::thread instead of Thread

    short server_choosen;
    bool reload;
    bool display_ready;
    status_type my_status;
    
    int level;

    std::string player_Name;
    

public:
    Display();
    void Run(GLFWwindow* window);
    void Render();
    void InitializeOpenGL();
    // Other methods...
    ~Display();
};

#endif