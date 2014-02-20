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

#ifndef UI_HANGAR_WIDGET_H
#define UI_HANGAR_WIDGET_H

#include "../referencing.h"
#include "widget.h"

class Character;
class Planet;

namespace UI
{
	class Button;
	class Label;
	class ProgressBar;
	
	class HangarWidget : public UI::Widget
	{
	public:
		HangarWidget(UI::Widget * SupWidget, Reference< Planet > Planet, Reference< Character > Character);
	private:
		// callbacks
		void _OnEnergyStateProgressBarUpdating(UI::ProgressBar * EnergyStateProgressBar, float RealTimeSeconds, float GameTimeSeconds);
		void _OnFuelStateProgressBarUpdating(UI::ProgressBar * FuelStateProgressBar, float RealTimeSeconds, float GameTimeSeconds);
		void _OnHullStateProgressBarUpdating(UI::ProgressBar * HullStateProgressBar, float RealTimeSeconds, float GameTimeSeconds);
		void _OnRechargeButtonClicked(void);
		void _OnRechargeButtonUpdating(UI::Button * RechargeButton, float RealTimeSeconds, float GameTimeSeconds);
		void _OnRefuelButtonClicked(void);
		void _OnRefuelButtonUpdating(UI::Button * RefuelButton, float RealTimeSeconds, float GameTimeSeconds);
		void _OnRepairButtonClicked(void);
		void _OnRepairButtonUpdating(UI::Button * RechargeButton, float RealTimeSeconds, float GameTimeSeconds);
		void _OnTakeOffButtonClicked(void);
		// member variables
		Reference< Character > _Character;
		Reference< Planet > _Planet;
	};
}

#endif
