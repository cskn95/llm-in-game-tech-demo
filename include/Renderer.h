#pragma once

#include <glad/glad.h>
#include <memory>
#include <vector>

// Custom deleter structures for OpenGL objects
struct VaoDeleter {
    void operator()(const GLuint* vao) const {
        if (vao) {
            glDeleteVertexArrays(1, vao);
            delete vao;
        }
    }
};

struct BufferDeleter {
    void operator()(const GLuint* buffer) const {
        if (buffer) {
            glDeleteBuffers(1, buffer);
            delete buffer;
        }
    }
};

// Type aliases for smart pointers
using UniqueVao = std::unique_ptr<GLuint, VaoDeleter>;
using UniqueBuffer = std::unique_ptr<GLuint, BufferDeleter>;

class Renderer {
public:
    Renderer(const std::vector<float>&vertices, const std::vector<unsigned short>&indices);
    void draw() const;
private:
    UniqueVao vao;
    UniqueBuffer vbo;
    UniqueBuffer ebo;
    GLsizei indexCount;
};