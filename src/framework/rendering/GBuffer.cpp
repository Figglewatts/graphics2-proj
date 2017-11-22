#include "GBuffer.h"

namespace Framework
{
	GBuffer::GBuffer(unsigned width, unsigned height)
	{
		_fbo = new Framebuffer(GBufferTextures::COUNT, GL_DRAW_FRAMEBUFFER, width, height, GL_RGB32F, GL_FLOAT, GL_RGB, true);
	}

	GBuffer::~GBuffer()
	{
		delete _fbo;
	}

	void GBuffer::BindForRead() const
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, _fbo->handle());
	}

	void GBuffer::BindForWrite() const
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo->handle());
	}

	void GBuffer::SetReadBuffer(GBufferTextures type) const
	{
		glReadBuffer(GL_COLOR_ATTACHMENT0 + type);
	}
}