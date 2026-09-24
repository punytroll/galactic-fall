/**
 * galactic-fall
 * Copyright (C) 2018-2026  Hagen Möbius
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

#include <cassert>

#include "blueprint_manager.h"
#include "visualization_prototype.h"

auto BlueprintManager::Create(std::string const & TypeIdentifier, std::string const & SubTypeIdentifier) -> Blueprint *
{
	auto [Iterator, Success] = m_Blueprints.insert({{TypeIdentifier, SubTypeIdentifier}, std::make_unique<Blueprint>(TypeIdentifier, SubTypeIdentifier)});
	
	return Iterator->second.get();
}
	
auto BlueprintManager::Get(std::string const & TypeIdentifier, std::string const & SubTypeIdentifier) const -> Blueprint const *
{
	auto Iterator = m_Blueprints.find({TypeIdentifier, SubTypeIdentifier});
	
	if(Iterator == m_Blueprints.end())
	{
		return nullptr;
	}
	else
	{
		return Iterator->second.get();
	}
}

auto BlueprintManager::GetBasePrice(std::string const & TypeIdentifier, std::string const & SubTypeIdentifier) const -> std::uint32_t
{
	auto Blueprint = Get(TypeIdentifier, SubTypeIdentifier);
	
	assert(Blueprint != nullptr);
	
	return Blueprint->GetValue<std::uint32_t>("base-price");
}

auto BlueprintManager::GetDescription(std::string const & TypeIdentifier, std::string const & SubTypeIdentifier) const -> std::string const &
{
	auto Blueprint = Get(TypeIdentifier, SubTypeIdentifier);
	
	assert(Blueprint != nullptr);
	
	return Blueprint->GetValue<std::string>("description");
}

auto BlueprintManager::GetName(std::string const & TypeIdentifier, std::string const & SubTypeIdentifier) const -> std::string const &
{
	auto Blueprint = Get(TypeIdentifier, SubTypeIdentifier);
	
	assert(Blueprint != nullptr);
	
	return Blueprint->GetValue<std::string>("name");
}

auto BlueprintManager::GetSpaceRequirement(std::string const & TypeIdentifier, std::string const & SubTypeIdentifier) const -> std::uint32_t
{
	auto Blueprint = Get(TypeIdentifier, SubTypeIdentifier);
	
	assert(Blueprint != nullptr);
	
	return Blueprint->GetValue<std::uint32_t>("space-requirement");
}

auto BlueprintManager::GetVisualizationPrototype(std::string const & TypeIdentifier, std::string const & SubTypeIdentifier) const -> VisualizationPrototype const *
{
	auto Blueprint = Get(TypeIdentifier, SubTypeIdentifier);
	
	assert(Blueprint != nullptr);
	
	return std::addressof(Blueprint->GetValue<VisualizationPrototype>("visualization-prototype"));
}

auto BlueprintManager::Has(std::string const & TypeIdentifier, std::string const & SubTypeIdentifier) const -> bool
{
	return m_Blueprints.find({TypeIdentifier, SubTypeIdentifier}) != m_Blueprints.end();
}
