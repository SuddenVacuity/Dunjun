#ifndef DUNJUN_EVENT_HPP
#define DUNJUN_EVENT_HPP

#include <Dunjun/Input.hpp>

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

		};

	};
} // end Dunjun

#endif
