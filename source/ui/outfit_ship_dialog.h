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
		OutfitShipDialog(UI::Widget * SupWidget, Reference< Ship > Ship);
	private:
		// callbacks
		bool _OnAccessoryListItemMouseButton(UI::AccessoryListItem * AccessoryListItem, int Button, int State, float X, float Y);
		bool _OnKey(const KeyEventInformation & KeyEventInformation);
		void _OnMountButtonClicked(void);
		void _OnOKButtonClicked(void);
		void _OnSizeChanged(void);
		bool _OnSlotListItemMouseButton(UI::SlotListItem * SlotListItem, int Button, int State, float X, float Y);
		void _OnUnmountButtonClicked(void);
		// helper function and actions
		void _RebuildAccessoryList(void);
		void _UpdateButtons(void);
		// member variables
		UI::ScrollBox * _AccessoryScrollBox;
		UI::Widget * _CenterPane;
		UI::Widget * _LeftPane;
		UI::Button * _MountButton;
		UI::Button * _OKButton;
		UI::Widget * _RightPane;
		UI::AccessoryListItem * _SelectedAccessoryListItem;
		UI::SlotListItem * _SelectedSlotListItem;
		Reference< Ship > _Ship;
		UI::ScrollBox * _SlotScrollBox;
		UI::Button * _UnmountButton;
	};
}

#endif
