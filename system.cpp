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

#include "planet.h"
#include "star.h"
#include "system.h"

System::System(const std::string & Identifier) :
	m_Identifier(Identifier),
	m_Star(0)
{
}

System::~System(void)
{
	while(m_Planets.size() > 0)
	{
		delete m_Planets.front();
		m_Planets.erase(m_Planets.begin());
	}
	delete m_Star;
}

void System::SetName(const std::string & Name)
{
	m_Name = Name;
}

bool System::IsLinkedToSystem(const System * LinkedSystem) const
{
	for(std::list< System * >::const_iterator LinkedSystemIterator = m_LinkedSystems.begin(); LinkedSystemIterator != m_LinkedSystems.end(); ++LinkedSystemIterator)
	{
		if(*LinkedSystemIterator == LinkedSystem)
		{
			return true;
		}
	}
	
	return false;
}

Planet * System::CreatePlanet(const std::string & Identifier)
{
	m_Planets.push_back(new Planet(Identifier));
	
	return m_Planets.back();
}

Star * System::CreateStar(void)
{
	if(m_Star != 0)
	{
		return 0;
	}
	else
	{
		m_Star = new Star();
		
		return m_Star;
	}
}

void System::AddLinkedSystem(System * LinkedSystem)
{
	m_LinkedSystems.push_back(LinkedSystem);
}

void System::AddShip(Ship * Ship)
{
	m_Ships.push_back(Ship);
}

void System::AddCargo(Cargo * Cargo)
{
	m_Cargos.push_back(Cargo);
}

void System::AddShot(Shot * Shot)
{
	m_Shots.push_back(Shot);
}

void System::ClearShips(void)
{
	m_Ships.clear();
}

void System::ClearCargos(void)
{
	m_Cargos.clear();
}

void System::ClearShots(void)
{
	m_Shots.clear();
}

void System::RemoveCargo(Cargo * Remove)
{
	std::list< Cargo * >::iterator Iterator(find(m_Cargos.begin(), m_Cargos.end(), Remove));
	
	if(Iterator != m_Cargos.end())
	{
		m_Cargos.erase(Iterator);
	}
}

void System::RemoveShip(Ship * Remove)
{
	std::list< Ship * >::iterator Iterator(find(m_Ships.begin(), m_Ships.end(), Remove));
	
	if(Iterator != m_Ships.end())
	{
		m_Ships.erase(Iterator);
	}
}
