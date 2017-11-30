#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "framework/graphics/GLContext.h"
#include "FBOAttachment.h"

namespace Framework
{
	class FramebufferBuilder;
	
	class Framebuffer
	{
	private:
		unsigned _fboHandle;
		std::vector<FBOAttachment> _colorAttachments;
		FBOAttachment _depthAttachment;
		unsigned _width, _height;
		GLenum _framebufferType;

		Framebuffer(FramebufferBuilder *builder);
	public:
		~Framebuffer();

		friend class FramebufferBuilder; // builder can access data directly

		void bind() const;
		void unbind() const;

		glm::vec2 size() const { return { _width, _height }; }
		unsigned handle() const { return _fboHandle; }
		unsigned color_attachment_count() const { return _colorAttachments.size(); }
		const std::vector<FBOAttachment>& get_color_attachments() const { return _colorAttachments; }
		void resize(glm::ivec2 size);
	};
}

#endif // FRAMEBUFFER_H