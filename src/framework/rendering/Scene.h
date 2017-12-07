#ifndef SCENE_H
#define SCENE_H
#pragma once
#include "framework/physics/Octree.h"
#include "Renderable.h"

namespace Framework
{
	class Scene
	{
	private:
		Octree _octree;
		std::vector<Renderable*> _objects;
		std::vector<const Rigidbody*> _octreeNeighbours;

		void rebuildOctree();

	public:
		Scene();
		~Scene() {}

		void add(Renderable* object);

		void render(glm::mat4 proj, glm::mat4 view);
		void update(double delta);
	};
}

#endif // SCENE_H