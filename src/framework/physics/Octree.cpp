#include "Octree.h"

namespace Framework
{
	void Octree::subdivide()
	{
		_nodes[0] = new Octree(_halfWidth, { _center.x - _halfWidth / 2, _center.y - _halfWidth / 2, _center.z - _halfWidth / 2 }, _level + 1); // left bottom front
		_nodes[1] = new Octree(_halfWidth, { _center.x + _halfWidth / 2, _center.y - _halfWidth / 2, _center.z - _halfWidth / 2 }, _level + 1); // right bottom front
		_nodes[2] = new Octree(_halfWidth, { _center.x + _halfWidth / 2, _center.y + _halfWidth / 2, _center.z - _halfWidth / 2 }, _level + 1); // right top front
		_nodes[3] = new Octree(_halfWidth, { _center.x - _halfWidth / 2, _center.y + _halfWidth / 2, _center.z - _halfWidth / 2 }, _level + 1); // left top front
		_nodes[4] = new Octree(_halfWidth, { _center.x - _halfWidth / 2, _center.y - _halfWidth / 2, _center.z + _halfWidth / 2 }, _level + 1); // left bottom back
		_nodes[5] = new Octree(_halfWidth, { _center.x + _halfWidth / 2, _center.y - _halfWidth / 2, _center.z + _halfWidth / 2 }, _level + 1); // right bottom back
		_nodes[6] = new Octree(_halfWidth, { _center.x + _halfWidth / 2, _center.y + _halfWidth / 2, _center.z + _halfWidth / 2 }, _level + 1); // right top back
		_nodes[7] = new Octree(_halfWidth, { _center.x - _halfWidth / 2, _center.y + _halfWidth / 2, _center.z + _halfWidth / 2 }, _level + 1); // left top back
	}

	// determine which node the bounding volume belongs to
	// -1 means it belongs to the parent volume
	int Octree::getIndex(const AABB & objBoundingVolume) const
	{
		bool frontHalf = (objBoundingVolume.min.z >= _center.z - _halfWidth && objBoundingVolume.max.z <= _center.z);
		bool backHalf = (objBoundingVolume.min.z >= _center.z && objBoundingVolume.max.z <= _center.z + _halfWidth);
		bool bottomHalf = (objBoundingVolume.min.y >= _center.y - _halfWidth && objBoundingVolume.max.y <= _center.y);
		bool topHalf = (objBoundingVolume.min.y >= _center.y && objBoundingVolume.max.y <= _center.y + _halfWidth);
		bool leftHalf = (objBoundingVolume.min.x >= _center.x - _halfWidth && objBoundingVolume.max.x <= _center.x);
		bool rightHalf = (objBoundingVolume.min.x >= _center.x && objBoundingVolume.max.x <= _center.x + _halfWidth);

		int idx = -1;
		if (frontHalf)
		{
			if (bottomHalf)
			{
				if (leftHalf)
				{
					idx = 0;
				}
				else if (rightHalf)
				{
					idx = 1;
				}
			}
			else if (topHalf)
			{
				if (leftHalf)
				{
					idx = 3;
				}
				else if (rightHalf)
				{
					idx = 2;
				}
			}
		}
		else if (backHalf)
		{
			if (bottomHalf)
			{
				if (leftHalf)
				{
					idx = 4;
				}
				else if (rightHalf)
				{
					idx = 5;
				}
			}
			else if (topHalf)
			{
				if (leftHalf)
				{
					idx = 7;
				}
				else if (rightHalf)
				{
					idx = 6;
				}
			}
		}
		return idx;
	}

	Octree::Octree(float size, glm::vec3 center, int level)
		: Octree(size, center)
	{
		_level = level;
	}

	Octree::Octree(float size, glm::vec3 center)
		: _center(center)
	{
		_level = 0;
		_halfWidth = size / 2;
		_boundingVolume = {
			{ _center.x - _halfWidth, _center.y - _halfWidth, _center.z - _halfWidth },
			{ _center.x + _halfWidth, _center.y + _halfWidth, _center.z + _halfWidth }
		};
		_nodes.resize(8);
	}

	Octree::~Octree()
	{
		for (auto& node : _nodes)
		{
			delete node;
			node = nullptr;
		}
	}

	void Octree::clear()
	{
		_bodies.clear();

		for (auto& node : _nodes)
		{
			if (node != nullptr)
			{
				node->clear();
				delete node;
				node = nullptr;
			}
		}
	}

	void Octree::insert(const Rigidbody * body)
	{
		// check to see if we need to subdivide
		if (_bodies.size() + 1 > MAX_OBJECTS && _level + 1 < MAX_DEPTH)
		{
			if (_nodes[0] == nullptr)
				subdivide();

			// loop through bodies in this node and see which ones we can
			// fit into the the subdivisions
			int i = 0;
			while(i < _bodies.size())
			{
				int idx = getIndex(body->toWorld->convert(body->boundingVolume));
				if (idx != -1)
				{
					std::swap(_bodies[i], _bodies.back());
					_nodes[idx]->insert(_bodies.back());
					_bodies.pop_back();
					continue;
				}
				i++;
			}

			int idx = getIndex(body->toWorld->convert(body->boundingVolume));
			if (idx != -1 && _nodes[idx]->_level <= MAX_DEPTH)
			{
				_nodes[idx]->insert(body);
				return;
			}
		}
		
		_bodies.push_back(body);
	}

	std::vector<const Rigidbody*> Octree::neighbours(const Rigidbody * body) const
	{
		std::vector<const Rigidbody*> neighbours;
		int idx = getIndex(body->toWorld->convert(body->boundingVolume));
		if (idx != -1 && _nodes[0] != nullptr)
		{
			auto n = _nodes[idx]->neighbours(body);
			neighbours.insert(neighbours.end(), n.begin(), n.end());
		}

		neighbours.insert(neighbours.end(), _bodies.begin(), _bodies.end());
		return neighbours;
	}
}