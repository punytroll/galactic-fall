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

#include "color.h"
#include "scroll_bar.h"
#include "scroll_box.h"

ScrollBox::ScrollBox(Widget * SupWidget) :
	Widget(SupWidget)
{
	m_View = new Widget(this);
	m_View->SetPosition(Vector2f(0.0f, 0.0f));
	m_View->SetSize(Vector2f(GetSize()[0] - 20.0f, GetSize()[1] - 20.0f));
	m_View->SetAnchorBottom(true);
	m_View->SetAnchorLeft(true);
	m_View->SetAnchorRight(true);
	m_View->SetAnchorTop(true);
	m_Content = new Widget(m_View);
	m_Content->SetBackgroundColor(Color(0.15f, 0.15f, 0.15f, 1.0f));
	m_HorizontalScrollBar = new ScrollBar(this, ScrollBar::HORIZONTAL);
	m_HorizontalScrollBar->SetPosition(Vector2f(0.0f, GetSize()[1] - 20.0f));
	m_HorizontalScrollBar->SetSize(Vector2f(GetSize()[0] - 20.0f, 20.0f));
	m_HorizontalScrollBar->AddScrollPositionChangedListener(this);
	m_HorizontalScrollBar->SetAnchorBottom(true);
	m_HorizontalScrollBar->SetAnchorLeft(true);
	m_HorizontalScrollBar->SetAnchorRight(true);
	m_HorizontalScrollBar->SetAnchorTop(false);
	m_VerticalScrollBar = new ScrollBar(this, ScrollBar::VERTICAL);
	m_VerticalScrollBar->SetPosition(Vector2f(GetSize()[0] - 20.0f, 0.0f));
	m_VerticalScrollBar->SetSize(Vector2f(20.0f, GetSize()[1] - 20.0f));
	m_VerticalScrollBar->AddScrollPositionChangedListener(this);
	m_VerticalScrollBar->SetAnchorBottom(true);
	m_VerticalScrollBar->SetAnchorLeft(false);
	m_VerticalScrollBar->SetAnchorRight(true);
	m_VerticalScrollBar->SetAnchorTop(true);
}

ScrollBox::~ScrollBox(void)
{
}

bool ScrollBox::OnScrollPositionChanged(Widget * EventSource)
{
	if(EventSource == m_VerticalScrollBar)
	{
		float ViewSize(m_View->GetSize()[1]);
		float ContentSize(m_Content->GetSize()[1]);
		
		if(ContentSize > ViewSize)
		{
			m_Content->SetPosition(Vector2f(m_Content->GetPosition()[0], m_VerticalScrollBar->GetCurrentPosition() * (ViewSize - ContentSize)));
		}
		
		return true;
	}
	else if(EventSource == m_HorizontalScrollBar)
	{
		float ViewSize(m_View->GetSize()[0]);
		float ContentSize(m_Content->GetSize()[0]);
		
		if(ContentSize > ViewSize)
		{
			m_Content->SetPosition(Vector2f(m_HorizontalScrollBar->GetCurrentPosition() * (ViewSize - ContentSize), m_Content->GetPosition()[1]));
		}
		
		return true;
	}
	
	return false;
}

void ScrollBox::SetHorizontalScrollBarVisible(bool Visible)
{
	if(Visible != m_HorizontalScrollBar->IsVisible())
	{
		if(Visible == true)
		{
			m_View->SetSize(Vector2f(m_View->GetSize()[0], GetSize()[1] - 20.0f));
			m_VerticalScrollBar->SetSize(Vector2f(m_VerticalScrollBar->GetSize()[0], GetSize()[1] - 20.0f));
		}
		else
		{
			m_View->SetSize(Vector2f(m_View->GetSize()[0], GetSize()[1]));
			m_VerticalScrollBar->SetSize(Vector2f(m_VerticalScrollBar->GetSize()[0], GetSize()[1]));
		}
		m_HorizontalScrollBar->SetVisible(Visible);
	}
}

void ScrollBox::SetVerticalScrollBarVisible(bool Visible)
{
	if(Visible != m_VerticalScrollBar->IsVisible())
	{
		if(Visible == true)
		{
			m_View->SetSize(Vector2f(GetSize()[0] - 20.0f, m_View->GetSize()[1]));
			m_HorizontalScrollBar->SetSize(Vector2f(GetSize()[0] - 20.0f, m_HorizontalScrollBar->GetSize()[1]));
		}
		else
		{
			m_View->SetSize(Vector2f(GetSize()[0], m_View->GetSize()[1]));
			m_HorizontalScrollBar->SetSize(Vector2f(GetSize()[0], m_HorizontalScrollBar->GetSize()[1]));
		}
		m_VerticalScrollBar->SetVisible(Visible);
	}
}
