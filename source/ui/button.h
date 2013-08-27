/**
 * galactic-fall
 * Copyright (C) 2006  Hagen Möbius
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

#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include <string>

#include "../callbacks/events.h"
#include "widget.h"

namespace UI
{
	class Button : public UI::Widget
	{
	public:
		// constructors and deconstructors
		Button(UI::Widget * SupWidget);
		virtual ~Button(void);
		// connect and disconnect events
		ConnectionHandle ConnectClickedCallback(Callback0< void > ClickedHandler);
		void DisconnectClickedCallback(ConnectionHandle & ConnectionHandle);
	private:
		// callbacks
		bool _OnMouseButton(int Button, int State, float X, float Y);
		void _OnMouseEnter(void);
		void _OnMouseLeave(void);
		// member variables
		// events
		Event0< void > _ClickedEvent;
	};
}

#endif
