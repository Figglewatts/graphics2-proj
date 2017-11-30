#ifndef RENDERABLE_H
#define RENDERABLE_H
#pragma once

#include <glm/glm.hpp>
#include "framework/graphics/Texture2D.h"
#include "framework/graphics/Shader.h"
#include "framework/graphics/Mesh.h"

namespace Framework
{
	class Renderable
	{
		Mesh *_pMesh;
		Shader *_pShader;
		Texture2D *_pTexture;
		glm::mat4 _model;
	public:
		Renderable(Mesh *mesh, Shader *shader, Texture2D *tex, glm::mat4 model = glm::mat4(1))
			: _pMesh(mesh), _pShader(shader), _pTexture(tex), _model(model) { }
		~Renderable();

		void set_shader(Shader* shader) { this->_pShader = shader; }
		void set_texture(Texture2D* tex) { this->_pTexture = tex; }
		glm::mat4& model() { return this->_model; }

		void draw(glm::mat4 view, glm::mat4 proj) const;
	};
}

#endif // RENDERABLE_H