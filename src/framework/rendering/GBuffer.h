#ifndef GBUFFER_H
#define GBUFFER_H
#pragma once
#include "framework/graphics/Framebuffer.h"

namespace Framework
{
	class GBuffer
	{
	private:
		Framebuffer *_fbo;
	public:
		GBuffer(unsigned width, unsigned height);
		~GBuffer();

		void StartFrame() const;
		void BindForGeomPass() const;
		void BindForStencilPass() const;
		void BindForLightPass() const;
		void BindForFinalPass() const;
		Framebuffer *get_framebuffer() const { return _fbo; }
	};
}

#endif // GBUFFER_H