#ifndef RIGIDBODY_H
#define RIGIDBODY_H
#pragma once

#include "Shapes.h"
#include "framework/math/Transform.h"
#include "AABB.h"
#include <memory>

namespace Framework
{
	struct Rigidbody
	{
		std::unique_ptr<Shape> shape;
		Transform *toWorld;
		AABB boundingVolume;
		Rigidbody(std::unique_ptr<Shape> shape, Transform* t) : shape(std::move(shape)), toWorld(t) {}
	};
}

#endif // RIGIDBODY_H