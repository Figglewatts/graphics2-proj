#include "Renderable.h"

namespace Framework
{
	Camera **Renderable::activeCamera;
	
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
		: _pMesh(mesh), _pShader(shader), _collision(collision), _transform(t)
	{
		if (_collision)
		{
			ConvexHull *collider = new ConvexHull();
			collider->verts = rigidbody_points();
			_pRigidbody = new Rigidbody(std::unique_ptr<Shape>(collider), &_transform);
		}
		_textures.push_back(tex);
	}

	Renderable::Renderable(Mesh * mesh, Shader * shader, Texture2D * tex, Shape *collisionShape, Transform t)
		: _pMesh(mesh), _pShader(shader), _collision(true), _transform(t)
	{
		_pRigidbody = new Rigidbody(std::unique_ptr<Shape>(collisionShape), &_transform);
		_textures.push_back(tex);
	}

	Renderable::~Renderable()
	{
		_pMesh->unbind();
		_pShader->unbind();
		for (const auto& tex : _textures)
		{
			tex->unbind();
		}
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
		_pShader->setUniform("ModelMatrix", _transform.matrix(), false);
		_pShader->setUniform("ViewMatrix", view, false);
		_pShader->setUniform("ProjectionMatrix", proj, false);
		_pShader->setUniform("tex1", 0);
		_pShader->setUniform("tex2", 1);
		_pShader->setUniform("tex3", 2);
		_pShader->setUniform("tex4", 3);
		_pShader->setUniform("ViewPos", (*activeCamera)->transform().position());
		for (int i = 0; i < _textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			_textures[i]->bind();
		}
		_pMesh->render();
		_pShader->unbind();
		for (const auto& tex : _textures)
		{
			tex->unbind();
		}
		glActiveTexture(GL_TEXTURE0);
	}
}