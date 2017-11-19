#ifndef TEXTURE2D_H
#define TEXTURE2D_H
#pragma once

#include "framework/graphics/GLContext.h"
#include <string>
#include <glm/glm.hpp>

namespace Framework
{
	class Texture2D
	{
	private:
		unsigned _width, _height;
		unsigned _handle;
	public:
		Texture2D(const std::string& filePath,
			GLint format = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE,
			GLint minFilter = GL_LINEAR_MIPMAP_LINEAR, GLint magFilter = GL_LINEAR,
			GLint sWrap = GL_CLAMP, GLint tWrap = GL_CLAMP);

		void bind() const;
		void unbind() const;

		unsigned handle() const { return _handle; }
		glm::vec2 get_size() const { return { _width, _height }; }
	};
}

#endif // TEXTURE2D_H