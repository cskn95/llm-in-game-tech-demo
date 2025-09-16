#pragma once
#include <glad/glad.h>
#include <string_view>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
	GLuint id = 0;

public:
	Shader() = default;
	~Shader() { if (id) glDeleteProgram(id); }

	// Kopyalama yasak
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

	// Taşınabilir
	Shader(Shader&& other) noexcept : id(other.id) {
		other.id = 0;
	}
	Shader& operator=(Shader&& other) noexcept {
		if (this != &other) {
			if (id) glDeleteProgram(id);
			id = other.id;
			other.id = 0;
		}
		return *this;
	}

	// Shader yükleme
	bool loadFromFile(std::string_view vertexPath, std::string_view fragmentPath);
	bool loadFromSource(std::string_view vertexSrc, std::string_view fragmentSrc);

	// Kullanım
	Shader& bind() {
		glUseProgram(id);
		return *this;
	}

	// Uniform
	[[nodiscard]] GLint uniformLocation(std::string_view name) const;

	[[nodiscard]] GLuint getId() const { return id; }
	explicit operator bool() const { return id != 0; }
};
