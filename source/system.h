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

#include "position.h"

class Cargo;
class Planet;
class Ship;
class Shot;
class Star;

class System : public Position
{
public:
	System(const std::string & Identifier);
	virtual ~System(void);
	const std::string & GetIdentifier(void) const;
	const std::string & GetName(void) const;
	const std::list< Cargo * > & GetCargos(void) const;
	const std::vector< Planet * > & GetPlanets(void) const;
	const std::list< Ship * > & GetShips(void) const;
	const std::list< Shot * > & GetShots(void) const;
	const std::list< System * > GetLinkedSystems(void) const;
	const Star * GetStar(void) const;
	bool IsLinkedToSystem(const System * System) const;
	void SetName(const std::string & Name);
	void AddLinkedSystem(System * LinkedSystem);
	// setters
	void SetTrafficDensity(float TrafficDensity);
	// getters
	float GetTrafficDensity(void) const;
protected:
	virtual bool OnAddContent(Object * Content);
	virtual bool OnRemoveContent(Object * Content);
private:
	std::string m_Identifier;
	std::string m_Name;
	float m_TrafficDensity;
	Star * m_Star;
	std::vector< Planet * > m_Planets;
	std::list< System * > m_LinkedSystems;
	std::list< Ship * > m_Ships;
	std::list< Cargo * > m_Cargos;
	std::list< Shot * > m_Shots;
};

inline const std::string & System::GetIdentifier(void) const
{
	return m_Identifier;
}

inline const std::string & System::GetName(void) const
{
	return m_Name;
}

inline const std::list< Cargo * > & System::GetCargos(void) const
{
	return m_Cargos;
}

inline const std::vector< Planet * > & System::GetPlanets(void) const
{
	return m_Planets;
}

inline const std::list< Ship * > & System::GetShips(void) const
{
	return m_Ships;
}

inline const std::list< Shot * > & System::GetShots(void) const
{
	return m_Shots;
}

inline const std::list< System * > System::GetLinkedSystems(void) const
{
	return m_LinkedSystems;
}

inline const Star * System::GetStar(void) const
{
	return m_Star;
}

inline void System::SetTrafficDensity(float TrafficDensity)
{
	m_TrafficDensity = TrafficDensity;
}

inline float System::GetTrafficDensity(void) const
{
	return m_TrafficDensity;
}

#endif
