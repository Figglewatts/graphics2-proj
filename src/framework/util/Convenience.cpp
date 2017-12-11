#include "Convenience.h"

#include <algorithm>
#include <glm/glm.hpp>
#include "framework/graphics/Camera.h"

std::string basename(std::string const & pathname)
{
	return std::string(
		std::find_if(pathname.rbegin(), pathname.rend(),
			MatchPathSeparator()).base(),
		pathname.end());
}

std::string removeExtension(std::string const & filename)
{
	std::string::const_reverse_iterator
		pivot
		= std::find(filename.rbegin(), filename.rend(), '.');
	return pivot == filename.rend()
		? filename
		: std::string(filename.begin(), pivot.base() - 1);
}

// code from: http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/
glm::quat rotationBetween(glm::vec3 start, glm::vec3 dest)
{
	start = glm::normalize(start);
	dest = glm::normalize(dest);

	float cosTheta = dot(start, dest);
	glm::vec3 rotationAxis;

	if (cosTheta < -1 + 0.001f) {
		// special case when vectors in opposite directions:
		// there is no "ideal" rotation axis
		// So guess one; any will do as long as it's perpendicular to start
		rotationAxis = cross(glm::vec3(0.0f, 0.0f, 1.0f), start);
		if (glm::length2(rotationAxis) < 0.01) // bad luck, they were parallel, try again!
			rotationAxis = cross(glm::vec3(1.0f, 0.0f, 0.0f), start);

		rotationAxis = normalize(rotationAxis);
		return glm::angleAxis(glm::radians(180.0f), rotationAxis);
	}

	rotationAxis = cross(start, dest);

	float s = sqrt((1 + cosTheta) * 2);
	float invs = 1 / s;

	return glm::quat(
		s * 0.5f,
		rotationAxis.x * invs,
		rotationAxis.y * invs,
		rotationAxis.z * invs
	);
}
