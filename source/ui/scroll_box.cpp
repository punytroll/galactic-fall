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

#include <expressions/operators.h>

#include "../graphics/color_rgbo.h"
#include "event.h"
#include "mouse_button_event.h"
#include "scroll_bar.h"
#include "scroll_box.h"

using namespace Expressions::Operators;

UI::ScrollBox::ScrollBox(UI::Widget * SupWidget) :
	UI::Widget(SupWidget)
{
	ConnectMouseButtonCallback(std::bind(&UI::ScrollBox::_OnMouseButton, this, std::placeholders::_1));
	SetBackgroundColor(Graphics::ColorRGBO(0.15f, 0.15f, 0.15f, 1.0f));
	_View = new UI::Widget{this};
	_View->SetName("view");
	_View->SetLeft(0.0_c);
	_View->SetTop(0.0f);
	_View->SetWidth(GetWidth() - 20.0f);
	_View->SetHeight(GetHeight() - 20.0f);
	_View->SetAnchorBottom(true);
	_View->SetAnchorLeft(true);
	_View->SetAnchorRight(true);
	_View->SetAnchorTop(true);
	_View->ConnectHeightChangedCallback(std::bind(&UI::ScrollBox::_OnContentOrViewSizeChanged, this, std::placeholders::_1));
	_View->ConnectWidthChangedCallback(std::bind(&UI::ScrollBox::_OnContentOrViewSizeChanged, this, std::placeholders::_1));
	_Content = new UI::Widget{_View};
	_Content->SetName("content");
	_Content->SetLeft(0.0_c);
	_Content->SetTop(0.0f);
	_Content->ConnectHeightChangedCallback(std::bind(&UI::ScrollBox::_OnContentOrViewSizeChanged, this, std::placeholders::_1));
	_Content->ConnectWidthChangedCallback(std::bind(&UI::ScrollBox::_OnContentOrViewSizeChanged, this, std::placeholders::_1));
	_HorizontalScrollBar = new UI::ScrollBar{this, UI::ScrollBar::Alignment::HORIZONTAL};
	_HorizontalScrollBar->SetName("horizontal_scroll_bar");
	_HorizontalScrollBar->SetLeft(0.0_c);
	_HorizontalScrollBar->SetTop(GetHeight() - 20.0f);
	_HorizontalScrollBar->SetWidth(GetWidth() - 20.0f);
	_HorizontalScrollBar->SetHeight(20.0f);
	_HorizontalScrollBar->ConnectScrollPositionChangedCallback(std::bind(&UI::ScrollBox::_OnHorizontalScrollPositionChanged, this));
	_HorizontalScrollBar->SetAnchorBottom(true);
	_HorizontalScrollBar->SetAnchorLeft(true);
	_HorizontalScrollBar->SetAnchorRight(true);
	_HorizontalScrollBar->SetAnchorTop(false);
	_HorizontalScrollBar->SetMinimumPosition(0.0f);
	_HorizontalScrollBar->SetMaximumPosition(std::max(0.0f, _Content->GetWidth() - _View->GetWidth()));
	_HorizontalScrollBar->SetCurrentPosition(0.0f);
	_HorizontalScrollBar->SetStepSize(_HorizontalScrollBar->GetMaximumPosition() / 10.0f);
	_VerticalScrollBar = new UI::ScrollBar{this, UI::ScrollBar::Alignment::VERTICAL};
	_HorizontalScrollBar->SetName("Vertical_scroll_bar");
	_VerticalScrollBar->SetLeft(constant(GetWidth() - 20.0f));
	_VerticalScrollBar->SetTop(0.0f);
	_VerticalScrollBar->SetWidth(20.0f);
	_VerticalScrollBar->SetHeight(GetHeight() - 20.0f);
	_VerticalScrollBar->ConnectScrollPositionChangedCallback(std::bind(&UI::ScrollBox::_OnVerticalScrollPositionChanged, this));
	_VerticalScrollBar->SetAnchorBottom(true);
	_VerticalScrollBar->SetAnchorLeft(false);
	_VerticalScrollBar->SetAnchorRight(true);
	_VerticalScrollBar->SetAnchorTop(true);
	_VerticalScrollBar->SetMinimumPosition(0.0f);
	_VerticalScrollBar->SetMaximumPosition(std::max(0.0f, _Content->GetHeight() - _View->GetHeight()));
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
		_HorizontalScrollBar->SetMaximumPosition(std::max(0.0f, _Content->GetWidth() - _View->GetWidth()));
		_HorizontalScrollBar->SetStepSize(_HorizontalScrollBar->GetMaximumPosition() / 10.0f);
		_VerticalScrollBar->SetMaximumPosition(std::max(0.0f, _Content->GetHeight() - _View->GetHeight()));
		_VerticalScrollBar->SetStepSize(_VerticalScrollBar->GetMaximumPosition() / 10.0f);
	}
}

void UI::ScrollBox::_OnHorizontalScrollPositionChanged(void)
{
	if(_Content->GetWidth() > _View->GetWidth())
	{
		_Content->SetLeft(constant(-_HorizontalScrollBar->GetCurrentPosition()));
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
	if(_Content->GetHeight() > _View->GetHeight())
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
			_View->SetHeight(GetHeight() - 20.0f);
			_VerticalScrollBar->SetHeight(GetHeight() - 20.0f);
		}
		else
		{
			_View->SetHeight(GetHeight());
			_VerticalScrollBar->SetHeight(GetHeight());
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
			_View->SetWidth(GetWidth() - 20.0f);
			_HorizontalScrollBar->SetWidth(GetWidth() - 20.0f);
		}
		else
		{
			_View->SetWidth(GetWidth());
			_HorizontalScrollBar->SetWidth(GetWidth());
		}
		_VerticalScrollBar->SetVisible(Visible);
	}
}
