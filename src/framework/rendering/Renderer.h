#ifndef RENDERER_H
#define RENDERER_H
#pragma once

#include "framework/graphics/GLContext.h"
#include <glm/glm.hpp>

namespace Framework
{
	class Renderer
	{
		GLContext *_context;
	public:
		Renderer(GLContext *context) : _context(context) {}
		virtual void init() = 0;
		virtual void render(void(*renderCallback)(glm::mat4 view, glm::mat4 proj), glm::mat4 view, glm::mat4 proj) = 0;
		virtual ~Renderer() = 0;
	};

	inline Renderer::~Renderer() { } // empty as default implementation of destructor
}

#endif // RENDERER_H