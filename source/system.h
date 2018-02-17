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

#ifndef SYSTEM_H
#define SYSTEM_H

#include <list>
#include <string>
#include <vector>

#include "object.h"

class Commodity;
class Faction;
class FactionInfluence;
class Planet;
class Ship;
class Shot;
class Star;
class Visualization;

class System : public Object
{
public:
	// constructor and destructor
	System(void);
	virtual ~System(void) override;
	// queries
	const std::list< Commodity * > & GetCommodities(void) const;
	const std::list< System * > GetLinkedSystems(void) const;
	const std::vector< Planet * > & GetPlanets(void) const;
	Faction * GetRandomFactionAccordingToInfluences(void);
	const std::list< Ship * > & GetShips(void) const;
	const std::list< Shot * > & GetShots(void) const;
	const Star * GetStar(void) const;
	float GetTrafficDensity(void) const;
	bool IsLinkedToSystem(const System * System) const;
	// modifiers
	void AddFactionInfluence(Faction * Faction, float Influence);
	void AddLinkedSystem(System * LinkedSystem);
	void SetTrafficDensity(float TrafficDensity);
private:
	// callbacks
	void _OnAdded(Object * Content);
	void _OnFactionDestroying(Faction * Faction);
	void _OnRemoved(Object * Content);
	void _UpdateVisualization(Visualization * Visualization);
	// member variables
	std::list< Commodity * > _Commodities;
	std::vector< FactionInfluence * > _FactionInfluences;
	std::list< System * > _LinkedSystems;
	std::vector< Planet * > _Planets;
	std::list< Ship * > _Ships;
	std::list< Shot * > _Shots;
	Star * _Star;
	float _TrafficDensity;
};

inline const std::list< Commodity * > & System::GetCommodities(void) const
{
	return _Commodities;
}

inline const std::list< System * > System::GetLinkedSystems(void) const
{
	return _LinkedSystems;
}

inline const std::vector< Planet * > & System::GetPlanets(void) const
{
	return _Planets;
}

inline const std::list< Ship * > & System::GetShips(void) const
{
	return _Ships;
}

inline const std::list< Shot * > & System::GetShots(void) const
{
	return _Shots;
}

inline const Star * System::GetStar(void) const
{
	return _Star;
}

inline float System::GetTrafficDensity(void) const
{
	return _TrafficDensity;
}

inline void System::SetTrafficDensity(float TrafficDensity)
{
	_TrafficDensity = TrafficDensity;
}

#endif
