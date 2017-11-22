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
		enum GBufferTextures
		{
			POSITION,
			DIFFUSE,
			NORMAL,
			COUNT
		};

		GBuffer(unsigned width, unsigned height);
		~GBuffer();

		void BindForWrite() const;
		void BindForRead() const;
		void SetReadBuffer(GBufferTextures type) const;
	};
}

#endif // GBUFFER_H