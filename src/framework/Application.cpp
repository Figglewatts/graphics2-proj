#include "Application.h"
#include "framework/util/Logger.h"
#include <stdexcept>
#include <GLFW/glfw3.h>

namespace Framework
{
	Application::Application(const std::string& windowTitle, unsigned majVer, 
		unsigned minVer, unsigned width, unsigned height)
	{
		glfwInit();
		this->_glContext = GLContext(majVer, minVer, _window);
		this->_window = glfwCreateWindow(width, height,
			windowTitle.c_str(), NULL, NULL);
		if (!_window)
		{
			LOG(LogLevel::FATAL, "Failed to create GLFW window!");
			glfwTerminate();
			throw std::runtime_error("Could not create GLFW window");
		}
		glfwMakeContextCurrent(_window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			LOG(LogLevel::FATAL, "Failed to initialize GLAD!");
			glfwTerminate();
			throw std::runtime_error("GLAD could not be initialized");
		}
	}

	Application::~Application()
	{
		glfwTerminate();
	}
}