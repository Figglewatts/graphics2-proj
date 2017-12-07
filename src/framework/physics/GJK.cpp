#include "GJK.h"

#include <limits> // for float max value

namespace Framework
{
	SupportPoint GJK::b, GJK::c, GJK::d;
	glm::vec3 GJK::v;
	unsigned GJK::n = 0;
	std::vector<Triangle> GJK::epa_triangles;
	std::vector<Edge> GJK::epa_edges;
	
	const float GJK::EPA_EXIT_THRESHOLD = 0.001f;
	const float GJK::GJK_EPSILON = std::numeric_limits<float>::epsilon();

	bool GJK::doSimplex(const SupportPoint& a)
	{
		switch (n)
		{
			case 0: // point case
			{
				b = a;
				v = -a.v; // search in opposite direction to find something with highest chance of enclosing origin
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

	bool GJK::line(const SupportPoint& a)
	{
		// search direction is perpendicular to the edge of the 1-simplex
		v = vec3_aba(b.v - a.v, -a.v); // (AB x AO) x AB

		c = b;
		b = a;
		n = 2;

		return false; // cannot enclose with a 1-simplex
	}

	bool GJK::triangle(const SupportPoint& a)
	{
		glm::vec3 ao = -a.v; // vector from A to origin (Minkowski space)

		// compute edges
		glm::vec3 ab = b.v - a.v;
		glm::vec3 ac = c.v - a.v;

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
		if (glm::dot(acN, ao) > -GJK_EPSILON) // is dir to origin similar to AC normal?
		{
			// c stays same
			b = a;
			v = vec3_aba(ac, ao); // in dir of origin

			return false;
		}

		// if we get here, then origin is within infinitely long
		// triangular prism along triangle's normal -- is it above or below?
		if (glm::dot(abc, ao) > -GJK_EPSILON)
		{
			// origin is above, build tetrahedron that way
			d = c;
			c = b;
			b = a;
			v = abc;
		}
		else
		{
			// origin is below, build tetrahedron that way
			d = b; // different winding order to preserve direction
			b = a;
			v = -abc;
		}

		n = 3; // move onto tetrahedron case
		return false; // cannot enclose origin in a triangle
	}

	bool GJK::tetrahedron(const SupportPoint& a)
	{
		glm::vec3 ao = -a.v; // dir to origin
		glm::vec3 ab = b.v - a.v;
		glm::vec3 ac = c.v - a.v;
		glm::vec3 ad = d.v - a.v;

		glm::vec3 abc = glm::cross(ab, ac);
		glm::vec3 acd = glm::cross(ac, ad);
		glm::vec3 adb = glm::cross(ad, ab);

		// if the origin is in dir of face ABC
		if (glm::dot(abc, ao) > -GJK_EPSILON)
		{
			return tetrahedronFace(a, ao, ab, ac, abc);
		}

		// if origin is in dir of face ACD
		if (glm::dot(acd, ao) > -GJK_EPSILON)
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
		if (glm::dot(adb, ao) > -GJK_EPSILON)
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

	bool GJK::tetrahedronFace(const SupportPoint& a, const glm::vec3 & ao, const glm::vec3 & ab, const glm::vec3 & ac, const glm::vec3 & abc)
	{
		// this is similar to triangle() case
		glm::vec3 abN = glm::cross(ab, abc);
		if (glm::dot(abN, ao) > -GJK_EPSILON)
		{
			c = b;
			b = a;
			v = vec3_aba(ab, ao);

			// d is no longer relevant, we're building a new triangle
			n = 2;

			return false;
		}

		glm::vec3 acN = glm::cross(abc, ac);
		if (glm::dot(acN, ao) > -GJK_EPSILON)
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

	bool GJK::gjk_test_collision(const Rigidbody& A, const Rigidbody& B, SupportPoint& a)
	{
		v = A.toWorld->convert(A.shape->getCenter()) - B.toWorld->convert(B.shape->getCenter());
		n = 0; // empty simplex

		unsigned steps = 0;
		while (steps < GJK_MAX_STEPS)
		{
			a = support(A, B, v);

			if (glm::dot(a.v, v) < GJK_EPSILON)
				return false; // no intersection, we didn't cross the origin

			if (doSimplex(a))
				return true; // origin was found in constructed simplex
			steps++;
		}
		return true;
	}

	void GJK::epa_addEdge(const SupportPoint & p1, const SupportPoint & p2)
	{
		for (int i = epa_edges.size()-1; i >= 0; i--)
		{
			Edge e = epa_edges[i];
			if (e.points[0] == p2 && e.points[1] == p1)
			{
				// opposite edge found, don't add new one and remove it
				std::swap(epa_edges[i], epa_edges.back());
				epa_edges.pop_back();
				return;
			}
		}
		epa_edges.push_back({ p1, p2 });
	}

	Collision GJK::generate_collision_data(Triangle & t)
	{
		const float distFromOrigin = glm::dot(t.n, t.points[0].v);
		const glm::vec3 barycentricCoord = barycentric(t.n * distFromOrigin, t.points[0].v, t.points[1].v, t.points[2].v);

		Collision ret;
		ret.point = {
			barycentricCoord.x * t.points[0].supA +
			barycentricCoord.y * t.points[1].supA +
			barycentricCoord.z * t.points[2].supA
		};
		ret.normal = -t.n;
		ret.depth = distFromOrigin;
		return ret;
	}

	// code from book "Real Time Collision Detection" by Christer Ericson
	// http://realtimecollisiondetection.net/books/rtcd/
	glm::vec3 GJK::barycentric(const glm::vec3 & p, const glm::vec3 & p1, const glm::vec3 & p2, const glm::vec3 & p3)
	{
		glm::vec3 ret;
		glm::vec3 v0 = p2 - p1, v1 = p3 - p1, v2 = p - p1;
		float d00 = glm::dot(v0, v0);
		float d01 = glm::dot(v0, v1);
		float d11 = glm::dot(v1, v1);
		float d20 = glm::dot(v2, v0);
		float d21 = glm::dot(v2, v1);
		float denom = d00 * d11 - d01 * d01;
		ret.y = (d11 * d20 - d01 * d21) / denom;
		ret.z = (d00 * d21 - d01 * d20) / denom;
		ret.x = 1.0f - ret.y - ret.z;
		return ret;
	}

	// the support function for the GJK algorithm
	SupportPoint GJK::support(const Rigidbody& A, const Rigidbody& B, glm::vec3 dir)
	{
		// figure out the furthest points in either direction
		SupportPoint ret;
		ret.supA = A.toWorld->convert(A.shape->support(dir));
		ret.supB = B.toWorld->convert(B.shape->support(-dir));
		ret.v = ret.supA - ret.supB;
		return ret;
	}

	bool GJK::intersect(const Rigidbody& A, const Rigidbody& B)
	{
		SupportPoint a;
		return gjk_test_collision(A, B, a);
	}

	bool GJK::intersect(const Rigidbody& A, const Rigidbody& B, Collision* col)
	{
		SupportPoint a;
		if (!gjk_test_collision(A, B, a))
		{
			// if they didn't collide we return no collision
			col = nullptr;
			return false;
		}

		// add initial polytope triangles to list
		epa_triangles.emplace_back(a, b, c);
		epa_triangles.emplace_back(a, c, d);
		epa_triangles.emplace_back(a, d, b);
		epa_triangles.emplace_back(b, d, c);

		// begin EPA iterations
		for (int i = 0; i < EPA_MAX_STEPS; i++)
		{
			int closestTriangle = 0;
			
			// find closest tri to origin
			float minDist = std::numeric_limits<float>::infinity();
			for (int i = 0; i < epa_triangles.size(); i++)
			{
				float dist = glm::dot(epa_triangles[i].n, epa_triangles[i].points[0].v);
				if (dist < minDist)
				{
					minDist = dist;
					closestTriangle = i;
				}
			}

			// get the support point in the dir of the closest triangle's normal
			const SupportPoint sup = support(A, B, epa_triangles[closestTriangle].n);

			// check to see how far from origin this point is; if we aren't over a
			// certain threshold then assume we've found the closest part
			// and generate contact info
			if (glm::dot(epa_triangles[closestTriangle].n, sup.v) - minDist < EPA_EXIT_THRESHOLD)
			{
				*col = generate_collision_data(epa_triangles[closestTriangle]);
				break;
			}
	
			for (int i = 0; i < epa_triangles.size();)
			{
				// can this face be seen by the support point?
				if (glm::dot(epa_triangles[i].n, sup.v - epa_triangles[i].points[0].v) > 0)
				{
					epa_addEdge(epa_triangles[i].points[0], epa_triangles[i].points[1]);
					epa_addEdge(epa_triangles[i].points[1], epa_triangles[i].points[2]);
					epa_addEdge(epa_triangles[i].points[2], epa_triangles[i].points[0]);
					std::swap(epa_triangles[i], epa_triangles.back());
					epa_triangles.pop_back();
					continue;
				}
				i++;
			}

			// create new tris from edges
			for (const auto& e : epa_edges)
			{
				epa_triangles.emplace_back(sup, e.points[0], e.points[1]);
			}
			epa_edges.clear();
		}
		epa_edges.clear();
		epa_triangles.clear();
		return true;
	}
}
