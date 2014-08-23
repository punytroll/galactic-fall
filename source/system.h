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

#ifndef SYSTEM_H
#define SYSTEM_H

#include <list>
#include <string>
#include <vector>

#include "object.h"

class Commodity;
class Planet;
class Ship;
class Shot;
class Star;
class Visualization;

class System : public Object
{
public:
	System(void);
	virtual ~System(void) override;
	const std::list< Commodity * > & GetCommodities(void) const;
	const std::vector< Planet * > & GetPlanets(void) const;
	const std::list< Ship * > & GetShips(void) const;
	const std::list< Shot * > & GetShots(void) const;
	const std::list< System * > GetLinkedSystems(void) const;
	const Star * GetStar(void) const;
	bool IsLinkedToSystem(const System * System) const;
	void AddLinkedSystem(System * LinkedSystem);
	// setters
	void SetTrafficDensity(float TrafficDensity);
	// getters
	float GetTrafficDensity(void) const;
private:
	void _OnAdded(Object * Content);
	void _OnRemoved(Object * Content);
	void _UpdateVisualization(Visualization * Visualization);
	float _TrafficDensity;
	Star * _Star;
	std::vector< Planet * > _Planets;
	std::list< System * > _LinkedSystems;
	std::list< Ship * > _Ships;
	std::list< Commodity * > _Commodities;
	std::list< Shot * > _Shots;
};

inline const std::list< Commodity * > & System::GetCommodities(void) const
{
	return _Commodities;
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

inline const std::list< System * > System::GetLinkedSystems(void) const
{
	return _LinkedSystems;
}

inline const Star * System::GetStar(void) const
{
	return _Star;
}

inline void System::SetTrafficDensity(float TrafficDensity)
{
	_TrafficDensity = TrafficDensity;
}

inline float System::GetTrafficDensity(void) const
{
	return _TrafficDensity;
}

#endif
