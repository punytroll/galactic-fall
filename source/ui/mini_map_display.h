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

#ifndef UI_MINI_MAP_DISPLAY_H
#define UI_MINI_MAP_DISPLAY_H

#include "view_display.h"

class Character;

namespace Graphics
{
	class RenderContext;
}

namespace UI
{
	class Event;
	
	class MiniMapDisplay : public UI::ViewDisplay
	{
	public:
		// constructor & destructor
		MiniMapDisplay(void);
		virtual ~MiniMapDisplay(void);
		// setters
		void SetCharacter(Character * Character);
	private:
		// modifiers
		void _ClearView(void);
		void _SetupView(void);
		// callbacks
		void _OnCharacterDestroying(void);
		void _OnDestroying(UI::Event & DestroyingEvent);
		void _OnDestroyInScene(Graphics::Node * Node);
		void _OnDraw(Graphics::RenderContext * RenderContext);
		void _OnSizeChanged(UI::Event & SizeChangedEvent);
		Character * _Character;
		Connection _CharacterDestroyingConnection;
		float _Scale;
	};
}

#endif
