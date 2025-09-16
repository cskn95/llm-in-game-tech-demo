#include "Renderer.h"

Renderer::Renderer(const std::vector<float>&vertices, const std::vector<unsigned short>& indices)
    : indexCount(static_cast<GLsizei>(indices.size()))
{
    auto vao_ptr = new GLuint;
    glGenVertexArrays(1, vao_ptr);
    vao.reset(vao_ptr);
    glBindVertexArray(*vao);

    auto vbo_ptr = new GLuint;
    glGenBuffers(1, vbo_ptr);
    vbo.reset(vbo_ptr);
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(float)), vertices.data(), GL_STATIC_DRAW);

    auto ebo_ptr = new GLuint;
    glGenBuffers(1, ebo_ptr);
    ebo.reset(ebo_ptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned short)), indices.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void*>(nullptr));
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::draw() const {
    glBindVertexArray(*vao);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, nullptr);
    glBindVertexArray(0);
}
