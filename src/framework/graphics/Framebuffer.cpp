#include "Framebuffer.h"
#include "framework/util/Logger.h"
#include "FramebufferBuilder.h"

namespace Framework
{
	Framebuffer::Framebuffer(FramebufferBuilder *builder)
		: _fboHandle(builder->_fboHandle)
		, _colorAttachments(builder->_colorAttachments)
		, _depthAttachment(builder->_depthAttachment)
		, _width(builder->_width), _height(builder->_height)
		, _framebufferType(builder->_framebufferType)
	{
		glBindFramebuffer(_framebufferType, _fboHandle);
		
		std::vector<GLenum> drawBuffers;
		for (int i = 0; i < _colorAttachments.size(); i++)
		{
			drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + i);
		}
		glDrawBuffers(_colorAttachments.size(), drawBuffers.data());

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			LOG(LogLevel::FATAL, "Error creating Framebuffer!");
			throw std::runtime_error("Could not create framebuffer");
		}

		glBindFramebuffer(_framebufferType, 0);
	}

	Framebuffer::~Framebuffer()
	{
		for (const auto& attachment : _colorAttachments)
			glDeleteTextures(1, &attachment.handle);
		
		if (_depthAttachment.handle != 0) glDeleteTextures(1, &_depthAttachment.handle);
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

	void Framebuffer::resize(glm::ivec2 size)
	{
		for (const auto& attachment : _colorAttachments)
		{
			glBindTexture(GL_TEXTURE_2D, attachment.handle);
			glTexImage2D(GL_TEXTURE_2D, 0, attachment.internalFormat, _width, _height, 0, attachment.format, attachment.type, nullptr);
		}

		if (_depthAttachment.handle != 0)
		{
			glBindTexture(GL_TEXTURE_2D, _depthAttachment.handle);
			glTexImage2D(GL_TEXTURE_2D, 0, _depthAttachment.internalFormat, _width, _height, 0, _depthAttachment.format, _depthAttachment.type, nullptr);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		_width = size.x;
		_height = size.y;
	}
}
