#include "GJK.h"

#include <algorithm>

namespace Framework
{
	glm::vec3 GJK::v, GJK::b, GJK::c, GJK::d; // direction and vertices in Minkowski space
	unsigned GJK::n = 0; // simplex type

	bool GJK::doSimplex(const glm::vec3& a)
	{
		switch (n)
		{
			case 0: // point case
			{
				b = a;
				v = -a; // search in opposite direction to find something with highest chance of enclosing origin
				n = 1;

				return false; // cannot enclose with one point
			}
			case 1: // line case
			{
				return line(a);
			}
			case 2: // triangle case
			{
				return triangle(a);
			}
			case 3: // tetrahedron case
			{
				return tetrahedron(a);
			}
			default:
			{
				throw std::runtime_error("ERROR: Unsupported simplex size");
			}
		}
	}

	bool GJK::line(const glm::vec3& a)
	{
		// search direction is perpendicular to the edge of the 1-simplex
		v = vec3_aba(b - a, -a); // (AB x AO) x AB

		c = b;
		b = a;
		n = 2;

		return false; // cannot enclose with a 1-simplex
	}

	bool GJK::triangle(const glm::vec3& a)
	{
		glm::vec3 ao = -a; // vector from A to origin (Minkowski space)

		// compute edges
		glm::vec3 ab = b - a;
		glm::vec3 ac = c - a;

		// compute triangle normal
		glm::vec3 abc = glm::cross(ab, ac);

		// compute edge direction of AB
		glm::vec3 abN = glm::cross(ab, abc);
		if (glm::dot(abN, ao) > 0) // is direction to origin similar to AB normal?
		{
			// origin is outside triangle near AB
			// build new triangle
			c = b;
			b = a;
			v = vec3_aba(ab, ao); // in dir of origin

			return false;
		}

		// compute edge direction of AC
		glm::vec3 acN = glm::cross(abc, ac);
		if (glm::dot(acN, ao) > 0) // is dir to origin similar to AC normal?
		{
			// c stays same
			b = a;
			v = vec3_aba(ac, ao); // in dir of origin

			return false;
		}

		// if we get here, then origin is within infinitely long
		// triangular prism along triangle's normal -- is it above or below?
		if (glm::dot(abc, ao) > 0)
		{
			// origin is above, build tetrahedron
			d = c;
			c = b;
			b = a;
			v = abc;
		}
		else
		{
			// origin is below, build tetrahedron
			d = b; // different winding order to preserve direction
			b = a;
			v = -abc;
		}

		n = 3; // move onto tetrahedron case
		return false; // cannot enclose origin in a triangle
	}

	bool GJK::tetrahedron(const glm::vec3& a)
	{
		glm::vec3 ao = -a; // dir to origin
		glm::vec3 ab = b - a;
		glm::vec3 ac = c - a;
		glm::vec3 ad = d - a;

		glm::vec3 abc = glm::cross(ab, ac);
		glm::vec3 acd = glm::cross(ac, ad);
		glm::vec3 adb = glm::cross(ad, ab);

		// if the origin is in dir of face ABC
		if (glm::dot(abc, ao) > 0)
		{
			return tetrahedronFace(a, ao, ab, ac, abc);
		}

		// if origin is in dir of face ACD
		if (glm::dot(acd, ao) > 0)
		{
			// reshuffle points/edges/plane to be in right variables
			b = c;
			c = d;
			ab = ac;
			ac = ad;
			abc = acd;

			return tetrahedronFace(a, ao, ab, ac, abc);
		}

		// if origin is in dir of face ADB
		if (glm::dot(adb, ao) > 0)
		{
			// reshuffle points/edges/plane to be in right variables
			c = b;
			b = d;
			ac = ab;
			ab = ad;
			abc = adb;

			return tetrahedronFace(a, ao, ab, ac, abc);
		}

		return true; // we have encompassed the origin
	}

	bool GJK::tetrahedronFace(const glm::vec3& a, const glm::vec3 & ao, const glm::vec3 & ab, const glm::vec3 & ac, const glm::vec3 & abc)
	{
		// this is similar to triangle() case
		glm::vec3 abN = glm::cross(ab, abc);
		if (glm::dot(abN, ao) > 0)
		{
			c = b;
			b = a;
			v = vec3_aba(ab, ao);

			// d is no longer relevant, we're building a new triangle
			n = 2;

			return false;
		}

		glm::vec3 acN = glm::cross(abc, ac);
		if (glm::dot(acN, ao) > 0)
		{
			b = a;
			v = vec3_aba(ac, ao);

			// d is no longer relevant, we're building a new triangle
			n = 2;

			return false;
		}

		// otherwise build new tetrahedron
		d = c;
		c = b;
		b = a;
		v = abc;
		n = 3;
		return false;
	}

	// (A x B) x A
	glm::vec3 GJK::vec3_aba(const glm::vec3 & A, const glm::vec3 & B)
	{
		return glm::cross(glm::cross(A, B), A);
	}

	glm::vec3 GJK::furthestInDir(const std::vector<glm::vec3>& s, const glm::vec3& dir)
	{
		float maxDot = glm::dot(s[0], dir);
		glm::vec3 returnVec = s[0];
		for (int i = 1; i < s.size(); i++)
		{
			float dot = glm::dot(s[i], dir);
			if (dot > maxDot)
			{
				maxDot = dot;
				returnVec = s[i];
			}
		}
		return returnVec;
	}
	
	// get the mean position of a list of vertices
	glm::vec3 GJK::getCenter(const std::vector<glm::vec3>& a)
	{
		glm::vec3 sum = { 0, 0, 0 };
		for (const auto& vert : a)
		{
			sum += vert;
		}
		return sum /= a.size();
	}

	// the support function for the GJK algorithm
	glm::vec3 GJK::support(std::vector<glm::vec3> A, std::vector<glm::vec3> B, glm::vec3 dir)
	{
		// figure out the furthest points in either direction with dot products
		std::sort(A.begin(), A.end(), [dir](const glm::vec3& v1, const glm::vec3& v2) -> bool {
			return glm::dot(v1, dir) > glm::dot(v2, dir);
		});
		std::sort(B.begin(), B.end(), [dir](const glm::vec3& v1, const glm::vec3& v2) -> bool {
			return glm::dot(v1, -dir) > glm::dot(v2, -dir);
		});

		return A[0] - B[0];
	}

	bool GJK::intersect(const std::vector<glm::vec3>& A, const std::vector<glm::vec3>& B)
	{
		v = getCenter(A) - getCenter(B);
		n = 0; // empty simplex

		unsigned steps = 0;
		while (steps < 50)
		{
			glm::vec3 a = support(A, B, v);

			if (glm::dot(a, v) < 0)
				return false; // no intersection, we didn't cross the origin

			if (doSimplex(a))
				return true; // origin was found in constructed simplex
			steps++;
		}
		return true;
	}
}
