#ifndef OCTREE_H
#define OCTREE_H
#pragma once

#include <vector>
#include "Rigidbody.h"
#include "AABB.h"

namespace Framework
{
	class Octree
	{
	private:
		const unsigned MAX_DEPTH = 8;
		const unsigned MAX_OBJECTS = 16;
		const unsigned MIN_OBJECTS = 8;

		int _level;
		std::vector<const Rigidbody*> _bodies;
		glm::vec3 _center;
		float _halfWidth;
		AABB _boundingVolume;
		std::vector<Octree*> _nodes;

		void subdivide();
		int getIndex(const AABB& objBoundingVolume) const;

		explicit Octree(float size, glm::vec3 center, int level);

	public:
		Octree(float size, glm::vec3 center = glm::vec3(0));
		~Octree();

		void clear();
		void insert(const Rigidbody *body);
		std::vector<const Rigidbody*> neighbours(const Rigidbody *body) const;
	};
}

#endif // OCTREE