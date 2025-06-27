/**
 * galactic-fall
 * Copyright (C) 2006-2019  Hagen Möbius
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

#include <map>

#include <expressions/operators.h>

#include <graphics/color_rgbo.h>

#include "../character.h"
#include "../globals.h"
#include "../object_aspect_name.h"
#include "../system.h"
#include "key_event.h"
#include "map_dialog.h"
#include "star_map_display.h"
#include "text_button.h"

using namespace Expressions::Operators;

UI::MapDialog::MapDialog(Character * Character) :
	_StarMapDisplay(nullptr)
{
	SetTitle("Map");
	ConnectKeyCallback(std::bind(&UI::MapDialog::_OnKey, this, std::placeholders::_1));
	
	// create components
	auto CancelButton{new UI::TextButton()};
	auto OKButton{new UI::TextButton{}};
	
	_StarMapDisplay = new UI::StarMapDisplay{Character};
	// initialize components
	CancelButton->SetLeft(left(OKButton) - 10.0_c - width(CancelButton));
	CancelButton->SetTop(height(this) - 10.0_c - height(CancelButton));
	CancelButton->SetWidth(100.0_c);
	CancelButton->SetHeight(20.0_c);
	CancelButton->SetText("Cancel");
	CancelButton->ConnectClickedCallback(std::bind(&UI::MapDialog::_Close, this, UI::Dialog::ClosingReason::CANCEL_BUTTON));
	OKButton->SetName("ok_button");
	OKButton->SetLeft(width(this) - 10.0_c - width(OKButton));
	OKButton->SetTop(height(this) - 10.0_c - height(OKButton));
	OKButton->SetWidth(100.0_c);
	OKButton->SetHeight(20.0_c);
	OKButton->SetText("OK");
	OKButton->ConnectClickedCallback(std::bind(&UI::MapDialog::_Close, this, UI::Dialog::ClosingReason::OK_BUTTON));
	OKButton->ConnectUpdatingCallback(std::bind(&UI::MapDialog::_OnOKButtonUpdating, this, OKButton, std::placeholders::_1, std::placeholders::_2));
	_StarMapDisplay->SetLeft(10.0_c);
	_StarMapDisplay->SetTop(40.0_c);
	_StarMapDisplay->SetWidth(width(this) - 2.0_c * 10.0_c);
	_StarMapDisplay->SetHeight(top(OKButton) - 10.0_c - top(_StarMapDisplay));
	_StarMapDisplay->SetBackgroundColor(Graphics::ColorRGBO(0.0f, 0.0f, 0.0f, 1.0f));
	// add components
	AddSubWidget(CancelButton);
	AddSubWidget(OKButton);
	AddSubWidget(_StarMapDisplay);
}

System * UI::MapDialog::GetSelectedSystem(void)
{
	assert(_StarMapDisplay != nullptr);
	
	return _StarMapDisplay->GetSelectedSystem();
}

void UI::MapDialog::SetSelectedSystem(System * SelectedSystem)
{
	assert(_StarMapDisplay != nullptr);
	_StarMapDisplay->SetSelectedSystem(SelectedSystem);
}

void UI::MapDialog::_OnKey(UI::KeyEvent & KeyEvent)
{
	if(KeyEvent.IsDown() == true)
	{
		if((KeyEvent.GetKeyCode() == 9 /* ESCAPE */) || (KeyEvent.GetKeyCode() == 58 /* M */))
		{
			_Close(UI::Dialog::ClosingReason::ESCAPE_KEY);
		}
		else if(KeyEvent.GetKeyCode() == 36 /* RETURN */)
		{
			auto OKButton{GetSubWidget("ok_button")};
			
			assert(OKButton != nullptr);
			if(OKButton->GetEnabled() == true)
			{
				_Close(UI::Dialog::ClosingReason::RETURN_KEY);
			}
		}
	}
}

void UI::MapDialog::_OnOKButtonUpdating(UI::Button * OKButton, [[maybe_unused]] float RealTimeSeconds, [[maybe_unused]] float GameTimeSeconds)
{
	assert(OKButton != nullptr);
	assert(_StarMapDisplay != nullptr);
	OKButton->SetEnabled((_StarMapDisplay->GetSelectedSystem() == nullptr) || ((_StarMapDisplay->GetCharacter() != nullptr) && (_StarMapDisplay->GetCharacter()->GetSystem() != nullptr) && (_StarMapDisplay->GetSelectedSystem()->IsLinkedToSystem(_StarMapDisplay->GetCharacter()->GetSystem()) == true)));
}
