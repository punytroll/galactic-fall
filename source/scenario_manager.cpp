/**
 * galactic-fall
 * Copyright (C) 2013  Hagen Möbius
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

#include <assert.h>

#include "scenario.h"
#include "scenario_manager.h"

ScenarioManager::ScenarioManager(void)
{
}

ScenarioManager::~ScenarioManager(void)
{
	while(_Scenarios.size() > 0)
	{
		Destroy(_Scenarios.begin()->first);
	}
}

const Scenario * ScenarioManager::Get(const std::string & Identifier) const
{
	typename std::map< std::string, Scenario * >::const_iterator ScenarioIterator(_Scenarios.find(Identifier));
	
	assert(ScenarioIterator != _Scenarios.end());
	
	return ScenarioIterator->second;
}

const std::map< std::string, Scenario * > & ScenarioManager::GetScenarios(void) const
{
	return _Scenarios;
}

Scenario * ScenarioManager::Create(const std::string & Identifier)
{
	assert(_Scenarios.find(Identifier) == _Scenarios.end());
	
	Scenario * NewScenario(new Scenario(Identifier));
	
	_Scenarios[NewScenario->GetIdentifier()] = NewScenario;
	
	return NewScenario;
}

void ScenarioManager::Destroy(const std::string & Identifier)
{
	typename std::map< std::string, Scenario * >::iterator ScenarioIterator(_Scenarios.find(Identifier));
	
	assert(ScenarioIterator != _Scenarios.end());
	
	delete ScenarioIterator->second;
	_Scenarios.erase(ScenarioIterator);
}
