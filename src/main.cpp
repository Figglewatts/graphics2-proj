#include <iostream>

#include "framework/Application.h"
#include "framework/util/Logger.h"
#include "framework/graphics/Color.h"

Framework::Application *app;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	app->get_context().viewport(0, 0, width, height);
}

int main()
{
	app = new Framework::Application("Graphics 2 project",
		3, 2, 800, 600);

	app->get_context().clearColor(Framework::Color::BLUE);
	while (!glfwWindowShouldClose(app->get_window()))
	{
		glfwSwapBuffers(app->get_window());
		glfwPollEvents();
		app->get_context().clear(GL_COLOR_BUFFER_BIT);
	}
	
	delete app;
	return 0;
}