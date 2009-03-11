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

#include <string>

#include "object.h"

class Color;

class Faction : public Object
{
public:
	Faction(const std::string & Identifier);
	~Faction(void);
	// getters
	Color * GetColor(void);
	const std::string & GetIdentifier(void) const;
	// setters
	/**
	 * @note This function passes memory management responsibility for the @a Color object to this faction object.
	 **/
	void SetColor(Color * Color);
private:
	Color * m_Color;
	std::string m_Identifier;
};

inline Color * Faction::GetColor(void)
{
	return m_Color;
}

inline const std::string & Faction::GetIdentifier(void) const
{
	return m_Identifier;
}

#endif
