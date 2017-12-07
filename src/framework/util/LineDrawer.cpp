#include "LineDrawer.h"

#include "framework/io/resourcemanager.h"

namespace Framework
{
	Shader* LineDrawer::_lineShader;

	void LineDrawer::init()
	{
		_lineShader = ResourceManager::Load<Shader>("assets/shaders/line");
	}

	void LineDrawer::drawLine(const Mesh& linePoints, const Color& col, glm::mat4 proj, glm::mat4 view)
	{
		linePoints.bind();
		_lineShader->bind();
		_lineShader->setUniform("ProjectionMatrix", proj, false);
		_lineShader->setUniform("ViewMatrix", view, false);
		_lineShader->setUniform("LineColor", col);
		glDrawElements(GL_LINE_STRIP, linePoints.indices().size(), GL_UNSIGNED_INT, nullptr);
		_lineShader->unbind();
		linePoints.unbind();
	}
}
