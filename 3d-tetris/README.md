# 3D Tetris

## Overview
3D Tetris is a 3D implementation of the classic Tetris game. The project utilizes OpenGL for rendering and GLFW for window management. The game features various Tetris blocks that fall from the top of the screen, and players must rotate and position them to create complete lines.

## Project Structure
```
3d-tetris
├── src
│   ├── main.cpp                # Entry point of the application
│   ├── shaders
│   │   ├── fragment_shader.glsl # Fragment shader for pixel coloring
│   │   └── vertex_shader.glsl   # Vertex shader for transforming vertex data
│   ├── textures
│   │   └── texture.png          # Texture image for blocks or background
│   └── utils
│       └── utils.cpp            # Utility functions for the game
├── CMakeLists.txt              # CMake configuration file
└── README.md                    # Project documentation
```

## Setup Instructions
1. Ensure you have CMake, GLFW, and GLEW installed on your system.
2. Clone the repository:
   ```
   git clone <repository-url>
   cd 3d-tetris
   ```
3. Create a build directory and navigate into it:
   ```
   mkdir build
   cd build
   ```
4. Run CMake to configure the project:
   ```
   cmake ..
   ```
5. Build the project:
   ```
   make
   ```
6. Run the application:
   ```
   ./3d-tetris
   ```

## Gameplay
- Use the arrow keys to move and rotate the falling blocks.
- Complete lines to score points and prevent the blocks from reaching the top of the screen.
- The game ends when the blocks stack up to the top.

## License
This project is licensed under the MIT License. See the LICENSE file for more details.