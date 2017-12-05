#ifndef RENDERABLE_H
#define RENDERABLE_H
#pragma once

#include <glm/glm.hpp>
#include "framework/graphics/Texture2D.h"
#include "framework/graphics/Shader.h"
#include "framework/graphics/Mesh.h"
#include "framework/math/Transform.h"

namespace Framework
{
	class Renderable
	{
		Mesh *_pMesh;
		Shader *_pShader;
		Texture2D *_pTexture;
		Transform _transform;
	public:
		Renderable(Mesh *mesh, Shader *shader, Texture2D *tex, Transform t = Transform())
			: _pMesh(mesh), _pShader(shader), _pTexture(tex), _transform(t) { }
		~Renderable();

		void set_shader(Shader* shader) { this->_pShader = shader; }
		void set_texture(Texture2D* tex) { this->_pTexture = tex; }
		Transform& transform() { return this->_transform; }
		const std::vector<glm::vec3> rigidbody() const;

		void draw(glm::mat4 view, glm::mat4 proj) const;
	};
}

#endif // RENDERABLE_H