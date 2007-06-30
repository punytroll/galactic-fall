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
#include "mouse_button_listener.h"
#include "window.h"

class Button;
class Character;
class Label;
class System;

class MapDialog : public ClickedListener, public KeyListener, virtual public MouseButtonListener, public WWindow
{
public:
	MapDialog(Widget * SupWidget, const Reference< System > & System, const Reference< Character > & Character);
	virtual void Draw(void) const;
	Reference< System > & GetSelectedSystem(void);
protected:
	virtual bool OnClicked(Widget * EventSource);
	virtual bool OnKey(Widget * EventSource, int Key, int State);
	virtual bool OnMouseButton(Widget * EventSource, int Button, int State, float X, float Y);
private:
	Reference< System > m_System;
	Reference< System > m_SelectedSystem;
	Reference< Character > m_Character;
	Button * m_OKButton;
	Label * m_OKButtonLabel;
	float m_Scale;
};

inline Reference< System > & MapDialog::GetSelectedSystem(void)
{
	return m_SelectedSystem;
}

#endif
