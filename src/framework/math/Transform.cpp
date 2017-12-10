#include "Transform.h"

#include "framework/physics/AABB.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

namespace Framework
{
	glm::vec3 Transform::up_ = { 0, 1, 0 };
	glm::vec3 Transform::forward_ = { 0, 0, 1 };
	glm::vec3 Transform::right_ = { 1, 0, 0 };
	
	glm::mat4 Transform::matrix() const
	{
		/*auto rotMat = glm::rotate(_rotation.z, glm::vec3(0, 0, 1));
		rotMat *= glm::rotate(_rotation.y, glm::vec3(0, 1, 0));
		rotMat *= glm::rotate(_rotation.x, glm::vec3(1, 0, 0));
		
		return glm::translate(_position) * rotMat * glm::scale(_scale);*/
		return this->_matrix;
	}

	glm::mat4 Transform::inverseMatrix() const
	{
		auto rotMat = glm::rotate(-_rotation.z, glm::vec3(0, 0, 1));
		rotMat *= glm::rotate(-_rotation.y, glm::vec3(0, 1, 0));
		rotMat *= glm::rotate(-_rotation.x, glm::vec3(1, 0, 0));

		return glm::translate(-_position) * rotMat * glm::scale(glm::vec3(1));
	}

	glm::vec3 Transform::convert(const glm::vec3 v) const
	{
		return glm::vec4(v, 1) * glm::transpose(matrix());
	}

	AABB Transform::convert(const AABB & aabb) const
	{
		return { glm::vec4(aabb.min, 1) * glm::transpose(this->matrix()), glm::vec4(aabb.max, 1) * glm::transpose(this->matrix()) };
	}

	glm::vec3 Transform::up() const
	{
		return glm::normalize(glm::mat3(matrix()) * up_);
	}

	glm::vec3 Transform::forward() const
	{
		return glm::normalize(glm::mat3(matrix()) * forward_);
	}

	glm::vec3 Transform::right() const
	{
		return glm::normalize(glm::mat3(matrix()) * right_);
	}

	Transform & Transform::translate(glm::vec3 t)
	{
		this->_position += t;
		this->_matrix = glm::translate(t) * this->_matrix;
		return *this;
	}

	Transform & Transform::scale(glm::vec3 scale)
	{
		this->_scale += scale;
		this->_matrix = glm::scale(scale) * this->_matrix;
		return *this;
	}

	/*Transform & Transform::rotate(glm::quat rot)
	{
		this->_rotation *= rot;
		this->_matrix = this->_matrix * glm::toMat4(rot);
		return *this;
	}*/

	Transform & Transform::rotate(glm::vec3 euler, bool local)
	{
		this->_rotation += euler;
		auto rotMat = glm::rotate(euler.x, glm::vec3(1, 0, 0));
		rotMat *= glm::rotate(euler.y, glm::vec3(0, 1, 0));
		rotMat *= glm::rotate(euler.z, glm::vec3(0, 0, 1));
		if (local)
		{
			this->_matrix = this->_matrix * rotMat;
		}
		else
		{
			this->_matrix = rotMat * this->_matrix;
		}
		return *this;
	}
}
