/**
 * galactic-fall
 * Copyright (C) 2014-2018  Hagen Möbius
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

#include <cassert>

#include <expressions/operators.h>

#include "label.h"
#include "text_button.h"

using namespace Expressions::Operators;

UI::TextButton::TextButton(void) :
	_Label(nullptr)
{
	// create components
	_Label = new UI::Label{};
	// initialize components
	_Label->SetLeft(0.0_c);
	_Label->SetTop(0.0_c);
	_Label->SetWidth(width(this));
	_Label->SetHeight(height(this));
	_Label->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Center);
	_Label->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	// add components
	AddSubWidget(_Label);
}

void UI::TextButton::SetText(const std::string & Text)
{
	assert(_Label != nullptr);
	_Label->SetText(Text);
}
