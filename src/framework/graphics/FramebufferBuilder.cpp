#include "FramebufferBuilder.h"
#include "Framebuffer.h"

namespace Framework
{
	FramebufferBuilder::FramebufferBuilder(unsigned width, unsigned height, GLenum type)
		: _width(width), _height(height), _framebufferType(type)
	{
		glGenFramebuffers(1, &_fboHandle);
		glBindFramebuffer(_framebufferType, _fboHandle);
	}

	FramebufferBuilder& FramebufferBuilder::addColorAttachment(FBOAttachment attachment)
	{
		glGenTextures(1, &attachment.handle);
		glBindTexture(GL_TEXTURE_2D, attachment.handle);
		glTexImage2D(GL_TEXTURE_2D, 0, attachment.internalFormat, _width, _height, 0, attachment.format, attachment.type, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, attachment.minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, attachment.magFilter);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + _colorAttachments.size(), GL_TEXTURE_2D, attachment.handle, 0);
		_colorAttachments.push_back(attachment);
		return *this;
	}

	FramebufferBuilder& FramebufferBuilder::setDepthAttachment(FBOAttachment attachment, GLenum target)
	{
		glGenTextures(1, &attachment.handle);
		glBindTexture(GL_TEXTURE_2D, attachment.handle);
		glTexImage2D(GL_TEXTURE_2D, 0, attachment.internalFormat, _width, _height, 0, attachment.format, attachment.type, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, attachment.minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, attachment.magFilter);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, target, GL_TEXTURE_2D, attachment.handle, 0);
		_depthAttachment = attachment;
		return *this;
	}

	Framebuffer FramebufferBuilder::build()
	{
		return Framebuffer(this);
	}

	Framebuffer* FramebufferBuilder::buildPtr()
	{
		return new Framebuffer(this);
	}

}