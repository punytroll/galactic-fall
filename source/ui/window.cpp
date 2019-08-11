/**
 * galactic-fall
 * Copyright (C) 2006-2019  Hagen Möbius
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

#include <expressions/operators.h>

#include <graphics/color_rgbo.h>

#include "../globals.h"
#include "border.h"
#include "label.h"
#include "mouse_button_event.h"
#include "mouse_move_event.h"
#include "user_interface.h"
#include "window.h"

using namespace Expressions::Operators;

UI::Window::Window(void) :
	_ResizeDragBox(nullptr),
	_TitleLabel(nullptr)
{
	SetBackgroundColor(Graphics::ColorRGBO(0.2f, 0.2f, 0.2f, 1.0f));
	
	// create components
	auto Border{new UI::Border{}};
	
	_ResizeDragBox = new Widget{};
	_TitleLabel = new UI::Label{};
	// initialize components
	Border->SetLeft(0.0_c);
	Border->SetTop(0.0_c);
	Border->SetWidth(width(this));
	Border->SetHeight(height(this));
	Border->SetLineWidth(1.0f);
	Border->SetColor(Graphics::ColorRGBO(0.4f, 0.4f, 0.4f, 1.0f));
	_TitleLabel->SetLeft(10.0_c);
	_TitleLabel->SetTop(10.0_c);
	_TitleLabel->SetWidth(width(this) - 2.0_c * 10.0_c);
	_TitleLabel->SetHeight(20.0_c);
	_TitleLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Center);
	_TitleLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	_TitleLabel->SetBackgroundColor(Graphics::ColorRGBO(0.23f, 0.35f, 0.55f, 1.0f));
	_TitleLabel->ConnectMouseButtonCallback(std::bind(&UI::Window::_OnTitleLabelMouseButton, this, std::placeholders::_1));
	_TitleLabel->ConnectMouseMoveCallback(std::bind(&UI::Window::_OnTitleLabelMouseMove, this, std::placeholders::_1));
	_ResizeDragBox->SetLeft(width(this) - 9.0_c);
	_ResizeDragBox->SetTop(height(this) - 9.0_c);
	_ResizeDragBox->SetWidth(7.0_c);
	_ResizeDragBox->SetHeight(7.0_c);
	_ResizeDragBox->SetBackgroundColor(Graphics::ColorRGBO(0.23f, 0.35f, 0.55f, 1.0f));
	_ResizeDragBox->ConnectMouseButtonCallback(std::bind(&UI::Window::_OnResizeDragBoxMouseButton, this, std::placeholders::_1));
	_ResizeDragBox->ConnectMouseMoveCallback(std::bind(&UI::Window::_OnResizeDragBoxMouseMove, this, std::placeholders::_1));
	// add components
	AddSubWidget(Border);
	AddSubWidget(_ResizeDragBox);
	AddSubWidget(_TitleLabel);
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
		SetLeft(constant(GetLeft() + MouseMoveEvent.GetPosition()[0] - _GrabPosition[0]));
		SetTop(constant(GetTop() + MouseMoveEvent.GetPosition()[1] - _GrabPosition[1]));
	}
}

void UI::Window::_OnResizeDragBoxMouseMove(UI::MouseMoveEvent & MouseMoveEvent)
{
	if((MouseMoveEvent.GetPhase() == UI::Event::Phase::Target) && (g_UserInterface->GetCaptureWidget() == _ResizeDragBox))
	{
		SetWidth(constant(GetWidth() + MouseMoveEvent.GetPosition()[0] - _GrabPosition[0]));
		SetHeight(constant(GetHeight() + MouseMoveEvent.GetPosition()[1] - _GrabPosition[1]));
	}
}
