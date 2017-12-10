#ifndef QUICKHULL_H
#define QUICKHULL_H
#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "framework/math/Vertex.h"

namespace Framework
{
	struct Face
	{
		int v1, v2, v3;
		std::vector<int> points;

		Face(int a, int b, int c)
			: v1(a), v2(b), v3(c) {}
	};
	
	class Quickhull
	{
	private:
		Quickhull() = delete;
		Quickhull(const Quickhull& other) = delete;
		~Quickhull() = delete;
		Quickhull& operator=(const Quickhull& other) = delete;

		static float dist2LineSegment(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
		static glm::vec3 planeNormal(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);

	public:
		static std::vector<glm::vec3> generateHull(std::vector<Vertex>& verts);
	};
}

#endif // QUICKHULL_H