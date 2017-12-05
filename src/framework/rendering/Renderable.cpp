#include "Renderable.h"

namespace Framework
{
	Renderable::~Renderable()
	{
		_pMesh->unbind();
		_pShader->unbind();
		_pTexture->unbind();
	}

	const std::vector<glm::vec3> Renderable::rigidbody() const
	{
		std::vector<glm::vec3> _rigidbody;
		for (const auto& v : _pMesh->verts())
		{
			const glm::vec4 worldPos = glm::vec4(v.Position, 1) * glm::transpose(_transform.matrix());
			_rigidbody.push_back(worldPos);
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