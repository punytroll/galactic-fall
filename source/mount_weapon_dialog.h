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

#ifndef MOUNT_WEAPON_DIALOG_H
#define MOUNT_WEAPON_DIALOG_H

#include "clicked_listener.h"
#include "dimension_listener.h"
#include "key_listener.h"
#include "mouse_button_listener.h"
#include "window.h"

class Button;
class ScrollBox;
class Ship;
class SlotListItem;
class WeaponListItem;

class MountWeaponDialog : public ClickedListener, public DimensionListener, public KeyListener, virtual public MouseButtonListener, public WWindow
{
public:
	MountWeaponDialog(Widget * SupWidget, Ship * Ship);
protected:
	virtual bool OnClicked(Widget * EventSource);
	virtual bool OnKey(Widget * EventSource, const KeyEventInformation & KeyEventInformation);
	virtual bool OnMouseButton(Widget * EventSource, int Button, int State, float X, float Y);
	virtual void OnSizeChanged(Widget * EventSource);
private:
	void RebuildWeaponList(void);
	void UpdateButtons(void);
	Ship * m_Ship;
	Button * m_OKButton;
	Button * m_MountButton;
	Button * m_UnmountButton;
	ScrollBox * m_SlotScrollBox;
	ScrollBox * m_WeaponScrollBox;
	SlotListItem * m_SelectedSlotListItem;
	WeaponListItem * m_SelectedWeaponListItem;
	Widget * m_LeftPane;
	Widget * m_CenterPane;
	Widget * m_RightPane;
};

#endif
