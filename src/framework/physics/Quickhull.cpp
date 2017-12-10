#include "Quickhull.h"

#include <glm/gtx/norm.inl>
#include <algorithm>

namespace Framework
{
	float Quickhull::dist2LineSegment(const glm::vec3 & a, const glm::vec3 & b, const glm::vec3 & c)
	{
		glm::vec3 ab = b - a;
		glm::vec3 ac = c - a;
		glm::vec3 bc = c - b;

		float ACdotAB = glm::dot(ac, ab);
		if (ACdotAB < 0) return glm::dot(ac, ac);
		float abSq = glm::dot(ab, ab);
		if (ACdotAB > abSq) return glm::dot(bc, bc);
		return glm::dot(ac, ac) - ACdotAB * ACdotAB / abSq;
	}

	glm::vec3 Quickhull::planeNormal(const glm::vec3 & a, const glm::vec3 & b, const glm::vec3 & c)
	{
		glm::vec3 ab = b - a;
		glm::vec3 ac = c - a;
		return glm::normalize(glm::cross(ab, ac));
	}

	std::vector<glm::vec3> Quickhull::generateHull(std::vector<Vertex>& verts)
	{
		// find extreme points
		std::vector<int> extremes(6, 0);
		for (int i = 1; i < verts.size(); i++)
		{
			const Vertex v = verts[i];
			if (v.Position.x < verts[extremes[0]].Position.x) extremes[0] = i;
			if (v.Position.x > verts[extremes[1]].Position.x) extremes[1] = i;
			if (v.Position.y < verts[extremes[2]].Position.y) extremes[2] = i;
			if (v.Position.y > verts[extremes[3]].Position.y) extremes[3] = i;
			if (v.Position.z < verts[extremes[4]].Position.z) extremes[4] = i;
			if (v.Position.z > verts[extremes[5]].Position.z) extremes[5] = i;
		}

		// find longest line between extreme points
		float maxDist = 0;
		int v1 = 0;
		int v2 = 0;
		for (int i = 0; i < extremes.size(); i++)
		{
			for (int j = i+1; j < extremes.size(); j++)
			{
				glm::vec3 *p1, *p2;
				p1 = &verts[extremes[i]].Position;
				p2 = &verts[extremes[j]].Position;
				float dist2 = glm::distance2(*p1, *p2);
				if (dist2 > maxDist)
				{
					maxDist = dist2;
					v1 = i;
					v2 = j;
				}
			}
		}

		// find most distant point to line segment created between v1 and v2
		// to create a plane
		int v3 = 0;
		maxDist = 0;
		for (int i = 0; i < extremes.size(); i++)
		{
			float curDist = dist2LineSegment(verts[v1].Position, verts[v2].Position, verts[i].Position);
			if (curDist > maxDist)
			{
				maxDist = curDist;
				v3 = i;
			}
		}

		// now find the most distant point to the plane
		glm::vec3 planeN = planeNormal(verts[v1].Position, verts[v2].Position, verts[v3].Position);
		float planeD = glm::dot(planeN, verts[v1].Position);
		maxDist = 0;
		int v4 = 0;
		for (int i = 0; i < verts.size(); i++)
		{
			float curDist = glm::abs(glm::dot(verts[i].Position, planeN) - planeD);
			if (curDist > maxDist)
			{
				maxDist = curDist;
				v4 = i;
			}
		}

		// we now have a tetrahedron which needs to be subdivided to enclose the geometry
		std::vector<Face> tetrahedron = {
			Face(v3, v2, v1),
			Face(v2, v4, v1),
			Face(v3, v4, v2),
			Face(v1, v4, v3)
		};

		glm::vec3 *A = &verts[v1].Position;
		glm::vec3 *B = &verts[v2].Position;
		glm::vec3 *C = &verts[v3].Position;
		glm::vec3 *D = &verts[v4].Position;
		glm::vec3 ABdir = glm::normalize(*B - *A);
		glm::vec3 ACdir = glm::normalize(*C - *A);
		glm::vec3 ADdir = glm::normalize(*D - *A);
		float sign = glm::dot(ADdir, glm::cross(ACdir, ABdir));

		// reverse winding if necessary
		if (sign < 0)
		{
			for (auto& f : tetrahedron)
			{
				std::swap(f.v1, f.v3);
			}
		}

		// create an array of all the points left over
		std::vector<glm::vec3> positions;
		for (int i = 0; i < verts.size(); i++)
		{
			if (i != v1 && i != v2 && i != v3 && i != v4)
			{
				positions.push_back(verts[i].Position);
			}
		}

		// for each face in the tetrahedron
		// figure out which points are in front of it
		for (int i = 0; i < 4; i++)
		{
			// calculate face normal
			glm::vec3 faceN = planeNormal(verts[tetrahedron[i].v1].Position, 
				verts[tetrahedron[i].v2].Position, 
				verts[tetrahedron[i].v3].Position);
			float faceD = glm::dot(faceN, verts[tetrahedron[i].v1].Position);

			// sort points by distance from plane
			std::sort(positions.begin(), positions.end(), [faceN](const glm::vec3& a, const glm::vec3& b) -> bool {
				return (glm::dot(faceN, a) > glm::dot(faceN, b));
			});
		}

		return std::vector<glm::vec3>();
	}
}
