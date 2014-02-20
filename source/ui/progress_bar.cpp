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

#include "../callbacks/callbacks.h"
#include "../color.h"
#include "border.h"
#include "label.h"
#include "progress_bar.h"

UI::ProgressBar::ProgressBar(UI::Widget * SupWidget) :
	UI::Widget(SupWidget),
	_Fill(nullptr),
	_FillLevel(1.0f),
	_Label(nullptr)
{
	SetSize(Vector2f(100.0f, 20.0f));
	ConnectSizeChangedCallback(Callback(this, &UI::ProgressBar::_OnSizeChanged));
	_Fill = new UI::Widget(this);
	_Fill->SetPosition(Vector2f(0.0f, 0.0f));
	_Fill->SetSize(GetSize());
	_Label = new UI::Label(this);
	_Label->SetPosition(Vector2f(0.0f, 0.0f));
	_Label->SetSize(GetSize());
	_Label->SetAnchorBottom(true);
	_Label->SetAnchorLeft(true);
	_Label->SetAnchorRight(true);
	_Label->SetAnchorTop(true);
	_Label->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	_Label->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	
	UI::Border * Border(new UI::Border(this));
	
	Border->SetPosition(Vector2f(0.0f, 0.0f));
	Border->SetSize(GetSize());
	Border->SetWidth(1.0f);
	Border->SetColor(Color(1.0f, 1.0f, 1.0f, 1.0f));
	Border->SetAnchorBottom(true);
	Border->SetAnchorLeft(true);
	Border->SetAnchorRight(true);
	Border->SetAnchorTop(true);
}

void UI::ProgressBar::_OnSizeChanged(void)
{
	_ResizeFill();
}

void UI::ProgressBar::_ResizeFill(void)
{
	_Fill->SetSize(Vector2f(_FillLevel * GetSize()[0], _Fill->GetSize()[1]));
}

void UI::ProgressBar::SetColor(const Color & Color)
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
