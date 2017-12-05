#ifndef GJK_H
#define GJK_H
#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Vertex.h"

namespace Framework
{
	class GJK
	{
	private:
		GJK() = delete;
		GJK(const GJK& other) = delete;
		~GJK() = delete;
		GJK& operator=(const GJK& other) = delete;

		static glm::vec3 v, b, c, d;
		static unsigned n;

		static bool doSimplex(const glm::vec3& a);

		static bool line(const glm::vec3& a);
		static bool triangle(const glm::vec3& a);
		static bool tetrahedron(const glm::vec3& a);
		static bool tetrahedronFace(const glm::vec3& a, const glm::vec3& ao, const glm::vec3& ab, const glm::vec3& ac, const glm::vec3& abc);

		static glm::vec3 vec3_aba(const glm::vec3& A, const glm::vec3& B);

		static glm::vec3 furthestInDir(const std::vector<glm::vec3>& s, const glm::vec3& dir);

	public:
		static glm::vec3 support(std::vector<glm::vec3> A, std::vector<glm::vec3> B, glm::vec3 dir);
		static glm::vec3 getCenter(const std::vector<glm::vec3>& a);

		static bool intersect(const std::vector<glm::vec3>& A, const std::vector<glm::vec3>& B);
	};
}

#endif // GJK_H