/**
 * galactic-fall
 * Copyright (C) 2013  Hagen Möbius
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

#ifndef UI_PLANET_WINDOW_H
#define UI_PLANET_WINDOW_H

#include "window.h"

class Character;
class Planet;

namespace UI
{
	class Label;
	class HangarWidget;
	class TradeCenterWidget;
	
	class PlanetWindow : public UI::Window
	{
	public:
		PlanetWindow(UI::Widget * SupWidget, Planet * Planet, Character * Character);
	private:
		// callbacks
		void _OnCharacterDestroying(void);
		void _OnDestroying(UI::Event & DestroyingEvent);
		void _OnHangarButtonClicked(void);
		void _OnHomeButtonClicked(void);
		void _OnKey(UI::KeyEvent & KeyEvent);
		void _OnPlanetDestroying(void);
		void _OnTradeCenterButtonClicked(void);
		// helper functions & actions
		void _OpenHangar(void);
		void _OpenHomeScreen(void);
		void _OpenTradeCenter(void);
		// member variables
		Character * _Character;
		Connection _CharacterDestroyingConnection;
		UI::Label * _DescriptionLabel;
		UI::HangarWidget * _HangarWidget;
		Planet * _Planet;
		Connection _PlanetDestroyingConnection;
		UI::TradeCenterWidget * _TradeCenterWidget;
	};
}

#endif
