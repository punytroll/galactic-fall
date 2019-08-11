/**
 * galactic-fall
 * Copyright (C) 2007-2019  Hagen Möbius
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
#include "button.h"
#include "mouse_button_event.h"
#include "mouse_move_event.h"
#include "scroll_bar.h"
#include "user_interface.h"

using namespace Expressions::Operators;

static const float g_ScrollBarTrackerBorderWidth = 4.0f;

UI::ScrollBar::ScrollBar(UI::ScrollBar::Alignment Alignment) :
	_Alignment(UI::ScrollBar::Alignment::UNDEFINED),
	_CurrentPosition(0.0f),
	_MaximumPosition(0.0f),
	_MinimumPosition(0.0f)
{
	ConnectHeightChangedCallback(std::bind(&ScrollBar::_OnSizeChanged, this, std::placeholders::_1));
	ConnectWidthChangedCallback(std::bind(&ScrollBar::_OnSizeChanged, this, std::placeholders::_1));
	SetBackgroundColor(Graphics::ColorRGBO(0.23f, 0.23f, 0.23f, 1.0f));
	// create components
	_LessButton = new UI::Button{};
	_MoreButton = new UI::Button{};
	_Tracker = new UI::Widget{};
	// initialize components
	_LessButton->ConnectClickedCallback(std::bind(&UI::ScrollBar::_OnLessClicked, this));
	_MoreButton->ConnectClickedCallback(std::bind(&UI::ScrollBar::_OnMoreClicked, this));
	_Tracker->SetBackgroundColor(Graphics::ColorRGBO(0.3f, 0.3f, 0.3f, 1.0f));
	_Tracker->ConnectMouseEnterCallback(std::bind(&UI::ScrollBar::_OnTrackerMouseEnter, this));
	_Tracker->ConnectMouseLeaveCallback(std::bind(&UI::ScrollBar::_OnTrackerMouseLeave, this));
	_Tracker->ConnectMouseButtonCallback(std::bind(&UI::ScrollBar::_OnTrackerMouseButton, this, std::placeholders::_1));
	_Tracker->ConnectMouseMoveCallback(std::bind(&UI::ScrollBar::_OnTrackerMouseMove, this, std::placeholders::_1));
	// add components
	AddSubWidget(_LessButton);
	AddSubWidget(_MoreButton);
	AddSubWidget(_Tracker);
	SetAlignment(Alignment);
	SetMinimumPosition(0.0f);
	SetMaximumPosition(1.0f);
	SetCurrentPosition(0.0f);
	SetStepSize(0.1f);
}

UI::ScrollBar::~ScrollBar(void)
{
}

Connection UI::ScrollBar::ConnectScrollPositionChangedCallback(std::function< void (void) > Callback)
{
	return _ScrollPositionChangedEvent.Connect(Callback);
}

void UI::ScrollBar::DisconnectScrollPositionChangedCallback(Connection & Connection)
{
	_ScrollPositionChangedEvent.Disconnect(Connection);
}

float UI::ScrollBar::_GetAvailableRange(void)
{
	if(_Alignment == UI::ScrollBar::Alignment::HORIZONTAL)
	{
		return GetWidth() - _LessButton->GetWidth() - g_ScrollBarTrackerBorderWidth - _Tracker->GetWidth() - g_ScrollBarTrackerBorderWidth - _MoreButton->GetWidth();
	}
	else if(_Alignment == UI::ScrollBar::Alignment::VERTICAL)
	{
		return GetHeight() - _LessButton->GetHeight() - g_ScrollBarTrackerBorderWidth - _Tracker->GetHeight() - g_ScrollBarTrackerBorderWidth - _MoreButton->GetHeight();
	}
	else
	{
		assert(false);
	}
}

void UI::ScrollBar::_OnLessClicked(void)
{
	SetCurrentPosition(GetCurrentPosition() - GetStepSize());
}

void UI::ScrollBar::_OnMoreClicked(void)
{
	SetCurrentPosition(GetCurrentPosition() + GetStepSize());
}

void UI::ScrollBar::_OnSizeChanged(UI::Event & SizeChangedEvent)
{
	if(SizeChangedEvent.GetPhase() == UI::Event::Phase::Target)
	{
		_AdjustTrackerPosition();
	}
}

void UI::ScrollBar::_OnTrackerMouseButton(UI::MouseButtonEvent & MouseButtonEvent)
{
	GetSupWidget()->RaiseSubWidget(this);
	if((MouseButtonEvent.GetPhase() == UI::Event::Phase::Target) && (MouseButtonEvent.GetMouseButton() == UI::MouseButtonEvent::MouseButton::Left))
	{
		if(MouseButtonEvent.IsDown() == true)
		{
			_GrabPosition = MouseButtonEvent.GetPosition();
			g_UserInterface->SetCaptureWidget(_Tracker);
		}
		else if(MouseButtonEvent.IsUp() == true)
		{
			g_UserInterface->ReleaseCaptureWidget();
		}
	}
}

void UI::ScrollBar::_OnTrackerMouseEnter(void)
{
	_Tracker->SetBackgroundColor(Graphics::ColorRGBO(0.4f, 0.4f, 0.4f, 1.0f));
}

void UI::ScrollBar::_OnTrackerMouseLeave(void)
{
	_Tracker->SetBackgroundColor(Graphics::ColorRGBO(0.3f, 0.3f, 0.3f, 1.0f));
}

void UI::ScrollBar::_OnTrackerMouseMove(UI::MouseMoveEvent & MouseMoveEvent)
{
	if(MouseMoveEvent.GetPhase() == UI::Event::Phase::Target)
	{
		if(g_UserInterface->GetCaptureWidget() == _Tracker)
		{
			if(_Alignment == ScrollBar::Alignment::HORIZONTAL)
			{
				SetCurrentPosition(GetCurrentPosition() + (((MouseMoveEvent.GetPosition()[0] - _GrabPosition[0]) * (GetMaximumPosition() - GetMinimumPosition()) / _GetAvailableRange())));
			}
			else if(_Alignment == ScrollBar::Alignment::VERTICAL)
			{
				SetCurrentPosition(GetCurrentPosition() + (((MouseMoveEvent.GetPosition()[1] - _GrabPosition[1]) * (GetMaximumPosition() - GetMinimumPosition()) / _GetAvailableRange())));
			}
		}
	}
}

void UI::ScrollBar::SetAlignment(UI::ScrollBar::Alignment Alignment)
{
	if(Alignment != GetAlignment())
	{
		if(Alignment == UI::ScrollBar::Alignment::HORIZONTAL)
		{
			_LessButton->SetLeft(0.0_c);
			_LessButton->SetTop(0.0_c);
			_LessButton->SetWidth(20.0_c);
			_LessButton->SetHeight(height(this));
			_MoreButton->SetLeft(width(this) - width(_MoreButton));
			_MoreButton->SetTop(0.0_c);
			_MoreButton->SetWidth(20.0_c);
			_MoreButton->SetHeight(height(this));
			_Tracker->SetLeft(right(_LessButton) + constant(g_ScrollBarTrackerBorderWidth));
			_Tracker->SetTop(constant(g_ScrollBarTrackerBorderWidth));
			_Tracker->SetWidth(20.0_c);
			_Tracker->SetHeight(height(this) - 2.0_c * constant(g_ScrollBarTrackerBorderWidth));
			_Alignment = UI::ScrollBar::Alignment::HORIZONTAL;
		}
		else if(Alignment == UI::ScrollBar::Alignment::VERTICAL)
		{
			_LessButton->SetLeft(0.0_c);
			_LessButton->SetTop(0.0_c);
			_LessButton->SetWidth(width(this));
			_LessButton->SetHeight(20.0_c);
			_MoreButton->SetLeft(0.0_c);
			_MoreButton->SetTop(height(this) - height(_MoreButton));
			_MoreButton->SetWidth(width(this));
			_MoreButton->SetHeight(20.0_c);
			_Tracker->SetLeft(constant(g_ScrollBarTrackerBorderWidth));
			_Tracker->SetTop(bottom(_LessButton) + constant(g_ScrollBarTrackerBorderWidth));
			_Tracker->SetWidth(width(this) - 2.0_c * constant(g_ScrollBarTrackerBorderWidth));
			_Tracker->SetHeight(20.0_c);
			_Alignment = UI::ScrollBar::Alignment::VERTICAL;
		}
	}
}

void UI::ScrollBar::SetCurrentPosition(float CurrentPosition)
{
	if(CurrentPosition > GetMaximumPosition())
	{
		CurrentPosition = GetMaximumPosition();
	}
	if(CurrentPosition < GetMinimumPosition())
	{
		CurrentPosition = GetMinimumPosition();
	}
	if(CurrentPosition != GetCurrentPosition())
	{
		_CurrentPosition = CurrentPosition;
		_AdjustTrackerPosition();
		// fire the scroll position listeners
		_ScrollPositionChangedEvent();
	}
}

void UI::ScrollBar::SetMinimumPosition(float MinimumPosition)
{
	_MinimumPosition = MinimumPosition;
	if(GetCurrentPosition() < GetMinimumPosition())
	{
		SetCurrentPosition(GetMinimumPosition());
	}
	if(GetMaximumPosition() < GetMinimumPosition())
	{
		SetMaximumPosition(GetMinimumPosition());
	}
	_AdjustTrackerPosition();
}

void UI::ScrollBar::SetMaximumPosition(float MaximumPosition)
{
	_MaximumPosition = MaximumPosition;
	if(GetCurrentPosition() > GetMaximumPosition())
	{
		SetCurrentPosition(GetMaximumPosition());
	}
	if(GetMinimumPosition() > GetMaximumPosition())
	{
		SetMinimumPosition(GetMaximumPosition());
	}
	_AdjustTrackerPosition();
}

void UI::ScrollBar::_AdjustTrackerPosition(void)
{
	// adjust the tracker's position
	if(GetMaximumPosition() != GetMinimumPosition())
	{
		_Tracker->SetVisible(true);
		if(_Alignment == UI::ScrollBar::Alignment::HORIZONTAL)
		{
			_Tracker->SetLeft(constant(_LessButton->GetWidth() + g_ScrollBarTrackerBorderWidth + _GetAvailableRange() * ((_CurrentPosition - GetMinimumPosition()) / (GetMaximumPosition() - GetMinimumPosition()))));
		}
		else if(_Alignment == UI::ScrollBar::Alignment::VERTICAL)
		{
			_Tracker->SetTop(constant(_LessButton->GetHeight() + g_ScrollBarTrackerBorderWidth + _GetAvailableRange() * ((_CurrentPosition - GetMinimumPosition()) / (GetMaximumPosition() - GetMinimumPosition()))));
		}
	}
	else
	{
		_Tracker->SetVisible(false);
	}
}
