/**
 * galactic-fall
 * Copyright (C) 2007  Hagen Möbius
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

#include "faction.h"
#include "galaxy.h"
#include "object_aspect_object_container.h"
#include "system.h"

Galaxy::Galaxy(void)
{
	// initialize object aspects
	AddAspectName();
	AddAspectObjectContainer();
	GetAspectObjectContainer()->ConnectContentAddedCallback(std::bind(&Galaxy::OnAdded, this, std::placeholders::_1));
	GetAspectObjectContainer()->ConnectContentRemovedCallback(std::bind(&Galaxy::OnRemoved, this, std::placeholders::_1));
}

Faction * Galaxy::GetFaction(const std::string & Identifier)
{
	std::map< std::string, Faction * >::iterator Faction(m_Factions.find(Identifier));
	
	if(Faction != m_Factions.end())
	{
		return Faction->second;
	}
	else
	{
		return 0;
	}
}

System * Galaxy::GetSystem(const std::string & Identifier)
{
	std::map< std::string, System * >::iterator System(m_Systems.find(Identifier));
	
	if(System != m_Systems.end())
	{
		return System->second;
	}
	else
	{
		return 0;
	}
}

void Galaxy::OnAdded(Object * Content)
{
	assert(Content != nullptr);
	if(Content->GetTypeIdentifier() == "system")
	{
		assert(m_Systems.find(Content->GetClassIdentifier()) == m_Systems.end());
		
		auto TheSystem(dynamic_cast< System * >(Content));
		
		assert(TheSystem != nullptr);
		m_Systems[Content->GetClassIdentifier()] = TheSystem;
	}
	else if(Content->GetTypeIdentifier() == "faction")
	{
		assert(m_Factions.find(Content->GetClassIdentifier()) == m_Factions.end());
		
		auto TheFaction(dynamic_cast< Faction * >(Content));
		
		assert(TheFaction != nullptr);
		m_Factions[Content->GetClassIdentifier()] = TheFaction;
	}
	else
	{
		assert(false);
	}
}

void Galaxy::OnRemoved(Object * Content)
{
	assert(Content != nullptr);
	if(Content->GetTypeIdentifier() == "system")
	{
		std::map< std::string, System * >::iterator Iterator(m_Systems.find(Content->GetClassIdentifier()));
		
		assert(Iterator != m_Systems.end());
		m_Systems.erase(Iterator);
	}
	else if(Content->GetTypeIdentifier() == "faction")
	{
		std::map< std::string, Faction * >::iterator Iterator(m_Factions.find(Content->GetClassIdentifier()));
		
		assert(Iterator != m_Factions.end());
		m_Factions.erase(Iterator);
	}
	else
	{
		assert(false);
	}
}
