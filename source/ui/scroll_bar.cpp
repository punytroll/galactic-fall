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

#include "../callbacks/callbacks.h"
#include "../color.h"
#include "../globals.h"
#include "../user_interface.h"
#include "button.h"
#include "scroll_bar.h"

static const float g_ScrollBarTrackerBorderWidth = 4.0f;

UI::ScrollBar::ScrollBar(Widget * SupWidget, UI::ScrollBar::Alignment Alignment) :
	Widget(SupWidget),
	m_Alignment(UI::ScrollBar::UNDEFINED),
	m_CurrentPosition(0.0f)
{
	ConnectSizeChangedCallback(Callback(this, &ScrollBar::OnSizeChanged));
	SetBackgroundColor(Color(0.23f, 0.23f, 0.23f, 1.0f));
	m_LessButton = new Button(this);
	m_LessButton->ConnectClickedCallback(Callback(this, &ScrollBar::OnLessClicked));
	m_MoreButton = new Button(this);
	m_MoreButton->ConnectClickedCallback(Callback(this, &ScrollBar::OnMoreClicked));
	m_Tracker = new Widget(this);
	m_Tracker->SetBackgroundColor(Color(0.3f, 0.3f, 0.3f, 1.0f));
	m_Tracker->ConnectMouseEnterCallback(Callback(this, &ScrollBar::OnTrackerMouseEnter));
	m_Tracker->ConnectMouseLeaveCallback(Callback(this, &ScrollBar::OnTrackerMouseLeave));
	m_Tracker->ConnectMouseButtonCallback(Callback(this, &ScrollBar::OnTrackerMouseButton));
	m_Tracker->ConnectMouseMovedCallback(Callback(this, &ScrollBar::OnTrackerMouseMoved));
	SetAlignment(Alignment);
	SetMinimumPosition(0.0f);
	SetMaximumPosition(1.0f);
	SetCurrentPosition(0.0f);
	SetStepSize(0.1f);
}

UI::ScrollBar::~ScrollBar(void)
{
}

ConnectionHandle UI::ScrollBar::ConnectScrollPositionChangedCallback(Callback0< void > Callback)
{
	return _ScrollPositionChangedEvent.Connect(Callback);
}

void UI::ScrollBar::DisconnectScrollPositionChangedCallback(ConnectionHandle ConnectionHandle)
{
	_ScrollPositionChangedEvent.Disconnect(ConnectionHandle);
}

void UI::ScrollBar::OnLessClicked(void)
{
	SetCurrentPosition(GetCurrentPosition() - GetStepSize());
}

void UI::ScrollBar::OnMoreClicked(void)
{
	SetCurrentPosition(GetCurrentPosition() + GetStepSize());
}

void UI::ScrollBar::OnSizeChanged(void)
{
	AdjustTrackerPosition();
}

bool UI::ScrollBar::OnTrackerMouseButton(int Button, int State, float X, float Y)
{
	GetSupWidget()->RaiseSubWidget(this);
	if(Button == 1)
	{
		if(State == EV_DOWN)
		{
			m_GrabPosition.Set(X, Y);
			g_UserInterface->SetCaptureWidget(m_Tracker);
		}
		else
		{
			g_UserInterface->ReleaseCaptureWidget();
		}
		
		return true;
	}
	
	return false;
}

void UI::ScrollBar::OnTrackerMouseEnter(void)
{
	m_Tracker->SetBackgroundColor(Color(0.4f, 0.4f, 0.4f, 1.0f));
}

void UI::ScrollBar::OnTrackerMouseLeave(void)
{
	m_Tracker->SetBackgroundColor(Color(0.3f, 0.3f, 0.3f, 1.0f));
}

void UI::ScrollBar::OnTrackerMouseMoved(float X, float Y)
{
	if(g_UserInterface->GetCaptureWidget() == m_Tracker)
	{
		if(m_Alignment == ScrollBar::HORIZONTAL)
		{
			float AvailableRange = GetSize()[0] - m_LessButton->GetSize()[0] - g_ScrollBarTrackerBorderWidth - m_Tracker->GetSize()[0] - g_ScrollBarTrackerBorderWidth - m_MoreButton->GetSize()[0];
			
			SetCurrentPosition(GetCurrentPosition() + (((X - m_GrabPosition[0]) * (GetMaximumPosition() - GetMinimumPosition()) / AvailableRange)));
		}
		else if(m_Alignment == ScrollBar::VERTICAL)
		{
			float AvailableRange = GetSize()[1] - m_LessButton->GetSize()[1] - g_ScrollBarTrackerBorderWidth - m_Tracker->GetSize()[1] - g_ScrollBarTrackerBorderWidth - m_MoreButton->GetSize()[1];
			
			SetCurrentPosition(GetCurrentPosition() + (((Y - m_GrabPosition[1]) * (GetMaximumPosition() - GetMinimumPosition()) / AvailableRange)));
		}
	}
}

