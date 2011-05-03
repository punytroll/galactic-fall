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

#ifndef UI_OUTFIT_SHIP_DIALOG_H
#define UI_OUTFIT_SHIP_DIALOG_H

#include "window.h"

class Ship;

namespace UI
{
	class AccessoryListItem;
	class Button;
	class ScrollBox;
	class SlotListItem;
	
	class OutfitShipDialog : public UI::Window
	{
	public:
		OutfitShipDialog(UI::Widget * SupWidget, Ship * Ship);
	private:
		// callbacks
		bool OnAccessoryListItemMouseButton(UI::AccessoryListItem * AccessoryListItem, int Button, int State, float X, float Y);
		bool OnKey(const KeyEventInformation & KeyEventInformation);
		void OnMountClicked(void);
		void OnOKClicked(void);
		void OnSizeChanged(void);
		bool OnSlotListItemMouseButton(UI::SlotListItem * SlotListItem, int Button, int State, float X, float Y);
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
		UI::SlotListItem * m_SelectedSlotListItem;
		UI::AccessoryListItem * m_SelectedAccessoryListItem;
		UI::Widget * m_LeftPane;
		UI::Widget * m_CenterPane;
		UI::Widget * m_RightPane;
	};
}

#endif
