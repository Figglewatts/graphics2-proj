#include "Renderable.h"

namespace Framework
{
	Renderable::~Renderable()
	{
		_pMesh->unbind();
		_pShader->unbind();
		_pTexture->unbind();
	}

	void Renderable::draw(glm::mat4 view, glm::mat4 proj) const
	{
		_pShader->bind();
		_pShader->setUniform("model", _model, false);
		_pShader->setUniform("view", view, false);
		_pShader->setUniform("projection", proj, false);
		_pTexture->bind();
		_pMesh->render();
		_pShader->unbind();
		_pTexture->unbind();
	}
}