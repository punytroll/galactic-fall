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

#include <stdexcept>

#include "character.h"
#include "class_manager.h"
#include "command_mind.h"
#include "commodity.h"
#include "commodity_class.h"
#include "globals.h"
#include "graphics_model.h"
#include "object_aspect_name.h"
#include "object_aspect_physical.h"
#include "object_aspect_visualization.h"
#include "object_factory.h"
#include "planet.h"
#include "ship.h"
#include "shot.h"
#include "weapon.h"
#include "weapon_class.h"
#include "visualization_prototype.h"

Object * ObjectFactory::Create(const std::string & TypeIdentifier, const std::string & ClassIdentifier) const
{
	Object * Result(0);
	
	if(TypeIdentifier == "character")
	{
		assert(ClassIdentifier.empty() == true);
		Result = new Character();
	}
	else if(TypeIdentifier == "commodity")
	{
		const CommodityClass * CommodityClass(g_CommodityClassManager->Get(ClassIdentifier));
		
		Result = new Commodity();
		// set up name aspect
		assert(Result->GetAspectName() != 0);
		Result->GetAspectName()->SetName(CommodityClass->GetName());
		// set up physical aspect
		assert(Result->GetAspectPhysical() != 0);
		Result->GetAspectPhysical()->SetRadialSize(CommodityClass->GetVisualizationPrototype()->GetModel()->GetRadialSize());
		Result->GetAspectPhysical()->SetSpaceRequirement(CommodityClass->GetSpaceRequirement());
		// set up visualization aspect
		assert(Result->GetAspectVisualization() != 0);
		Result->GetAspectVisualization()->SetVisualizationPrototype(CommodityClass->GetVisualizationPrototype());
	}
	else if(TypeIdentifier == "mind")
	{
		if(ClassIdentifier == "command_mind")
		{
			Result = new CommandMind();
		}
		else
		{
			throw std::runtime_error("Create: Unknown class '" + ClassIdentifier + "' (for object type '" + TypeIdentifier + "').");
		}
	}
	else if(TypeIdentifier == "planet")
	{
		Result = new Planet(ClassIdentifier);
	}
	else if(TypeIdentifier == "ship")
	{
		const ShipClass * ShipClass(g_ShipClassManager->Get(ClassIdentifier));
		
		Result = new Ship(ShipClass);
		assert(Result->GetAspectVisualization() != 0);
		Result->GetAspectVisualization()->SetVisualizationPrototype(ShipClass->GetVisualizationPrototype());
	}
	else if(TypeIdentifier == "shot")
	{
		const WeaponClass * WeaponClass(g_WeaponClassManager->Get(ClassIdentifier));
		
		Result = new Shot(WeaponClass);
	}
	else if(TypeIdentifier == "weapon")
	{
		const WeaponClass * WeaponClass(g_WeaponClassManager->Get(ClassIdentifier));
		
		Result = new Weapon(WeaponClass);
	}
	else
	{
		throw std::runtime_error("Create: Unknown object type '" + TypeIdentifier + "' (for object class '" + ClassIdentifier + "').");
	}
	Result->SetTypeIdentifier(TypeIdentifier);
	Result->SetClassIdentifier(ClassIdentifier);
	
	return Result;
}

float ObjectFactory::GetSpaceRequirement(const std::string & Type, const std::string & Class) const
{
	if(Type == "commodity")
	{
		const CommodityClass * CommodityClass(g_CommodityClassManager->Get(Class));
		
		return CommodityClass->GetSpaceRequirement();
	}
	else if(Type == "weapon")
	{
		const WeaponClass * WeaponClass(g_WeaponClassManager->Get(Class));
		
		return WeaponClass->GetSpaceRequirement();
	}
	else
	{
		throw std::runtime_error("SpaceRequirement: Unknown object type '" + Type + "' (for object class '" + Class + "').");
	}
}
