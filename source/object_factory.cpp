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
#include "battery_class.h"
#include "character.h"
#include "class_manager.h"
#include "command_mind.h"
#include "commodity.h"
#include "commodity_class.h"
#include "faction.h"
#include "galaxy.h"
#include "game_time.h"
#include "generator.h"
#include "generator_class.h"
#include "globals.h"
#include "graphics/material.h"
#include "graphics/model.h"
#include "graphics/particle_system.h"
#include "hangar.h"
#include "object_aspect_accessory.h"
#include "object_aspect_name.h"
#include "object_aspect_object_container.h"
#include "object_aspect_outfitting.h"
#include "object_aspect_physical.h"
#include "object_aspect_position.h"
#include "object_aspect_visualization.h"
#include "object_factory.h"
#include "planet.h"
#include "ship.h"
#include "ship_class.h"
#include "shot.h"
#include "slot.h"
#include "star.h"
#include "storage.h"
#include "system.h"
#include "weapon.h"
#include "weapon_class.h"
#include "visualization_prototype.h"

Graphics::ParticleSystem * CreateParticleSystem(const std::string & ParticleSystemClassIdentifier);

Object * ObjectFactory::Create(const std::string & TypeIdentifier, const std::string & ClassIdentifier, bool CreateNestedObjects) const
{
	Object * Result(0);
	
	if(TypeIdentifier == "battery")
	{
		const BatteryClass * BatteryClass(g_BatteryClassManager->Get(ClassIdentifier));
		Battery * NewBattery(new Battery());
		
		// set up type specific things
		NewBattery->SetEnergy(BatteryClass->GetEnergyCapacity());
		NewBattery->SetEnergyCapacity(BatteryClass->GetEnergyCapacity());
		// set up aspects
		// set up accessory aspect
		assert(NewBattery->GetAspectAccessory() != 0);
		NewBattery->GetAspectAccessory()->SetSlotClassIdentifier(BatteryClass->GetSlotClassIdentifier());
		// set up name aspect
		assert(NewBattery->GetAspectName() != 0);
		NewBattery->GetAspectName()->SetName(BatteryClass->GetName());
		// set up physical aspect
		assert(NewBattery->GetAspectPhysical() != 0);
		NewBattery->GetAspectPhysical()->SetSpaceRequirement(BatteryClass->GetSpaceRequirement());
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
		Result->GetAspectVisualization()->SetVisualizationPrototype(new VisualizationPrototype(CommodityClass->GetVisualizationPrototype()));
	}
	else if(TypeIdentifier == "faction")
	{
		assert(ClassIdentifier.empty() == false);
		Result = new Faction();
	}
	else if(TypeIdentifier == "generator")
	{
		const GeneratorClass * GeneratorClass(g_GeneratorClassManager->Get(ClassIdentifier));
		Generator * NewGenerator(new Generator());
		
		// set up type specific things
		NewGenerator->SetEnergyProvisionPerSecond(GeneratorClass->GetEnergyProvisionPerSecond());
		// set up aspects
		// set up accessory aspect
		assert(NewGenerator->GetAspectAccessory() != 0);
		NewGenerator->GetAspectAccessory()->SetSlotClassIdentifier(GeneratorClass->GetSlotClassIdentifier());
		// set up name aspect
		assert(NewGenerator->GetAspectName() != 0);
		NewGenerator->GetAspectName()->SetName(GeneratorClass->GetName());
		// set up physical aspect
		assert(NewGenerator->GetAspectPhysical() != 0);
		NewGenerator->GetAspectPhysical()->SetSpaceRequirement(GeneratorClass->GetSpaceRequirement());
		Result = NewGenerator;
	}
	else if(TypeIdentifier == "hangar")
	{
		assert(ClassIdentifier.empty() == true);
		Result = new Hangar();
	}
	else if(TypeIdentifier == "mind")
	{
		if(ClassIdentifier == "command_mind")
		{
			Result = new CommandMind();
		}
		else
		{
			throw std::runtime_error("ObjectFactory::Create(): Unknown class '" + ClassIdentifier + "' (for object type '" + TypeIdentifier + "').");
		}
	}
	else if(TypeIdentifier == "planet")
	{
		assert(ClassIdentifier.empty() == false);
		Result = new Planet();
	}
	else if(TypeIdentifier == "ship")
	{
		auto ShipClass(g_ShipClassManager->Get(ClassIdentifier));
		auto NewShip(new Ship());
		
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
		NewShip->SetFaction(g_Galaxy->GetFaction("neutral")->GetReference());
		NewShip->SetHull(ShipClass->GetHull());
		
		auto EngineGlowParticleSystem(CreateParticleSystem("engine_glow"));
		
		EngineGlowParticleSystem->SetPosition(Vector3f(ShipClass->GetExhaustOffset()));
		NewShip->SetEngineGlowParticleSystem(EngineGlowParticleSystem);
		// set up aspects
		// set up name aspect
		assert(NewShip->GetAspectName() != nullptr);
		NewShip->GetAspectName()->SetName(ShipClass->GetName());
		// set up outfitting aspect
		assert(NewShip->GetAspectOutfitting() != nullptr);
		for(auto SlotPair : ShipClass->GetSlots())
		{
			auto NewSlot(new Slot(SlotPair.second->GetSlotClass(), SlotPair.first));
			
			NewSlot->SetName(SlotPair.second->GetName());
			NewSlot->SetPosition(SlotPair.second->GetPosition());
			NewSlot->SetOrientation(SlotPair.second->GetOrientation());
			NewSlot->SetVisualizeAccessory(SlotPair.second->GetVisualizeAccessory());
			NewShip->GetAspectOutfitting()->AddSlot(NewSlot);
		}
		// set up physical aspect
		assert(NewShip->GetAspectPhysical() != nullptr);
		NewShip->GetAspectPhysical()->SetRadialSize(ShipClass->GetVisualizationPrototype()->GetModel()->GetRadialSize());
		NewShip->GetAspectPhysical()->SetSpaceRequirement(ShipClass->GetSpaceRequirement());
		// set up visualization aspect
		assert(NewShip->GetAspectVisualization() != nullptr);
		NewShip->GetAspectVisualization()->SetVisualizationPrototype(new VisualizationPrototype(ShipClass->GetVisualizationPrototype()));
		
		std::map< std::string, Graphics::Material * > & PartMaterials(NewShip->GetAspectVisualization()->GetVisualizationPrototype()->GetPartMaterials());
		
		if(PartMaterials.find("faction") != PartMaterials.end())
		{
			PartMaterials["faction"]->SetDiffuseColor(g_Galaxy->GetFaction("neutral")->GetColor());
		}
		// set up storage if required
		if(CreateNestedObjects == true)
		{
			auto NewStorage(dynamic_cast< Storage * >(Create("storage", "", CreateNestedObjects)));
			
			NewStorage->SetSpaceCapacity(ShipClass->GetMaximumAvailableSpace());
			assert(NewShip->GetAspectObjectContainer() != nullptr);
			NewShip->GetAspectObjectContainer()->AddContent(NewStorage);
		}
		Result = NewShip;
	}
	else if(TypeIdentifier == "shot")
	{
		const WeaponClass * WeaponClass(g_WeaponClassManager->Get(ClassIdentifier));
		Shot * NewShot(new Shot());
		
		// set up type specific things
		NewShot->SetDamage(WeaponClass->GetParticleDamage());
		NewShot->SetTimeOfDeath(GameTime::Get() + WeaponClass->GetParticleLifeTime());
		// set up physical aspect
		assert(NewShot->GetAspectPhysical() != 0);
		NewShot->GetAspectPhysical()->SetRadialSize(0.54f);
		// set up visualization aspect
		assert(NewShot->GetAspectVisualization() != 0);
		NewShot->GetAspectVisualization()->SetVisualizationPrototype(new VisualizationPrototype(WeaponClass->GetParticleVisualizationPrototype()));
		Result = NewShot;
	}
	else if(TypeIdentifier == "star")
	{
		assert(ClassIdentifier.empty() == false);
		Result = new Star();
	}
	else if(TypeIdentifier == "storage")
	{
		assert(ClassIdentifier.empty() == true);
		Result = new Storage();
	}
	else if(TypeIdentifier == "system")
	{
		assert(ClassIdentifier.empty() == false);
		Result = new System();
	}
	else if(TypeIdentifier == "weapon")
	{
		const WeaponClass * WeaponClass(g_WeaponClassManager->Get(ClassIdentifier));
		Weapon * NewWeapon(new Weapon());
		
		NewWeapon->SetEnergyUsagePerShot(WeaponClass->GetEnergyUsagePerShot());
		NewWeapon->SetParticleExitPosition(WeaponClass->GetParticleExitPosition());
		NewWeapon->SetParticleExitSpeed(WeaponClass->GetParticleExitSpeed());
		NewWeapon->SetReloadTime(WeaponClass->GetReloadTime());
		NewWeapon->SetShotClassIdentifier(WeaponClass->GetIdentifier());
		// set up accessory aspect
		assert(NewWeapon->GetAspectAccessory() != 0);
		NewWeapon->GetAspectAccessory()->SetSlotClassIdentifier(WeaponClass->GetSlotClassIdentifier());
		// set up name aspect
		assert(NewWeapon->GetAspectName() != 0);
		NewWeapon->GetAspectName()->SetName(WeaponClass->GetName());
		// set up physical aspect
		assert(NewWeapon->GetAspectPhysical() != 0);
		NewWeapon->GetAspectPhysical()->SetSpaceRequirement(WeaponClass->GetSpaceRequirement());
		// set up position aspect
		assert(NewWeapon->GetAspectPosition() != 0);
		NewWeapon->GetAspectPosition()->SetOrientation(WeaponClass->GetOrientation());
		// set up visualization aspect
		assert(NewWeapon->GetAspectVisualization() != 0);
		NewWeapon->GetAspectVisualization()->SetVisualizationPrototype(new VisualizationPrototype(WeaponClass->GetVisualizationPrototype()));
		Result = NewWeapon;
	}
	else
	{
		throw std::runtime_error("ObjectFactory::Create(): Unknown object type '" + TypeIdentifier + "' (for object class '" + ClassIdentifier + "').");
	}
	Result->SetTypeIdentifier(TypeIdentifier);
	Result->SetClassIdentifier(ClassIdentifier);
	
	return Result;
}

