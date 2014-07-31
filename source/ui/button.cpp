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

#include "../color.h"
#include "../globals.h"
#include "button.h"
#include "mouse_button_event.h"
#include "user_interface.h"

UI::Button::Button(Widget * SupWidget) :
	Widget(SupWidget)
{
	SetBackgroundColor(Color(0.3f, 0.3f, 0.3f, 1.0f));
	SetDisabledBackgroundColor(Color(0.23f, 0.23f, 0.23f, 1.0f));
	ConnectMouseButtonCallback(std::bind(&UI::Button::_OnMouseButton, this, std::placeholders::_1));
	ConnectMouseEnterCallback(std::bind(&UI::Button::_OnMouseEnter, this));
	ConnectMouseLeaveCallback(std::bind(&UI::Button::_OnMouseLeave, this));
}

UI::Button::~Button(void)
{
}

Connection UI::Button::ConnectClickedCallback(std::function< void (void) > ClickedHandler)
{
	return _ClickedEvent.Connect(ClickedHandler);
}

void UI::Button::_OnMouseButton(UI::MouseButtonEvent & MouseButtonEvent)
{
	if((MouseButtonEvent.GetPhase() == UI::MouseButtonEvent::Phase::Capturing) && (MouseButtonEvent.GetMouseButton() == UI::MouseButtonEvent::MouseButton::Left))
	{
		if(MouseButtonEvent.IsDown() == true)
		{
			g_UserInterface->SetCaptureWidget(this);
		}
		else if((MouseButtonEvent.IsUp() == true) && (g_UserInterface->GetCaptureWidget() == this))
		{
			g_UserInterface->ReleaseCaptureWidget();
			if((MouseButtonEvent.GetPosition()[0] >= 0) && (MouseButtonEvent.GetPosition()[0] <= GetSize()[0]) && (MouseButtonEvent.GetPosition()[1] >= 0) && (MouseButtonEvent.GetPosition()[1] <= GetSize()[1]))
			{
				_ClickedEvent();
			}
		}
	}
}

void UI::Button::_OnMouseEnter(void)
{
	SetBackgroundColor(Color(0.4f, 0.4f, 0.4f, 1.0f));
}

void UI::Button::_OnMouseLeave(void)
{
	SetBackgroundColor(Color(0.3f, 0.3f, 0.3f, 1.0f));
}
