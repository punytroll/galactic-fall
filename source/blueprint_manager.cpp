/**
 * galactic-fall
 * Copyright (C) 2018-2019  Hagen Möbius
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

#include "blueprint.h"
#include "blueprint_manager.h"
#include "visualization_prototype.h"

BlueprintManager::~BlueprintManager(void)
{
	while(_Blueprints.size() > 0)
	{
		auto Blueprint{_Blueprints.begin()->second};
		
		_Blueprints.erase(_Blueprints.begin());
		delete Blueprint;
	}
}

Blueprint * BlueprintManager::Create(const std::string & TypeIdentifier, const std::string & SubTypeIdentifier)
{
	auto Result{new Blueprint(TypeIdentifier, SubTypeIdentifier)};
	
	_Blueprints.insert({{TypeIdentifier, SubTypeIdentifier}, Result});
	
	return Result;
}
	
const Blueprint * BlueprintManager::Get(const std::string & TypeIdentifier, const std::string & SubTypeIdentifier) const
{
	auto Iterator{_Blueprints.find({TypeIdentifier, SubTypeIdentifier})};
	
	if(Iterator == _Blueprints.end())
	{
		return nullptr;
	}
	else
	{
		return Iterator->second;
	}
}

std::uint32_t BlueprintManager::GetBasePrice(const std::string & TypeIdentifier, const std::string & SubTypeIdentifier) const
{
	auto Blueprint{Get(TypeIdentifier, SubTypeIdentifier)};
	
	assert(Blueprint != nullptr);
	
	return Blueprint->GetValue< std::uint32_t >("base-price");
}

const std::string & BlueprintManager::GetDescription(const std::string & TypeIdentifier, const std::string & SubTypeIdentifier) const
{
	auto Blueprint{Get(TypeIdentifier, SubTypeIdentifier)};
	
	assert(Blueprint != nullptr);
	
	return Blueprint->GetValue< std::string >("description");
}

const std::string & BlueprintManager::GetName(const std::string & TypeIdentifier, const std::string & SubTypeIdentifier) const
{
	auto Blueprint{Get(TypeIdentifier, SubTypeIdentifier)};
	
	assert(Blueprint != nullptr);
	
	return Blueprint->GetValue< std::string >("name");
}

std::uint32_t BlueprintManager::GetSpaceRequirement(const std::string & TypeIdentifier, const std::string & SubTypeIdentifier) const
{
	auto Blueprint{Get(TypeIdentifier, SubTypeIdentifier)};
	
	assert(Blueprint != nullptr);
	
	return Blueprint->GetValue< std::uint32_t >("space-requirement");
}

const VisualizationPrototype * BlueprintManager::GetVisualizationPrototype(const std::string & TypeIdentifier, const std::string & SubTypeIdentifier) const
{
	auto Blueprint{Get(TypeIdentifier, SubTypeIdentifier)};
	
	assert(Blueprint != nullptr);
	
	return &(Blueprint->GetValue< VisualizationPrototype >("visualization-prototype"));
}

bool BlueprintManager::Has(const std::string & TypeIdentifier, const std::string & SubTypeIdentifier) const
{
	return _Blueprints.find({TypeIdentifier, SubTypeIdentifier}) != _Blueprints.end();
}
