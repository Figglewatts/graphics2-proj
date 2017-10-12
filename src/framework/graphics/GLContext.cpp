#include "GLContext.h"
#include "framework/util/Logger.h"
#include <stdexcept>

namespace Framework
{
	GLContext::GLContext(unsigned majVer, unsigned minVer, GLFWwindow *window)
		: _minVersion(minVer), _majVersion(majVer), _window(window)
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majVer);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minVer);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			LOG(LogLevel::FATAL, "Failed to initialize GLAD!");
			glfwTerminate();
			throw std::runtime_error("GLAD could not be initialized");
		}
	}

	void GLContext::viewport(int x, int y, unsigned w, unsigned h)
	{
		glViewport(x, y, w, h);
	}

	void GLContext::clearColor(const Color& color)
	{
		glClearColor(color.get_r(), color.get_g(), color.get_b(), color.get_a());
	}

	void GLContext::clear(GLbitfield mask)
	{
		glClear(mask);
	}
}