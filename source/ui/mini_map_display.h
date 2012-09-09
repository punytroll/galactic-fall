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

#include "../referencing.h"
#include "viewport.h"

class Ship;

namespace UI
{
	class MiniMapDisplay : public UI::Viewport
	{
	public:
		MiniMapDisplay(UI::Widget * SupWidget = 0);
		void SetOwner(Reference< Ship > Owner);
		virtual void DrawInViewport(void) const;
	private:
		Reference< Ship > m_Owner;
	};
}

#endif