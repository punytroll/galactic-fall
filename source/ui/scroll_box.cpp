/**
 * galactic-fall
 * Copyright (C) 2007-2018  Hagen Möbius
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

UI::ScrollBox::ScrollBox(void)
{
	ConnectMouseButtonCallback(std::bind(&UI::ScrollBox::_OnMouseButton, this, std::placeholders::_1));
	SetBackgroundColor(Graphics::ColorRGBO(0.15f, 0.15f, 0.15f, 1.0f));
	// create components
	_HorizontalScrollBar = new UI::ScrollBar{this, UI::ScrollBar::Alignment::HORIZONTAL};
	_VerticalScrollBar = new UI::ScrollBar{this, UI::ScrollBar::Alignment::VERTICAL};
	_View = new UI::Widget{this};
	_Content = new UI::Widget{_View};
	// setup components
	_View->SetName("view");
	_View->SetLeft(0.0_c);
	_View->SetTop(0.0_c);
	_View->SetWidth(width(this) - width(_VerticalScrollBar));
	_View->SetHeight(height(this) - height(_HorizontalScrollBar));
	_View->ConnectHeightChangedCallback(std::bind(&UI::ScrollBox::_OnContentOrViewSizeChanged, this, std::placeholders::_1));
	_View->ConnectWidthChangedCallback(std::bind(&UI::ScrollBox::_OnContentOrViewSizeChanged, this, std::placeholders::_1));
	_Content->SetName("content");
	_Content->SetLeft(0.0_c);
	_Content->SetTop(0.0_c);
	_Content->ConnectHeightChangedCallback(std::bind(&UI::ScrollBox::_OnContentOrViewSizeChanged, this, std::placeholders::_1));
	_Content->ConnectWidthChangedCallback(std::bind(&UI::ScrollBox::_OnContentOrViewSizeChanged, this, std::placeholders::_1));
	_HorizontalScrollBar->SetName("horizontal_scroll_bar");
	_HorizontalScrollBar->SetLeft(0.0_c);
	_HorizontalScrollBar->SetTop(height(this) - height(_HorizontalScrollBar));
	_HorizontalScrollBar->SetWidth(width(this) - width(_VerticalScrollBar));
	_HorizontalScrollBar->SetHeight(20.0_c);
	_HorizontalScrollBar->ConnectScrollPositionChangedCallback(std::bind(&UI::ScrollBox::_OnHorizontalScrollPositionChanged, this));
	_HorizontalScrollBar->SetMinimumPosition(0.0f);
	_HorizontalScrollBar->SetMaximumPosition(std::max(0.0f, _Content->GetWidth() - _View->GetWidth()));
	_HorizontalScrollBar->SetCurrentPosition(0.0f);
	_HorizontalScrollBar->SetStepSize(_HorizontalScrollBar->GetMaximumPosition() / 10.0f);
	_HorizontalScrollBar->SetName("vertical_scroll_bar");
	_VerticalScrollBar->SetLeft(width(this) - width(_VerticalScrollBar));
	_VerticalScrollBar->SetTop(0.0_c);
	_VerticalScrollBar->SetWidth(20.0_c);
	_VerticalScrollBar->SetHeight(height(this) - height(_HorizontalScrollBar));
	_VerticalScrollBar->ConnectScrollPositionChangedCallback(std::bind(&UI::ScrollBox::_OnVerticalScrollPositionChanged, this));
	_VerticalScrollBar->SetMinimumPosition(0.0f);
	_VerticalScrollBar->SetMaximumPosition(std::max(0.0f, _Content->GetHeight() - _View->GetHeight()));
	_VerticalScrollBar->SetCurrentPosition(0.0f);
	_VerticalScrollBar->SetStepSize(_VerticalScrollBar->GetMaximumPosition() / 10.0f);
}

UI::ScrollBox::~ScrollBox(void)
{
}

void UI::ScrollBox::Clear(void)
{
	while(GetContent()->GetSubWidgets().empty() == false)
	{
		GetContent()->GetSubWidgets().front()->Destroy();
	}
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
		_Content->SetTop(constant(-_VerticalScrollBar->GetCurrentPosition()));
	}
}

void UI::ScrollBox::SetHorizontalScrollBarVisible(bool Visible)
{
	if(Visible != _HorizontalScrollBar->IsVisible())
	{
		if(Visible == true)
		{
			_View->SetHeight(height(this) - height(_HorizontalScrollBar));
			_VerticalScrollBar->SetHeight(height(this) - height(_HorizontalScrollBar));
		}
		else
		{
			_View->SetHeight(height(this));
			_VerticalScrollBar->SetHeight(height(this));
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
			_View->SetWidth(width(this) - width(_VerticalScrollBar));
			_HorizontalScrollBar->SetWidth(width(this) - width(_VerticalScrollBar));
		}
		else
		{
			_View->SetWidth(width(this));
			_HorizontalScrollBar->SetWidth(width(this));
		}
		_VerticalScrollBar->SetVisible(Visible);
	}
}
