#include <iostream>

#include "framework/Application.h"
#include "framework/util/Logger.h"
#include "framework/graphics/Color.h"
#include "framework/input/InputHandler.h"
using namespace Framework;

Framework::Application *app;
float xPos = 0;
float yPos = 0;

double t = 0.0;
const double dt = 1.0 / 60.0;
double currentTime = glfwGetTime();
double accumulator = 0.0;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	app->get_context().viewport(0, 0, width, height);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	InputHandler::updateKeys(key, action);
}

void render_sample(Application *app, float x, float y)
{
	float ratio;
	int width, height;
	glfwGetFramebufferSize(app->get_window(), &width, &height);
	ratio = width / (float)height;
	app->get_context().viewport(0, 0, width, height);
	app->get_context().clear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x, y, 0);
	//glRotatef((float)glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
	glBegin(GL_TRIANGLES);
	glColor3f(1.f, 0.f, 0.f);
	glVertex3f(-0.6f, -0.4f, 0.f);
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(0.6f, -0.4f, 0.f);
	glColor3f(0.f, 0.f, 1.f);
	glVertex3f(0.f, 0.6f, 0.f);
	glEnd();

	glfwSwapBuffers(app->get_window());
	glfwPollEvents();
}

void update()
{
	if (InputHandler::checkButton("Up", ButtonState::HELD))
	{
		yPos += 0.01F;
	}
	else if (InputHandler::checkButton("Down", ButtonState::HELD))
	{
		yPos -= 0.01F;
	}

	if (InputHandler::checkButton("Left", ButtonState::HELD))
	{
		xPos -= 0.01F;
	}
	else if (InputHandler::checkButton("Right", ButtonState::HELD))
	{
		xPos += 0.01F;
	}
}

int main()
{
	app = new Framework::Application("Graphics 2 project",
		3, 2, 800, 600);

	glfwSetFramebufferSizeCallback(app->get_window(), framebuffer_size_callback);
	glfwSetKeyCallback(app->get_window(), key_callback);

	InputHandler::addInput("Up", GLFW_KEY_UP);
	InputHandler::addInput("Down", GLFW_KEY_DOWN);
	InputHandler::addInput("Left", GLFW_KEY_LEFT);
	InputHandler::addInput("Right", GLFW_KEY_RIGHT);

	app->get_context().clearColor(Framework::Color::RED);
	while (!glfwWindowShouldClose(app->get_window()))
	{
		double newTime = glfwGetTime();
		double frameTime = newTime - currentTime;
		currentTime = newTime;

		accumulator += frameTime;

		// only update at 60fps
		while (accumulator >= dt)
		{
			InputHandler::handleInput();
			update();
			accumulator -= dt;
			t += dt;
		}

		render_sample(app, xPos, yPos);

		std::cout << "FPS: " << (1.0 / frameTime) << " dt:" << frameTime << std::endl;
	}
	
	delete app;
	return 0;
}