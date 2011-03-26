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

#ifndef PLANET_DIALOG_H
#define PLANET_DIALOG_H

#include "window.h"

class Button;
class Character;
class Planet;
class TradeCenterDialog;

class PlanetDialog : public WWindow
{
public:
	PlanetDialog(Widget * SupWidget, Planet * Planet, Character * Character);
private:
	// callbacks
	void OnDestroying(void);
	bool OnKey(const KeyEventInformation & KeyEventInformation);
	void OnRefuelClicked(void);
	void OnTakeOffClicked(void);
	void OnTradeCenterClicked(void);
	void OnTradeCenterDialogDestroying(void);
	// helper functions & actions
	void OpenTradeCenterDialog(void);
	// member variables
	Planet * m_Planet;
	Character * m_Character;
	Button * m_TakeOffButton;
	Button * m_TradeCenterButton;
	Button * m_RefuelButton;
	TradeCenterDialog * m_TradeCenterDialog;
};

#endif
