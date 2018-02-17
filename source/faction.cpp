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

#include <algorithm>

#include "faction.h"

class FactionStanding
{
public:
	Connection _DestroyingConnection;
	Faction * _Faction;
	float _Standing;
};

Faction::Faction(void)
{
	// initialize object aspects
	AddAspectName();
}

Faction::~Faction(void)
{
	while(_FactionStandings.size() > 0)
	{
		auto FactionStanding{_FactionStandings.back()};
		
		assert(FactionStanding->_Faction != nullptr);
		FactionStanding->_Faction = nullptr;
		assert(FactionStanding->_DestroyingConnection.IsValid() == true);
		FactionStanding->_DestroyingConnection.Disconnect();
		assert(FactionStanding->_DestroyingConnection.IsValid() == false);
		delete FactionStanding;
		_FactionStandings.pop_back();
	}
}

void Faction::AddFactionStanding(Faction * Faction, float Standing)
{
	assert(Faction != nullptr);
	for(auto FactionStanding : _FactionStandings)
	{
		assert(FactionStanding != nullptr);
		assert(FactionStanding->_Faction != Faction);
	}
	
	auto NewFactionStanding{new FactionStanding{}};
	
	NewFactionStanding->_Faction = Faction;
	NewFactionStanding->_Standing = Standing;
	NewFactionStanding->_DestroyingConnection = Faction->ConnectDestroyingCallback(std::bind(&Faction::_OnFactionDestroying, this, Faction));
	_FactionStandings.push_back(NewFactionStanding);
}

float Faction::GetStanding(Faction * Faction)
{
	// If we don't know the other faction, assume slightly above neutral to avoid conflict
	// This default value might later be tweaked per faction in relation to how aggressive a faction is when meeting new factions.
	auto Result{0.6f};
	
	assert(Faction != nullptr);
	for(auto FactionStanding : _FactionStandings)
	{
		assert(FactionStanding != nullptr);
		if(FactionStanding->_Faction == Faction)
		{
			Result = FactionStanding->_Standing;
			
			break;
		}
	}
	
	return Result;
}

void Faction::_OnFactionDestroying(Faction * Faction)
{
	auto FactionStandingIterator{std::find_if(_FactionStandings.begin(), _FactionStandings.end(), [&] (FactionStanding * FactionStanding) { return FactionStanding->_Faction == Faction; })};
	
	assert(FactionStandingIterator != _FactionStandings.end());
	
	auto FactionStanding{*FactionStandingIterator};
	
	assert(FactionStanding->_Faction != nullptr);
	FactionStanding->_Faction = nullptr;
	assert(FactionStanding->_DestroyingConnection.IsValid() == true);
	FactionStanding->_DestroyingConnection.Disconnect();
	assert(FactionStanding->_DestroyingConnection.IsValid() == false);
	delete FactionStanding;
	_FactionStandings.erase(FactionStandingIterator);
}