unsigned_numeric ObjectFactory::GetSpaceRequirement(const std::string & TypeIdentifier, const std::string & ClassIdentifier) const
{
	if(TypeIdentifier == "battery")
	{
		return g_BatteryClassManager->Get(ClassIdentifier)->GetSpaceRequirement();
	}
	else if(TypeIdentifier == "commodity")
	{
		return g_CommodityClassManager->Get(ClassIdentifier)->GetSpaceRequirement();
	}
	else if(TypeIdentifier == "generator")
	{
		return g_GeneratorClassManager->Get(ClassIdentifier)->GetSpaceRequirement();
	}
	else if(TypeIdentifier == "ship")
	{
		return g_ShipClassManager->Get(ClassIdentifier)->GetSpaceRequirement();
	}
	else if(TypeIdentifier == "weapon")
	{
		return g_WeaponClassManager->Get(ClassIdentifier)->GetSpaceRequirement();
	}
	else
	{
		throw std::runtime_error("ObjectFactory::GetSpaceRequirement(): Unknown object type '" + TypeIdentifier + "' (for object class '" + ClassIdentifier + "').");
	}
}

const VisualizationPrototype * ObjectFactory::GetVisualizationPrototype(const std::string & TypeIdentifier, const std::string & ClassIdentifier) const
{
	if(TypeIdentifier == "battery")
	{
		return g_BatteryClassManager->Get(ClassIdentifier)->GetVisualizationPrototype();
	}
	else if(TypeIdentifier == "commodity")
	{
		return g_CommodityClassManager->Get(ClassIdentifier)->GetVisualizationPrototype();
	}
	else if(TypeIdentifier == "generator")
	{
		return 0;
	}
	else if(TypeIdentifier == "ship")
	{
		return g_ShipClassManager->Get(ClassIdentifier)->GetVisualizationPrototype();
	}
	else if(TypeIdentifier == "weapon")
	{
		return g_WeaponClassManager->Get(ClassIdentifier)->GetVisualizationPrototype();
	}
	else
	{
		throw std::runtime_error("ObjectFactory::GetVisualizationPrototype(): Unknown object type '" + TypeIdentifier + "' (for object class '" + ClassIdentifier + "').");
	}
}
