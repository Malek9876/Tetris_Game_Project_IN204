#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <GL/glew.h>

GLuint loadShader(const std::string& filePath, GLenum shaderType);
GLuint loadTexture(const std::string& filePath);

#endif // UTILS_H