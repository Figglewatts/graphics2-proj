#include "Spaceship.h"
#include "framework/input/InputHandler.h"
#include "framework/util/Convenience.h"

void Spaceship::draw(glm::mat4 view, glm::mat4 proj) const
{
	Renderable::draw(view, proj);
}

void Spaceship::update(double delta)
{
	if (InputHandler::checkButton("SpeedUp", ButtonState::HELD))
	{
		if (_moveSpeed <= _maxMoveSpeed)
			_moveSpeed += _speedup;
	}
	else if (InputHandler::checkButton("SpeedDown", ButtonState::HELD))
	{
		if (_moveSpeed > 0)
			_moveSpeed -= _speedup;
		else
			_moveSpeed = 0;
	}

	if (InputHandler::checkButton("RollLeft", ButtonState::HELD))
	{
		_transform.rotate({ 0, 0, -_rollSpeed });
		_pilotViewDirValid = false;
	}
	else if (InputHandler::checkButton("RollRight", ButtonState::HELD))
	{
		_transform.rotate({ 0, 0, _rollSpeed });
		_pilotViewDirValid = false;
	}

	if (InputHandler::checkButton("PitchUp", ButtonState::HELD))
	{
		_transform.rotate({ _pitchSpeed, 0, 0 });
		_pilotViewDirValid = false;
	}
	else if (InputHandler::checkButton("PitchDown", ButtonState::HELD))
	{
		_transform.rotate({ -_pitchSpeed, 0, 0 });
		_pilotViewDirValid = false;
	}

	_transform.translate(_transform.forward() * _moveSpeed);
}

void Spaceship::onCollide(Collision c, const Rigidbody * body)
{
	if (_collisionResponse)
		_transform.translate(c.normal * c.depth);
}
