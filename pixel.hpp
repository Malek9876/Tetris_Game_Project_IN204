#ifndef PIXEL
#define PIXEL

#include "config.inl"

class Pixel
{
private:
    int x;
    int y;
    int z; // Add Z coordinate
public:
    Pixel();
    Pixel(int argX, int argY, int argZ); // Update constructor
    int getX() const;
    int getY() const;
    int getZ() const; // Add getter for Z
    void setX(int argX);
    void setY(int argY);
    void setZ(int argZ); // Add setter for Z
    void modX(int argModX);
    void modY(int argModY);
    void modZ(int argModZ); // Add modifier for Z
    void Rotate(); // Update to handle 3D rotation
    Pixel operator - (const Pixel& operand) const;
    Pixel operator + (const Pixel& operand) const;
    ~Pixel();
};

#endif