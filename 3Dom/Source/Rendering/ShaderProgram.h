#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <string>
#include <vector>

#include <glad/glad.h>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

struct Resources;

struct Shader {
	GLenum m_type;
	std::string m_filepath;
	int m_id;
};

struct  ShaderProgram {
	GLuint m_id;
	std::vector<Shader> m_shaders;

	void SetUniform(const std::string& name, float value);
	void SetUniform(const std::string& name, const glm::vec3& value);
	void SetUniform(const std::string& name, const glm::mat4& value);
};

//-----------------------------------------------------------------------------
// Shader Program API
//-----------------------------------------------------------------------------
void CompileShaderAndAddToProgram(ShaderProgram& program, GLenum type, const std::string& filepath);

Shader CompileShader(GLenum type, const std::string filepath);

void LinkShaderProgram(ShaderProgram& program);

void LoadShaderProgram(Resources& resources,
	const std::string name,
	const std::string vertexShaderPath,
	const std::string fragShaderPath);

#endif 