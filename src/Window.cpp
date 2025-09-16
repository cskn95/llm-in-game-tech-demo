#include "Window.h"

void initializeGlfw() {
    if (!glfwInit()) {
        throw std::runtime_error("GLFW initialization failed!");
    }
}

UniqueWindow createWindow(int width, int height, const char* title, bool fullscreen) {
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#endif

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* monitor = nullptr;
    if (fullscreen) {
        monitor = glfwGetPrimaryMonitor();
        if (!monitor) {
            throw std::runtime_error("Primary monitor not found for fullscreen!");
        }
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        if (!mode) {
            throw std::runtime_error("Video mode not available!");
        }
        width  = mode->width;
        height = mode->height;
    }

    GLFWwindow* window = glfwCreateWindow(width, height, title, monitor, nullptr);
    if (!window) {
        throw std::runtime_error("Window could not be created!");
    }
    return UniqueWindow(window);
}