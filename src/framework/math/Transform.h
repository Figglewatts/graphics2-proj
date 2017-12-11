#ifndef TRANSFORM_H
#define TRANSFORM_H
#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Framework
{
	struct AABB;

	class Transform
	{
	private:
		glm::vec3 _position;
		glm::vec3 _scale;
		glm::quat _rotation;
		glm::mat4 _matrix;

		static glm::vec3 up_;
		static glm::vec3 forward_;
		static glm::vec3 right_;

		void recomputeMatrix();

	public:
		Transform(glm::vec3 pos = glm::vec3(0), glm::vec3 scale = glm::vec3(1), glm::quat rot = glm::quat(1, 0, 0, 0)) 
			: _position(0), _scale(1), _rotation(1, 0, 0, 0) {}

		glm::vec3 position() const { return _position; }
		glm::vec3 scale() const { return _scale; }
		glm::vec3 rotEuler() const { return glm::eulerAngles(_rotation); }
		glm::quat rotation() const { return _rotation; }
		glm::mat4 matrix() const;
		glm::mat4 inverseMatrix() const;
		glm::vec3 convert(const glm::vec3 v) const;
		AABB convert(const AABB& aabb) const;

		glm::vec3 up() const;
		glm::vec3 forward() const;
		glm::vec3 right() const;
		
		Transform& setPosition(glm::vec3 position);
		Transform& setRotation(glm::quat rotation);
		Transform& translate(glm::vec3 t);
		Transform& scale(glm::vec3 scale);
		Transform& rotate(glm::quat rot, bool local = true);
		Transform& rotate(glm::vec3 euler, bool local = true, glm::mat3 *retMat = nullptr);
	};
}

#endif // TRANSFORM_H