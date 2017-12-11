#ifndef SPACESHIP_H
#define SPACESHIP_H
#pragma once
#include "framework/rendering/Renderable.h"
#include "framework/graphics/Camera.h"
using namespace Framework;

class Spaceship : public Renderable
{
private:
	float _moveSpeed = 0.f;
	const float _speedup = 0.05f;
	const float _maxMoveSpeed = 6.f;
	const float _rollSpeed = glm::radians(0.9f);
	const float _pitchSpeed = glm::radians(0.4f);
	glm::vec3 _pilotViewDir;
	bool _pilotViewDirValid = true;

public:
	Spaceship(Mesh *mesh, Shader *shader, Texture2D *tex, Mesh* collisionMesh, Transform t = Transform())
		: Renderable(mesh, shader, tex, t)
	{
		_collision = true;
		_collisionResponse = true;
		ConvexHull *collisionHull = new ConvexHull();
		collisionHull->verts = collisionMesh->points();
		_pRigidbody = new Rigidbody(std::unique_ptr<Shape>(collisionHull), &_transform);
		_transform.rotate({ 0, glm::radians(90.f), 0 });
		_pilotViewDir = _transform.forward();
	}

	void draw(glm::mat4 view, glm::mat4 proj) const override;
	void update(double delta) override;
	void onCollide(Collision c, const Rigidbody* body) override;
	glm::vec3 pilotViewDir() const { return _transform.rotation() * _pilotViewDir; }
	void setPilotViewDir(glm::vec3 viewDir) { _pilotViewDir = viewDir; }
	void invalidatePilotViewDir() { _pilotViewDirValid = false; }

	float getCurrentSpeed() const { return _moveSpeed; }
};

#endif // SPACESHIP_H