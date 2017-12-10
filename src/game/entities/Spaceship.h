#ifndef SPACESHIP_H
#define SPACESHIP_H
#pragma once
#include "framework/rendering/Renderable.h"
using namespace Framework;

class Spaceship : public Renderable
{
public:
	Spaceship(Mesh *mesh, Shader *shader, Texture2D *tex, Mesh* collisionMesh, Transform t = Transform())
		: Renderable(mesh, shader, tex, t)
	{
		_collision = true;
		_collisionResponse = true;
		ConvexHull *collisionHull = new ConvexHull();
		collisionHull->verts = mesh->points();
		_pRigidbody = new Rigidbody(std::unique_ptr<Shape>(collisionHull), &_transform);
	}

	void draw(glm::mat4 view, glm::mat4 proj) const override;
	void update(double delta) override;
	void onCollide(Collision c, const Rigidbody* body) override;
};

#endif // SPACESHIP_H