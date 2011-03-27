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

#ifndef MAP_DIALOG_H
#define MAP_DIALOG_H

#include "ui/window.h"

class Button;
class Character;
class StarMapDisplay;
class System;

class MapDialog : public UI::Window
{
public:
	MapDialog(Widget * SupWidget, System * System, Character * Character);
	StarMapDisplay * GetStarMapDisplay(void);
protected:
	// callbacks
	bool OnKey(const KeyEventInformation & KeyEventInformation);
	void OnOKClicked(void);
private:
	Button * m_OKButton;
	StarMapDisplay * m_StarMapDisplay;
};

inline StarMapDisplay * MapDialog::GetStarMapDisplay(void)
{
	return m_StarMapDisplay;
}

#endif
