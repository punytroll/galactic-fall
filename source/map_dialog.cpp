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

#include "button.h"
#include "globals.h"
#include "key_event_information.h"
#include "label.h"
#include "map_dialog.h"
#include "object_aspect_name.h"
#include "star_map_display.h"
#include "system.h"

MapDialog::MapDialog(Widget * SupWidget, System * System, Character * Character) :
	WWindow(SupWidget, "Map: " + System->GetAspectName()->GetName())
{
	SetPosition(Vector2f(70.0f, 400.0f));
	SetSize(Vector2f(500.0f, 530.0f));
	AddKeyListener(this);
	AddMouseButtonListener(this);
	m_OKButton = new Button(this);
	m_OKButton->SetPosition(Vector2f(390.0f, 500.0f));
	m_OKButton->SetSize(Vector2f(100.0f, 20.0f));
	m_OKButton->AddClickedListener(this);
	m_OKButtonLabel = new Label(m_OKButton, "OK");
	m_OKButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	m_OKButtonLabel->SetSize(m_OKButton->GetSize());
	m_OKButtonLabel->SetHorizontalAlignment(Label::ALIGN_HORIZONTAL_CENTER);
	m_OKButtonLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	m_StarMapDisplay = new StarMapDisplay(this, System, Character);
	m_StarMapDisplay->SetPosition(Vector2f(10.0f, 40.0f));
	m_StarMapDisplay->SetSize(Vector2f(480.0f, 450.0f));
	m_StarMapDisplay->SetBackgroundColor(Color(0.15f, 0.15f, 0.15f, 1.0f));
}

bool MapDialog::OnClicked(Widget * EventSource)
{
	if(EventSource == m_OKButton)
	{
		Destroy();
		
		return true;
	}
	
	return false;
}

bool MapDialog::OnKey(Widget * EventSource, const KeyEventInformation & KeyEventInformation)
{
	if(((KeyEventInformation.GetKeyCode() == 9 /* ESCAPE */) || (KeyEventInformation.GetKeyCode() == 36 /* RETURN */) || (KeyEventInformation.GetKeyCode() == 58 /* M */)) && (KeyEventInformation.IsDown() == true))
	{
		Destroy();
	}
	// eat all other input
	return true;
}
