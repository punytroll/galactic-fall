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

#include "button.h"
#include "callbacks/callbacks.h"
#include "color.h"
#include "globals.h"

Button::Button(Widget * SupWidget) :
	Widget(SupWidget)
{
	SetBackgroundColor(Color(0.3f, 0.3f, 0.3f, 1.0f));
	SetDisabledBackgroundColor(Color(0.23f, 0.23f, 0.23f, 1.0f));
	ConnectMouseButtonCallback(Callback(this, &Button::OnMouseButton));
	ConnectMouseEnterCallback(Callback(this, &Button::OnMouseEnter));
	ConnectMouseLeaveCallback(Callback(this, &Button::OnMouseLeave));
}

Button::~Button(void)
{
}

ConnectionHandle Button::ConnectClickedCallback(Callback0< void > ClickedHandler)
{
	return _ClickedEvent.Connect(ClickedHandler);
}

void Button::DisconnectClickedCallback(ConnectionHandle & ConnectionHandle)
{
	_ClickedEvent.Disconnect(ConnectionHandle);
}

bool Button::OnMouseButton(int Button, int State, float X, float Y)
{
	if((Button == 1 /* LEFT */) && (State == EV_UP))
	{
		_ClickedEvent();
		
		return true;
	}
	
	return false;
}

void Button::OnMouseEnter(void)
{
	SetBackgroundColor(Color(0.4f, 0.4f, 0.4f, 1.0f));
}

void Button::OnMouseLeave(void)
{
	SetBackgroundColor(Color(0.3f, 0.3f, 0.3f, 1.0f));
}
