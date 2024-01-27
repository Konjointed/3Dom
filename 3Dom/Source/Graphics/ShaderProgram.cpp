#include "ShaderProgram.h"

#include <iostream>
#include <iostream>
#include <sstream>
#include <fstream>

#include "Core/Resources.h"

void addShaderToProgram(ShaderProgram& program, GLenum type, const std::string& filepath)
{
	Shader compiledShader = compileShader(type, filepath);
	program.shaders.push_back(compiledShader);
}

void linkShaderProgram(ShaderProgram& program)
{
	program.id = glCreateProgram();

	for (auto& shader : program.shaders)
	{
		glAttachShader(program.id, shader.id);
	}
	glLinkProgram(program.id);

	GLint success;
	glGetProgramiv(program.id, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetProgramInfoLog(program.id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	for (auto& shader : program.shaders)
	{
		glDetachShader(program.id, shader.id);
		glDeleteShader(shader.id);
	}
}

Shader compileShader(GLenum type, const std::string filepath)
{
	Shader result;
	result.type = type;
	result.filename = filepath;

	// 1. Load file content
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
		std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
	}

	// 2.  Create shader and compile it
	result.id = glCreateShader(type);
	const GLchar* source = shaderSource.c_str();
	glShaderSource(result.id, 1, &source, nullptr);
	glCompileShader(result.id);

	GLint compiled = GL_FALSE;
	glGetShaderiv(result.id, GL_COMPILE_STATUS, &compiled);
	if (compiled == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(result.id, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* infoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(result.id, infoLogLength, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
		delete[] infoLog;
		result.id = 0;
	}

	return result;
}

ShaderProgram createShaderProgram()
{
	ShaderProgram program;
	program.id = glCreateProgram();
	return program;
}

void loadShaderProgram(Resources& resources,
	const std::string& programName, 
	const std::string& vertexShaderFilepath, 
	const std::string& fragmentShaderFilepath)
{
	ShaderProgram program = createShaderProgram();
	addShaderToProgram(program, GL_VERTEX_SHADER, vertexShaderFilepath);
	addShaderToProgram(program, GL_FRAGMENT_SHADER, fragmentShaderFilepath);
	linkShaderProgram(program);
	resources.shaderPrograms[programName] = program;
}
