#ifndef RENDERABLE_H
#define RENDERABLE_H
#pragma once

#include "framework/graphics/Texture2D.h"
#include "framework/graphics/Shader.h"
#include "framework/graphics/Mesh.h"
#include "framework/math/Transform.h"
#include "framework/physics/Rigidbody.h"
#include "framework/physics/GJK.h"

namespace Framework
{
	class Renderable
	{
	protected:
		Mesh *_pMesh;
		Shader *_pShader;
		std::vector<Texture2D*> _textures;
		Transform _transform;
		bool _collision;
		Rigidbody *_pRigidbody;

		AABB calculateBoundingVolume() const;
	public:
		Renderable(Mesh *mesh, Shader *shader, Texture2D *tex, Transform t = Transform())
			: _pMesh(mesh), _pShader(shader), _transform(t), _collision(false), _pRigidbody(nullptr)
		{
			_textures.push_back(tex);
		}
		Renderable(Mesh *mesh, Shader *shader, Texture2D *tex, bool collision, Transform t = Transform());
		Renderable(Mesh *mesh, Shader *shader, Texture2D *tex, Shape *collisionShape, Transform t = Transform());
		virtual ~Renderable();

		void set_shader(Shader* shader) { this->_pShader = shader; }
		Transform& transform() { return this->_transform; }
		const std::vector<glm::vec3> rigidbody_points() const;
		Rigidbody *rigidbody() const { return _pRigidbody; }
		bool collideable() const { return _collision; }
		void addTexture(Texture2D *tex) { _textures.push_back(tex); }
		std::vector<Texture2D*>& textures() { return _textures; }

		virtual void draw(glm::mat4 view, glm::mat4 proj) const;
		virtual void update(double delta) {}
		virtual void onCollide(Collision c, const Rigidbody *body)
		{
			_transform.translate(c.normal * c.depth);
		}
	};
}

#endif // RENDERABLE_H