#include "Framebuffer.h"
#include "framework/util/Logger.h"

namespace Framework
{
	Framebuffer::Framebuffer(unsigned numColorAttachments, GLenum framebufferType, unsigned width, unsigned height, GLint internalFormat, GLenum format, GLenum colorType, bool depth = false)
		: _width(width), _height(height), _framebufferType(framebufferType)
	{
		glGenFramebuffers(1, &_fboHandle);
		glBindFramebuffer(framebufferType, _fboHandle);

		std::vector<GLenum> drawBuffers;

		for (int i = 0; i < numColorAttachments; i++)
		{
			unsigned handle;
			glGenTextures(1, &handle);
			_colorAttachmentHandles.push_back(handle);
			glBindTexture(GL_TEXTURE_2D, _colorAttachmentHandles[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
			glBindTexture(GL_TEXTURE_2D, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _colorAttachmentHandles[i], 0);
			/*
			glBindTexture(GL_TEXTURE_2D, _colorAttachmentHandles[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, _width, _height, 0, format, colorType, nullptr);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _colorAttachmentHandles[i], 0);
			*/
			drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + i);
		}

		if (depth)
		{
			glGenTextures(1, &_depthAttachmentHandle);
			glBindTexture(GL_TEXTURE_2D, _depthAttachmentHandle);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, _width, _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
			glBindTexture(GL_TEXTURE_2D, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthAttachmentHandle, 0);
		}

		GLenum drawBufs[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(numColorAttachments, drawBufs);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			LOG(LogLevel::FATAL, "Error creating Framebuffer!");
			throw std::runtime_error("Could not create framebuffer");
		}

		glBindFramebuffer(framebufferType, 0);
	}

	Framebuffer::~Framebuffer()
	{
		glDeleteTextures(_colorAttachmentHandles.size(), _colorAttachmentHandles.data());
		if (_depthAttachmentHandle != 0) glDeleteTextures(1, &_depthAttachmentHandle);
		glDeleteFramebuffers(1, &_fboHandle);
	}

	void Framebuffer::bind() const
	{
		glBindFramebuffer(_framebufferType, _fboHandle);
	}

	void Framebuffer::unbind() const
	{
		glBindFramebuffer(_framebufferType, 0);
	}
}
