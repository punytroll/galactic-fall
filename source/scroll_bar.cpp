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

#include "button.h"
#include "color.h"
#include "scroll_bar.h"
#include "scroll_position_changed_listener.h"

ScrollBar::ScrollBar(Widget * SupWidget, ScrollBar::Alignment Alignment) :
	Widget(SupWidget),
	m_CurrentPosition(0.0f)
{
	SetBackgroundColor(Color(0.23f, 0.23f, 0.23f, 1.0f));
	m_LessButton = new Button(this);
	m_LessButton->AddClickedListener(this);
	m_MoreButton = new Button(this);
	m_MoreButton->AddClickedListener(this);
	SetAlignment(Alignment);
	SetMinimumPosition(0.0f);
	SetMaximumPosition(1.0f);
	SetCurrentPosition(0.0f);
	SetStepSize(0.1f);
}

ScrollBar::~ScrollBar(void)
{
}

void ScrollBar::AddScrollPositionChangedListener(ScrollPositionChangedListener * ScrollPositionChangedListener)
{
	m_ScrollPositionChangedListeners.push_back(ScrollPositionChangedListener);
}

bool ScrollBar::OnClicked(Widget * EventSource)
{
	if(EventSource == m_LessButton)
	{
		SetCurrentPosition(GetCurrentPosition() - GetStepSize());
		
		return true;
	}
	else if(EventSource == m_MoreButton)
	{
		SetCurrentPosition(GetCurrentPosition() + GetStepSize());
		
		return true;
	}
	
	return false;
}

void ScrollBar::SetAlignment(ScrollBar::Alignment Alignment)
{
	if(Alignment == ScrollBar::HORIZONTAL)
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
	}
	else if(Alignment == ScrollBar::VERTICAL)
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
	}
}

void ScrollBar::SetCurrentPosition(float CurrentPosition)
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
		for(std::list< ScrollPositionChangedListener * >::iterator ScrollPositionChangedListenerIterator = m_ScrollPositionChangedListeners.begin(); ScrollPositionChangedListenerIterator != m_ScrollPositionChangedListeners.end(); ++ScrollPositionChangedListenerIterator)
		{
			if((*ScrollPositionChangedListenerIterator)->OnScrollPositionChanged(this) == true)
			{
				return;
			}
		}
	}
}
