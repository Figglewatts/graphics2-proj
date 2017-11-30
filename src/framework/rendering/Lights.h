#ifndef LIGHTS_H
#define LIGHTS_H
#pragma once

#include <glm/glm.hpp>
#include "framework/graphics/Color.h"

namespace Framework
{
	struct PointLight
	{
		glm::vec3 position;
		Color diffuse;
		float intensity;
		float linearAttenuation;
		float expAttenuation;
	};

	struct SpotLight
	{
		glm::vec3 position;
		glm::vec3 direction;
		Color diffuse;
		float cutoff;
		float distance;
	};

	struct DirectionalLight
	{
		glm::vec3 direction;
		float intensity;
		Color diffuse;
		Color ambient;
	};
}

#endif // LIGHTS_H