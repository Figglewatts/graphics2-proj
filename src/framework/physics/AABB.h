#ifndef AABB_H
#define AABB_H
#pragma once

#include <glm/glm.hpp>

namespace Framework
{
	struct AABB
	{
		glm::vec3 min;
		glm::vec3 max;

		// returns true if this AABB intersects another
		bool intersects(const AABB& other) const
		{
			return (this->min.x <= other.max.x && this->max.x >= other.min.x)
				&& (this->min.y <= other.max.y && this->max.y >= other.min.y)
				&& (this->min.z <= other.max.z && this->max.z >= other.min.z);
		}

		// returns true if this AABB contains anotheer
		bool engulfs(const AABB& other) const
		{
			return (this->min.x <= other.min.x && this->max.x >= other.max.x)
				&& (this->min.y <= other.min.y && this->max.y >= other.max.y)
				&& (this->min.z <= other.min.z && this->max.z >= other.max.z);
		}
	};
}

#endif // AABB_H
