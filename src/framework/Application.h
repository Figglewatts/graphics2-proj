#ifndef APPLICATION_H
#define APPLICATION_H
#pragma once

#include "graphics/GLContext.h"
#include <string>

namespace Framework
{
	class Application
	{
	private:
		GLContext _glContext;
		GLFWwindow *_window;
	public:
		Application(std::string windowTitle, unsigned width, unsigned height);
		
		virtual ~Application() {}
	};
}

#endif // APPLICATION_H