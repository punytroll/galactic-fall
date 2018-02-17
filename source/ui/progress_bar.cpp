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

#include <assert.h>

#include <expressions/operators.h>

#include "../graphics/color_rgbo.h"
#include "border.h"
#include "event.h"
#include "label.h"
#include "progress_bar.h"

using namespace Expressions::Operators;

UI::ProgressBar::ProgressBar(void) :
	_Fill(nullptr),
	_FillLevel(1.0_c),
	_Label(nullptr)
{
	// create components
	_Fill = new UI::Widget{};
	_Label = new UI::Label{};
	
	auto Border{new UI::Border{}};
	
	// initialize components
	_Fill->SetName("fill");
	_Fill->SetLeft(0.0_c);
	_Fill->SetTop(0.0_c);
	_Fill->SetWidth(width(this) * _FillLevel);
	_Fill->SetHeight(height(this));
	_Label->SetName("label");
	_Label->SetLeft(0.0_c);
	_Label->SetTop(0.0_c);
	_Label->SetWidth(width(this));
	_Label->SetHeight(height(this));
	_Label->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Center);
	_Label->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	Border->SetName("border");
	Border->SetLeft(0.0_c);
	Border->SetTop(0.0_c);
	Border->SetWidth(width(this));
	Border->SetHeight(height(this));
	Border->SetLineWidth(1.0f);
	Border->SetColor(Graphics::ColorRGBO(1.0f, 1.0f, 1.0f, 1.0f));
	// add components
	AddSubWidget(_Fill);
	AddSubWidget(_Label);
	AddSubWidget(Border);
}

void UI::ProgressBar::SetColor(const Graphics::ColorRGBO & Color)
{
	_Fill->SetBackgroundColor(Color);
}

void UI::ProgressBar::SetFillLevel(float FillLevel)
{
	_FillLevel = constant(FillLevel);
}

void UI::ProgressBar::SetText(const std::string & Text)
{
	assert(_Label != nullptr);
	_Label->SetText(Text);
}
