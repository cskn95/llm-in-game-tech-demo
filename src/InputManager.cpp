#include "InputManager.h"

InputManager inputManager;

void InputManager::process(GLFWwindow* window, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) positionX -= speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) positionX += speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) positionY += speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) positionY -= speed * deltaTime;
}

void InputManager::onKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods) const {
    if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
        toggleFullscreen(window);
    }
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void InputManager::toggleFullscreen(GLFWwindow* window) const {
    static bool fullscreen = false;
    fullscreen = !fullscreen;

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    if (fullscreen) {
        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    } else {
        glfwSetWindowMonitor(window, nullptr, windowedX, windowedY, windowedW, windowedH, 0);
    }
}

void InputManager::resize_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    inputManager.onKeyPress(window, key, scancode, action, mods);
}
