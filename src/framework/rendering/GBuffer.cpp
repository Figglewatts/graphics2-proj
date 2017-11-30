#include "GBuffer.h"
#include "framework/graphics/FramebufferBuilder.h"

namespace Framework
{
	GBuffer::GBuffer(unsigned width, unsigned height)
	{
		FBOAttachment position;
		position.internalFormat = GL_RGB32F;
		position.format = GL_RGB;
		position.type = GL_FLOAT;

		FBOAttachment diffuse;
		diffuse.internalFormat = GL_RGB32F;
		diffuse.format = GL_RGB;
		diffuse.type = GL_FLOAT;

		FBOAttachment normal;
		normal.internalFormat = GL_RGB32F;
		normal.format = GL_RGB;
		normal.type = GL_FLOAT;

		FBOAttachment depthAndStencil;
		depthAndStencil.internalFormat = GL_DEPTH24_STENCIL8;
		depthAndStencil.format = GL_DEPTH_STENCIL;
		depthAndStencil.type = GL_UNSIGNED_INT_24_8;

		FBOAttachment finalTex;
		finalTex.internalFormat = GL_RGBA;
		finalTex.format = GL_RGB;
		finalTex.type = GL_FLOAT;

		FramebufferBuilder b = FramebufferBuilder(width, height, GL_DRAW_FRAMEBUFFER)
			.addColorAttachment(position)
			.addColorAttachment(diffuse)
			.addColorAttachment(normal)
			.setDepthAttachment(depthAndStencil, GL_DEPTH_STENCIL_ATTACHMENT)
			.addColorAttachment(finalTex);
		_fbo = b.buildPtr();
	}

	GBuffer::~GBuffer()
	{
		delete _fbo;
	}

	void GBuffer::StartFrame() const
	{
		// clear final texture
		_fbo->bind();
		glDrawBuffer(GL_COLOR_ATTACHMENT3);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void GBuffer::BindForGeomPass() const
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo->handle());
		GLenum drawBufs[] = {
			GL_COLOR_ATTACHMENT0,
			GL_COLOR_ATTACHMENT1,
			GL_COLOR_ATTACHMENT2
		};
		glDrawBuffers(3, drawBufs);
	}

	void GBuffer::BindForStencilPass() const
	{
		// disable the draw buffers, we're only writing to the stencil buffer after this
		glDrawBuffer(GL_NONE);
	}

	void GBuffer::BindForLightPass() const
	{
		// bind the GBuffer textures for the light pass to use
		glDrawBuffer(GL_COLOR_ATTACHMENT3);
		for (int i = 0; i < _fbo->color_attachment_count(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, _fbo->get_color_attachments()[i].handle);
		}
		glActiveTexture(GL_TEXTURE0);
	}

	void GBuffer::BindForFinalPass() const
	{
		// get ready to draw to the screen
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, _fbo->handle());
		glReadBuffer(GL_COLOR_ATTACHMENT3);
	}
}