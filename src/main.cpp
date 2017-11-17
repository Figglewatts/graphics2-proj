#include <iostream>

#include "framework/Application.h"
#include "framework/util/Logger.h"
#include "framework/input/InputHandler.h"
#include "framework/graphics/Shader.h"

#include "framework/io/ResourceManager.h"
#include "framework/graphics/Mesh.h"
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

	Shader *shader = ResourceManager::Load<Shader>("assets/shaders/basic");

	std::vector<Vertex> vertices = {
		Vertex(glm::vec3({0.5f,  0.5f, 0.0f}), glm::vec3(0), glm::vec2(0), glm::vec4({1.f, 0.f, 0.f, 1.f})),  // top right
		Vertex(glm::vec3({ 0.5f,  0.5f, 0.0f }), glm::vec3(0), glm::vec2(0), glm::vec4({ 0.f, 1.f, 0.f, 1.f})),  // bottom right
		Vertex(glm::vec3({ 0.5f,  0.5f, 0.0f }), glm::vec3(0), glm::vec2(0), glm::vec4({ 0.f, 0.f, 1.f, 1.f})),  // bottom left
		Vertex(glm::vec3({ 0.5f,  0.5f, 0.0f }), glm::vec3(0), glm::vec2(0), glm::vec4({ 1.0f, 0.f, 0.5f, 1.f}))   // top left 
	};
	std::vector<unsigned> indices = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	Mesh m = Mesh(vertices, indices, shader);

	while (!glfwWindowShouldClose(app->get_window()))
	{
		double newTime = glfwGetTime();
		double frameTime = newTime - currentTime;
		currentTime = newTime;

		accumulator += frameTime;

		// only update at specified delta-time
		while (accumulator >= dt)
		{
			InputHandler::handleInput();
			update();
			accumulator -= dt;
			t += dt;
		}

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw our first triangle
		glUseProgram(shader->handle());

		m.draw();
		
		glfwSwapBuffers(app->get_window());
		glfwPollEvents();

		//std::cout << "FPS: " << (1.0 / frameTime) << " dt:" << frameTime << std::endl;
	}
	
	delete app;
	return 0;
}