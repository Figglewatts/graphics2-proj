#ifndef MESH_H
#define MESH_H
#pragma once

#include <vector>

#include "framework/graphics/Shader.h"
#include "framework/math/Vertex.h"
#include "Texture2D.h"

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
		

	public:
		Mesh(std::vector<Vertex> verts, std::vector<unsigned> indices);
		~Mesh();

		const std::vector<Vertex>& verts() const { return _verts; }
		std::vector<Vertex>& verts() { return _verts; }
		const std::vector<unsigned>& indices() const { return _indices; }
		std::vector<unsigned>& indices() { return _indices; }
		void bind() const;
		void unbind() const;
		void render() const;
	};
}

#endif // MESH_H