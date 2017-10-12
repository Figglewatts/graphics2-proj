#include "Application.h"
#include "framework/util/Logger.h"
#include <stdexcept>

namespace Framework
{
	Application::Application(const std::string& windowTitle, unsigned majVer, 
		unsigned minVer, unsigned width, unsigned height)
	{
		glfwInit();
		this->_window = glfwCreateWindow(width, height,
			windowTitle.c_str(), NULL, NULL);
		if (!_window)
		{
			LOG(LogLevel::FATAL, "Failed to create GLFW window!");
			glfwTerminate();
			throw std::runtime_error("Could not create GLFW window");
		}
		
		glfwMakeContextCurrent(_window);
		this->_glContext = GLContext(majVer, minVer, _window);
		this->_glContext.viewport(0, 0, width, height);
	}

	Application::~Application()
	{
		glfwTerminate();
	}
}