void UI::ScrollBar::SetAlignment(UI::ScrollBar::Alignment Alignment)
{
	if(Alignment != GetAlignment())
	{
		if(Alignment == UI::ScrollBar::HORIZONTAL)
		{
			m_LessButton->SetPosition(Vector2f(0.0f, 0.0f));
			m_LessButton->SetSize(Vector2f(20.0f, GetSize()[1]));
			m_LessButton->SetAnchorBottom(true);
			m_LessButton->SetAnchorLeft(true);
			m_LessButton->SetAnchorRight(false);
			m_LessButton->SetAnchorTop(true);
			m_MoreButton->SetPosition(Vector2f(GetSize()[0] - 20.0f, 0.0f));
			m_MoreButton->SetSize(Vector2f(20.0f, GetSize()[1]));
			m_MoreButton->SetAnchorBottom(true);
			m_MoreButton->SetAnchorLeft(false);
			m_MoreButton->SetAnchorRight(true);
			m_MoreButton->SetAnchorTop(true);
			m_Tracker->SetPosition(Vector2f(m_LessButton->GetSize()[0] + g_ScrollBarTrackerBorderWidth, g_ScrollBarTrackerBorderWidth));
			m_Tracker->SetSize(Vector2f(20.0f, GetSize()[1] - 2 * g_ScrollBarTrackerBorderWidth));
			m_Tracker->SetAnchorBottom(true);
			m_Tracker->SetAnchorLeft(false);
			m_Tracker->SetAnchorRight(false);
			m_Tracker->SetAnchorTop(true);
			m_Alignment = UI::ScrollBar::HORIZONTAL;
		}
		else if(Alignment == UI::ScrollBar::VERTICAL)
		{
			m_LessButton->SetPosition(Vector2f(0.0f, 0.0f));
			m_LessButton->SetSize(Vector2f(GetSize()[0], 20.0f));
			m_LessButton->SetAnchorBottom(false);
			m_LessButton->SetAnchorLeft(true);
			m_LessButton->SetAnchorRight(true);
			m_LessButton->SetAnchorTop(true);
			m_MoreButton->SetPosition(Vector2f(0.0f, GetSize()[1] - 20.0f));
			m_MoreButton->SetSize(Vector2f(GetSize()[0], 20.0f));
			m_MoreButton->SetAnchorBottom(true);
			m_MoreButton->SetAnchorLeft(true);
			m_MoreButton->SetAnchorRight(true);
			m_MoreButton->SetAnchorTop(false);
			m_Tracker->SetPosition(Vector2f(g_ScrollBarTrackerBorderWidth, m_LessButton->GetSize()[1] + g_ScrollBarTrackerBorderWidth));
			m_Tracker->SetSize(Vector2f(GetSize()[0] - 2 * g_ScrollBarTrackerBorderWidth, 20.0f));
			m_Tracker->SetAnchorBottom(false);
			m_Tracker->SetAnchorLeft(true);
			m_Tracker->SetAnchorRight(true);
			m_Tracker->SetAnchorTop(false);
			m_Alignment = UI::ScrollBar::VERTICAL;
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
		m_CurrentPosition = CurrentPosition;
		AdjustTrackerPosition();
		// fire the scroll position listeners
		_ScrollPositionChangedEvent();
	}
}

void UI::ScrollBar::SetMinimumPosition(float MinimumPosition)
{
	m_MinimumPosition = MinimumPosition;
	if(GetCurrentPosition() < GetMinimumPosition())
	{
		SetCurrentPosition(GetMinimumPosition());
	}
	if(GetMaximumPosition() < GetMinimumPosition())
	{
		SetMaximumPosition(GetMinimumPosition());
	}
	AdjustTrackerPosition();
}

void UI::ScrollBar::SetMaximumPosition(float MaximumPosition)
{
	m_MaximumPosition = MaximumPosition;
	if(GetCurrentPosition() > GetMaximumPosition())
	{
		SetCurrentPosition(GetMaximumPosition());
	}
	if(GetMinimumPosition() > GetMaximumPosition())
	{
		SetMinimumPosition(GetMaximumPosition());
	}
	AdjustTrackerPosition();
}

void UI::ScrollBar::AdjustTrackerPosition(void)
{
	// adjust the tracker's position
	if(GetMaximumPosition() != GetMinimumPosition())
	{
		m_Tracker->SetVisible(true);
		if(GetAlignment() == UI::ScrollBar::HORIZONTAL)
		{
			float AvailableRange = GetSize()[0] - m_LessButton->GetSize()[0] - g_ScrollBarTrackerBorderWidth - m_Tracker->GetSize()[0] - g_ScrollBarTrackerBorderWidth - m_MoreButton->GetSize()[0];
			
			m_Tracker->SetPosition(Vector2f(m_LessButton->GetSize()[0] + g_ScrollBarTrackerBorderWidth + AvailableRange * ((m_CurrentPosition - GetMinimumPosition()) / (GetMaximumPosition() - GetMinimumPosition())), g_ScrollBarTrackerBorderWidth));
		}
		else if(GetAlignment() == UI::ScrollBar::VERTICAL)
		{
			float AvailableRange = GetSize()[1] - m_LessButton->GetSize()[1] - g_ScrollBarTrackerBorderWidth - m_Tracker->GetSize()[1] - g_ScrollBarTrackerBorderWidth - m_MoreButton->GetSize()[1];
			
			m_Tracker->SetPosition(Vector2f(g_ScrollBarTrackerBorderWidth, m_LessButton->GetSize()[1] + g_ScrollBarTrackerBorderWidth + AvailableRange * ((m_CurrentPosition - GetMinimumPosition()) / (GetMaximumPosition() - GetMinimumPosition()))));
		}
	}
	else
	{
		m_Tracker->SetVisible(false);
	}
}
