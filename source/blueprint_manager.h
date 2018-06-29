/**
 * galactic-fall
 * Copyright (C) 2018  Hagen Möbius
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

#ifndef BLUEPRINT_MANAGER_H
#define BLUEPRINT_MANAGER_H

#include <map>
#include <string>

class Blueprint;
class VisualizationPrototype;

class BlueprintManager
{
public:
	// destructor
	~BlueprintManager(void);
	// getters
	const Blueprint * Get(const std::string & TypeIdentifier, const std::string & SubTypeIdentifier) const;
	std::uint32_t GetBasePrice(const std::string & TypeIdentifier, const std::string & SubTypeIdentifier) const;
	const std::string & GetDescription(const std::string & TypeIdentifier, const std::string & SubTypeIdentifier) const;
	const std::string & GetName(const std::string & TypeIdentifier, const std::string & SubTypeIdentifier) const;
	std::uint32_t GetSpaceRequirement(const std::string & TypeIdentifier, const std::string & SubTypeIdentifier) const;
	const VisualizationPrototype * GetVisualizationPrototype(const std::string & TypeIdentifier, const std::string & SubTypeIdentifier) const;
	bool Has(const std::string & TypeIdentifier, const std::string & SubTypeIdentifier) const;
	// modifiers
	Blueprint * Create(const std::string & TypeIdentifier, const std::string & SubTypeIdentifier);
private:
	std::map< std::pair< std::string, std::string >, Blueprint * > _Blueprints;
};

#endif
