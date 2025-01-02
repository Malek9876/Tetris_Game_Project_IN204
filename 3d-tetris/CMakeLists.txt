cmake_minimum_required(VERSION 3.10)

# Set the project name
project(3D_Tetris)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find OpenGL and GLFW
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

# Include directories
include_directories(${OPENGL_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} src)

# Source files
set(SOURCES
    src/main.cpp
    src/utils/utils.cpp
)

# Add shaders
set(SHADERS
    src/shaders/vertex_shader.glsl
    src/shaders/fragment_shader.glsl
)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES} ${SHADERS})

# Link libraries
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glfw GLEW::GLEW)