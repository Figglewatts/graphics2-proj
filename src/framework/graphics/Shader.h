#ifndef SHADER_H
#define SHADER_H
#pragma once

#include <string>
#include "framework/graphics/GLContext.h"

namespace Framework
{
	class Shader
	{
		// TODO: add uniforms as class (override operator=)
	private:
		static const unsigned INFOLOG_BUFF_LEN = 512;

		unsigned _vertHandle;
		unsigned _fragHandle;
		unsigned _progHandle;
		std::string _name;

		void compileAndLink(const std::string& path);
		static GLuint loadSource(const std::string& path, GLenum type);
		bool checkCompileErr(GLuint shader) const;
		bool checkLinkErr(GLuint program) const;

	public:
		Shader(const std::string& name, const std::string& path);
		Shader(const Shader& other) = delete;
		Shader& operator=(const Shader& other) = delete;
		~Shader();

		unsigned handle() const { return this->_progHandle; }
		void use() const;
	};
}

#endif // SHADER_H