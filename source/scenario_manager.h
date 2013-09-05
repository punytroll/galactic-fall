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

#ifndef SCENARIO_MANAGER_H
#define SCENARIO_MANAGER_H

#include <map>
#include <string>

class ScenarioManager
{
public:
	// constructor & destructor
	ScenarioManager(void);
	~ScenarioManager(void);
	// getters
	const Scenario * Get(const std::string & Identifier) const;
	const std::map< std::string, Scenario * > & GetScenarios(void) const;
	// modifiers
	Scenario * Create(const std::string & Identifier);
	void Destroy(const std::string & Identifier);
private:
	std::map< std::string, Scenario * > _Scenarios;
};

#endif
