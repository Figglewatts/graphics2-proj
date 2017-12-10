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
#include "framework/graphics/Skybox.h"
using namespace Framework;

glm::ivec2 initialSize = { 800, 600 };

Framework::Application *app;
float xPos = 0;
float yPos = 0;

double t = 0.0;
const double dt = 1.0 / 60.0;
double currentTime = glfwGetTime();
double accumulator = 0.0;

Camera cam = Camera({ -20.f, 0, 0 }, { 0, 1, 0 }, 0, 0);
glm::mat4 projection;
DeferredRenderer *renderer;

Scene scene;
Skybox *spaceBackdrop;

Renderable *earth;
Renderable *spaceship;

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

void update(double delta)
{
	glm::vec3 camPos = cam.get_position();
	float speed = 0.2f;
	if (InputHandler::checkButton("Up", ButtonState::HELD))
	{
		//camPos += cam.front() * speed;
		spaceship->transform().translate(spaceship->transform().up() * speed);
	}
	else if (InputHandler::checkButton("Down", ButtonState::HELD))
	{
		//camPos -= cam.front() * speed;
		spaceship->transform().translate(spaceship->transform().up() * -speed);
	}

	if (InputHandler::checkButton("Left", ButtonState::HELD))
	{
		//camPos -= cam.right() * speed;
		spaceship->transform().translate(spaceship->transform().right() * -speed);
	}
	else if (InputHandler::checkButton("Right", ButtonState::HELD))
	{
		//camPos += cam.right() * speed;
		spaceship->transform().translate(spaceship->transform().right() * speed);
	}

	cam.set_position(spaceship->transform().position() + glm::vec3(0, 5, 0) - cam.front() * 40.f);
	//cam.set_rotation(-glm::degrees(spaceship->transform().rotEuler().y), -glm::degrees(spaceship->transform().rotEuler().x));

	float yaw = cam.yaw();
	float pitch = cam.pitch();
	if (InputHandler::checkButton("RotLeft", ButtonState::HELD))
	{
		//yaw -= speed;
		spaceship->transform().rotate({ 0, speed, 0});
	}
	else if (InputHandler::checkButton("RotRight", ButtonState::HELD))
	{
		//yaw += speed;
		spaceship->transform().rotate({ 0, -speed, 0 });
	}

	if (InputHandler::checkButton("RotUp", ButtonState::HELD))
	{
		//pitch += speed;
		spaceship->transform().rotate({ speed, 0, 0});
	}
	else if (InputHandler::checkButton("RotDown", ButtonState::HELD))
	{
		//pitch -= speed;
		spaceship->transform().rotate({-speed, 0, 0});
	}
	cam.set_rotation(yaw, pitch);
}

void init()
{
	InputHandler::addInput("Up", GLFW_KEY_UP);
	InputHandler::addInput("Down", GLFW_KEY_DOWN);
	InputHandler::addInput("Left", GLFW_KEY_LEFT);
	InputHandler::addInput("Right", GLFW_KEY_RIGHT);
	InputHandler::addInput("RotLeft", GLFW_KEY_A);
	InputHandler::addInput("RotRight", GLFW_KEY_D);
	InputHandler::addInput("RotUp", GLFW_KEY_W);
	InputHandler::addInput("RotDown", GLFW_KEY_S);

	renderer = new DeferredRenderer(800, 600, &app->get_context(), nullptr, nullptr);
	renderer->init();
	renderer->setCamera(&cam);
	renderer->setProjection(projection);

	LineDrawer::init();

	DirectionalLight dirLight;
	dirLight.diffuse = Color::WHITE;
	dirLight.ambient = { 0.4f, 0.4f, 0.4f, 1 }; // grey
	dirLight.direction = { 180, -45, 0 };
	dirLight.intensity = 0.005f;
	renderer->setDirLight(dirLight);

	Shader *shader = ResourceManager::Load<Shader>("assets/shaders/basicDeferred");
	Shader *planetShader = ResourceManager::Load<Shader>("assets/shaders/planetDeferred");

	Cubemap *spaceCubemap = ResourceManager::Load<Cubemap>("assets/textures/sky/space");
	spaceBackdrop = new Skybox(spaceCubemap);

	Mesh *planetMesh = ResourceManager::Load<Mesh>("assets/models/planet.obj");
	Texture2D *earthTex = ResourceManager::Load<Texture2D>("assets/textures/earthmap1k.png");
	Texture2D *earthSpecTex = ResourceManager::Load<Texture2D>("assets/textures/earthspec1k.png");
	Sphere *earthCollider = new Sphere();
	earthCollider->radius = 1;
	earth = new Renderable(planetMesh, planetShader, earthTex, earthCollider);
	earth->addTexture(earthSpecTex);
	earth->transform().scale(glm::vec3(4));
	earth->collisionResponse() = false;
	scene.add(earth);

	Mesh *spaceshipMesh = ResourceManager::Load<Mesh>("assets/models/spaceship.obj");
	Mesh *spaceshipColMesh = ResourceManager::Load<Mesh>("assets/models/spaceship-collider.obj");
	Texture2D *spaceshipTex = ResourceManager::Load<Texture2D>("assets/textures/spaceship.png");
	ConvexHull *spaceshipHull = new ConvexHull();
	spaceshipHull->verts = spaceshipColMesh->points();
	spaceship = new Renderable(spaceshipMesh, shader, spaceshipTex, spaceshipHull);
	spaceship->transform().translate({ -20, 0, 0 });
	scene.add(spaceship);
}

int main()
{
	app = new Framework::Application("Graphics 2 project",
		3, 2, initialSize.x, initialSize.y);

	projection = glm::perspective(glm::radians(46.f), (float)initialSize.x / (float)initialSize.y, 0.1f, 1000.f);;

	glfwSetFramebufferSizeCallback(app->get_window(), framebuffer_size_callback);
	glfwSetWindowSizeCallback(app->get_window(), framebuffer_size_callback);
	glfwSetKeyCallback(app->get_window(), key_callback);

	init();

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
			update(frameTime);
			accumulator -= dt;
			t += dt;
		}

		glClearColor(0.f, 0.f, 0.f, 1.f);

		glm::mat4 view = cam.view();

		renderer->beginFrame();
		
		scene.render(projection, view);

		renderer->endFrame();
		
		spaceBackdrop->render(view, projection);

		glfwSwapBuffers(app->get_window());
		glfwPollEvents();
	}
	
	delete spaceBackdrop;
	delete renderer;
	delete app;
	return 0;
}