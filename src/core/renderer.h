#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <string>
#include "camera.h"

class Renderer {
public:
    Renderer() = default;
    ~Renderer() = default;

    GLuint createShaderProgram(const char* vertexPath, const char* fragmentPath);
    GLuint createCube();
    
private:
    GLuint compileShader(const char* source, GLenum type);
    std::string readShaderFile(const char* shaderPath);
};

#endif