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

#ifndef STAR_H
#define STAR_H

#include "graphics/color_rgb.h"
#include "object.h"

class Star : public Object
{
public:
	Star(void);
	virtual ~Star(void) override;
	// getters
	const Graphics::ColorRGB & GetColor(void) const;
	// setters
	void SetColor(const Graphics::ColorRGB & Color);
private:
	Graphics::ColorRGB _Color;
};

inline const Graphics::ColorRGB & Star::GetColor(void) const
{
	return _Color;
}

inline void Star::SetColor(const Graphics::ColorRGB & Color)
{
	_Color = Color;
}

#endif
