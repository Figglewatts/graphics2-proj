#include "Planet.h"

void Planet::draw(glm::mat4 view, glm::mat4 proj) const
{
	Renderable::draw(view, proj);
}

void Planet::update(double delta)
{
	
}

void Planet::onCollide(Collision c, const Rigidbody* body)
{
	
}
