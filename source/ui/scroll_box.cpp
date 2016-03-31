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

#include "../graphics/color_rgbo.h"
#include "event.h"
#include "mouse_button_event.h"
#include "scroll_bar.h"
#include "scroll_box.h"

UI::ScrollBox::ScrollBox(UI::Widget * SupWidget) :
	UI::Widget(SupWidget)
{
	ConnectMouseButtonCallback(std::bind(&UI::ScrollBox::_OnMouseButton, this, std::placeholders::_1));
	_View = new UI::Widget(this);
	_View->SetLeft(0.0f);
	_View->SetTop(0.0f);
	_View->SetSize(Vector2f(GetSize()[0] - 20.0f, GetSize()[1] - 20.0f));
	_View->SetAnchorBottom(true);
	_View->SetAnchorLeft(true);
	_View->SetAnchorRight(true);
	_View->SetAnchorTop(true);
	_View->ConnectSizeChangedCallback(std::bind(&UI::ScrollBox::_OnContentOrViewSizeChanged, this, std::placeholders::_1));
	_Content = new UI::Widget(_View);
	_Content->SetLeft(0.0f);
	_Content->SetTop(0.0f);
	_Content->SetBackgroundColor(Graphics::ColorRGBO(0.15f, 0.15f, 0.15f, 1.0f));
	_Content->ConnectSizeChangedCallback(std::bind(&UI::ScrollBox::_OnContentOrViewSizeChanged, this, std::placeholders::_1));
	_HorizontalScrollBar = new UI::ScrollBar(this, UI::ScrollBar::Alignment::HORIZONTAL);
	_HorizontalScrollBar->SetLeft(0.0f);
	_HorizontalScrollBar->SetTop(GetSize()[1] - 20.0f);
	_HorizontalScrollBar->SetSize(Vector2f(GetSize()[0] - 20.0f, 20.0f));
	_HorizontalScrollBar->ConnectScrollPositionChangedCallback(std::bind(&UI::ScrollBox::_OnHorizontalScrollPositionChanged, this));
	_HorizontalScrollBar->SetAnchorBottom(true);
	_HorizontalScrollBar->SetAnchorLeft(true);
	_HorizontalScrollBar->SetAnchorRight(true);
	_HorizontalScrollBar->SetAnchorTop(false);
	_HorizontalScrollBar->SetMinimumPosition(0.0f);
	_HorizontalScrollBar->SetMaximumPosition(std::max(0.0f, _Content->GetSize()[0] - _View->GetSize()[0]));
	_HorizontalScrollBar->SetCurrentPosition(0.0f);
	_HorizontalScrollBar->SetStepSize(_HorizontalScrollBar->GetMaximumPosition() / 10.0f);
	_VerticalScrollBar = new UI::ScrollBar(this, UI::ScrollBar::Alignment::VERTICAL);
	_VerticalScrollBar->SetLeft(GetSize()[0] - 20.0f);
	_VerticalScrollBar->SetTop(0.0f);
	_VerticalScrollBar->SetSize(Vector2f(20.0f, GetSize()[1] - 20.0f));
	_VerticalScrollBar->ConnectScrollPositionChangedCallback(std::bind(&UI::ScrollBox::_OnVerticalScrollPositionChanged, this));
	_VerticalScrollBar->SetAnchorBottom(true);
	_VerticalScrollBar->SetAnchorLeft(false);
	_VerticalScrollBar->SetAnchorRight(true);
	_VerticalScrollBar->SetAnchorTop(true);
	_VerticalScrollBar->SetMinimumPosition(0.0f);
	_VerticalScrollBar->SetMaximumPosition(std::max(0.0f, _Content->GetSize()[1] - _View->GetSize()[1]));
	_VerticalScrollBar->SetCurrentPosition(0.0f);
	_VerticalScrollBar->SetStepSize(_VerticalScrollBar->GetMaximumPosition() / 10.0f);
}

UI::ScrollBox::~ScrollBox(void)
{
}

