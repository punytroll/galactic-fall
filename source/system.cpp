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

#include <algorithm>

#include "commodity.h"
#include "object_aspect_object_container.h"
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
	// initialize object aspects
	AddAspectName();
	AddAspectObjectContainer();
	GetAspectObjectContainer()->SetAllowAddingCallback(Callback(this, &System::AllowAdding));
	GetAspectObjectContainer()->SetAllowRemovingCallback(Callback(this, &System::AllowRemoving));
	GetAspectObjectContainer()->SetOnAddedCallback(Callback(this, &System::OnAdded));
	GetAspectObjectContainer()->SetOnRemovedCallback(Callback(this, &System::OnRemoved));
	AddAspectPosition();
}

System::~System(void)
{
	assert(m_Star == 0);
	assert(m_Planets.empty() == true);
	assert(m_Ships.empty() == true);
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

bool System::AllowAdding(Object * Content)
{
	Star * TheStar(dynamic_cast< Star * >(Content));
	
	if(TheStar != 0)
	{
		return m_Star == 0;
	}
	
	return (Content->GetTypeIdentifier() == "commodity") || (Content->GetTypeIdentifier() == "planet") || (Content->GetTypeIdentifier() == "ship") || (Content->GetTypeIdentifier() == "shot");
}

bool System::AllowRemoving(Object * Content)
{
	return true;
}

void System::OnAdded(Object * Content)
{
	if(Content->GetTypeIdentifier() == "commodity")
	{
		m_Commodities.push_back(dynamic_cast< Commodity * >(Content));
	}
	else if(Content->GetTypeIdentifier() == "planet")
	{
		for(std::vector< Planet *>::const_iterator PlanetIterator = m_Planets.begin(); PlanetIterator != m_Planets.end(); ++PlanetIterator)
		{
			if((*PlanetIterator)->GetIdentifier() == Content->GetClassIdentifier())
			{
				assert(false);
			}
		}
		m_Planets.push_back(dynamic_cast< Planet * >(Content));
	}
	else if(Content->GetTypeIdentifier() == "ship")
	{
		m_Ships.push_back(dynamic_cast< Ship * >(Content));
	}
	else if(Content->GetTypeIdentifier() == "shot")
	{
		m_Shots.push_back(dynamic_cast< Shot * >(Content));
	}
	
	Star * TheStar(dynamic_cast< Star * >(Content));
	
	if(TheStar != 0)
	{
		assert(m_Star == 0);
		m_Star = TheStar;
	}
}

void System::OnRemoved(Object * Content)
{
	if(Content->GetTypeIdentifier() == "commodity")
	{
		std::list< Commodity * >::iterator CommodityIterator(std::find(m_Commodities.begin(), m_Commodities.end(), Content));
		
		assert(CommodityIterator != m_Commodities.end());
		m_Commodities.erase(CommodityIterator);
	}
	else if(Content->GetTypeIdentifier() == "planet")
	{
		for(std::vector< Planet *>::iterator PlanetIterator = m_Planets.begin(); PlanetIterator != m_Planets.end(); ++PlanetIterator)
		{
			if((*PlanetIterator)->GetIdentifier() == Content->GetClassIdentifier())
			{
				m_Planets.erase(PlanetIterator);
				
				break;
			}
		}
	}
	else if(Content->GetTypeIdentifier() == "ship")
	{
		std::list< Ship * >::iterator ShipIterator(std::find(m_Ships.begin(), m_Ships.end(), Content));
		
		assert(ShipIterator != m_Ships.end());
		m_Ships.erase(ShipIterator);
	}
	else if(Content->GetTypeIdentifier() == "shot")
	{
		std::list< Shot * >::iterator ShotIterator(std::find(m_Shots.begin(), m_Shots.end(), Content));
		
		assert(ShotIterator != m_Shots.end());
		m_Shots.erase(ShotIterator);
	}
	
	Star * TheStar(dynamic_cast< Star * >(Content));
	
	if(TheStar != 0)
	{
		assert(m_Star == TheStar);
		m_Star = 0;
	}
}

void System::AddLinkedSystem(System * LinkedSystem)
{
	m_LinkedSystems.push_back(LinkedSystem);
}
