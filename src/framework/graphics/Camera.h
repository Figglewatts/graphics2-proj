#ifndef CAMERA_H
#define CAMERA_H
#pragma once
#include <glm/glm.hpp>
#include "framework/math/Transform.h"

namespace Framework
{
	class Camera
	{
	private:
		glm::vec3 _worldUp;
		Transform _transform;
		glm::vec3 _lookAt;
	public:
		Camera(glm::vec3 pos);

		glm::mat4 view();
		Transform& transform() { return this->_transform; }
		void setLookAt(glm::vec3 pos, glm::vec3 up) { _lookAt = pos; _worldUp = up; }
	};
}

#endif // CAMERA_H