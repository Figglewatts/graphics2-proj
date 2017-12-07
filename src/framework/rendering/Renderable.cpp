#include "Renderable.h"

namespace Framework
{
	AABB Renderable::calculateBoundingVolume() const
	{
		glm::vec3 max = glm::vec3(0);
		glm::vec3 min = glm::vec3(0);
		for (const auto& v : _pMesh->verts())
		{
			if (v.Position.x > max.x) max.x = v.Position.x;
			if (v.Position.y > max.y) max.y = v.Position.y;
			if (v.Position.z > max.z) max.z = v.Position.z;
			if (v.Position.x < min.x) min.x = v.Position.x;
			if (v.Position.y < min.y) min.y = v.Position.y;
			if (v.Position.z < min.z) min.z = v.Position.z;
		}
		return { min, max };
	}

	Renderable::Renderable(Mesh * mesh, Shader * shader, Texture2D * tex, bool collision, Transform t)
		: _pMesh(mesh), _pShader(shader), _pTexture(tex), _collision(collision), _transform(t)
	{
		if (_collision)
		{
			ConvexHull *collider = new ConvexHull();
			collider->verts = rigidbody_points();
			_pRigidbody = new Rigidbody(std::unique_ptr<Shape>(collider), &_transform);
		}
	}

	Renderable::Renderable(Mesh * mesh, Shader * shader, Texture2D * tex, Shape *collisionShape, Transform t)
		: _pMesh(mesh), _pShader(shader), _pTexture(tex), _collision(true), _transform(t)
	{
		_pRigidbody = new Rigidbody(std::unique_ptr<Shape>(collisionShape), &_transform);
	}

	Renderable::~Renderable()
	{
		_pMesh->unbind();
		_pShader->unbind();
		_pTexture->unbind();
		delete _pRigidbody;
	}

	const std::vector<glm::vec3> Renderable::rigidbody_points() const
	{
		std::vector<glm::vec3> _rigidbody;
		for (const auto& v : _pMesh->verts())
		{
			//const glm::vec4 worldPos = glm::vec4(v.Position, 1) * glm::transpose(_transform.matrix());
			_rigidbody.push_back(v.Position);
		}
		return _rigidbody;
	}

	void Renderable::draw(glm::mat4 view, glm::mat4 proj) const
	{
		_pShader->bind();
		_pShader->setUniform("model", _transform.matrix(), false);
		_pShader->setUniform("view", view, false);
		_pShader->setUniform("projection", proj, false);
		_pTexture->bind();
		_pMesh->render();
		_pShader->unbind();
		_pTexture->unbind();
	}
}