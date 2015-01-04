/**
 * galactic-fall
 * Copyright (C) 2015  Hagen Möbius
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

#include "widget.h"

namespace UI
{
	class Label;
	class MiniMapDisplay;
	class ScannerDisplay;
	
	class HeadsUpDisplay : public UI::Widget
	{
	public:
		HeadsUpDisplay(UI::Widget * SupWidget);
	private:
		void _UpdateCreditsLabel(UI::Label * CreditsLabel, float RealTimeSeconds, float GameTimeSeconds);
		void _UpdateEnergyLabel(UI::Label * EnergyLabel, float RealTimeSeconds, float GameTimeSeconds);
		void _UpdateFuelLabel(UI::Label * EnergyLabel, float RealTimeSeconds, float GameTimeSeconds);
		void _UpdateHullLabel(UI::Label * EnergyLabel, float RealTimeSeconds, float GameTimeSeconds);
		void _UpdateLinkedSystemTargetLabel(UI::Label * LinkedSystemTargetLabel, float RealTimeSeconds, float GameTimeSeconds);
		void _UpdateMiniMapSystemNameLabel(UI::Label * MiniMapSystemNameLabel, float RealTimeSeconds, float GameTimeSeconds);
		void _UpdateMiniMapDisplay(UI::MiniMapDisplay * MiniMapDisplay, float RealTimeSeconds, float GameTimeSeconds);
		void _UpdateMiniMapWidget(UI::Widget * MiniMapWidget, float RealTimeSeconds, float GameTimeSeconds);
		void _UpdateScannerDisplay(UI::ScannerDisplay * ScannerDisplay, float RealTimeSeconds, float GameTimeSeconds);
		void _UpdateScannerTargetNameLabel(UI::Label * ScannerTargetNameLabel, float RealTimeSeconds, float GameTimeSeconds);
		void _UpdateScannerTargetFactionNameLabel(UI::Label * ScannerTargetFactionNameLabel, float RealTimeSeconds, float GameTimeSeconds);
		void _UpdateScannerWidget(UI::Widget * ScannerWidget, float RealTimeSeconds, float GameTimeSeconds);
		void _UpdateTimeWarpLabel(UI::Label * TimeWarpLabel, float RealTimeSeconds, float GameTimeSeconds);
	};
}
