#include "Shader.h"
#include "InputManager.h"
#include "Window.h"
#include "Renderer.h"

// Vertex data
const std::vector<float> vertices = {
    0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
   -0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
   -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
};

const std::vector<unsigned short> indices = {
    0, 1, 2,
    0, 2, 3,
};

int main() {
    try {
        initializeGlfw();
        UniqueWindow window = createWindow(640, 480, "Modern OpenGL Window", false);

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

        Renderer renderer(vertices, indices);

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        glfwSetKeyCallback(window.get(), key_callback);

        while (!glfwWindowShouldClose(window.get())) {
            glClear(GL_COLOR_BUFFER_BIT);
            glfwPollEvents();

            auto currentFrame = static_cast<float>(glfwGetTime());
            float deltaTime = currentFrame - inputManager.lastFrame;
            inputManager.lastFrame = currentFrame;

            inputManager.process(window.get(), deltaTime);

            glUniform1f(u_movementX, inputManager.positionX);
            glUniform1f(u_movementY, inputManager.positionY);

            renderer.draw();

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
