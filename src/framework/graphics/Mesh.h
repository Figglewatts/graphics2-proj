#ifndef MESH_H
#define MESH_H
#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "framework/graphics/Shader.h"

namespace Framework
{
	class Mesh
	{
	private:
		std::vector<glm::vec3> _verts;
		std::vector<unsigned> _indices;
		unsigned _vao;
		unsigned _vbo;
		unsigned _ebo;
		Shader *_pShader;

	public:
		Mesh(std::vector<glm::vec3> verts, std::vector<unsigned> indices);
		~Mesh();

		void draw(glm::mat4 proj, glm::mat4 view) const;
	};
}

#endif // MESH_H