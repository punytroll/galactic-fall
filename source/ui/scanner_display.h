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

#ifndef SCANNER_DISPLAY_H
#define SCANNER_DISPLAY_H

#include "../referencing.h"
#include "view_display.h"

class Object;

namespace Graphics
{
	class Node;
}

namespace UI
{
	class ScannerDisplay : public UI::ViewDisplay
	{
	public:
		// constructor & destructor
		ScannerDisplay(UI::Widget * SupWidget = 0);
		// getters
		const Reference< Object > & GetTarget(void) const;
		// setters
		void SetTarget(Reference< Object > Target);
	private:
		// modifiers
		void _Clear(void);
		void _OnDestroying(void);
		void _OnDestroyInScene(Graphics::Node * Node);
		void _OnUpdating(float RealTimeSeconds, float GameTimeSeconds);
		void _Setup(void);
		// variables
		Reference< Object > _Target;
	};

	inline const Reference< Object > & ScannerDisplay::GetTarget(void) const
	{
		return _Target;
	}
}

#endif
