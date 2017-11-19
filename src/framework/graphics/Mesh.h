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
		Shader *_pShader;
		Texture2D *_pTexture;
		glm::mat4 _model;

	public:
		Mesh(std::vector<Vertex> verts, std::vector<unsigned> indices, Shader *shader = nullptr, Texture2D *tex = nullptr);
		~Mesh();

		void draw(glm::mat4 view, glm::mat4 proj) const;
		std::vector<Vertex>& verts() { return _verts; }
		std::vector<unsigned>& indices() { return _indices; }
		void set_shader(Shader* shader) { this->_pShader = shader; }
		void set_texture(Texture2D* tex) { this->_pTexture = tex; }
		glm::mat4& model() { return this->_model; }
	};
}

#endif // MESH_H