#include "ShaderProgram.h"

#include <sstream>
#include <fstream>

#include <glm/gtc/type_ptr.hpp>

#include "Log/Logger.h"
#include "Core/Resources.h"

void ShaderProgram::SetUniform(const std::string& name, float value)
{
	GLint location = glGetUniformLocation(m_id, name.c_str());
	glUniform1f(location, value);
}

void ShaderProgram::SetUniform(const std::string& name, const glm::vec3& value)
{
	GLint location = glGetUniformLocation(m_id, name.c_str());
	glUniform3fv(location, 1, glm::value_ptr(value));
}

void ShaderProgram::SetUniform(const std::string& name, const glm::mat4& value)
{
	GLint location = glGetUniformLocation(m_id, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void CompileShaderAndAddToProgram(ShaderProgram& program, GLenum type, const std::string& filepath)
{
	Shader compiledShader = CompileShader(type, filepath);
	program.m_shaders.push_back(compiledShader);
}

Shader CompileShader(GLenum type, const std::string filepath) 
{
	Shader result;
	result.m_type = type;
	result.m_filepath = filepath;

	// Load file content 
	std::string shaderSource;
	try
	{
		std::ifstream shaderFile(filepath);
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		shaderSource = shaderStream.str();
	}
	catch (std::exception e)
	{
		spdlog::error("SHADER: Failed to read shader files");
	}

	// Create shader and compile it
	result.m_id = glCreateShader(type);
	const GLchar* source = shaderSource.c_str();
	glShaderSource(result.m_id, 1, &source, nullptr);
	glCompileShader(result.m_id);

	GLint compiled = GL_FALSE;
	glGetShaderiv(result.m_id, GL_COMPILE_STATUS, &compiled);
	if (compiled == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(result.m_id, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* infoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(result.m_id, infoLogLength, NULL, infoLog);
		spdlog::error("SHADER::COMPILATION_FAILED {}", infoLog);
		delete[] infoLog;
		result.m_id = 0;
	}

	return result;
}

void LinkShaderProgram(ShaderProgram& program)
{
	program.m_id = glCreateProgram();

	for (auto& shader : program.m_shaders)
	{
		glAttachShader(program.m_id, shader.m_id);
	}
	glLinkProgram(program.m_id);

	GLint success;
	glGetProgramiv(program.m_id, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetProgramInfoLog(program.m_id, 512, NULL, infoLog);
		spdlog::error("SHADER::PROGRAM::LINKING_FAILED {}", infoLog);
	}

	for (auto& shader : program.m_shaders)
	{
		glDetachShader(program.m_id, shader.m_id);
		glDeleteShader(shader.m_id);
	}
}

void LoadShaderProgram(Resources& resources, 
	const std::string name, 
	const std::string vertexShaderPath, 
	const std::string fragShaderPath) 
{
	ShaderProgram program;
	program.m_id = glCreateProgram();

	CompileShaderAndAddToProgram(program, GL_VERTEX_SHADER, vertexShaderPath);
	CompileShaderAndAddToProgram(program, GL_FRAGMENT_SHADER, fragShaderPath);

	LinkShaderProgram(program);

	resources.m_shaderPrograms.emplace(name, program);
}
