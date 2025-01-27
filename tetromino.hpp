#ifndef TETROMINO_HPP
#define TETROMINO_HPP

#include <vector>
#include <tuple>
#include <GL/glut.h>
#include <cmath>

extern float floor_pos;
extern float wall_pos;

class tetromino {
public:
    enum Shape { I, J, O, S, T };
    
    tetromino(Shape shape);
    
    Shape getShape() const;
    const std::vector<std::vector<std::vector<int>>>& getBlocks() const;
    const std::vector<std::tuple<float, float, float>>& getBlockPositions() const;
    
    void updatePosition(float fallSpeed);
    void move(float dx, float dy, float dz);
    void rotate(int axis, float angle);
    
    float getPositionX() const;
    float getPositionY() const;
    float getPositionZ() const;
    
    float getRotationX() const;
    void setRotationX(float value);
    float getRotationY() const;
    void setRotationY(float value);
    float getRotationZ() const;
    void setRotationZ(float value);

private:
    Shape shape;
    std::vector<std::vector<std::vector<int>>> blocks;
    std::vector<std::tuple<float, float, float>> blockPositions;
    float rotationX;
    float rotationY;
    float rotationZ;

    void initializeShape();
    void updateBlockPositionsAfterRotation(int axis, float angle);
};

#endif // TETROMINO_HPP