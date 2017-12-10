/*
 * GJK code adapted from:
 * http://in2gpu.com/2014/05/18/gjk-algorithm-3d/
 * 
 * EPA code adapted from:
 * 
 */

#ifndef GJK_H
#define GJK_H
#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "framework/math/Vertex.h"
#include "Rigidbody.h"

namespace Framework
{
	struct SupportPoint
	{
		glm::vec3 v;

		glm::vec3 supA;
		glm::vec3 supB;

		bool operator==(const SupportPoint& other) const { return v == other.v; }
	};

	struct Triangle
	{
		SupportPoint points[3];
		glm::vec3 n;

		Triangle(const SupportPoint& a, const SupportPoint& b, const SupportPoint& c)
		{
			points[0] = a;
			points[1] = b;
			points[2] = c;
			n = glm::normalize(glm::cross(b.v - a.v, c.v - a.v));
		}
	};

	struct Edge
	{
		SupportPoint points[2];

		Edge(const SupportPoint& a, const SupportPoint& b)
		{
			points[0] = a;
			points[1] = b;
		}
	};

	struct Collision
	{
		glm::vec3 point;
		glm::vec3 normal;
		float depth;
	};
	
	class GJK
	{
	private:
		GJK() = delete;
		GJK(const GJK& other) = delete;
		~GJK() = delete;
		GJK& operator=(const GJK& other) = delete;

		static const int GJK_MAX_STEPS = 64;
		static const int EPA_MAX_STEPS = 92;
		static const float EPA_EXIT_THRESHOLD;
		static const float GJK_EPSILON;

		static SupportPoint b, c, d;
		static glm::vec3 v;
		static unsigned n;
		static std::vector<Triangle> epa_triangles;
		static std::vector<Edge> epa_edges;

		static bool doSimplex(const SupportPoint& a);

		static bool line(const SupportPoint& a);
		static bool triangle(const SupportPoint& a);
		static bool tetrahedron(const SupportPoint& a);
		static bool tetrahedronFace(const SupportPoint& a, const glm::vec3& ao, const glm::vec3& ab, const glm::vec3& ac, const glm::vec3& abc);

		static glm::vec3 vec3_aba(const glm::vec3& A, const glm::vec3& B);

		static SupportPoint support(const Rigidbody& A, const Rigidbody& B, glm::vec3 dir);

		static bool gjk_test_collision(const Rigidbody& A, const Rigidbody& B, SupportPoint& a);

		static void epa_addEdge(const SupportPoint& p1, const SupportPoint& p2);

		static Collision generate_collision_data(Triangle& t);

		static glm::vec3 barycentric(const glm::vec3& p, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);

	public:
		static bool intersect(const Rigidbody& A, const Rigidbody& B);
		static bool intersect(const Rigidbody& A, const Rigidbody& B, Collision* col);

	};
}

#endif // GJK_H