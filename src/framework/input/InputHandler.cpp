#include "InputHandler.h"

namespace Framework
{
	std::map<std::string, InputButton> InputHandler::inputs;
	std::vector<ButtonState> InputHandler::states;
	bool InputHandler::keys[348] = {};

	void InputHandler::updateButtonState(ButtonState *state, bool button)
	{
		if (button)
		{
			if (*state == ButtonState::DOWN)
				*state = ButtonState::HELD;
			else if (*state != ButtonState::HELD)
				*state = ButtonState::DOWN;
		}
		else
		{
			if (*state != ButtonState::UP
				&& (*state == ButtonState::DOWN
					|| *state == ButtonState::HELD))
				*state = ButtonState::UP;
			else
				*state = ButtonState::IDLE;
		}
	}

	void InputHandler::addInput(const std::string& name, int code)
	{
		InputButton b = { code, states.size() };
		states.push_back(ButtonState::IDLE);
		inputs[name] = b;
	}

	bool InputHandler::checkButton(const std::string& name, ButtonState state)
	{
		return states[inputs[name].stateIndex] == state;
	}

	void InputHandler::handleInput()
	{
		for (auto it = inputs.begin();
			it != inputs.end(); it++)
		{
			InputButton button = it->second;
			ButtonState *state = &states[button.stateIndex];

			updateButtonState(state, keys[button.code]);
		}
	}

	void InputHandler::updateKeys(int key, int action)
	{
		keys[key] = action;
	}
}