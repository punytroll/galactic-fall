/**
 * galactic-fall
 * Copyright (C) 2014  Hagen Möbius
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

#include <assert.h>

#include <expressions/operators.h>

#include "../graphics/color_rgbo.h"
#include "border.h"
#include "event.h"
#include "label.h"
#include "progress_bar.h"

using namespace Expressions::Operators;

UI::ProgressBar::ProgressBar(UI::Widget * SupWidget) :
	UI::Widget(SupWidget),
	_Fill(nullptr),
	_FillLevel(1.0f),
	_Label(nullptr)
{
	ConnectWidthChangedCallback(std::bind(&UI::ProgressBar::_OnWidthChanged, this, std::placeholders::_1));
	_Fill = new UI::Widget{this};
	_Fill->SetLeft(0.0_c);
	_Fill->SetTop(0.0_c);
	_Fill->SetWidth(constant(GetWidth()));
	_Fill->SetHeight(constant(GetHeight()));
	_Fill->SetAnchorBottom(true);
	_Fill->SetAnchorLeft(true);
	_Fill->SetAnchorTop(true);
	_Label = new UI::Label{this};
	_Label->SetLeft(0.0_c);
	_Label->SetTop(0.0_c);
	_Label->SetWidth(constant(GetWidth()));
	_Label->SetHeight(constant(GetHeight()));
	_Label->SetAnchorBottom(true);
	_Label->SetAnchorLeft(true);
	_Label->SetAnchorRight(true);
	_Label->SetAnchorTop(true);
	_Label->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Center);
	_Label->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	
	auto Border{new UI::Border{this}};
	
	Border->SetLeft(0.0_c);
	Border->SetTop(0.0_c);
	Border->SetWidth(constant(GetWidth()));
	Border->SetHeight(constant(GetHeight()));
	Border->SetLineWidth(1.0f);
	Border->SetColor(Graphics::ColorRGBO(1.0f, 1.0f, 1.0f, 1.0f));
	Border->SetAnchorBottom(true);
	Border->SetAnchorLeft(true);
	Border->SetAnchorRight(true);
	Border->SetAnchorTop(true);
}

void UI::ProgressBar::_OnWidthChanged(UI::Event & WidthChangedEvent)
{
	if(WidthChangedEvent.GetPhase() == UI::Event::Phase::Target)
	{
		_ResizeFill();
	}
}

void UI::ProgressBar::_ResizeFill(void)
{
	_Fill->SetWidth(constant(_FillLevel * GetWidth()));
}

void UI::ProgressBar::SetColor(const Graphics::ColorRGBO & Color)
{
	_Fill->SetBackgroundColor(Color);
}

void UI::ProgressBar::SetFillLevel(float FillLevel)
{
	_FillLevel = FillLevel;
	_ResizeFill();
}

void UI::ProgressBar::SetText(const std::string & Text)
{
	assert(_Label != nullptr);
	_Label->SetText(Text);
}
