#ifndef SCENENODE_H
#define SCENENODE_H
#pragma once
#include <list>

namespace Framework
{
	class SceneNode
	{
	protected:
		std::list<SceneNode*> _children;
	public:
		SceneNode() {}
		virtual ~SceneNode() { destroy(); }

		virtual void start() {} // intentionally empty

		virtual void update(double delta)
		{
			for (const auto& child : _children)
			{
				child->update(delta);
			}
		}

		virtual void render() {} // intentionally empty

		void release() { delete this; }

		void destroy()
		{
			for (const auto& child : _children)
			{
				child->release();
			}
			_children.clear();
		}

		void addChild(SceneNode *child)
		{
			_children.push_back(child);
		}
	};
}

#endif