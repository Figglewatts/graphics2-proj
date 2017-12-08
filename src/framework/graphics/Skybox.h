#ifndef SKYBOX_H
#define SKYBOX_H
#pragma once

#include "Mesh.h"
#include "Cubemap.h"

namespace Framework
{
	class Skybox
	{
	private:
		Mesh *_pSkyboxMesh;
		Cubemap *_pCubemap;
		Shader *_pSkyboxShader;

		void createMesh();
	public:
		Skybox(Cubemap *cubemap);
		~Skybox();

		void render(glm::mat4 view, glm::mat4 proj);
	};
}

#endif // SKYBOX_H