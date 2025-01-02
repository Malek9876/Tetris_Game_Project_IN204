cmake_minimum_required(VERSION 3.10)
project(3D_Tetris)

set(CMAKE_CXX_STANDARD 11)

# Add the include directory for stb_image.h
include_directories(src/include)

# Add source files
set(SOURCES
    src/main.cpp
    src/utils/utils.cpp
)

add_executable(3D_Tetris ${SOURCES})

# Link necessary libraries
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

target_link_libraries(3D_Tetris OpenGL::GL GLUT::GLUT GLEW::GLEW glfw)