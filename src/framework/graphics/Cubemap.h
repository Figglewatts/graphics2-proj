#ifndef CUBEMAP_H
#define CUBEMAP_H
#pragma once

#include <string>
#include "framework/graphics/GLContext.h"
#include <vector>

namespace Framework
{
	class Cubemap
	{
	private:
		unsigned _width, _height;
		unsigned _handle;

		const std::vector<std::string> _sides = {
			"_right",
			"_left",
			"_bottom",
			"_top",
			"_front",
			"_back"
		};
	public:
		Cubemap(const std::string& filePath,
			GLint format = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE,
			GLint minFilter = GL_LINEAR, GLint magFilter = GL_LINEAR);

		void bind() const;
		void unbind() const;

		unsigned handle() const { return _handle; }
		glm::vec2 get_size() const { return { _width, _height }; }
	};
}

#endif // CUBEMAP_H