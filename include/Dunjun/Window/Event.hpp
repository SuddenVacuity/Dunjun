#ifndef DUNJUN_WINDOW_EVENT_HPP
#define DUNJUN_WINDOW_EVENT_HPP

#include <Dunjun/Window/Input.hpp>

namespace Dunjun
{
	struct Event
	{
		struct Size
		{
			u32 width;
			u32 height;
		};

		struct Move
		{
			s32 x;
			s32 y;
		};

		struct Key
		{
			Input::Key code;
			bool alt;
			bool ctrl;
			bool shift;
			bool system;
			bool capsLock;
			bool numLock;
		};

		struct MouseMove
		{
			int x;
			int y;
		};

		struct MouseButton
		{
			Input::MouseButton button;
			u8 clicks;
			int x;
			int y;
		};

		struct MouseScrollWheel
		{
			int deltaX;
			int deltaY;
		};

		struct Gamepad
		{
			u32 id;
		};

		struct GamepadButton
		{
			u32 id;
			Input::GamepadButton button;
		};

		struct GamepadAxis
		{
			u32 id;
			Input::GamepadAxis axis;
			f32 value;
		};

		enum EventType
		{
			Closed				 ,
			Resized				 ,
			LostFocus			 ,
			GainedFocus			 ,
			Moved				 ,
								 
			KeyPressed			 ,
			KeyReleased			 ,
								
			MouseWheelScrolled	 ,
			MouseButtonPressed	 ,
			MouseButtonReleased	 ,
			MouseMoved			 ,
			MouseEntered		 ,
			MouseLeft			 ,

			GamepadAdded		 ,
			GamepadRemoved		 ,
			GamepadRemapped		 ,

			GamepadButtonPressed ,
			GamepadButtonReleased,
		 
			GamepadAxisChanged	 ,


			Count
		};

		EventType type;

		union
		{
			Size			 size;
			Move			 move;

			Key				 key;

			MouseMove		 mouseMove;
			MouseButton		 mouseButton;
			MouseScrollWheel mouseScrollWheel;

			Gamepad			 gamepad;
			GamepadButton	 gamepadButton;
			GamepadAxis		 gamepadAxis;
		};

	};
} // end Dunjun

#endif
