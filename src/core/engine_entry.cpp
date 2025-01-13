#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <filesystem>
#include "camera.h"
#include "renderer.h"

// Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Mouse control
bool mouseLocked = true;
bool justToggledLock = false;
glm::dvec2 lastMousePos(SCR_WIDTH / 2.0f, SCR_HEIGHT / 2.0f);

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (!mouseLocked) return;

    if (firstMouse) {
        lastMousePos.x = SCR_WIDTH / 2.0f;
        lastMousePos.y = SCR_HEIGHT / 2.0f;
        glfwSetCursorPos(window, lastMousePos.x, lastMousePos.y); 
        firstMouse = false;
        return;
    }

    // Calc offset from last position
    float xoffset = static_cast<float>(xpos - lastMousePos.x);
    float yoffset = static_cast<float>(lastMousePos.y - ypos);

    // Set cursor position back to center
    lastMousePos.x = SCR_WIDTH / 2.0f;
    lastMousePos.y = SCR_HEIGHT / 2.0f;
    glfwSetCursorPos(window, lastMousePos.x, lastMousePos.y); 

    // Sensitivity
    const float MOUSE_SENSITIVITY = 0.1f;
    xoffset *= MOUSE_SENSITIVITY;
    yoffset *= MOUSE_SENSITIVITY;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // (temp) toggle mouse lock with M key
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        if (!justToggledLock) {
            mouseLocked = !mouseLocked;
            if (mouseLocked) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                // Center cursor immediately when locking
                lastMousePos.x = SCR_WIDTH / 2.0f;
                lastMousePos.y = SCR_HEIGHT / 2.0f;
                glfwSetCursorPos(window, lastMousePos.x, lastMousePos.y);
                firstMouse = true;
            } else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
            justToggledLock = true;
        }
    } else {
        justToggledLock = false;
    }

    // Only process movement if mouse is locked
    if (mouseLocked) {
        const float cameraSpeed = 2.5f * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
    }
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "3D Game Engine", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Configure global OpenGL state
    glEnable(GL_DEPTH_TEST);
    
    // Set callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    // Initialize mouse lock
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(window, SCR_WIDTH / 2.0f, SCR_HEIGHT / 2.0f);

    // Create renderer
    Renderer renderer;
    
    // Create shader program
    GLuint shaderProgram = renderer.createShaderProgram(
        "core/vertex_shader.glsl",
        "core/fragment_shader.glsl"
    );
    
    if (!shaderProgram) {
        std::cerr << "Failed to create shader program" << std::endl;
        return -1;
    }

    // Create cube VAO
    GLuint cubeVAO = renderer.createCube();
    if (!cubeVAO) {
        std::cerr << "Failed to create cube VAO" << std::endl;
        return -1;
    }

    // Set clear color to teal
    glClearColor(0.0f, 0.5f, 0.5f, 1.0f);

    // Enter render loop
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // Set up transformation matrices
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, 
                          static_cast<float>(glfwGetTime()), 
                          glm::vec3(0.5f, 1.0f, 0.0f));

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = camera.GetProjectionMatrix(static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT));

        // Set uniforms
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // Render cube
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteProgram(shaderProgram);
    glDeleteVertexArrays(1, &cubeVAO);
    glfwTerminate();

    return 0;
}