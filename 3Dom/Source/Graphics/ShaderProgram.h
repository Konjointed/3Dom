#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <string>
#include <vector>

#include <glad/glad.h>

#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.inl"

struct Resources;

struct Shader {
	GLenum type;
	std::string filename;
	int id;
};

struct ShaderProgram {
	GLuint id;
	std::vector<Shader> shaders;

    void setUniform(const std::string& name, float value) {
        GLint loc = glGetUniformLocation(id, name.c_str());
        glUniform1f(loc, value);
    }

    void setUniform(const std::string& name, const glm::vec3& value) {
        GLint loc = glGetUniformLocation(id, name.c_str());
        glUniform3fv(loc, 1, glm::value_ptr(value));
    }

    void setUniform(const std::string& name, const glm::mat4& value) {
        GLint loc = glGetUniformLocation(id, name.c_str());
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
    }
};

void addShaderToProgram(ShaderProgram& program, GLenum type, const std::string& filepath);

Shader compileShader(GLenum type, const std::string filepath);

void linkShaderProgram(ShaderProgram& program);

ShaderProgram createShaderProgram();

void loadShaderProgram(Resources& resources,
	const std::string& programName,
	const std::string& vertexShaderFilepath,
	const std::string& fragmentShaderFilepath);

//template <typename... T>
//void AddShadersToProgram(GLenum type, const std::string& filepath, T... shaders) {
//	addShaderToProgram(type, filepath);
//	AddShadersToProgram(shaders...);
//}

#endif