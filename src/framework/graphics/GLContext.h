#ifndef GLCONTEXT_H
#define GLCONTEXT_H
#pragma once

#include "framework/graphics/Color.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace Framework
{
	class GLContext
	{
	private:
		unsigned _minVersion;
		unsigned _majVersion;
		GLFWwindow *_window;
	public:
		GLContext() : _minVersion(2), _majVersion(3), _window(nullptr) {}
		GLContext(unsigned majVer, unsigned minVer, GLFWwindow *window);
		void viewport(int x, int y, unsigned w, unsigned h);
		void clearColor(const Color& color);
		void clear(GLbitfield mask);
	};
}

#endif // GLCONTEXT_H