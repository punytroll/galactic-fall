/**
 * galactic-fall
 * Copyright (C) 2006-2018  Hagen Möbius
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

#include <vector>

#include "graphics/color_rgbo.h"
#include "object.h"

class FactionStanding;

class Faction : public Object
{
public:
	Faction(void);
	virtual ~Faction(void) override;
	// queries
	const Graphics::ColorRGBO & GetColor(void) const;
	float GetStanding(Faction * Faction);
	// modifiers
	void AddFactionStanding(Faction * Faction, float Standing);
	void SetColor(const Graphics::ColorRGBO & Color);
private:
	// callbacks
	void _OnFactionDestroying(Faction * Faction);
	// member variables
	Graphics::ColorRGBO _Color;
	std::vector< FactionStanding * > _FactionStandings;
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
