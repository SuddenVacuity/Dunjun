
#include <Dunjun/Input.hpp>

namespace Dunjun
{
	namespace Input
	{
		void setInputMode(InputMode mode, int value)
		{
			int m = 0;
			if (mode == InputMode::Cursor)
				m = GLFW_CURSOR;
			if (mode == InputMode::StickyKeys)
				m = GLFW_STICKY_KEYS;
			if (mode == InputMode::StickyMouseButtons)
				m = GLFW_STICKY_MOUSE_BUTTONS;

			glfwSetInputMode(Game::getGlfwWindow(), m, value);
		}

		// get keyboard key pressed
		KeyState getKey(Key key)
		{
			return static_cast<KeyState>(glfwGetKey(Game::getGlfwWindow(), key));
		}

		// mouse
		Vector2 getCursorPosition()
		{
			f64 x, y;
			glfwGetCursorPos(Game::getGlfwWindow(), &x, &y);

			return Vector2(x, y);
		}
		void setCursorPosition(const Vector2& pos)
		{
			glfwSetCursorPos(Game::getGlfwWindow(), 
							 static_cast<f64>(pos.x), 
							 static_cast<f64>(pos.y));
		}


		f64 getTime()
		{
			return glfwGetTime();
		}
		void setTime(f64 time)
		{
			glfwSetTime(time);
		}

		// Gamepads
		GLOBAL std::array<XINPUT_STATE, Gamepad_maxCount> g_gamepadStates;

		void updateGamepads()
		{
			for(size_t i = 0; i < Gamepad_maxCount; i++)
			{
				isGamepadPresent((GamepadId)i);
			}
		}

		bool isGamepadPresent(GamepadId gamepadId)
		{
			if(gamepadId < Gamepad_maxCount)
				return XInputGetState(gamepadId, &g_gamepadStates[gamepadId]) == 0;

			return false;
		}

		GamepadAxes getGamepadAxes(GamepadId gamepadId)
		{
			GamepadAxes axes;

			axes.leftTrigger = g_gamepadStates[gamepadId].Gamepad.bLeftTrigger / 255.0f; // convert to correct size
			axes.rightTrigger = g_gamepadStates[gamepadId].Gamepad.bRightTrigger / 255.0f;
			
			axes.leftThumbStick.x = g_gamepadStates[gamepadId].Gamepad.sThumbLX / 32767.0f; // convert to correct size
			axes.leftThumbStick.y = g_gamepadStates[gamepadId].Gamepad.sThumbLY / 32767.0f;
			
			axes.rightThumbStick.x = g_gamepadStates[gamepadId].Gamepad.sThumbRX / 32767.0f; // convert to correct size
			axes.rightThumbStick.y = g_gamepadStates[gamepadId].Gamepad.sThumbRY / 32767.0f;

			return axes;
		}
		GamepadButtons getGamepadButtons(GamepadId gamepadId)
		{
			GamepadButtons buttons((size_t)XboxButton::Count);

			buttons[(size_t)XboxButton::DpadUp] =
				(g_gamepadStates[gamepadId].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP > 0);
			buttons[(size_t)XboxButton::DpadDown] =
				(g_gamepadStates[gamepadId].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN > 0);
			buttons[(size_t)XboxButton::DpadLeft] =
				(g_gamepadStates[gamepadId].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT > 0);
			buttons[(size_t)XboxButton::DpadRight] =
				(g_gamepadStates[gamepadId].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT > 0);

			buttons[(size_t)XboxButton::Start] =
				(g_gamepadStates[gamepadId].Gamepad.wButtons & XINPUT_GAMEPAD_START > 0);
			buttons[(size_t)XboxButton::Back] =
				(g_gamepadStates[gamepadId].Gamepad.wButtons & XINPUT_GAMEPAD_BACK > 0);

			buttons[(size_t)XboxButton::LeftThumb] =
				(g_gamepadStates[gamepadId].Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB > 0);
			buttons[(size_t)XboxButton::RightThumb] =
				(g_gamepadStates[gamepadId].Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB > 0);

			buttons[(size_t)XboxButton::LeftShoulder] =
				(g_gamepadStates[gamepadId].Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER > 0);
			buttons[(size_t)XboxButton::RightShoulder] =
				(g_gamepadStates[gamepadId].Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER > 0);

			buttons[(size_t)XboxButton::A] =
				(g_gamepadStates[gamepadId].Gamepad.wButtons & XINPUT_GAMEPAD_A > 0);
			buttons[(size_t)XboxButton::B] =
				(g_gamepadStates[gamepadId].Gamepad.wButtons & XINPUT_GAMEPAD_B > 0);
			buttons[(size_t)XboxButton::X] =
				(g_gamepadStates[gamepadId].Gamepad.wButtons & XINPUT_GAMEPAD_X > 0);
			buttons[(size_t)XboxButton::Y] =
				(g_gamepadStates[gamepadId].Gamepad.wButtons & XINPUT_GAMEPAD_Y > 0);

			return buttons;
		}
		std::string getGamepadName(GamepadId gamepadId)
		{
			return glfwGetJoystickName(gamepadId);
		}


	} // end Input
} // end Dunjun
