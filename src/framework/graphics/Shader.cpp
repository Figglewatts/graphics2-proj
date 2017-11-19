#include "Shader.h"

#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

#include "framework/util/Logger.h"

namespace Framework
{
	Shader::Shader(const std::string& name, const std::string& path)
		: _name(name)
	{
		compileAndLink(path);
	}

	Shader::~Shader()
	{
		glDeleteProgram(this->_progHandle);
	}

	void Shader::compileAndLink(const std::string& path)
	{
		// load and compile vertex shader
		this->_vertHandle = this->loadSource(path, GL_VERTEX_SHADER);
		glCompileShader(this->_vertHandle);
		checkCompileErr(this->_vertHandle);

		// load and compile fragment shader
		this->_fragHandle = this->loadSource(path, GL_FRAGMENT_SHADER);
		glCompileShader(this->_fragHandle);
		checkCompileErr(this->_fragHandle);

		// link the shader program
		this->_progHandle = glCreateProgram();
		glAttachShader(this->_progHandle, this->_vertHandle);
		glAttachShader(this->_progHandle, this->_fragHandle);
		glLinkProgram(this->_progHandle);
		checkLinkErr(this->_progHandle);

		// clean up unused handles
		glDeleteShader(this->_vertHandle);
		glDeleteShader(this->_fragHandle);
		this->_vertHandle = this->_fragHandle = 0;
	}

	GLuint Shader::loadSource(const std::string& path, GLenum type)
	{
		std::ifstream source(path + (type == GL_VERTEX_SHADER ? ".vert" : ".frag"));

		if (!source.is_open())
		{
			LOG(LogLevel::FATAL, "Shader %s could not be loaded", path.c_str());
			throw std::runtime_error("Could not load shader file");
		}
		const std::string shaderSrc(
			(std::istreambuf_iterator<char>(source)),
			(std::istreambuf_iterator<char>()));
		const char *src_str = shaderSrc.c_str();
		GLuint shader = glCreateShader(type);
		glShaderSource(shader, 1, &src_str, 0);

		return shader;
	}

	bool Shader::checkCompileErr(GLuint shader) const
	{
		int success;
		char infoLog[INFOLOG_BUFF_LEN];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, INFOLOG_BUFF_LEN, nullptr, infoLog);
			LOG(LogLevel::ERR, "Error compiling shader %s", this->_name.c_str());
			LOG(LogLevel::ERR, "Info log:\n%s", infoLog);
		}
		return success;
	}

	bool Shader::checkLinkErr(GLuint program) const
	{
		int success;
		char infoLog[INFOLOG_BUFF_LEN];
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(program, INFOLOG_BUFF_LEN, nullptr, infoLog);
			LOG(LogLevel::ERR, "Error linking program %s", this->_name.c_str());
			LOG(LogLevel::ERR, "Info log:\n%s", infoLog);
		}
		return success;
	}

	unsigned Shader::getLocation(const std::string & name) const
	{
		return glGetUniformLocation(this->_progHandle, name.c_str());
	}

	void Shader::bind() const
	{
		glUseProgram(this->_progHandle);
	}

	void Shader::unbind() const
	{
		glUseProgram(0);
	}

	void Shader::setUniform(const std::string & name, bool value) const
	{
		glUniform1i(getLocation(name), (int)value);
	}

	void Shader::setUniform(const std::string & name, int value) const
	{
		glUniform1i(getLocation(name), value);
	}

	void Shader::setUniform(const std::string & name, float value) const
	{
		glUniform1f(getLocation(name), value);
	}

	void Shader::setUniform(const std::string & name, glm::mat4 value, bool transpose) const
	{
		glUniformMatrix4fv(getLocation(name), 1, transpose, glm::value_ptr(value));
	}

	void Shader::setUniform(const std::string& name, glm::mat3 value, bool transpose) const
	{
		glUniformMatrix3fv(getLocation(name), 1, transpose, glm::value_ptr(value));
	}

	void Shader::setUniform(const std::string & name, glm::vec2 value) const
	{
		glUniform2fv(getLocation(name), 1, glm::value_ptr(value));
	}

	void Shader::setUniform(const std::string & name, glm::vec3 value) const
	{
		glUniform3fv(getLocation(name), 1, glm::value_ptr(value));
	}

	void Shader::setUniform(const std::string & name, glm::vec4 value) const
	{
		glUniform4fv(getLocation(name), 1, glm::value_ptr(value));
	}



}