void UI::ScrollBox::_OnContentOrViewSizeChanged(UI::Event & SizeChangedEvent)
{
	if(SizeChangedEvent.GetPhase() == UI::Event::Phase::Target)
	{
		// setting the position
		if(_Content->GetLeft() + _Content->GetSize()[0] < _View->GetSize()[0])
		{
			_Content->SetLeft(std::min(0.0f, _View->GetSize()[0] - _Content->GetSize()[0]));
		}
		if(_Content->GetTop() + _Content->GetSize()[1] < _View->GetSize()[1])
		{
			_Content->SetTop(std::min(0.0f, _View->GetSize()[1] - _Content->GetSize()[1]));
		}
		
		// setting the position
		float Bottom(0.0f);
		float Right(0.0f);
		
		for(auto ContentWidget : _Content->GetSubWidgets())
		{
			Bottom = std::max(Bottom, ContentWidget->GetTop() + ContentWidget->GetSize()[1]);
			Right = std::max(Right, ContentWidget->GetLeft() + ContentWidget->GetSize()[0]);
		}
		//~ // add padding at the bottom and the right
		//~ /// @todo This has to leave eventually since it is not general.
		Bottom += 5.0f;
		Right += 5.0f;
		_Content->SetSize(Vector2f(std::max(Right, _View->GetSize()[0]), std::max(Bottom, _View->GetSize()[1])));
		
		_HorizontalScrollBar->SetMaximumPosition(std::max(0.0f, _Content->GetSize()[0] - _View->GetSize()[0]));
		_HorizontalScrollBar->SetStepSize(_HorizontalScrollBar->GetMaximumPosition() / 10.0f);
		_VerticalScrollBar->SetMaximumPosition(std::max(0.0f, _Content->GetSize()[1] - _View->GetSize()[1]));
		_VerticalScrollBar->SetStepSize(_VerticalScrollBar->GetMaximumPosition() / 10.0f);
	}
}

void UI::ScrollBox::_OnHorizontalScrollPositionChanged(void)
{
	if(_Content->GetSize()[0] > _View->GetSize()[0])
	{
		_Content->SetLeft(-_HorizontalScrollBar->GetCurrentPosition());
	}
}

void UI::ScrollBox::_OnMouseButton(UI::MouseButtonEvent & MouseButtonEvent)
{
	if((MouseButtonEvent.GetMouseButton() == UI::MouseButtonEvent::MouseButton::WheelUp) && (MouseButtonEvent.IsDown() == true))
	{
		if(_VerticalScrollBar->IsVisible() == true)
		{
			_VerticalScrollBar->StepLess();
			MouseButtonEvent.StopPropagation();
		}
		else if(_HorizontalScrollBar->IsVisible() == true)
		{
			_HorizontalScrollBar->StepLess();
			MouseButtonEvent.StopPropagation();
		}
	}
	else if((MouseButtonEvent.GetMouseButton() == UI::MouseButtonEvent::MouseButton::WheelDown) && (MouseButtonEvent.IsDown() == true))
	{
		if(_VerticalScrollBar->IsVisible() == true)
		{
			_VerticalScrollBar->StepMore();
			MouseButtonEvent.StopPropagation();
		}
		else if(_HorizontalScrollBar->IsVisible() == true)
		{
			_HorizontalScrollBar->StepMore();
			MouseButtonEvent.StopPropagation();
		}
	}
}

void UI::ScrollBox::_OnVerticalScrollPositionChanged(void)
{
	if(_Content->GetSize()[1] > _View->GetSize()[1])
	{
		_Content->SetTop(-_VerticalScrollBar->GetCurrentPosition());
	}
}

void UI::ScrollBox::SetHorizontalScrollBarVisible(bool Visible)
{
	if(Visible != _HorizontalScrollBar->IsVisible())
	{
		if(Visible == true)
		{
			_View->SetSize(Vector2f(_View->GetSize()[0], GetSize()[1] - 20.0f));
			_VerticalScrollBar->SetSize(Vector2f(_VerticalScrollBar->GetSize()[0], GetSize()[1] - 20.0f));
		}
		else
		{
			_View->SetSize(Vector2f(_View->GetSize()[0], GetSize()[1]));
			_VerticalScrollBar->SetSize(Vector2f(_VerticalScrollBar->GetSize()[0], GetSize()[1]));
		}
		_HorizontalScrollBar->SetVisible(Visible);
	}
}

void UI::ScrollBox::SetVerticalScrollBarVisible(bool Visible)
{
	if(Visible != _VerticalScrollBar->IsVisible())
	{
		if(Visible == true)
		{
			_View->SetSize(Vector2f(GetSize()[0] - 20.0f, _View->GetSize()[1]));
			_HorizontalScrollBar->SetSize(Vector2f(GetSize()[0] - 20.0f, _HorizontalScrollBar->GetSize()[1]));
		}
		else
		{
			_View->SetSize(Vector2f(GetSize()[0], _View->GetSize()[1]));
			_HorizontalScrollBar->SetSize(Vector2f(GetSize()[0], _HorizontalScrollBar->GetSize()[1]));
		}
		_VerticalScrollBar->SetVisible(Visible);
	}
}
