#ifndef CAMERA_H
#define CAMERA_H
#pragma once
#include <glm/glm.hpp>

namespace Framework
{
	class Camera
	{
	private:
		glm::vec3 _position;
		glm::vec3 _front;
		glm::vec3 _cameraRight;
		glm::vec3 _cameraUp;
		glm::vec3 _worldUp;

		float _yaw = 0;
		float _pitch = 0;

		void calculateVectors();
	public:
		Camera(glm::vec3 pos, glm::vec3 worldUp, float yaw, float pitch);

		glm::vec3 get_position() const { return this->_position; }
		glm::vec3 up() const { return this->_cameraUp; }
		glm::vec3 right() const { return this->_cameraRight; }
		glm::vec3 front() const { return this->_front; }
		glm::mat4 view() const;
		void set_position(glm::vec3 position);
		void set_rotation(float yaw = 0, float pitch = 0);
		float& yaw() { return this->_yaw; }
		float& pitch() { return this->_pitch; }
	};
}

#endif // CAMERA_H