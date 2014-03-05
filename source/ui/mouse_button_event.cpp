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

#include <assert.h>

#include <iostream>

#include "mouse_button_event.h"

UI::MouseButtonEvent::MouseButtonEvent(void) :
	_IsDown(true),
	_MouseButton(UI::MouseButtonEvent::MouseButton::Unspecified)
{
}

void UI::MouseButtonEvent::SetButtonEvent(XButtonEvent * ButtonEvent)
{
	assert(ButtonEvent != nullptr);
	_IsDown = ButtonEvent->type == ButtonPress;
	switch(ButtonEvent->button)
	{
	case 1:
		{
			_MouseButton = UI::MouseButtonEvent::MouseButton::Left;
			
			break;
		}
	case 2:
		{
			_MouseButton = UI::MouseButtonEvent::MouseButton::Middle;
			
			break;
		}
	case 3:
		{
			_MouseButton = UI::MouseButtonEvent::MouseButton::Right;
			
			break;
		}
	case 4:
		{
			_MouseButton = UI::MouseButtonEvent::MouseButton::WheelUp;
			
			break;
		}
	case 5:
		{
			_MouseButton = UI::MouseButtonEvent::MouseButton::WheelDown;
			
			break;
		}
	default:
		{
			assert(false);
		}
	}
	_Position = Vector2f(static_cast< float >(ButtonEvent->x), static_cast< float >(ButtonEvent->y));
}
