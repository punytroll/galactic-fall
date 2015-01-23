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

#ifndef FACTION_H
#define FACTION_H

#include "graphics/color_rgbo.h"
#include "object.h"

class Faction : public Object
{
public:
	Faction(void);
	virtual ~Faction(void) override;
	// getters
	const Graphics::ColorRGBO & GetColor(void) const;
	// setters
	void SetColor(const Graphics::ColorRGBO & Color);
private:
	Graphics::ColorRGBO _Color;
};

inline const Graphics::ColorRGBO & Faction::GetColor(void) const
{
	return _Color;
}

inline void Faction::SetColor(const Graphics::ColorRGBO & Color)
{
	_Color = Color;
}

#endif
