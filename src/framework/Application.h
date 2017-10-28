#ifndef APPLICATION_H
#define APPLICATION_H
#pragma once

#include "framework/graphics/GLContext.h"
#include <string>
#include <iostream>

namespace Framework
{
	
	
	class Application
	{
	private:
		GLContext _glContext;
		GLFWwindow *_window;
	public:
		Application(const std::string& windowTitle, unsigned majVer, 
			unsigned minVer, unsigned width, unsigned height);

		virtual ~Application();

		inline GLFWwindow *get_window() { return _window; }
		inline GLContext& get_context() { return _glContext; }
	};
}

#endif // APPLICATION_H