#ifndef SHAPES_H
#define SHAPES_H
#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace Framework
{
	struct Shape
	{
		virtual ~Shape() {};
		virtual glm::vec3 support(glm::vec3 dir, glm::mat4 model) { return glm::vec3(0); }
		virtual glm::vec3 getCenter() const { return glm::vec3(0); }
	};

	struct ConvexHull : Shape
	{
		std::vector<glm::vec3> verts;
		glm::vec3 support(glm::vec3 dir, glm::mat4 model) override;
		glm::vec3 getCenter() const override;
	};

	struct Sphere : Shape
	{
		float radius;
		glm::vec3 support(glm::vec3 dir, glm::mat4 model) override;
		glm::vec3 getCenter() const override;
	};
}

#endif // SHAPES_H