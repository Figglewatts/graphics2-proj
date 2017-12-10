#ifndef PLANET_H
#define PLANET_H
#pragma once
#include "framework/rendering/Renderable.h"
using namespace Framework;

class Planet : public Renderable
{
public:
	Planet(Mesh *mesh, Shader *shader, Texture2D *tex, Texture2D *specularTex, Transform t = Transform())
		: Renderable(mesh, shader, tex, t)
	{
		_collision = true;
		_collisionResponse = false;
		_textures.push_back(specularTex);
		Sphere *collisionSphere = new Sphere();
		collisionSphere->radius = 1;
		_pRigidbody = new Rigidbody(std::unique_ptr<Shape>(collisionSphere), &_transform);
	}

	void draw(glm::mat4 view, glm::mat4 proj) const override;
	void update(double delta) override;
	void onCollide(Collision c, const Rigidbody* body) override;
};

#endif // SPACESHIP_H#pragma once
