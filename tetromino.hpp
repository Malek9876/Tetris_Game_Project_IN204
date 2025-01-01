#ifndef TETROMINO
#define TETROMINO

#include <random>
#include <cstdlib>
#include <vector>
#include "pixel.hpp"
#include <chrono>
#include "config.inl"

class Tetromino
{
private:
    std::vector<Pixel> pixels;
    bool active;
    std::string color;
    int id;
public:
    Tetromino();
    Tetromino(Pixel, int);
    Tetromino(const Tetromino &);
    std::vector<Pixel> getPixels() const;
    std::string getColor() const;
    int GetId() const;
    void setPixels(const std::vector<Pixel>&);
    void deactivate();
    ~Tetromino();
};

#endif