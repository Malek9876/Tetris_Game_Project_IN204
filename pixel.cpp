#include "pixel.hpp"

Pixel::Pixel() : x(0), y(0), z(0) {}

Pixel::Pixel(int argX, int argY, int argZ) : x(argX), y(argY), z(argZ) {}

int Pixel::getX() const {
    return x;
}

int Pixel::getY() const {
    return y;
}

int Pixel::getZ() const {
    return z;
}

void Pixel::setX(int argX) {
    x = argX;
}

void Pixel::setY(int argY) {
    y = argY;
}

void Pixel::setZ(int argZ) {
    z = argZ;
}

void Pixel::modX(int argModX) {
    x += argModX;
}

void Pixel::modY(int argModY) {
    y += argModY;
}

void Pixel::modZ(int argModZ) {
    z += argModZ;
}

void Pixel::Rotate() {
    // Implement 3D rotation logic if needed
    int temp = x;
    x = -y;
    y = temp;
}

Pixel Pixel::operator - (const Pixel& operand) const {
    Pixel result = *this;
    result.x -= operand.x;
    result.y -= operand.y;
    result.z -= operand.z;
    return result;
}

Pixel Pixel::operator + (const Pixel& operand) const {
    Pixel result = *this;
    result.x += operand.x;
    result.y += operand.y;
    result.z += operand.z;
    return result;
}

Pixel::~Pixel() {}