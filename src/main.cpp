#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "framework/Application.h"
#include "framework/input/InputHandler.h"
#include "framework/graphics/Shader.h"

#include "framework/io/ResourceManager.h"
#include "framework/graphics/Camera.h"
#include "framework/rendering/DeferredRenderer.h"
#include "framework/util/LineDrawer.h"
#include "framework/rendering/Scene.h"
using namespace Framework;

glm::ivec2 initialSize = { 800, 600 };

Framework::Application *app;
float xPos = 0;
float yPos = 0;

double t = 0.0;
const double dt = 1.0 / 60.0;
double currentTime = glfwGetTime();
double accumulator = 0.0;

Camera cam = Camera({ -5.f, 0, 0 }, { 0, 1, 0 }, 0, 0);
glm::mat4 projection;
DeferredRenderer *renderer;

Scene scene;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	app->get_context().viewport(0, 0, width, height);
	projection = glm::perspective(glm::radians(45.f), (float)width / (float)height, 0.1f, 1000.f);
	renderer->resize({ width, height });
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	InputHandler::updateKeys(key, action);
}

int main()
{
	app = new Framework::Application("Graphics 2 project",
		3, 2, initialSize.x, initialSize.y);

	projection = glm::perspective(glm::radians(46.f), (float)initialSize.x / (float)initialSize.y, 0.1f, 1000.f);;

	glfwSetFramebufferSizeCallback(app->get_window(), framebuffer_size_callback);
	glfwSetWindowSizeCallback(app->get_window(), framebuffer_size_callback);
	glfwSetKeyCallback(app->get_window(), key_callback);

	InputHandler::addInput("Up", GLFW_KEY_UP);
	InputHandler::addInput("Down", GLFW_KEY_DOWN);
	InputHandler::addInput("Left", GLFW_KEY_LEFT);
	InputHandler::addInput("Right", GLFW_KEY_RIGHT);
	InputHandler::addInput("RotLeft", GLFW_KEY_A);
	InputHandler::addInput("RotRight", GLFW_KEY_D);

	renderer = new DeferredRenderer(800, 600, &app->get_context(), nullptr, nullptr);
	renderer->init();
	renderer->setCamera(&cam);
	renderer->setProjection(projection);

	LineDrawer::init();

	DirectionalLight dirLight;
	dirLight.diffuse = Color::WHITE;
	dirLight.ambient = {0.4f, 0.4f, 0.4f, 1}; // grey
	dirLight.direction = { 180, 0, 0 };
	dirLight.intensity = 2;
	renderer->setDirLight(dirLight);

	Shader *shader = ResourceManager::Load<Shader>("assets/shaders/basicDeferred");

	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	while (!glfwWindowShouldClose(app->get_window()))
	{
		double newTime = glfwGetTime();
		double frameTime = newTime - currentTime;
		currentTime = newTime;

		accumulator += frameTime;

		// update as fast as we can, only render at specified framerate
		while (accumulator >= dt)
		{
			InputHandler::handleInput();
			scene.update(frameTime);
			accumulator -= dt;
			t += dt;
		}

		glClearColor(0.f, 0.f, 0.f, 1.f);

		glm::mat4 view = cam.view();

		renderer->beginFrame();
		
		scene.render(projection, view);

		renderer->endFrame();
		
		glfwSwapBuffers(app->get_window());
		glfwPollEvents();
	}
	
	delete renderer;
	delete app;
	return 0;
}