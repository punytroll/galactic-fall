/**
 * galactic-fall
 * Copyright (C) 2014  Hagen Möbius
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**/

#ifndef UI_MOUSE_BUTTON_EVENT_H
#define UI_MOUSE_BUTTON_EVENT_H

#include <X11/Xlib.h>

#include <algebra/vector2f.h>

#include "event.h"

namespace UI
{
	class MouseButtonEvent : public UI::Event
	{
	public:
		enum class MouseButton
		{
			Unspecified,
			Left,
			Middle,
			Right,
			WheelDown,
			WheelUp
		};
		
		MouseButtonEvent(void);
		// getters
		UI::MouseButtonEvent::MouseButton GetMouseButton(void) const;
		const Vector2f & GetPosition(void) const;
		bool IsDown(void) const;
		bool IsUp(void) const;
		// setters
		void SetButtonEvent(XButtonEvent * ButtonEvent);
		void SetPosition(const Vector2f & Position);
	private:
		// member variables
		bool _IsDown;
		UI::MouseButtonEvent::MouseButton _MouseButton;
		Vector2f _Position;
	};
}

inline UI::MouseButtonEvent::MouseButton UI::MouseButtonEvent::GetMouseButton(void) const
{
	return _MouseButton;
}

inline const Vector2f & UI::MouseButtonEvent::GetPosition(void) const
{
	return _Position;
}

inline bool UI::MouseButtonEvent::IsDown(void) const
{
	return _IsDown;
}

inline bool UI::MouseButtonEvent::IsUp(void) const
{
	return _IsDown == false;
}

inline void UI::MouseButtonEvent::SetPosition(const Vector2f & Position)
{
	_Position = Position;
}

#endif
