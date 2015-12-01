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

#ifndef UI_STAR_MAP_DISPLAY_H
#define UI_STAR_MAP_DISPLAY_H

#include "widget.h"

class Character;
class System;

namespace Graphics
{
	class RenderContext;
}

namespace UI
{
	class StarMapDisplay : public UI::Widget
	{
	public:
		StarMapDisplay(UI::Widget * SupWidget, Character * Character);
		virtual void Draw(Graphics::RenderContext * RenderContext) override;
		// getters
		System * GetSelectedSystem(void);
		// setters
		void SetSelectedSystem(System * SelectedSystem);
	private:
		// callbacks
		void _OnCharacterDestroying(void);
		void _OnDestroying(UI::Event & DestroyingEvent);
		void _OnMouseButton(UI::MouseButtonEvent & MouseButtonEvent);
		void _OnMouseMove(UI::MouseMoveEvent & MouseMoveEvent);
		void _OnSelectedSystemDestroying(void);
		// member variables
		Character * _Character;
		Connection _CharacterDestroyingConnection;
		Vector2f _GrabPosition;
		Vector2f _OffsetPosition;
		float _Scale;
		System * _SelectedSystem;
		Connection _SelectedSystemDestroyingConnection;
	};

	inline System * StarMapDisplay::GetSelectedSystem(void)
	{
		return _SelectedSystem;
	}
}

#endif

