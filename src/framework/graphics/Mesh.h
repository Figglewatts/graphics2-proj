#ifndef MESH_H
#define MESH_H
#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "framework/graphics/Shader.h"
#include "framework/math/Vertex.h"

namespace Framework
{
	class Mesh
	{
	private:
		std::vector<Vertex> _verts;
		std::vector<unsigned> _indices;
		unsigned _vao;
		unsigned _vbo;
		unsigned _ebo;
		Shader *_pShader;

	public:
		Mesh(std::vector<Vertex> verts, std::vector<unsigned> indices, Shader *shader);
		~Mesh();

		void draw() const;
	};
}

#endif // MESH_H