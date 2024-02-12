#include "Shader.h"

#include <sstream>
#include <fstream>

#include "Log/Logger.h"
#include "Core/Resources.h"

void ShaderProgramLoader::Load(const std::string name, const std::string vertexShaderPath, const std::string fragShaderPath, const std::string geomShaderPath)
{
	ShaderProgram program;

	compileShaderAndAddToProgram(program, GL_VERTEX_SHADER, vertexShaderPath);
	compileShaderAndAddToProgram(program, GL_FRAGMENT_SHADER, fragShaderPath);
	if (!geomShaderPath.empty()) {
		compileShaderAndAddToProgram(program, GL_GEOMETRY_SHADER, geomShaderPath);
	}

	linkShaderProgram(program);

	gResources.mShaderPrograms.emplace(name, program);
}

void ShaderProgramLoader::compileShaderAndAddToProgram(ShaderProgram& program, GLenum type, const std::string& filepath)
{
	Shader compiledShader = compileShader(type, filepath);
	program.mShaders.push_back(compiledShader);
}

Shader ShaderProgramLoader::compileShader(GLenum type, const std::string filepath)
{
	Shader result;
	result.mType = type;
	result.mFilepath = filepath;

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
		spdlog::error("SHADER::COMPILESHADER: Failed to read shader files");
	}

	// Create shader and compile it
	result.mId = glCreateShader(type);
	const GLchar* source = shaderSource.c_str();
	glShaderSource(result.mId, 1, &source, nullptr);
	glCompileShader(result.mId);

	GLint compiled = GL_FALSE;
	glGetShaderiv(result.mId, GL_COMPILE_STATUS, &compiled);
	if (compiled == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(result.mId, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* infoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(result.mId, infoLogLength, NULL, infoLog);
		spdlog::error("SHADER::COMPILESHADER: Compilation failed {}", infoLog);
		delete[] infoLog;
		result.mId = 0;
	}

	return result;
}

void ShaderProgramLoader::linkShaderProgram(ShaderProgram& program)
{
	program.mId = glCreateProgram();

	for (auto& shader : program.mShaders)
	{
		glAttachShader(program.mId, shader.mId);
	}
	glLinkProgram(program.mId);

	GLint success;
	glGetProgramiv(program.mId, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetProgramInfoLog(program.mId, 512, NULL, infoLog);
		spdlog::error("SHADER::LINKSHADERPROGRAM: Linking failed {}", infoLog);
	}

	for (auto& shader : program.mShaders)
	{
		glDetachShader(program.mId, shader.mId);
		glDeleteShader(shader.mId);
	}
}

void LoadShaderProgram(const std::string name, const std::string vertexShaderPath, const std::string fragShaderPath, const std::string geomShaderPath)
{
    ShaderProgramLoader::Load(name, vertexShaderPath, fragShaderPath, geomShaderPath);
}

void ShaderProgram::SetUniformInt(const std::string& name, int value)
{
	GLint location = glGetUniformLocation(mId, name.c_str());
	glUniform1i(location, value);
}

void ShaderProgram::SetUniformFloat(const std::string& name, float value)
{
	GLint location = glGetUniformLocation(mId, name.c_str());
	glUniform1f(location, value);
}

void ShaderProgram::SetUniform(const std::string& name, const glm::vec3& value)
{
	GLint location = glGetUniformLocation(mId, name.c_str());
	glUniform3fv(location, 1, glm::value_ptr(value));
}

void ShaderProgram::SetUniform(const std::string& name, const glm::mat4& value)
{
	GLint location = glGetUniformLocation(mId, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}
