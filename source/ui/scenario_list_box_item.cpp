/**
 * galactic-fall
 * Copyright (C) 2016  Hagen Möbius
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

#include "../scenario.h"
#include "label.h"
#include "scenario_list_box_item.h"

using namespace Expressions::Operators;

UI::ScenarioListBoxItem::ScenarioListBoxItem(Scenario * Scenario) :
	_Scenario(Scenario)
{
	auto CaptionLabel{new UI::Label{this, Scenario->GetName()}};
	
	CaptionLabel->SetLeft(5.0_c);
	CaptionLabel->SetTop(0.0_c);
	CaptionLabel->SetWidth(constant(GetWidth() - 10.0f));
	CaptionLabel->SetHeight(constant(GetHeight()));
	CaptionLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	CaptionLabel->SetAnchorBottom(true);
	CaptionLabel->SetAnchorLeft(true);
	CaptionLabel->SetAnchorRight(true);
	CaptionLabel->SetAnchorTop(true);
	SetHeight(20.0_c);
}
