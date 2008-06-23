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

#include "battery.h"
#include "character.h"
#include "class_manager.h"
#include "command_mind.h"
#include "commodity.h"
#include "commodity_class.h"
#include "globals.h"
#include "graphics_model.h"
#include "object_aspect_accessory.h"
#include "object_aspect_name.h"
#include "object_aspect_outfitting.h"
#include "object_aspect_physical.h"
#include "object_aspect_visualization.h"
#include "object_factory.h"
#include "planet.h"
#include "ship.h"
#include "ship_class.h"
#include "shot.h"
#include "slot.h"
#include "storage.h"
#include "weapon.h"
#include "weapon_class.h"
#include "visualization_prototype.h"

Object * ObjectFactory::Create(const std::string & TypeIdentifier, const std::string & ClassIdentifier) const
{
	Object * Result(0);
	
	if(TypeIdentifier == "battery")
	{
		assert(ClassIdentifier == "light_battery");
		
		Battery * NewBattery(new Battery());
		
		// set up type specific things
		NewBattery->SetEnergy(200.0f);
		NewBattery->SetEnergyCapacity(200.0f);
		// set up aspects
		// set up accessory aspect
		assert(NewBattery->GetAspectAccessory() != 0);
		NewBattery->GetAspectAccessory()->SetSlotClassIdentifier("light_battery");
		// set up name aspect
		assert(NewBattery->GetAspectName() != 0);
		NewBattery->GetAspectName()->SetName("Light Battery");
		// set up physical aspect
		assert(NewBattery->GetAspectPhysical() != 0);
		NewBattery->GetAspectPhysical()->SetSpaceRequirement(80);
		Result = NewBattery;
	}
	else if(TypeIdentifier == "character")
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
		Ship * NewShip(new Ship());
		
		// set up type specific things
		NewShip->SetExhaustOffset(ShipClass->GetExhaustOffset());
		NewShip->SetExhaustRadius(ShipClass->GetExhaustRadius());
		NewShip->SetFuelCapacity(ShipClass->GetFuelCapacity());
		NewShip->SetFuelNeededToAccelerate(ShipClass->GetForwardFuel());
		NewShip->SetFuelNeededToJump(ShipClass->GetJumpFuel());
		NewShip->SetFuelNeededToTurn(ShipClass->GetTurnFuel());
		NewShip->SetHullCapacity(ShipClass->GetHull());
		NewShip->SetMaximumForwardThrust(ShipClass->GetForwardThrust());
		NewShip->SetMaximumSpeed(ShipClass->GetMaximumSpeed());
		NewShip->SetMaximumTurnSpeed(ShipClass->GetTurnSpeed());
		// set up aspects
		// set up outfitting aspect
		assert(NewShip->GetAspectOutfitting() != 0);
		
		const std::map< std::string, Slot * > & ShipClassSlots(ShipClass->GetSlots());
		
		for(std::map< std::string, Slot * >::const_iterator SlotIterator = ShipClassSlots.begin(); SlotIterator != ShipClassSlots.end(); ++SlotIterator)
		{
			Slot * NewSlot(new Slot(SlotIterator->second->GetSlotClass(), SlotIterator->first));
			
			NewSlot->SetName(SlotIterator->second->GetName());
			NewSlot->SetPosition(SlotIterator->second->GetPosition());
			NewSlot->SetOrientation(SlotIterator->second->GetOrientation());
			NewShip->GetAspectOutfitting()->AddSlot(NewSlot);
		}
		// set up physical aspect
		assert(NewShip->GetAspectPhysical() != 0);
		NewShip->GetAspectPhysical()->SetRadialSize(ShipClass->GetVisualizationPrototype()->GetModel()->GetRadialSize());
		NewShip->GetAspectPhysical()->SetSpaceRequirement(ShipClass->GetSpaceRequirement());
		// set up visualization aspect
		assert(NewShip->GetAspectVisualization() != 0);
		NewShip->GetAspectVisualization()->SetVisualizationPrototype(ShipClass->GetVisualizationPrototype());
		Result = NewShip;
	}
	else if(TypeIdentifier == "shot")
	{
		const WeaponClass * WeaponClass(g_WeaponClassManager->Get(ClassIdentifier));
		Shot * NewShot(new Shot(WeaponClass));
		
		// set up visualization aspect
		assert(NewShot->GetAspectVisualization() != 0);
		NewShot->GetAspectVisualization()->SetVisualizationPrototype(WeaponClass->GetParticleVisualizationPrototype());
		Result = NewShot;
	}
	else if(TypeIdentifier == "storage")
	{
		assert(ClassIdentifier == "");
		Result = new Storage();
	}
	else if(TypeIdentifier == "weapon")
	{
		const WeaponClass * WeaponClass(g_WeaponClassManager->Get(ClassIdentifier));
		Weapon * NewWeapon(new Weapon(WeaponClass));
		
		NewWeapon->SetEnergyUsagePerShot(WeaponClass->GetEnergyUsagePerShot());
		NewWeapon->SetParticleExitPosition(WeaponClass->GetParticleExitPosition());
		NewWeapon->SetParticleExitSpeed(WeaponClass->GetParticleExitSpeed());
		NewWeapon->SetReloadTime(WeaponClass->GetReloadTime());
		// set up visualization aspect
		assert(NewWeapon->GetAspectVisualization() != 0);
		NewWeapon->GetAspectVisualization()->SetVisualizationPrototype(WeaponClass->GetVisualizationPrototype());
		Result = NewWeapon;
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
