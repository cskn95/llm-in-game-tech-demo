#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <Shader.h>
#include "InputManager.h"
#include "Window.h"
#include "Renderer.h"
#include <glm/glm.hpp>
#include <iostream>

// Sınırlayıcı kutu yapısı
struct BoundingBox {
    float x_min, x_max, y_min, y_max;
};

// AABB (Axis-Aligned Bounding Box) çarpışma kontrolü
bool checkCollision(const BoundingBox& a, const BoundingBox& b) {
    // Eksenlerde kesişim olup olmadığını kontrol et
    bool collisionX = a.x_min <= b.x_max && a.x_max >= b.x_min;
    bool collisionY = a.y_min <= b.y_max && a.y_max >= b.y_min;
    // Sadece iki eksende de kesişim varsa çarpışma vardır
    return collisionX && collisionY;
}

// Vertex data
const std::vector<float> groundVertices = {
    1.0f, -0.9f, 0.0f,   0.0f, 0.0f, 0.0f, // sağ üst
   -1.0f, -0.9f, 0.0f,   0.0f, 0.0f, 0.0f, // sol üst
   -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 0.0f, // sol alt
    1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 0.0f, // sağ alt
};

const std::vector<unsigned short> groundIndices = {
    0, 1, 2,
    0, 2, 3,
};

const std::vector<float> playerVertices = {
    0.1f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f, // sağ üst
   -0.1f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f, // sol üst
   -0.1f, -0.7f, 0.0f,   1.0f, 0.0f, 0.0f, // sol alt
    0.1f, -0.7f, 0.0f,   1.0f, 0.0f, 0.0f, // sağ alt
};

const std::vector<unsigned short> playerIndices = {
    0, 1, 2,
    0, 2, 3,
};

int main() {
    try {
        initializeGlfw();
        UniqueWindow window = createWindow(1280, 720, "Modern OpenGL Window", false);

        glfwMakeContextCurrent(window.get());
        glfwSetFramebufferSizeCallback(window.get(), InputManager::resize_callback);

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            throw std::runtime_error("GLAD could not be loaded!");
        }

        glfwSwapInterval(1);
        enableReportGlErrors();

        Shader shader;
        shader.loadFromFile(RESOURCES_PATH "shader.vert", RESOURCES_PATH "shader.frag");
        shader.bind();

        GLint u_movementX = shader.uniformLocation("u_movementX");
        glUniform1f(u_movementX, 0.0);
        GLint u_movementY = shader.uniformLocation("u_movementY");
        glUniform1f(u_movementY, 0.0);

        Renderer groundRenderer(groundVertices, groundIndices);
        Renderer playerRenderer(playerVertices, playerIndices);

        BoundingBox groundBox = {-1.0f, 1.0f, -1.0f, -0.9f};
        BoundingBox playerBox = {-0.1f, 0.1f, -0.7f, -0.5f};

        // Fizik değişkenleri
        float velocityY = 0.0f;
        const float gravity = 1.8f;
        bool isGrounded = false;

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        glfwSetKeyCallback(window.get(), key_callback);

        while (!glfwWindowShouldClose(window.get())) {
            glClear(GL_COLOR_BUFFER_BIT);
            glfwPollEvents();

            auto currentFrame = static_cast<float>(glfwGetTime());
            float deltaTime = currentFrame - inputManager.lastFrame;
            inputManager.lastFrame = currentFrame;

            // Store old position for collision response
            float oldX = inputManager.positionX;
            float oldY = inputManager.positionY;

            // Yatay hareketi işle
            inputManager.process(window.get(), deltaTime);

            // Yer çekimini uygula
            if (!isGrounded) {
                velocityY -= gravity * deltaTime;
            }

            // Zıplama kontrolü (InputManager'da isJumping ayarlanmalı)
            if (inputManager.isJumping && isGrounded) {
                velocityY = 0.8f; // Zıplama hızı
            }

            // Dikey pozisyonu güncelle
            inputManager.positionY += velocityY * deltaTime;

            // Update player bounding box based on the new potential position
            BoundingBox nextPlayerBox = {
                playerBox.x_min + inputManager.positionX,
                playerBox.x_max + inputManager.positionX,
                playerBox.y_min + inputManager.positionY,
                playerBox.y_max + inputManager.positionY,
            };

            // Zeminle çarpışmayı kontrol et
            if (checkCollision(nextPlayerBox, groundBox)) {
                // Çarpışma durumunda, pozisyonu düzelt ve hızı sıfırla
                inputManager.positionY = oldY; // Şimdilik basit bir geri alma
                velocityY = 0.0f;
                isGrounded = true;
            } else {
                isGrounded = false;
            }

            // Yatay çarpışma (örnek olarak bırakıldı, dikey ile birleştirilebilir)
            nextPlayerBox = {
                playerBox.x_min + inputManager.positionX,
                playerBox.x_max + inputManager.positionX,
                playerBox.y_min + oldY, // Dikey pozisyonu eski haldeyken kontrol et
                playerBox.y_max + oldY,
            };
            if (checkCollision(nextPlayerBox, groundBox)) {
                 inputManager.positionX = oldX;
            }

            // Draw ground (no movement)
            glUniform1f(u_movementX, 0.0f);
            glUniform1f(u_movementY, 0.0f);
            groundRenderer.draw();

            // Draw player (with movement)
            glUniform1f(u_movementX, inputManager.positionX);
            glUniform1f(u_movementY, inputManager.positionY);
            playerRenderer.draw();

            glfwSwapBuffers(window.get());
        }

    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwTerminate();
    return 0;
}
