#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "openglErrorReporting.h"

struct InputManager {
    float positionX = 0.0f;
    float positionY = 0.0f;
    const float speed = 1.0f;
    float lastFrame = 0.0f;
    int windowedX = 100, windowedY = 100;
    int windowedW = 1280, windowedH = 720;

    // Klavye inputlarını sürekli kontrol eden fonksiyon
    void process(GLFWwindow* window, float deltaTime);

    // Tek seferlik tuş basışlarını yakalayan fonksiyon
    void onKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods) const;

    // Pencereyi fullscreen ↔ windowed arasında değiştirir
    void toggleFullscreen(GLFWwindow* window) const;

    // Pencere yeniden boyutlandırıldığında viewport’u günceller
    static void resize_callback(GLFWwindow* window, int width, int height);
};

// Global InputManager örneği (main.cpp’de tanımlanacak)
extern InputManager inputManager;

// GLFW key callback fonksiyonu
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
