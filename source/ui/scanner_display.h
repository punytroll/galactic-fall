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
#include "widget.h"

class Ship;

namespace UI
{
	class ScannerDisplay : public UI::Widget
	{
	public:
		ScannerDisplay(UI::Widget * SupWidget = 0);
		// getters
		const Reference< Ship > & GetOwner(void) const;
		// setters
		void SetOwner(Reference< Ship > Owner);
		// actors
		void Update(void);
		virtual void Draw(void) const;
	private:
		float _CameraZ;
		float _FieldOfView;
		Reference< Ship > _Owner;
	};

	inline const Reference< Ship > & ScannerDisplay::GetOwner(void) const
	{
		return _Owner;
	}

	inline void ScannerDisplay::SetOwner(Reference< Ship > Owner)
	{
		_Owner = Owner;
	}
}

#endif
