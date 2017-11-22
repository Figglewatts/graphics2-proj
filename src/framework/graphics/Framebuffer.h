#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "framework/graphics/GLContext.h"

namespace Framework
{
	class Framebuffer
	{
	private:
		unsigned _fboHandle;
		std::vector<unsigned> _colorAttachmentHandles;
		unsigned _depthAttachmentHandle = 0;
		unsigned _width, _height;
		GLenum _framebufferType;
	public:
		Framebuffer(unsigned numColorAttachments, GLenum framebufferType, unsigned width, unsigned height, GLint internalFormat, GLenum format, GLenum colorType, bool depth);
		~Framebuffer();

		void bind() const;
		void unbind() const;

		glm::vec2 size() const { return { _width, _height }; }
		unsigned handle() const { return _fboHandle; }
	};
}

#endif // FRAMEBUFFER_H