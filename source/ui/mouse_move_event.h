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

#ifndef UI_MOUSE_MOVE_EVENT_H
#define UI_MOUSE_MOVE_EVENT_H

#include <X11/Xlib.h>

#include <algebra/vector2f.h>

#include "event.h"

namespace UI
{
	class MouseMoveEvent : public UI::Event
	{
	public:
		MouseMoveEvent(void);
		// getters
		const Vector2f & GetPosition(void) const;
		// setters
		void SetButtonEvent(XButtonEvent * ButtonEvent);
		void SetPosition(const Vector2f & Position);
	private:
		// member variables
		Vector2f _Position;
	};
}

inline const Vector2f & UI::MouseMoveEvent::GetPosition(void) const
{
	return _Position;
}

inline void UI::MouseMoveEvent::SetPosition(const Vector2f & Position)
{
	_Position = Position;
}

#endif
