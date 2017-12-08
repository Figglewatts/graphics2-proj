#include "Skybox.h"
#include "framework/io/resourcemanager.h"

namespace Framework
{
	void Skybox::createMesh()
	{
		_pSkyboxMesh = ResourceManager::Load<Mesh>("assets/models/skybox.obj");
	}

	Skybox::Skybox(Cubemap * cubemap)
		: _pCubemap(cubemap)
	{
		createMesh();
		_pSkyboxShader = ResourceManager::Load<Shader>("assets/shaders/skybox");
	}

	Skybox::~Skybox()
	{
	}

	void Skybox::render(glm::mat4 view, glm::mat4 proj)
	{
		glDepthFunc(GL_LEQUAL);
		_pSkyboxShader->bind();
		_pSkyboxShader->setUniform("ProjectionMatrix", proj, false);
		_pSkyboxShader->setUniform("ViewMatrix", glm::mat4(glm::mat3(view)), false); // conversions here are to drop translation info
		_pSkyboxMesh->bind();
		_pCubemap->bind();
		_pSkyboxMesh->render();
		_pCubemap->unbind();
		_pSkyboxMesh->unbind();
		_pSkyboxShader->unbind();
		glDepthFunc(GL_LESS);
	}
}
