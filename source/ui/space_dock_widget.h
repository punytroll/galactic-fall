/**
 * galactic-fall
 * Copyright (C) 2014  Hagen Möbius
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

#ifndef UI_SPACE_DOCK_WIDGET_H
#define UI_SPACE_DOCK_WIDGET_H

#include "../referencing.h"
#include "widget.h"

class Character;
class Planet;

namespace UI
{
	class Button;
	
	class SpaceDockWidget : public UI::Widget
	{
	public:
		SpaceDockWidget(UI::Widget * SupWidget, Reference< Planet > Planet, Reference< Character > Character);
	private:
		// callbacks
		void _OnRechargeButtonClicked(void);
		void _OnRefuelButtonClicked(void);
		void _OnRepairButtonClicked(void);
		void _OnTakeOffButtonClicked(void);
		// helper functions & actions
		void _Recharge(void);
		void _Refuel(void);
		void _Repair(void);
		void _TakeOff(void);
		// member variables
		Reference< Character > _Character;
		Reference< Planet > _Planet;
	};
}

#endif
