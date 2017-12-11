#include "Camera.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Framework
{
	Camera::Camera(glm::vec3 pos)
	{
		_transform.translate(pos);
	}
	
	glm::mat4 Camera::view()
	{
		glm::mat4 rotation = glm::toMat4(transform().rotation());
		glm::mat4 translation = glm::translate(-_transform.position());

		//return rotation * translation;
		return glm::lookAt(_transform.position(), _lookAt, _worldUp);
	}

}
