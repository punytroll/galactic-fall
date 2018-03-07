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

#include "faction.h"
#include "galaxy.h"
#include "object_aspect_object_container.h"
#include "system.h"

Galaxy::Galaxy(void)
{
	// initialize object aspects
	AddAspectName();
	AddAspectObjectContainer();
	GetAspectObjectContainer()->ConnectContentAddedCallback(std::bind(&Galaxy::_OnAdded, this, std::placeholders::_1));
	GetAspectObjectContainer()->ConnectContentRemovedCallback(std::bind(&Galaxy::_OnRemoved, this, std::placeholders::_1));
}

Faction * Galaxy::GetFaction(const std::string & Identifier)
{
	auto FactionIterator{_Factions.find(Identifier)};
	
	if(FactionIterator != _Factions.end())
	{
		return FactionIterator->second;
	}
	else
	{
		return nullptr;
	}
}

System * Galaxy::GetSystem(const std::string & Identifier)
{
	auto SystemIterator{_Systems.find(Identifier)};
	
	if(SystemIterator != _Systems.end())
	{
		return SystemIterator->second;
	}
	else
	{
		return nullptr;
	}
}

void Galaxy::_OnAdded(Object * Content)
{
	assert(Content != nullptr);
	if(Content->GetTypeIdentifier() == "system")
	{
		assert(_Systems.find(Content->GetSubTypeIdentifier()) == _Systems.end());
		
		auto AddedSystem{dynamic_cast< System * >(Content)};
		
		assert(AddedSystem != nullptr);
		_Systems[Content->GetSubTypeIdentifier()] = AddedSystem;
	}
	else if(Content->GetTypeIdentifier() == "faction")
	{
		assert(_Factions.find(Content->GetSubTypeIdentifier()) == _Factions.end());
		
		auto AddedFaction{dynamic_cast< Faction * >(Content)};
		
		assert(AddedFaction != nullptr);
		_Factions[Content->GetSubTypeIdentifier()] = AddedFaction;
	}
	else
	{
		// we don't allow objects of any other type
		assert(false);
	}
}

void Galaxy::_OnRemoved(Object * Content)
{
	assert(Content != nullptr);
	if(Content->GetTypeIdentifier() == "system")
	{
		auto SystemIterator(_Systems.find(Content->GetSubTypeIdentifier()));
		
		assert(SystemIterator != _Systems.end());
		_Systems.erase(SystemIterator);
	}
	else if(Content->GetTypeIdentifier() == "faction")
	{
		auto FactionIterator(_Factions.find(Content->GetSubTypeIdentifier()));
		
		assert(FactionIterator != _Factions.end());
		_Factions.erase(FactionIterator);
	}
	else
	{
		// we don't allow objects of any other type
		assert(false);
	}
}
