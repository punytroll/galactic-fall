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

#include "clicked_listener.h"
#include "key_listener.h"
#include "window.h"

class Button;
class Character;
class Label;
class StarMapDisplay;
class System;

class MapDialog : public ClickedListener, public KeyListener, public WWindow
{
public:
	MapDialog(Widget * SupWidget, System * System, Character * Character);
	StarMapDisplay * GetStarMapDisplay(void);
protected:
	virtual bool OnClicked(Widget * EventSource);
	virtual bool OnKey(Widget * EventSource, int Key, int State);
private:
	Button * m_OKButton;
	Label * m_OKButtonLabel;
	StarMapDisplay * m_StarMapDisplay;
};

inline StarMapDisplay * MapDialog::GetStarMapDisplay(void)
{
	return m_StarMapDisplay;
}

#endif
