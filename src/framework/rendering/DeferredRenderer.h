#ifndef DEFERREDRENDERER_H
#define DEFERREDRENDERER_H
#pragma once

#include "Renderer.h"
#include "GBuffer.h"
#include "framework/graphics/Shader.h"

namespace Framework
{
	class DeferredRenderer : Renderer
	{
	private:
		GBuffer *_gBuffer;
		unsigned _width, _height;

		void GeometryPass(void(*renderCallback)(glm::mat4 view, glm::mat4 proj), glm::mat4 view, glm::mat4 proj);
		void LightingPass();
	public:
		DeferredRenderer(unsigned width, unsigned height, GLContext *context);
		void init() override;
		void render(void (*renderCallback)(glm::mat4 view, glm::mat4 proj), glm::mat4 view, glm::mat4 proj) override;
		~DeferredRenderer();
	};
}

#endif