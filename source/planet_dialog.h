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

#include "clicked_listener.h"
#include "key_listener.h"
#include "window.h"

class Button;
class Character;
class Label;
class Planet;
class TradeCenterDialog;

class PlanetDialog : public ClickedListener, public KeyListener, public WWindow
{
public:
	PlanetDialog(Widget * SupWidget, Planet * Planet, Character * Character);
protected:
	virtual bool OnClicked(Widget * EventSource);
	virtual void OnDestroy(Widget * EventSource);
	virtual bool OnKey(Widget * EventSource, int Key, int State);
private:
	Planet * m_Planet;
	Character * m_Character;
	Label * m_DescriptionLabel;
	Button * m_OKButton;
	Label * m_OKButtonLabel;
	Button * m_TradeCenterButton;
	Label * m_TradeCenterLabel;
	Button * m_RefuelButton;
	Label * m_RefuelButtonLabel;
	TradeCenterDialog * m_TradeCenterDialog;
};

#endif
