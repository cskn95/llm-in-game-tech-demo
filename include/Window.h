#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "openglErrorReporting.h"
#include <memory>
#include <stdexcept>

void initializeGlfw();

struct WindowDeleter {
    void operator()(GLFWwindow* window) const {
        if (window) {
            glfwDestroyWindow(window);
        }
    }
};

using UniqueWindow = std::unique_ptr<GLFWwindow, WindowDeleter>;

UniqueWindow createWindow(int width, int height, const char* title, bool fullscreen = false);