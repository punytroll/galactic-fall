/**
 * galactic-fall
 * Copyright (C) 2007  Hagen Möbius
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
#include "button.h"
#include "mouse_button_event.h"
#include "mouse_move_event.h"
#include "scroll_bar.h"
#include "user_interface.h"

static const float g_ScrollBarTrackerBorderWidth = 4.0f;

UI::ScrollBar::ScrollBar(Widget * SupWidget, UI::ScrollBar::Alignment Alignment) :
	Widget(SupWidget),
	_Alignment(UI::ScrollBar::Alignment::UNDEFINED),
	_CurrentPosition(0.0f),
	_MaximumPosition(0.0f),
	_MinimumPosition(0.0f)
{
	ConnectSizeChangedCallback(std::bind(&ScrollBar::_OnSizeChanged, this, std::placeholders::_1));
	SetBackgroundColor(Graphics::ColorRGBO(0.23f, 0.23f, 0.23f, 1.0f));
	_LessButton = new UI::Button{this};
	_LessButton->ConnectClickedCallback(std::bind(&UI::ScrollBar::_OnLessClicked, this));
	_MoreButton = new UI::Button{this};
	_MoreButton->ConnectClickedCallback(std::bind(&UI::ScrollBar::_OnMoreClicked, this));
	_Tracker = new UI::Widget{this};
	_Tracker->SetBackgroundColor(Graphics::ColorRGBO(0.3f, 0.3f, 0.3f, 1.0f));
	_Tracker->ConnectMouseEnterCallback(std::bind(&UI::ScrollBar::_OnTrackerMouseEnter, this));
	_Tracker->ConnectMouseLeaveCallback(std::bind(&UI::ScrollBar::_OnTrackerMouseLeave, this));
	_Tracker->ConnectMouseButtonCallback(std::bind(&UI::ScrollBar::_OnTrackerMouseButton, this, std::placeholders::_1));
	_Tracker->ConnectMouseMoveCallback(std::bind(&UI::ScrollBar::_OnTrackerMouseMove, this, std::placeholders::_1));
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
				float AvailableRange = GetWidth() - _LessButton->GetWidth() - g_ScrollBarTrackerBorderWidth - _Tracker->GetWidth() - g_ScrollBarTrackerBorderWidth - _MoreButton->GetWidth();
				
				SetCurrentPosition(GetCurrentPosition() + (((MouseMoveEvent.GetPosition()[0] - _GrabPosition[0]) * (GetMaximumPosition() - GetMinimumPosition()) / AvailableRange)));
			}
			else if(_Alignment == ScrollBar::Alignment::VERTICAL)
			{
				float AvailableRange = GetHeight() - _LessButton->GetHeight() - g_ScrollBarTrackerBorderWidth - _Tracker->GetHeight() - g_ScrollBarTrackerBorderWidth - _MoreButton->GetHeight();
				
				SetCurrentPosition(GetCurrentPosition() + (((MouseMoveEvent.GetPosition()[1] - _GrabPosition[1]) * (GetMaximumPosition() - GetMinimumPosition()) / AvailableRange)));
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
			_LessButton->SetLeft(0.0f);
			_LessButton->SetTop(0.0f);
			_LessButton->SetSize(Vector2f(20.0f, GetHeight()));
			_LessButton->SetAnchorBottom(true);
			_LessButton->SetAnchorLeft(true);
			_LessButton->SetAnchorRight(false);
			_LessButton->SetAnchorTop(true);
			_MoreButton->SetLeft(GetWidth() - 20.0f);
			_MoreButton->SetTop(0.0f);
			_MoreButton->SetSize(Vector2f(20.0f, GetHeight()));
			_MoreButton->SetAnchorBottom(true);
			_MoreButton->SetAnchorLeft(false);
			_MoreButton->SetAnchorRight(true);
			_MoreButton->SetAnchorTop(true);
			_Tracker->SetLeft(_LessButton->GetWidth() + g_ScrollBarTrackerBorderWidth);
			_Tracker->SetTop(g_ScrollBarTrackerBorderWidth);
			_Tracker->SetSize(Vector2f(20.0f, GetHeight() - 2 * g_ScrollBarTrackerBorderWidth));
			_Tracker->SetAnchorBottom(true);
			_Tracker->SetAnchorLeft(false);
			_Tracker->SetAnchorRight(false);
			_Tracker->SetAnchorTop(true);
			_Alignment = UI::ScrollBar::Alignment::HORIZONTAL;
		}
		else if(Alignment == UI::ScrollBar::Alignment::VERTICAL)
		{
			_LessButton->SetLeft(0.0f);
			_LessButton->SetTop(0.0f);
			_LessButton->SetSize(Vector2f(GetWidth(), 20.0f));
			_LessButton->SetAnchorBottom(false);
			_LessButton->SetAnchorLeft(true);
			_LessButton->SetAnchorRight(true);
			_LessButton->SetAnchorTop(true);
			_MoreButton->SetLeft(0.0f);
			_MoreButton->SetTop(GetHeight() - 20.0f);
			_MoreButton->SetSize(Vector2f(GetWidth(), 20.0f));
			_MoreButton->SetAnchorBottom(true);
			_MoreButton->SetAnchorLeft(true);
			_MoreButton->SetAnchorRight(true);
			_MoreButton->SetAnchorTop(false);
			_Tracker->SetLeft(g_ScrollBarTrackerBorderWidth);
			_Tracker->SetTop(_LessButton->GetHeight() + g_ScrollBarTrackerBorderWidth);
			_Tracker->SetSize(Vector2f(GetWidth() - 2 * g_ScrollBarTrackerBorderWidth, 20.0f));
			_Tracker->SetAnchorBottom(false);
			_Tracker->SetAnchorLeft(true);
			_Tracker->SetAnchorRight(true);
			_Tracker->SetAnchorTop(false);
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
		if(GetAlignment() == UI::ScrollBar::Alignment::HORIZONTAL)
		{
			float AvailableRange = GetWidth() - _LessButton->GetWidth() - g_ScrollBarTrackerBorderWidth - _Tracker->GetWidth() - g_ScrollBarTrackerBorderWidth - _MoreButton->GetWidth();
			
			_Tracker->SetLeft(_LessButton->GetWidth() + g_ScrollBarTrackerBorderWidth + AvailableRange * ((_CurrentPosition - GetMinimumPosition()) / (GetMaximumPosition() - GetMinimumPosition())));
		}
		else if(GetAlignment() == UI::ScrollBar::Alignment::VERTICAL)
		{
			float AvailableRange = GetHeight() - _LessButton->GetHeight() - g_ScrollBarTrackerBorderWidth - _Tracker->GetHeight() - g_ScrollBarTrackerBorderWidth - _MoreButton->GetHeight();
			
			_Tracker->SetTop(_LessButton->GetHeight() + g_ScrollBarTrackerBorderWidth + AvailableRange * ((_CurrentPosition - GetMinimumPosition()) / (GetMaximumPosition() - GetMinimumPosition())));
		}
	}
	else
	{
		_Tracker->SetVisible(false);
	}
}
