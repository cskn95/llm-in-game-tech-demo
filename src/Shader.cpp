#include "Shader.h"

namespace {

// Dosya okuma
std::string readFile(std::string_view path) {
    std::ifstream file(path.data(), std::ios::in | std::ios::binary);
    if (!file) {
        throw std::runtime_error("Dosya açılamadı: " + std::string(path));
    }
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

// Kaynaktan shader derleme
GLuint compileShader(std::string_view source, GLenum type) {
    GLuint shader = glCreateShader(type);
    const char* src = source.data();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint status = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        GLint logLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
        std::string log(logLen, '\0');
        glGetShaderInfoLog(shader, logLen, nullptr, log.data());
        glDeleteShader(shader);
        throw std::runtime_error("Shader compile error:\n" + log);
    }
    return shader;
}

} // namespace

bool Shader::loadFromFile(std::string_view vertexPath, std::string_view fragmentPath) {
    try {
        auto vertSrc = readFile(vertexPath);
        auto fragSrc = readFile(fragmentPath);
        return loadFromSource(vertSrc, fragSrc);
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return false;
    }
}

bool Shader::loadFromSource(std::string_view vertexSrc, std::string_view fragmentSrc) {
    GLuint vert = 0, frag = 0, prog = 0;
    try {
        vert = compileShader(vertexSrc, GL_VERTEX_SHADER);
        frag = compileShader(fragmentSrc, GL_FRAGMENT_SHADER);

        prog = glCreateProgram();
        glAttachShader(prog, vert);
        glAttachShader(prog, frag);
        glLinkProgram(prog);

        glDeleteShader(vert);
        glDeleteShader(frag);

        GLint status = 0;
        glGetProgramiv(prog, GL_LINK_STATUS, &status);
        if (!status) {
            GLint logLen = 0;
            glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLen);
            std::string log(logLen, '\0');
            glGetProgramInfoLog(prog, logLen, nullptr, log.data());
            glDeleteProgram(prog);
            throw std::runtime_error("Program link error:\n" + log);
        }

        if (id) glDeleteProgram(id);
        id = prog;
        return true;

    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        if (vert) glDeleteShader(vert);
        if (frag) glDeleteShader(frag);
        if (prog) glDeleteProgram(prog);
        return false;
    }
}

GLint Shader::uniformLocation(std::string_view name) const {
    GLint loc = glGetUniformLocation(id, name.data());
    if (loc == -1) {
        std::cerr << "Uniform bulunamadı: " << name << "\n";
    }
    return loc;
}
