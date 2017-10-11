#ifndef GLCONTEXT_H
#define GLCONTEXT_H
#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace Framework
{
	class GLContext
	{
	private:
		unsigned _minVersion;
		unsigned _majVersion;
	public:
		GLContext(unsigned minVer, unsigned majVer);
		void viewport();
	};
}

#endif // GLCONTEXT_H