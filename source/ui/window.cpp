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

#include "../globals.h"
#include "../graphics/color_rgbo.h"
#include "border.h"
#include "label.h"
#include "mouse_button_event.h"
#include "mouse_move_event.h"
#include "user_interface.h"
#include "window.h"

UI::Window::Window(Widget * SupWidget, const std::string & Title) :
	Widget(SupWidget),
	_ResizeDragBox(nullptr),
	_TitleLabel(nullptr)
{
	SetBackgroundColor(Graphics::ColorRGBO(0.2f, 0.2f, 0.2f, 1.0f));
	
	auto Border(new UI::Border(this));
	
	Border->SetPosition(Vector2f(0.0f, 0.0f));
	Border->SetSize(GetSize());
	Border->SetAnchorBottom(true);
	Border->SetAnchorRight(true);
	Border->SetWidth(1.0f);
	Border->SetColor(Graphics::ColorRGBO(0.4f, 0.4f, 0.4f, 1.0f));
	_TitleLabel = new UI::Label(this, Title);
	_TitleLabel->SetPosition(Vector2f(10.0f, 10.0f));
	_TitleLabel->SetSize(Vector2f(GetSize()[0] - 20.0f, 20.0f));
	_TitleLabel->SetAnchorRight(true);
	_TitleLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	_TitleLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	_TitleLabel->SetBackgroundColor(Graphics::ColorRGBO(0.23f, 0.35f, 0.55f, 1.0f));
	_TitleLabel->ConnectMouseButtonCallback(std::bind(&UI::Window::_OnTitleLabelMouseButton, this, std::placeholders::_1));
	_TitleLabel->ConnectMouseMoveCallback(std::bind(&UI::Window::_OnTitleLabelMouseMove, this, std::placeholders::_1));
	_ResizeDragBox = new Widget(this);
	_ResizeDragBox->SetPosition(Vector2f(GetSize()[0] - 9.0f, GetSize()[1] - 9.0f));
	_ResizeDragBox->SetSize(Vector2f(7.0f, 7.0f));
	_ResizeDragBox->SetAnchorBottom(true);
	_ResizeDragBox->SetAnchorLeft(false);
	_ResizeDragBox->SetAnchorRight(true);
	_ResizeDragBox->SetAnchorTop(false);
	_ResizeDragBox->SetBackgroundColor(Graphics::ColorRGBO(0.23f, 0.35f, 0.55f, 1.0f));
	_ResizeDragBox->ConnectMouseButtonCallback(std::bind(&UI::Window::_OnResizeDragBoxMouseButton, this, std::placeholders::_1));
	_ResizeDragBox->ConnectMouseMoveCallback(std::bind(&UI::Window::_OnResizeDragBoxMouseMove, this, std::placeholders::_1));
}

void UI::Window::SetTitle(const std::string & Title)
{
	_TitleLabel->SetText(Title);
}

void UI::Window::HideResizeDragBox(void)
{
	_ResizeDragBox->SetVisible(false);
}

void UI::Window::ShowResizeDragBox(void)
{
	_ResizeDragBox->SetVisible(true);
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

void UI::Window::_OnResizeDragBoxMouseButton(UI::MouseButtonEvent & MouseButtonEvent)
{
	if((MouseButtonEvent.GetPhase() == UI::Event::Phase::Bubbling) && (MouseButtonEvent.GetMouseButton() == UI::MouseButtonEvent::MouseButton::Left))
	{
		if(MouseButtonEvent.IsDown() == true)
		{
			_GrabPosition = MouseButtonEvent.GetPosition();
			g_UserInterface->SetCaptureWidget(_ResizeDragBox);
		}
		else
		{
			g_UserInterface->ReleaseCaptureWidget();
		}
	}
}

void UI::Window::_OnTitleLabelMouseMove(UI::MouseMoveEvent & MouseMoveEvent)
{
	if((MouseMoveEvent.GetPhase() == UI::Event::Phase::Target) && (g_UserInterface->GetCaptureWidget() == _TitleLabel))
	{
		SetPosition(GetPosition() + MouseMoveEvent.GetPosition() - _GrabPosition);
	}
}

void UI::Window::_OnResizeDragBoxMouseMove(UI::MouseMoveEvent & MouseMoveEvent)
{
	if((MouseMoveEvent.GetPhase() == UI::Event::Phase::Target) && (g_UserInterface->GetCaptureWidget() == _ResizeDragBox))
	{
		SetSize(GetSize() + MouseMoveEvent.GetPosition() - _GrabPosition);
	}
}
