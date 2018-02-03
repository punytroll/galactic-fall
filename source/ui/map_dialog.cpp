/**
 * galactic-fall
 * Copyright (C) 2006-2018  Hagen Möbius
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

#include "../character.h"
#include "../globals.h"
#include "../graphics/color_rgbo.h"
#include "../object_aspect_name.h"
#include "../system.h"
#include "key_event.h"
#include "map_dialog.h"
#include "star_map_display.h"
#include "text_button.h"

using namespace Expressions::Operators;

UI::MapDialog::MapDialog(UI::Widget * SupWidget, Character * Character) :
	UI::Dialog(SupWidget),
	_OKButton(nullptr),
	_StarMapDisplay(nullptr)
{
	SetTitle("Map");
	ConnectKeyCallback(std::bind(&UI::MapDialog::_OnKey, this, std::placeholders::_1));
	
	// create components
	auto CancelButton{new UI::TextButton(this, "Cancel")};
	
	_OKButton = new UI::TextButton{this, "OK"};
	_StarMapDisplay = new UI::StarMapDisplay{this, Character};
	// setup components
	_OKButton->SetLeft(width(this) - 10.0_c - width(_OKButton));
	_OKButton->SetTop(height(this) - 10.0_c - height(_OKButton));
	_OKButton->SetWidth(100.0_c);
	_OKButton->SetHeight(20.0_c);
	_OKButton->ConnectClickedCallback(std::bind(&UI::MapDialog::_Close, this, UI::Dialog::ClosingReason::OK_BUTTON));
	_OKButton->ConnectUpdatingCallback(std::bind(&UI::MapDialog::_OnOKButtonUpdating, this, std::placeholders::_1, std::placeholders::_2));
	CancelButton->SetLeft(left(_OKButton) - 10.0_c - width(CancelButton));
	CancelButton->SetTop(height(this) - 10.0_c - height(CancelButton));
	CancelButton->SetWidth(100.0_c);
	CancelButton->SetHeight(20.0_c);
	CancelButton->ConnectClickedCallback(std::bind(&UI::MapDialog::_Close, this, UI::Dialog::ClosingReason::CANCEL_BUTTON));
	_StarMapDisplay->SetLeft(10.0_c);
	_StarMapDisplay->SetTop(40.0_c);
	_StarMapDisplay->SetWidth(width(this) - 2.0_c * 10.0_c);
	_StarMapDisplay->SetHeight(top(_OKButton) - 10.0_c - top(_StarMapDisplay));
	_StarMapDisplay->SetBackgroundColor(Graphics::ColorRGBO(0.0f, 0.0f, 0.0f, 1.0f));
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
			if(_OKButton->GetEnabled() == true)
			{
				_Close(UI::Dialog::ClosingReason::RETURN_KEY);
			}
		}
	}
}

void UI::MapDialog::_OnOKButtonUpdating(float RealTimeSeconds, float GameTimeSeconds)
{
	assert(_OKButton != nullptr);
	assert(_StarMapDisplay != nullptr);
	_OKButton->SetEnabled((_StarMapDisplay->GetSelectedSystem() == nullptr) || ((_StarMapDisplay->GetCharacter() != nullptr) && (_StarMapDisplay->GetCharacter()->GetSystem() != nullptr) && (_StarMapDisplay->GetSelectedSystem()->IsLinkedToSystem(_StarMapDisplay->GetCharacter()->GetSystem()) == true)));
}
