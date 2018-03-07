/**
 * galactic-fall
 * Copyright (C) 2007-2018  Hagen Möbius
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

#ifndef GALAXY_H
#define GALAXY_H

#include <map>
#include <string>

#include "object.h"

class Faction;
class System;

class Galaxy : public Object
{
public:
	// constructor & destructor
	Galaxy(void);
	// getters
	Faction * GetFaction(const std::string & Identifier);
	const std::map< std::string, Faction * > & GetFactions(void) const;
	System * GetSystem(const std::string & Identifier);
	const std::map< std::string, System * > & GetSystems(void) const;
private:
	void _OnAdded(Object * Content);
	void _OnRemoved(Object * Content);
	std::map< std::string, System * > _Systems;
	std::map< std::string, Faction * > _Factions;
};

inline const std::map< std::string, Faction * > & Galaxy::GetFactions(void) const
{
	return _Factions;
}

inline const std::map< std::string, System * > & Galaxy::GetSystems(void) const
{
	return _Systems;
}

#endif
