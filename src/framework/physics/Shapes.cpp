#include "Shapes.h"

#include <algorithm>

namespace Framework
{
	glm::vec3 ConvexHull::support(glm::vec3 dir, glm::mat4 model)
	{
		// convert model-space direction to world-space
		glm::vec3 worldDir = dir * glm::mat3(model);
		std::sort(verts.begin(), verts.end(), [worldDir](glm::vec3& v1, glm::vec3& v2) -> bool {
			return glm::dot(v1, worldDir) > glm::dot(v2, worldDir);
		});
		return verts[0];
	}

	glm::vec3 ConvexHull::getCenter() const
	{
		glm::vec3 sum = { 0, 0, 0 };
		for (const auto& vert : verts)
		{
			sum += vert;
		}
		return sum /= verts.size();
	}

	glm::vec3 Sphere::support(glm::vec3 dir, glm::mat4 model)
	{
		// convert model-space direction to world-space
		glm::vec3 worldDir = dir * glm::mat3(model);
		return (glm::normalize(worldDir) * radius);
	}

	glm::vec3 Sphere::getCenter() const
	{
		return glm::vec3(0);
	}
}
