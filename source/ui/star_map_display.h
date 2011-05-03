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

namespace UI
{
	class StarMapDisplay : public UI::Widget
	{
	public:
		StarMapDisplay(UI::Widget * SupWidget, System * System, Character * Character);
		virtual void Draw(void) const;
		System * GetSelectedSystem(void);
	private:
		// callbacks
		bool OnMouseButton(int Button, int State, float X, float Y);
		void OnMouseMoved(float X, float Y);
		// member variables
		Character * m_Character;
		System * m_System;
		System * m_SelectedSystem;
		float m_Scale;
		Vector2f m_GrabPosition;
		Vector2f m_OffsetPosition;
	};

	inline System * StarMapDisplay::GetSelectedSystem(void)
	{
		return m_SelectedSystem;
	}
}

#endif

