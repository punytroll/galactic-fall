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

#include "label.h"
#include "text_button.h"

UI::TextButton::TextButton(UI::Widget * SupWidget, const std::string & Text) :
	UI::Button(SupWidget),
	_Label(nullptr)
{
	SetSize(Vector2f(100.0f, 20.0f));
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
	_Label->SetText(Text);
}

void UI::TextButton::SetText(const std::string & Text)
{
	assert(_Label != nullptr);
	_Label->SetText(Text);
}
