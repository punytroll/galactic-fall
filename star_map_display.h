/**
 * galactic-fall
 * Copyright (C) 2007  Hagen Möbius
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

#ifndef STAR_MAP_DISPLAY_H
#define STAR_MAP_DISPLAY_H

#include "mouse_button_listener.h"
#include "widget.h"

class Character;
class System;

class StarMapDisplay : virtual public MouseButtonListener, public Widget
{
public:
	StarMapDisplay(Widget * SupWidget, System * System, Character * Character);
	virtual void Draw(void) const;
	System * GetSelectedSystem(void);
protected:
	virtual bool OnMouseButton(Widget * EventSource, int Button, int State, float X, float Y);
private:
	System * m_System;
	System * m_SelectedSystem;
	Character * m_Character;
	float m_Scale;
};

inline System * StarMapDisplay::GetSelectedSystem(void)
{
	return m_SelectedSystem;
}

#endif

