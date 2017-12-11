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
#include "game/entities/Planet.h"
#include "game/entities/Spaceship.h"
using namespace Framework;

glm::ivec2 initialSize = { 1280, 720 };
glm::ivec2 currentSize;

Framework::Application *app;
float xPos = 0;
float yPos = 0;

double t = 0.0;
const double dt = 1.0 / 60.0;
double currentTime = glfwGetTime();
double accumulator = 0.0;

Camera shipCam = Camera(glm::vec3(-20.f, 0, 0));
Camera earthCam = Camera(glm::vec3(0));
Camera pilotCam = Camera(glm::vec3(0));
Camera *currentCamera = &shipCam;
glm::mat4 projection;
DeferredRenderer *renderer;

float pilotViewHorizAngle = 0;
float pilotViewVertAngle = 0;
float mouseSpeed = 0.0010f;
glm::vec3 pilotPos;

Scene scene;
Skybox *spaceBackdrop;

Planet *earth;
Spaceship *spaceship;
Renderable *spaceshipCockpit;

PointLight *cockpitLight1;
PointLight *cockpitLight2;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	app->get_context().viewport(0, 0, width, height);
	projection = glm::perspective(glm::radians(45.f), (float)width / (float)height, 0.1f, 10000.f);
	renderer->resize({ width, height });
	currentSize = { width, height };
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	glfwSetCursorPos(window, currentSize.x / 2, currentSize.y / 2);
	pilotViewHorizAngle += mouseSpeed * float(currentSize.x / 2 - xpos);
	pilotViewVertAngle += mouseSpeed * float(currentSize.y / 2 - ypos);
	pilotViewVertAngle = glm::clamp(pilotViewVertAngle, glm::radians(-70.f), glm::radians(70.f));
	glm::vec3 viewdir = glm::vec3(
		cos(pilotViewVertAngle) * sin(pilotViewHorizAngle),
		sin(pilotViewVertAngle),
		cos(pilotViewVertAngle) * cos(pilotViewHorizAngle)
	);
	spaceship->setPilotViewDir(viewdir);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	InputHandler::updateKeys(key, action);
}

void checkCameraKeys()
{
	if (InputHandler::checkButton("Camera1", ButtonState::DOWN))
	{
		currentCamera = &shipCam;
		spaceship->setActive(true);
		spaceshipCockpit->setActive(false);
	}
	else if (InputHandler::checkButton("Camera2", ButtonState::DOWN))
	{
		currentCamera = &earthCam;
		spaceship->setActive(true);
		spaceshipCockpit->setActive(false);
	}
	else if (InputHandler::checkButton("Camera3", ButtonState::DOWN))
	{
		pilotViewHorizAngle = 0;
		pilotViewVertAngle = 0;
		glm::vec3 viewdir = glm::vec3(
			cos(pilotViewVertAngle) * sin(pilotViewHorizAngle),
			sin(pilotViewVertAngle),
			cos(pilotViewVertAngle) * cos(pilotViewHorizAngle)
		);
		spaceship->setPilotViewDir(viewdir);

		currentCamera = &pilotCam;
		spaceship->setActive(false);
		spaceshipCockpit->setActive(true);
	}
	renderer->setCamera(currentCamera);
}

void update(double delta)
{
	shipCam.transform().setPosition(spaceship->transform().position() - (spaceship->transform().forward() * 50.f * (1 + abs(spaceship->getCurrentSpeed()) / 4)) + (spaceship->transform().up() * 10.f));
	shipCam.setLookAt(spaceship->transform().position(), spaceship->transform().up());
	
	earthCam.setLookAt(spaceship->transform().position(), glm::vec3(0, 1, 0));

	pilotPos = spaceship->transform().position() + (spaceship->transform().forward() * 7.08f) + (spaceship->transform().up() * 0.6f) + (spaceship->transform().right() * 0.25f);

	cockpitLight1->position = pilotPos;
	cockpitLight2->position = pilotPos - spaceship->transform().right() * 0.5f;

	pilotCam.transform().setPosition(pilotPos);
	pilotCam.setLookAt(pilotPos + spaceship->pilotViewDir(), spaceship->transform().up());

	spaceshipCockpit->transform().setPosition(spaceship->transform().position());
	spaceshipCockpit->transform().setRotation(spaceship->transform().rotation());

	if (InputHandler::checkButton("ExitGame", ButtonState::UP))
	{
		glfwSetWindowShouldClose(app->get_window(), GLFW_TRUE);
	}

	checkCameraKeys();
}

