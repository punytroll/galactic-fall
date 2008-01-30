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

#include "commodity.h"
#include "planet.h"
#include "ship.h"
#include "shot.h"
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

bool System::IsAddingAllowed(Object * Content)
{
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
	}
	
	Star * TheStar(dynamic_cast< Star * >(Content));
	
	if(TheStar != 0)
	{
		return m_Star == 0;
	}
	
	return (dynamic_cast< Shot * >(Content) != 0) || (dynamic_cast< Ship * >(Content) != 0) || (dynamic_cast< Commodity * >(Content) != 0) || (Position::IsAddingAllowed(Content) == true);
}

bool System::IsRemovingAllowed(Object * Content)
{
	return Position::IsRemovingAllowed(Content);
}

void System::OnContentAdded(Object * Content)
{
	Position::OnContentAdded(Content);
	
	Shot * TheShot(dynamic_cast< Shot * >(Content));
	
	if(TheShot != 0)
	{
		m_Shots.push_back(TheShot);
		return;
	}
	
	Ship * TheShip(dynamic_cast< Ship * >(Content));
	
	if(TheShip != 0)
	{
		m_Ships.push_back(TheShip);
		return;
	}
	
	Commodity * TheCommodity(dynamic_cast< Commodity * >(Content));
	
	if(TheCommodity != 0)
	{
		m_Commodities.push_back(TheCommodity);
		return;
	}
	
	Planet * ThePlanet(dynamic_cast< Planet * >(Content));
	
	if(ThePlanet != 0)
	{
		for(std::vector< Planet *>::const_iterator PlanetIterator = m_Planets.begin(); PlanetIterator != m_Planets.end(); ++PlanetIterator)
		{
			if((*PlanetIterator)->GetIdentifier() == ThePlanet->GetIdentifier())
			{
				assert(false);
			}
		}
		m_Planets.push_back(ThePlanet);
		return;
	}
	
	Star * TheStar(dynamic_cast< Star * >(Content));
	
	if(TheStar != 0)
	{
		assert(m_Star == 0);
		m_Star = TheStar;
		return;
	}
	assert(false);
}

void System::OnContentRemoved(Object * Content)
{
	Shot * TheShot(dynamic_cast< Shot * >(Content));
	
	if(TheShot != 0)
	{
		std::list< Shot * >::iterator ShotIterator(std::find(m_Shots.begin(), m_Shots.end(), TheShot));
		
		assert(ShotIterator != m_Shots.end());
		m_Shots.erase(ShotIterator);
	}
	
	Ship * TheShip(dynamic_cast< Ship * >(Content));
	
	if(TheShip != 0)
	{
		std::list< Ship * >::iterator ShipIterator(std::find(m_Ships.begin(), m_Ships.end(), TheShip));
		
		assert(ShipIterator != m_Ships.end());
		m_Ships.erase(ShipIterator);
	}
	
	Commodity * TheCommodity(dynamic_cast< Commodity * >(Content));
	
	if(TheCommodity != 0)
	{
		std::list< Commodity * >::iterator CommodityIterator(std::find(m_Commodities.begin(), m_Commodities.end(), TheCommodity));
		
		assert(CommodityIterator != m_Commodities.end());
		m_Commodities.erase(CommodityIterator);
	}
	
	Planet * ThePlanet(dynamic_cast< Planet * >(Content));
	
	if(ThePlanet != 0)
	{
		for(std::vector< Planet *>::iterator PlanetIterator = m_Planets.begin(); PlanetIterator != m_Planets.end(); ++PlanetIterator)
		{
			if((*PlanetIterator)->GetIdentifier() == ThePlanet->GetIdentifier())
			{
				m_Planets.erase(PlanetIterator);
				
				break;
			}
		}
	}
	
	Star * TheStar(dynamic_cast< Star * >(Content));
	
	if(TheStar != 0)
	{
		assert(m_Star == TheStar);
		m_Star = 0;
	}
	Position::OnContentRemoved(Content);
}

void System::AddLinkedSystem(System * LinkedSystem)
{
	m_LinkedSystems.push_back(LinkedSystem);
}
