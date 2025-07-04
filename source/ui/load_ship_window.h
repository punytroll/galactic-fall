/**
 * Copyright (C) 2014-2018  Hagen Möbius
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

#ifndef LOAD_SHIP_WINDOW_H
#define LOAD_SHIP_WINDOW_H

#include <events/connection.h>

#include "window.h"

class Hangar;
class Ship;

namespace UI
{
	class Button;
	class ListBox;
	
	class LoadShipWindow : public UI::Window
	{
	public:
		// constructor and destructor
		LoadShipWindow(Hangar * Hangar, Ship * Ship);
	private:
		// event handler
		void _OnDestroying(UI::Event & DestroyingEvent);
		void _OnHangarContentAdded(Object * Content);
		void _OnHangarContentRemoved(Object * Content);
		void _OnHangarDestroying(void);
		void _OnKey(UI::KeyEvent & KeyEvent);
		void _OnMoveToHangarButtonClicked(void);
		void _OnMoveToHangarButtonUpdating(UI::Button * MoveToHangarButton, float RealTimeSeconds, float GameTimeSeconds);
		void _OnMoveToShipButtonClicked(void);
		void _OnMoveToShipButtonUpdating(UI::Button * MoveToShipButton, float RealTimeSeconds, float GameTimeSeconds);
		void _OnOKButtonClicked(void);
		void _OnShipContentAdded(Object * Content);
		void _OnShipContentRemoved(Object * Content);
		void _OnShipDestroying(void);
		void _OnShipFreeSpaceLabelUpdating(UI::Label * ShipFreeSpaceLabel, float RealTimeSeconds, float GameTimeSeconds);
		// member variables
		Hangar * _Hangar;
		Connection _HangarContentAddedCallbackConnection;
		Connection _HangarContentRemovedCallbackConnection;
		Connection _HangarDestroyingCallbackConnection;
		UI::ListBox * _HangarListBox;
		Ship * _Ship;
		Connection _ShipContentAddedCallbackConnection;
		Connection _ShipContentRemovedCallbackConnection;
		Connection _ShipDestroyingCallbackConnection;
		UI::ListBox * _ShipListBox;
	};
}

#endif
