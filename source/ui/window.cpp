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
#include "border.h"
#include "label.h"
#include "mouse_button_event.h"
#include "user_interface.h"
#include "window.h"

UI::Window::Window(Widget * SupWidget, const std::string & Title) :
	Widget(SupWidget)
{
	SetBackgroundColor(Color(0.2f, 0.2f, 0.2f, 1.0f));
	
	auto Border(new UI::Border(this));
	
	Border->SetPosition(Vector2f(0.0f, 0.0f));
	Border->SetSize(GetSize());
	Border->SetAnchorBottom(true);
	Border->SetAnchorRight(true);
	Border->SetWidth(1.0f);
	Border->SetColor(Color(0.4f, 0.4f, 0.4f, 1.0f));
	_TitleLabel = new UI::Label(this, Title);
	_TitleLabel->SetPosition(Vector2f(10.0f, 10.0f));
	_TitleLabel->SetSize(Vector2f(GetSize()[0] - 20.0f, 20.0f));
	_TitleLabel->SetAnchorRight(true);
	_TitleLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	_TitleLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	_TitleLabel->SetBackgroundColor(Color(0.2f, 0.2f, 0.4f, 1.0f));
	_TitleLabel->ConnectMouseButtonCallback(std::bind(&UI::Window::_OnTitleLabelMouseButton, this, std::placeholders::_1));
	_TitleLabel->ConnectMouseMovedCallback(std::bind(&UI::Window::_OnTitleLabelMouseMoved, this, std::placeholders::_1, std::placeholders::_2));
	
	auto ResizeDragBox(new Widget(this));
	
	ResizeDragBox->SetPosition(Vector2f(GetSize()[0] - 9.0f, GetSize()[1] - 9.0f));
	ResizeDragBox->SetSize(Vector2f(7.0f, 7.0f));
	ResizeDragBox->SetAnchorBottom(true);
	ResizeDragBox->SetAnchorLeft(false);
	ResizeDragBox->SetAnchorRight(true);
	ResizeDragBox->SetAnchorTop(false);
	ResizeDragBox->SetBackgroundColor(Color(0.2f, 0.2f, 0.4f, 1.0f));
	ResizeDragBox->ConnectMouseButtonCallback(std::bind(&UI::Window::_OnResizeDragBoxMouseButton, this, std::placeholders::_1, ResizeDragBox));
	ResizeDragBox->ConnectMouseMovedCallback(std::bind(&UI::Window::_OnResizeDragBoxMouseMoved, this, std::placeholders::_1, std::placeholders::_2, ResizeDragBox));
}

void UI::Window::SetTitle(const std::string & Title)
{
	_TitleLabel->SetText(Title);
}

void UI::Window::_OnTitleLabelMouseButton(UI::MouseButtonEvent & MouseButtonEvent)
{
	if((MouseButtonEvent.GetPhase() == UI::Event::Phase::Bubbling) && (MouseButtonEvent.GetMouseButton() == UI::MouseButtonEvent::MouseButton::Left))
	{
		if(MouseButtonEvent.IsDown() == true)
		{
			_GrabPosition = MouseButtonEvent.GetPosition();
			g_UserInterface->SetCaptureWidget(_TitleLabel);
		}
		else
		{
			g_UserInterface->ReleaseCaptureWidget();
		}
	}
}

void UI::Window::_OnResizeDragBoxMouseButton(UI::MouseButtonEvent & MouseButtonEvent, Widget * ResizeDragBox)
{
	if((MouseButtonEvent.GetPhase() == UI::Event::Phase::Bubbling) && (MouseButtonEvent.GetMouseButton() == UI::MouseButtonEvent::MouseButton::Left))
	{
		if(MouseButtonEvent.IsDown() == true)
		{
			_GrabPosition = MouseButtonEvent.GetPosition();
			g_UserInterface->SetCaptureWidget(ResizeDragBox);
		}
		else
		{
			g_UserInterface->ReleaseCaptureWidget();
		}
	}
}

void UI::Window::_OnTitleLabelMouseMoved(float X, float Y)
{
	if(g_UserInterface->GetCaptureWidget() == _TitleLabel)
	{
		SetPosition(GetPosition() + Vector2f(X, Y) - _GrabPosition);
	}
}

void UI::Window::_OnResizeDragBoxMouseMoved(float X, float Y, Widget * ResizeDragBox)
{
	if(g_UserInterface->GetCaptureWidget() == ResizeDragBox)
	{
		SetSize(GetSize() + Vector2f(X, Y) - _GrabPosition);
	}
}
