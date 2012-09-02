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

#ifndef UI_PLANET_DIALOG_H
#define UI_PLANET_DIALOG_H

#include "window.h"

class Character;
class Planet;

namespace UI
{
	class Button;
	class Label;
	class TradeCenterWidget;
	
	class PlanetDialog : public UI::Window
	{
	public:
		PlanetDialog(UI::Widget * SupWidget, Planet * Planet, Character * Character);
	private:
		// callbacks
		void _OnDestroying(void);
		bool _OnKey(const KeyEventInformation & KeyEventInformation);
		void _OnHomeButtonClicked(void);
		void _OnRefuelButtonClicked(void);
		void _OnTakeOffButtonClicked(void);
		void _OnTradeCenterButtonClicked(void);
		// helper functions & actions
		void _OpenTradeCenterWidget(void);
		// member variables
		Planet * _Planet;
		Character * _Character;
		UI::Label * _DescriptionLabel;
		UI::TradeCenterWidget * _TradeCenterWidget;
	};
}

#endif
