#include <vector>
#include <tuple>
#include <GL/glut.h>
#include <cmath>



extern float floor_pos;
extern float wall_pos;
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
                newY < floor_pos || newY >= wall_pos ||
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
               std::get<1>(pos) < floor_pos || std::get<1>(pos) >= wall_pos ||
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
        glutPostRedisplay();
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
        float initial_lower_Y = 17.5f;
        float initial_lower_X = 6.5f;
        float initial_lower_Z = 3.5f;
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
                // newY=floor(newY)+0.5;
                newZ = y * sinA + z * cosA;
                // newZ=int(newZ)+0.5;
                std::get<1>(pos) = newY + centerY;
                std::get<2>(pos) = newZ + centerZ;
                break;

            case 1: // Y-axis rotation
                newX = x * cosA + z * sinA;
                // newX=floor(newX)+0.5;   
                newZ = -x * sinA + z * cosA;
                // newZ=floor(newZ)+0.5;
                std::get<0>(pos) = newX + centerX;
                std::get<2>(pos) = newZ + centerZ;
                break;

            case 2: // Z-axis rotation
                newX = x * cosA - y * sinA;
                // newX=floor(newX)+0.5;
                newY = x * sinA + y * cosA;
                //newY=floor(newY)+0.5;
                std::get<0>(pos) = newX + centerX;
                std::get<1>(pos) = newY + centerY;
                break;
        }
    }
}
};