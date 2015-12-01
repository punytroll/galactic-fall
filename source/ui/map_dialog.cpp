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

#include <GL/gl.h>

#include "../globals.h"
#include "../graphics/color_rgbo.h"
#include "../object_aspect_name.h"
#include "../system.h"
#include "key_event.h"
#include "map_dialog.h"
#include "star_map_display.h"
#include "text_button.h"

UI::MapDialog::MapDialog(UI::Widget * SupWidget, Character * Character) :
	UI::Window(SupWidget, "Map")
{
	SetPosition(Vector2f(70.0f, 200.0f));
	SetSize(Vector2f(500.0f, 530.0f));
	ConnectKeyCallback(std::bind(&UI::MapDialog::_OnKey, this, std::placeholders::_1));
	
	auto OKButton(new UI::TextButton(this, "OK"));
	
	OKButton->SetPosition(Vector2f(390.0f, 500.0f));
	OKButton->SetSize(Vector2f(100.0f, 20.0f));
	OKButton->SetAnchorBottom(true);
	OKButton->SetAnchorLeft(false);
	OKButton->SetAnchorRight(true);
	OKButton->SetAnchorTop(false);
	OKButton->ConnectClickedCallback(std::bind(&UI::MapDialog::Destroy, this));
	_StarMapDisplay = new UI::StarMapDisplay(this, Character);
	_StarMapDisplay->SetPosition(Vector2f(10.0f, 40.0f));
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
	if(((KeyEvent.GetKeyCode() == 9 /* ESCAPE */) || (KeyEvent.GetKeyCode() == 36 /* RETURN */) || (KeyEvent.GetKeyCode() == 58 /* M */)) && (KeyEvent.IsDown() == true))
	{
		Destroy();
	}
}
