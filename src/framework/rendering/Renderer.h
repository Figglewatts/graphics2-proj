#ifndef RENDERER_H
#define RENDERER_H
#pragma once

#include "framework/graphics/GLContext.h"
#include <glm/glm.hpp>
#include "framework/graphics/Camera.h"

namespace Framework
{
	class Renderer
	{
	protected:
		glm::mat4 _projection;
		Camera* _pCamera;
		GLContext *_context;
	public:
		Renderer(GLContext *context) : _context(context) {}
		virtual void init() = 0;
		virtual void beginFrame() = 0;
		virtual void endFrame() = 0;
		virtual void resize(glm::ivec2 size) = 0;
		virtual void setProjection(glm::mat4 projection) final { this->_projection = projection; }
		virtual void setCamera(Camera *cam) final { this->_pCamera = cam; }
		virtual ~Renderer() = 0;
	};

	inline Renderer::~Renderer() { } // empty as default implementation of destructor
}

#endif // RENDERER_H