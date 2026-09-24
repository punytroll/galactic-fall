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

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

#include "blueprint.h"
#include "hashing.h"

class VisualizationPrototype;

class BlueprintManager
{
public:
	auto Get(std::string const & TypeIdentifier, std::string const & SubTypeIdentifier) const -> Blueprint const *;
	auto GetBasePrice(std::string const & TypeIdentifier, std::string const & SubTypeIdentifier) const -> std::uint32_t;
	auto GetDescription(std::string const & TypeIdentifier, std::string const & SubTypeIdentifier) const -> std::string const &;
	auto GetName(std::string const & TypeIdentifier, std::string const & SubTypeIdentifier) const -> std::string const &;
	auto GetSpaceRequirement(std::string const & TypeIdentifier, std::string const & SubTypeIdentifier) const -> std::uint32_t;
	auto GetVisualizationPrototype(std::string const & TypeIdentifier, std::string const & SubTypeIdentifier) const -> VisualizationPrototype const *;
	auto Has(std::string const & TypeIdentifier, std::string const & SubTypeIdentifier) const -> bool;
	auto Create(std::string const & TypeIdentifier, std::string const & SubTypeIdentifier) -> Blueprint *;
    
private:
	std::unordered_map<std::pair<std::string, std::string>, std::unique_ptr<Blueprint>, PairHash> m_Blueprints;
};

#endif
