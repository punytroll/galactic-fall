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

#include "../callbacks/callbacks.h"
#include "../color.h"
#include "../globals.h"
#include "button.h"
#include "user_interface.h"

UI::Button::Button(Widget * SupWidget) :
	Widget(SupWidget)
{
	SetBackgroundColor(Color(0.3f, 0.3f, 0.3f, 1.0f));
	SetDisabledBackgroundColor(Color(0.23f, 0.23f, 0.23f, 1.0f));
	ConnectMouseButtonCallback(Callback(this, &Button::_OnMouseButton));
	ConnectMouseEnterCallback(Callback(this, &Button::_OnMouseEnter));
	ConnectMouseLeaveCallback(Callback(this, &Button::_OnMouseLeave));
}

UI::Button::~Button(void)
{
}

ConnectionHandle UI::Button::ConnectClickedCallback(std::function< void (void) > ClickedHandler)
{
	return _ClickedEvent.Connect(ClickedHandler);
}

void UI::Button::DisconnectClickedCallback(ConnectionHandle & ConnectionHandle)
{
	_ClickedEvent.Disconnect(ConnectionHandle);
}

bool UI::Button::_OnMouseButton(int Button, int State, float X, float Y)
{
	if(Button == EV_MOUSE_BUTTON_LEFT)
	{
		if(State == EV_DOWN)
		{
			g_UserInterface->SetCaptureWidget(this);
			
			return true;
		}
		else if((State == EV_UP) && (g_UserInterface->GetCaptureWidget() == this))
		{
			g_UserInterface->ReleaseCaptureWidget();
			if((X >= 0) && (X <= GetSize()[0]) && (Y >= 0) && (Y <= GetSize()[1]))
			{
				_ClickedEvent();
			}
			
			return true;
		}
	}
	
	return false;
}

void UI::Button::_OnMouseEnter(void)
{
	SetBackgroundColor(Color(0.4f, 0.4f, 0.4f, 1.0f));
}

void UI::Button::_OnMouseLeave(void)
{
	SetBackgroundColor(Color(0.3f, 0.3f, 0.3f, 1.0f));
}
