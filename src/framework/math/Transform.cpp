#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Framework
{
	glm::mat4 Transform::matrix() const
	{
		return glm::translate(glm::mat4(1), _position) * glm::scale(glm::mat4(1), _scale) *  glm::toMat4(_rotation);
	}
	Transform & Transform::translate(glm::vec3 t)
	{
		this->_position += t;
		this->_matrix *= glm::translate(glm::mat4(1), t);
		return *this;
	}

	Transform & Transform::scale(glm::vec3 scale)
	{
		this->_scale += scale;
		this->_matrix *= glm::scale(glm::mat4(1), scale);
		return *this;
	}

	Transform & Transform::rotate(glm::quat rot)
	{
		this->_rotation *= rot;
		this->_matrix *= glm::toMat4(rot);
		return *this;
	}
}
