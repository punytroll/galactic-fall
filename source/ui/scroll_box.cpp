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

#include "../color.h"
#include "scroll_bar.h"
#include "scroll_box.h"

UI::ScrollBox::ScrollBox(UI::Widget * SupWidget) :
	UI::Widget(SupWidget)
{
	m_View = new UI::Widget(this);
	m_View->SetPosition(Vector2f(0.0f, 0.0f));
	m_View->SetSize(Vector2f(GetSize()[0] - 20.0f, GetSize()[1] - 20.0f));
	m_View->SetAnchorBottom(true);
	m_View->SetAnchorLeft(true);
	m_View->SetAnchorRight(true);
	m_View->SetAnchorTop(true);
	m_View->ConnectSizeChangedCallback(std::bind(&UI::ScrollBox::OnContentOrViewSizeChanged, this));
	m_Content = new UI::Widget(m_View);
	m_Content->SetPosition(Vector2f(0.0f, 0.0f));
	m_Content->SetBackgroundColor(Color(0.15f, 0.15f, 0.15f, 1.0f));
	m_Content->ConnectSizeChangedCallback(std::bind(&UI::ScrollBox::OnContentOrViewSizeChanged, this));
	m_HorizontalScrollBar = new UI::ScrollBar(this, UI::ScrollBar::Alignment::HORIZONTAL);
	m_HorizontalScrollBar->SetPosition(Vector2f(0.0f, GetSize()[1] - 20.0f));
	m_HorizontalScrollBar->SetSize(Vector2f(GetSize()[0] - 20.0f, 20.0f));
	m_HorizontalScrollBar->ConnectScrollPositionChangedCallback(std::bind(&UI::ScrollBox::OnHorizontalScrollPositionChanged, this));
	m_HorizontalScrollBar->SetAnchorBottom(true);
	m_HorizontalScrollBar->SetAnchorLeft(true);
	m_HorizontalScrollBar->SetAnchorRight(true);
	m_HorizontalScrollBar->SetAnchorTop(false);
	m_HorizontalScrollBar->SetMinimumPosition(0.0f);
	m_HorizontalScrollBar->SetMaximumPosition(std::max(0.0f, GetContent()->GetSize()[0] - GetView()->GetSize()[0]));
	m_HorizontalScrollBar->SetCurrentPosition(0.0f);
	m_HorizontalScrollBar->SetStepSize(m_HorizontalScrollBar->GetMaximumPosition() / 10.0f);
	m_VerticalScrollBar = new UI::ScrollBar(this, UI::ScrollBar::Alignment::VERTICAL);
	m_VerticalScrollBar->SetPosition(Vector2f(GetSize()[0] - 20.0f, 0.0f));
	m_VerticalScrollBar->SetSize(Vector2f(20.0f, GetSize()[1] - 20.0f));
	m_VerticalScrollBar->ConnectScrollPositionChangedCallback(std::bind(&UI::ScrollBox::OnVerticalScrollPositionChanged, this));
	m_VerticalScrollBar->SetAnchorBottom(true);
	m_VerticalScrollBar->SetAnchorLeft(false);
	m_VerticalScrollBar->SetAnchorRight(true);
	m_VerticalScrollBar->SetAnchorTop(true);
	m_VerticalScrollBar->SetMinimumPosition(0.0f);
	m_VerticalScrollBar->SetMaximumPosition(std::max(0.0f, GetContent()->GetSize()[1] - GetView()->GetSize()[1]));
	m_VerticalScrollBar->SetCurrentPosition(0.0f);
	m_VerticalScrollBar->SetStepSize(m_VerticalScrollBar->GetMaximumPosition() / 10.0f);
}

UI::ScrollBox::~ScrollBox(void)
{
}

void UI::ScrollBox::OnContentOrViewSizeChanged(void)
{
	// setting the position
	Vector2f NewContentPosition(GetContent()->GetPosition());
	bool SetContentPosition(false);
	
	if(GetContent()->GetPosition()[0] + GetContent()->GetSize()[0] < GetView()->GetSize()[0])
	{
		NewContentPosition[0] = std::min(0.0f, GetView()->GetSize()[0] - GetContent()->GetSize()[0]);
		SetContentPosition = true;
	}
	if(GetContent()->GetPosition()[1] + GetContent()->GetSize()[1] < GetView()->GetSize()[1])
	{
		NewContentPosition[1] = std::min(0.0f, GetView()->GetSize()[1] - GetContent()->GetSize()[1]);
		SetContentPosition = true;
	}
	if(SetContentPosition == true)
	{
		GetContent()->SetPosition(NewContentPosition);
	}
	
	// setting the position
	const std::list< Widget * > & ContentWidgets(GetContent()->GetSubWidgets());
	float Bottom(0.0f);
	float Right(0.0f);
	
	for(std::list< Widget * >::const_iterator ContentWidgetIterator = ContentWidgets.begin(); ContentWidgetIterator != ContentWidgets.end(); ++ContentWidgetIterator)
	{
		Bottom = std::max(Bottom, (*ContentWidgetIterator)->GetPosition()[1] + (*ContentWidgetIterator)->GetSize()[1]);
		Right = std::max(Right, (*ContentWidgetIterator)->GetPosition()[0] + (*ContentWidgetIterator)->GetSize()[0]);
	}
	//~ // add padding at the bottom and the right
	//~ /// @todo This has to leave eventually since it is not general.
	Bottom += 5.0f;
	Right += 5.0f;
	GetContent()->SetSize(Vector2f(std::max(Right, GetView()->GetSize()[0]), std::max(Bottom, GetView()->GetSize()[1])));
	
	m_HorizontalScrollBar->SetMaximumPosition(std::max(0.0f, GetContent()->GetSize()[0] - GetView()->GetSize()[0]));
	m_HorizontalScrollBar->SetStepSize(m_HorizontalScrollBar->GetMaximumPosition() / 10.0f);
	m_VerticalScrollBar->SetMaximumPosition(std::max(0.0f, GetContent()->GetSize()[1] - GetView()->GetSize()[1]));
	m_VerticalScrollBar->SetStepSize(m_VerticalScrollBar->GetMaximumPosition() / 10.0f);
}

void UI::ScrollBox::OnHorizontalScrollPositionChanged(void)
{
	float ViewSize(m_View->GetSize()[0]);
	float ContentSize(m_Content->GetSize()[0]);
	
	if(ContentSize > ViewSize)
	{
		m_Content->SetPosition(Vector2f(-m_HorizontalScrollBar->GetCurrentPosition(), m_Content->GetPosition()[1]));
	}
}

void UI::ScrollBox::OnVerticalScrollPositionChanged(void)
{
	float ViewSize(m_View->GetSize()[1]);
	float ContentSize(m_Content->GetSize()[1]);
	
	if(ContentSize > ViewSize)
	{
		m_Content->SetPosition(Vector2f(m_Content->GetPosition()[0], -m_VerticalScrollBar->GetCurrentPosition()));
	}
}

void UI::ScrollBox::SetHorizontalScrollBarVisible(bool Visible)
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

void UI::ScrollBox::SetVerticalScrollBarVisible(bool Visible)
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
