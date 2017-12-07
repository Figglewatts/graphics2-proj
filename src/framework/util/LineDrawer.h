#ifndef LINEDRAWER_H
#define LINEDRAWER_H
#pragma once

#include "framework/graphics/Shader.h"

namespace Framework
{
	class Mesh;
	struct AABB;

	class LineDrawer
	{
	private:
		LineDrawer() = delete;
		LineDrawer(const LineDrawer& other) = delete;
		LineDrawer& operator=(const LineDrawer& other) = delete;

		static Shader* _lineShader;
	public:
		static void init();
		static void drawLine(const Mesh& linePoints, const Color& col, glm::mat4 proj, glm::mat4 view);
	};
}

#endif // LINEDRAWER_H