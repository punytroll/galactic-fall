/**
 * galactic-fall
 * Copyright (C) 2006  Hagen Möbius
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

#include "../character.h"
#include "../globals.h"
#include "../graphics/color_rgbo.h"
#include "../object_aspect_name.h"
#include "../system.h"
#include "key_event.h"
#include "map_dialog.h"
#include "star_map_display.h"
#include "text_button.h"

UI::MapDialog::MapDialog(UI::Widget * SupWidget, Character * Character) :
	UI::Dialog(SupWidget),
	_OKButton(nullptr),
	_StarMapDisplay(nullptr)
{
	SetTitle("Map");
	SetLeft(70.0f);
	SetTop(200.0f);
	SetSize(Vector2f(500.0f, 530.0f));
	ConnectKeyCallback(std::bind(&UI::MapDialog::_OnKey, this, std::placeholders::_1));
	_OKButton = new UI::TextButton{this, "OK"};
	_OKButton->SetSize(Vector2f(100.0f, 20.0f));
	_OKButton->SetLeft(GetSize()[0] - 10.0f - _OKButton->GetSize()[0]);
	_OKButton->SetTop(500.0f);
	_OKButton->SetAnchorBottom(true);
	_OKButton->SetAnchorLeft(false);
	_OKButton->SetAnchorRight(true);
	_OKButton->SetAnchorTop(false);
	_OKButton->ConnectClickedCallback(std::bind(&UI::MapDialog::_Close, this, UI::Dialog::ClosingReason::OK_BUTTON));
	_OKButton->ConnectUpdatingCallback(std::bind(&UI::MapDialog::_OnOKButtonUpdating, this, std::placeholders::_1, std::placeholders::_2));
	
	auto CancelButton{new UI::TextButton(this, "Cancel")};
	
	CancelButton->SetSize(Vector2f(100.0f, 20.0f));
	CancelButton->SetLeft(_OKButton->GetLeft() - 10.0f - CancelButton->GetSize()[0]);
	CancelButton->SetTop(500.0f);
	CancelButton->SetAnchorBottom(true);
	CancelButton->SetAnchorLeft(false);
	CancelButton->SetAnchorRight(true);
	CancelButton->SetAnchorTop(false);
	CancelButton->ConnectClickedCallback(std::bind(&UI::MapDialog::_Close, this, UI::Dialog::ClosingReason::CANCEL_BUTTON));
	_StarMapDisplay = new UI::StarMapDisplay{this, Character};
	_StarMapDisplay->SetLeft(10.0f);
	_StarMapDisplay->SetTop(40.0f);
	_StarMapDisplay->SetSize(Vector2f(480.0f, 450.0f));
	_StarMapDisplay->SetAnchorBottom(true);
	_StarMapDisplay->SetAnchorLeft(true);
	_StarMapDisplay->SetAnchorRight(true);
	_StarMapDisplay->SetAnchorTop(true);
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
