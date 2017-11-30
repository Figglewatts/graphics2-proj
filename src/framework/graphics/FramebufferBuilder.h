#ifndef FRAMEBUFFERBUILDER_H
#define FRAMEBUFFERBUILDER_H
#pragma once

#include <vector>
#include "framework/graphics/GLContext.h"
#include "FBOAttachment.h"

namespace Framework
{	
	class Framebuffer; // forward declaration
	
	class FramebufferBuilder
	{
	private:
		unsigned _fboHandle;
		std::vector<FBOAttachment> _colorAttachments;
		FBOAttachment _depthAttachment;
		unsigned _width;
		unsigned _height;
		GLenum _framebufferType;

	public:
		FramebufferBuilder(unsigned width, unsigned height, GLenum type);

		friend class Framebuffer; // framebuffer can access data directly

		FramebufferBuilder& addColorAttachment(FBOAttachment attachment);
		FramebufferBuilder& setDepthAttachment(FBOAttachment attachment, GLenum target);
		Framebuffer build();
		Framebuffer *buildPtr();
	};
}

#endif // FRAMEBUFFERBUILDER_H