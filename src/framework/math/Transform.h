#ifndef TRANSFORM_H
#define TRANSFORM_H
#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Framework
{
	class Transform
	{
	private:
		glm::vec3 _position;
		glm::vec3 _scale;
		glm::quat _rotation;
		glm::mat4 _matrix;

	public:
		Transform(glm::vec3 pos = glm::vec3(1), glm::vec3 scale = glm::vec3(1), glm::quat rot = glm::quat(1, 0, 0, 0)) 
			: _position(1), _scale(1), _rotation(1, 0, 0, 0) {}

		glm::vec3& position() { return _position; }
		glm::vec3 scale() const { return _scale; }
		glm::vec3 rotEuler() const { return glm::eulerAngles(_rotation); }
		glm::quat rotation() const { return _rotation; }
		glm::mat4 matrix() const;
		glm::vec3 convert(const glm::vec3 v) const;
		
		Transform& translate(glm::vec3 t);
		Transform& scale(glm::vec3 scale);
		Transform& rotate(glm::quat rot);
	};
}

#endif // TRANSFORM_H