void init()
{
	InputHandler::addInput("SpeedUp", GLFW_KEY_LEFT_SHIFT);
	InputHandler::addInput("SpeedDown", GLFW_KEY_LEFT_CONTROL);
	InputHandler::addInput("RollLeft", GLFW_KEY_LEFT);
	InputHandler::addInput("RollRight", GLFW_KEY_RIGHT);
	InputHandler::addInput("PitchUp", GLFW_KEY_UP);
	InputHandler::addInput("PitchDown", GLFW_KEY_DOWN);
	InputHandler::addInput("Camera1", GLFW_KEY_1);
	InputHandler::addInput("Camera2", GLFW_KEY_2);
	InputHandler::addInput("Camera3", GLFW_KEY_3);
	InputHandler::addInput("ExitGame", GLFW_KEY_ESCAPE);

	renderer = new DeferredRenderer(initialSize.x, initialSize.y, &app->get_context(), nullptr, nullptr);
	renderer->init();
	renderer->setCamera(&shipCam);
	renderer->setProjection(projection);

	LineDrawer::init();

	DirectionalLight dirLight;
	dirLight.diffuse = Color::WHITE;
	dirLight.ambient = { 0.4f, 0.4f, 0.4f, 1 }; // grey
	dirLight.direction = { 180, -45, 0 };
	dirLight.intensity = 0.005f;
	renderer->setDirLight(dirLight);

	cockpitLight1 = new PointLight();
	cockpitLight1->diffuse = Color::WHITE;
	cockpitLight1->intensity = 1;
	cockpitLight1->linearAttenuation = 0.001f;
	renderer->pointLights().push_back(cockpitLight1);

	cockpitLight2 = new PointLight();
	cockpitLight2->diffuse = Color::RED;
	cockpitLight2->intensity = 1;
	cockpitLight2->linearAttenuation = 0.1f;
	renderer->pointLights().push_back(cockpitLight2);

	Shader *shader = ResourceManager::Load<Shader>("assets/shaders/basicDeferred");
	Shader *planetShader = ResourceManager::Load<Shader>("assets/shaders/planetDeferred");

	Cubemap *spaceCubemap = ResourceManager::Load<Cubemap>("assets/textures/sky/space");
	spaceBackdrop = new Skybox(spaceCubemap);

	Mesh *planetMesh = ResourceManager::Load<Mesh>("assets/models/planet-highp.obj");
	Texture2D *earthTex = ResourceManager::Load<Texture2D>("assets/textures/earthmap1k.png");
	Texture2D *earthSpecTex = ResourceManager::Load<Texture2D>("assets/textures/earthspec1k.png");
	earth = new Planet(planetMesh, planetShader, earthTex, earthSpecTex, 1000);
	earth->transform().rotate({ 0, glm::radians(90.f), 0 });
	scene.add(earth);

	earthCam.transform().setPosition(earth->transform().position());

	Mesh *spaceshipMesh = ResourceManager::Load<Mesh>("assets/models/spaceship.obj");
	Mesh *spaceshipColMesh = ResourceManager::Load<Mesh>("assets/models/spaceship-collider.obj");
	Texture2D *spaceshipTex = ResourceManager::Load<Texture2D>("assets/textures/spaceship.png");
	spaceship = new Spaceship(spaceshipMesh, shader, spaceshipTex, spaceshipColMesh);
	spaceship->transform().translate({ -10000, 0, 0 });
	scene.add(spaceship);

	Mesh *spaceshipCockpitMesh = ResourceManager::Load<Mesh>("assets/models/spaceship-cockpit.obj");
	Texture2D *spaceshipCockpitTex = ResourceManager::Load<Texture2D>("assets/textures/spaceship-cockpit.png");
	spaceshipCockpit = new Renderable(spaceshipCockpitMesh, shader, spaceshipCockpitTex, false);
	spaceshipCockpit->setActive(false);
	scene.add(spaceshipCockpit);

	spaceship->setPilotViewDir(spaceship->transform().forward());
}

int main()
{
	app = new Framework::Application("Graphics 2 Project - Sam Gibson (100115871)",
		3, 2, initialSize.x, initialSize.y);
	currentSize = initialSize;

	projection = glm::perspective(glm::radians(45.f), (float)initialSize.x / (float)initialSize.y, 0.1f, 10000.f);;

	glfwSetFramebufferSizeCallback(app->get_window(), framebuffer_size_callback);
	glfwSetWindowSizeCallback(app->get_window(), framebuffer_size_callback);
	glfwSetKeyCallback(app->get_window(), key_callback);
	glfwSetCursorPosCallback(app->get_window(), cursor_position_callback);
	glfwSetInputMode(app->get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

		glm::mat4 view = currentCamera->view();

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