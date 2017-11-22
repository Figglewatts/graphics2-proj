#include "DeferredRenderer.h"

namespace Framework
{
	DeferredRenderer::DeferredRenderer(unsigned width, unsigned height, GLContext *context)
		: Renderer(context), _width(width), _height(height)
	{
		_gBuffer = new GBuffer(width, height);
	}

	void DeferredRenderer::init()
	{
		
	}

	void DeferredRenderer::render(void(*renderCallback)(glm::mat4 view, glm::mat4 proj), glm::mat4 view, glm::mat4 proj)
	{
		GeometryPass(renderCallback, view, proj);
		LightingPass();
	}

	DeferredRenderer::~DeferredRenderer()
	{
		delete _gBuffer;
	}

	void DeferredRenderer::GeometryPass(void(*renderCallback)(glm::mat4 view, glm::mat4 proj), glm::mat4 view, glm::mat4 proj)
	{
		_gBuffer->BindForWrite();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderCallback(view, proj);
	}

	void DeferredRenderer::LightingPass()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		_gBuffer->BindForRead();

		GLsizei HalfWidth = (GLsizei)(_width / 2.0f);
		GLsizei HalfHeight = (GLsizei)(_height / 2.0f);

		_gBuffer->SetReadBuffer(GBuffer::POSITION);
		glBlitFramebuffer(0, 0, _width, _height,
			0, 0, HalfWidth, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		_gBuffer->SetReadBuffer(GBuffer::DIFFUSE);
		glBlitFramebuffer(0, 0, _width, _height,
			0, HalfHeight, HalfWidth, _height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		_gBuffer->SetReadBuffer(GBuffer::NORMAL);
		glBlitFramebuffer(0, 0, _width, _height,
			HalfWidth, HalfHeight, _width, _height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	}

}