#include "Camera.h"
#include <glm/gtc/matrix_transform.inl>

namespace Framework
{
	void Camera::calculateVectors()
	{
		_front = {
			glm::cos(glm::radians(_yaw)) * glm::cos(glm::radians(_pitch)),
			glm::sin(glm::radians(_pitch)),
			glm::sin(glm::radians(_yaw)) * glm::cos(glm::radians(_pitch))
		};
		_front = glm::normalize(_front);
		_cameraRight = glm::normalize(glm::cross(_front, _worldUp));
		_cameraUp = glm::normalize(glm::cross(_cameraRight, _front));
	}

	Camera::Camera(glm::vec3 pos, glm::vec3 worldUp, float yaw = -90.f, float pitch = 0.f)
		:_position(pos), _front(0, 0, -1.f), _worldUp(worldUp), _yaw(yaw), _pitch(pitch)
	{
		this->calculateVectors();
	}
	
	glm::mat4 Camera::view() const
	{
		return glm::lookAt(_position, _position + _front, _cameraUp);
	}

	void Camera::set_position(glm::vec3 position)
	{
		this->_position = position;
	}

	void Camera::set_rotation(float yaw, float pitch)
	{
		_yaw = yaw;
		_pitch = pitch;
		this->calculateVectors();
	}
}
