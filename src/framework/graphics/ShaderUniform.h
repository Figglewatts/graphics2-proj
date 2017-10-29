#ifndef SHADER_UNIFORM_H
#define SHADER_UNIFORM_H
#pragma once

#include <string>
#include <functional>
#include "framework/graphics/GLContext.h"

namespace Framework
{
	template <typename DataType, std::size_t Count>
	class GenericShaderUniform
	{
	protected:
		std::string _name;
		unsigned _uniformLocation;
		DataType _value;
		std::function<void(GLint, GLsizei, DataType*)> _glSetFunc;

		GenericShaderUniform(const std::string& name, unsigned program) : _name(name)
		{
			_uniformLocation = glGetUniformLocation(program, name.c_str());
		}

		void setValue(DataType value) { _value = value; }

		GenericShaderUniform<DataType, Count>& operator=(DataType& v)
		{
			_glSetFunc(this->_uniformLocation, Count, &this->_value);
			this->_value = v;
			return *this;
		}
	};
	
	template <typename DataType, std::size_t Count>
	class ShaderUniform final : GenericShaderUniform<DataType, Count>; // no implementation

	template <std::size_t Count>
	class ShaderUniform<float> final : GenericShaderUniform<float, Count>
	{
	public:
		ShaderUniform(const std::string& name, unsigned program) 
			: GenericShaderUniform(name, program)
		{
			this->_glSetFunc = glUniform1fv;
		}
	};
}

#endif // SHADER_UNIFORM_H