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

#ifndef OUTFIT_SHIP_DIALOG_H
#define OUTFIT_SHIP_DIALOG_H

#include "ui/window.h"

class Ship;
class SlotListItem;
class AccessoryListItem;

namespace UI
{
	class Button;
	class ScrollBox;
}

class OutfitShipDialog : public UI::Window
{
public:
	OutfitShipDialog(Widget * SupWidget, Ship * Ship);
private:
	// callbacks
	bool OnAccessoryListItemMouseButton(AccessoryListItem * AccessoryListItem, int Button, int State, float X, float Y);
	bool OnKey(const KeyEventInformation & KeyEventInformation);
	void OnMountClicked(void);
	void OnOKClicked(void);
	void OnSizeChanged(void);
	bool OnSlotListItemMouseButton(SlotListItem * SlotListItem, int Button, int State, float X, float Y);
	void OnUnmountClicked(void);
	// helper function and actions
	void RebuildAccessoryList(void);
	void UpdateButtons(void);
	// member variables
	Ship * m_Ship;
	UI::Button * m_OKButton;
	UI::Button * m_MountButton;
	UI::Button * m_UnmountButton;
	UI::ScrollBox * m_SlotScrollBox;
	UI::ScrollBox * m_AccessoryScrollBox;
	SlotListItem * m_SelectedSlotListItem;
	AccessoryListItem * m_SelectedAccessoryListItem;
	Widget * m_LeftPane;
	Widget * m_CenterPane;
	Widget * m_RightPane;
};

#endif
