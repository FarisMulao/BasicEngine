#ifndef SHADERS_H
#define SHADERS_H

#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {
    // Create the shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Read vertex shader code
    FILE* vertexFile = fopen(vertex_file_path, "r");
    if (!vertexFile) {
        fprintf(stderr, "Could not open vertex shader file: %s\n", vertex_file_path);
        return 0;
    }
    fseek(vertexFile, 0, SEEK_END);
    long vertexLength = ftell(vertexFile);
    fseek(vertexFile, 0, SEEK_SET);
    char* vertexCode = (char*)malloc(vertexLength + 1);
    fread(vertexCode, 1, vertexLength, vertexFile);
    vertexCode[vertexLength] = '\0';
    fclose(vertexFile);

    // Read fragment shader code
    FILE* fragmentFile = fopen(fragment_file_path, "r");
    if (!fragmentFile) {
        fprintf(stderr, "Could not open fragment shader file: %s\n", fragment_file_path);
        free(vertexCode);
        return 0;
    }
    fseek(fragmentFile, 0, SEEK_END);
    long fragmentLength = ftell(fragmentFile);
    fseek(fragmentFile, 0, SEEK_SET);
    char* fragmentCode = (char*)malloc(fragmentLength + 1);
    fread(fragmentCode, 1, fragmentLength, fragmentFile);
    fragmentCode[fragmentLength] = '\0';
    fclose(fragmentFile);

    // Compile vertex shader
    glShaderSource(vertexShader, 1, (const char**)&vertexCode, NULL);
    glCompileShader(vertexShader);
    GLint result;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        fprintf(stderr, "Vertex shader compilation failed: %s\n", infoLog);
        free(vertexCode);
        free(fragmentCode);
        return 0;
    }

    // Compile fragment shader
    glShaderSource(fragmentShader, 1, (const char**)&fragmentCode, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        fprintf(stderr, "Fragment shader compilation failed: %s\n", infoLog);
        free(vertexCode);
        free(fragmentCode);
        return 0;
    }

    // Link the program
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // Clean up
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    free(vertexCode);
    free(fragmentCode);

    return program;
}

#endif // SHADERS_H
