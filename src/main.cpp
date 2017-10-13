#include <iostream>

#include "framework/Application.h"
#include "framework/util/Logger.h"
#include "framework/graphics/Color.h"
#include "framework/input/InputHandler.h"

Framework::Application *app;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	app->get_context().viewport(0, 0, width, height);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	Framework::InputHandler::updateKeys(key, action);
}

int main()
{
	app = new Framework::Application("Graphics 2 project",
		3, 2, 800, 600);

	glfwSetFramebufferSizeCallback(app->get_window(), framebuffer_size_callback);
	glfwSetKeyCallback(app->get_window(), key_callback);

	Framework::InputHandler::addInput("Test", GLFW_KEY_SPACE);

	app->get_context().clearColor(Framework::Color::RED);
	while (!glfwWindowShouldClose(app->get_window()))
	{
		glfwSwapBuffers(app->get_window());
		glfwPollEvents();
		app->get_context().clear(GL_COLOR_BUFFER_BIT);

		Framework::InputHandler::handleInput();

		if (Framework::InputHandler::checkButton("Test", Framework::ButtonState::HELD))
		{
			std::cout << "Space held!" << std::endl;
		}
	}
	
	delete app;
	return 0;
}