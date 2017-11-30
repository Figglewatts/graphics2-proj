#ifndef FBOATTACHMENT_H
#define FBOATTACHMENT_H
#pragma once

#include "framework/graphics/GLContext.h"

namespace Framework
{
	struct FBOAttachment
	{
		GLint internalFormat;
		GLenum format;
		GLenum type;
		GLenum minFilter = GL_NEAREST;
		GLenum magFilter = GL_NEAREST;
		unsigned handle = 0;
	};
}

#endif // FBOATTACHMENT_H