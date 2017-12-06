#ifndef RIGIDBODY_H
#define RIGIDBODY_H
#pragma once

#include "Shapes.h"
#include "framework/math/Transform.h"

namespace Framework
{
	struct Rigidbody
	{
		Shape& shape;
		Transform *toWorld;

		Rigidbody(Shape& s, Transform* t) : shape(s), toWorld(t) {}
	};
}

#endif // RIGIDBODY_H