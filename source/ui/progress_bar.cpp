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

#include "../graphics/color_rgbo.h"
#include "border.h"
#include "event.h"
#include "label.h"
#include "progress_bar.h"

UI::ProgressBar::ProgressBar(UI::Widget * SupWidget) :
	UI::Widget(SupWidget),
	_Fill(nullptr),
	_FillLevel(1.0f),
	_Label(nullptr)
{
	ConnectSizeChangedCallback(std::bind(&UI::ProgressBar::_OnSizeChanged, this, std::placeholders::_1));
	_Fill = new UI::Widget{this};
	_Fill->SetLeft(0.0f);
	_Fill->SetTop(0.0f);
	_Fill->SetSize(GetSize());
	_Fill->SetAnchorBottom(true);
	_Fill->SetAnchorLeft(true);
	_Fill->SetAnchorTop(true);
	_Label = new UI::Label{this};
	_Label->SetLeft(0.0f);
	_Label->SetTop(0.0f);
	_Label->SetSize(GetSize());
	_Label->SetAnchorBottom(true);
	_Label->SetAnchorLeft(true);
	_Label->SetAnchorRight(true);
	_Label->SetAnchorTop(true);
	_Label->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Center);
	_Label->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	
	auto Border{new UI::Border{this}};
	
	Border->SetLeft(0.0f);
	Border->SetTop(0.0f);
	Border->SetSize(GetSize());
	Border->SetLineWidth(1.0f);
	Border->SetColor(Graphics::ColorRGBO(1.0f, 1.0f, 1.0f, 1.0f));
	Border->SetAnchorBottom(true);
	Border->SetAnchorLeft(true);
	Border->SetAnchorRight(true);
	Border->SetAnchorTop(true);
}

void UI::ProgressBar::_OnSizeChanged(UI::Event & SizeChangedEvent)
{
	if(SizeChangedEvent.GetPhase() == UI::Event::Phase::Target)
	{
		_ResizeFill();
	}
}

void UI::ProgressBar::_ResizeFill(void)
{
	_Fill->SetSize(Vector2f(_FillLevel * GetWidth(), _Fill->GetHeight()));
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
