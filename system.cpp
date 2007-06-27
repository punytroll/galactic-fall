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

#include <float.h>

#include "planet.h"
#include "ship.h"
#include "star.h"
#include "system.h"

System::System(const std::string & Identifier) :
	m_Identifier(Identifier),
	m_TrafficDensity(FLT_MAX),
	m_Star(0)
{
}

System::~System(void)
{
	assert(m_Star == 0);
	assert(m_Planets.empty() == true);
	assert(m_Ships.empty() == true);
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

bool System::OnAddContent(Object * Content)
{
	Ship * TheShip(dynamic_cast< Ship * >(Content));
	
	if(TheShip != 0)
	{
		if(Position::OnAddContent(Content) == true)
		{
			m_Ships.push_back(TheShip);
			
			return true;
		}
		
		return false;
	}
	
	Planet * ThePlanet(dynamic_cast< Planet * >(Content));
	
	if(ThePlanet != 0)
	{
		for(std::vector< Planet *>::const_iterator PlanetIterator = m_Planets.begin(); PlanetIterator != m_Planets.end(); ++PlanetIterator)
		{
			if((*PlanetIterator)->GetIdentifier() == ThePlanet->GetIdentifier())
			{
				return false;
			}
		}
		if(Position::OnAddContent(Content) == true)
		{
			m_Planets.push_back(ThePlanet);
			
			return true;
		}
		
		return false;
	}
	
	Star * TheStar(dynamic_cast< Star * >(Content));
	
	if(TheStar != 0)
	{
		if(m_Star == 0)
		{
			if(Position::OnAddContent(Content) == true)
			{
				m_Star = TheStar;
				
				return true;
			}
		}
		
		return false;
	}
	
	return Position::OnAddContent(Content);
}

bool System::OnRemoveContent(Object * Content)
{
	Ship * TheShip(dynamic_cast< Ship * >(Content));
	
	if(TheShip != 0)
	{
		std::list< Ship * >::iterator ShipIterator(std::find(m_Ships.begin(), m_Ships.end(), TheShip));
		
		if(ShipIterator != m_Ships.end())
		{
			if(Position::OnAddContent(Content) == true)
			{
				m_Ships.erase(ShipIterator);
				
				return true;
			}
		}
		
		return false;
	}
	
	Planet * ThePlanet(dynamic_cast< Planet * >(Content));
	
	if(ThePlanet != 0)
	{
		for(std::vector< Planet *>::iterator PlanetIterator = m_Planets.begin(); PlanetIterator != m_Planets.end(); ++PlanetIterator)
		{
			if((*PlanetIterator)->GetIdentifier() == ThePlanet->GetIdentifier())
			{
				if(Position::OnAddContent(Content) == true)
				{
					m_Planets.erase(PlanetIterator);
					
					return true;
				}
			}
		}
		
		return false;
	}
	
	Star * TheStar(dynamic_cast< Star * >(Content));
	
	if(TheStar != 0)
	{
		if(m_Star != 0)
		{
			if(Position::OnAddContent(Content) == true)
			{
				m_Star = 0;
				
				return true;
			}
		}
		
		return false;
	}
	
	return Position::OnAddContent(Content);
}

void System::AddLinkedSystem(System * LinkedSystem)
{
	m_LinkedSystems.push_back(LinkedSystem);
}

void System::AddCargo(Cargo * Cargo)
{
	m_Cargos.push_back(Cargo);
}

void System::AddShot(Shot * Shot)
{
	m_Shots.push_back(Shot);
}
