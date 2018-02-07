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
#include "graphics/model.h"
#include "graphics/particle_system.h"
#include "graphics/style.h"
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
#include "turret.h"
#include "turret_class.h"
#include "weapon.h"
#include "weapon_class.h"
#include "visualization_prototype.h"

Graphics::ParticleSystem * CreateParticleSystem(const std::string & ParticleSystemClassIdentifier);

ObjectFactory::ObjectFactory(void) :
	_BatteryClassManager(new ClassManager< BatteryClass >()),
	_CommodityClassManager(new ClassManager< CommodityClass >()),
	_GeneratorClassManager(new ClassManager< GeneratorClass >()),
	_ShipClassManager(new ClassManager< ShipClass >()),
	_TurretClassManager(new ClassManager< TurretClass >()),
	_WeaponClassManager(new ClassManager< WeaponClass >())
{
}

ObjectFactory::~ObjectFactory(void)
{
	delete _BatteryClassManager;
	_BatteryClassManager = nullptr;
	delete _CommodityClassManager;
	_CommodityClassManager = nullptr;
	delete _GeneratorClassManager;
	_GeneratorClassManager = nullptr;
	delete _ShipClassManager;
	_ShipClassManager = nullptr;
	delete _TurretClassManager;
	_TurretClassManager = nullptr;
	delete _WeaponClassManager;
	_WeaponClassManager = nullptr;
}

Object * ObjectFactory::Create(const std::string & TypeIdentifier, const std::string & ClassIdentifier, bool CreateNestedObjects) const
{
	Object * Result(nullptr);
	
	if(TypeIdentifier == "battery")
	{
		assert(_BatteryClassManager != nullptr);
		
		auto BatteryClass(_BatteryClassManager->Get(ClassIdentifier));
		
		assert(BatteryClass != nullptr);
		
		auto NewBattery(new Battery());
		
		// set up type specific things
		NewBattery->SetEnergy(BatteryClass->GetEnergyCapacity());
		NewBattery->SetEnergyCapacity(BatteryClass->GetEnergyCapacity());
		// set up aspects
		// set up accessory aspect
		assert(NewBattery->GetAspectAccessory() != nullptr);
		NewBattery->GetAspectAccessory()->SetSlotClassIdentifier(BatteryClass->GetSlotClassIdentifier());
		// set up name aspect
		assert(NewBattery->GetAspectName() != nullptr);
		NewBattery->GetAspectName()->SetName(BatteryClass->GetName());
		// set up physical aspect
		assert(NewBattery->GetAspectPhysical() != nullptr);
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
		assert(_CommodityClassManager != nullptr);
		
		auto CommodityClass(_CommodityClassManager->Get(ClassIdentifier));
		
		assert(CommodityClass != nullptr);
		
		auto NewCommodity(new Commodity());
		
		// set up name aspect
		assert(NewCommodity->GetAspectName() != nullptr);
		NewCommodity->GetAspectName()->SetName(CommodityClass->GetName());
		// set up physical aspect
		assert(NewCommodity->GetAspectPhysical() != nullptr);
		NewCommodity->GetAspectPhysical()->SetRadialSize(CommodityClass->GetVisualizationPrototype()->GetModel()->GetRadialSize());
		NewCommodity->GetAspectPhysical()->SetSpaceRequirement(CommodityClass->GetSpaceRequirement());
		// set up visualization aspect
		assert(NewCommodity->GetAspectVisualization() != nullptr);
		NewCommodity->GetAspectVisualization()->SetVisualizationPrototype(CommodityClass->GetVisualizationPrototype());
		Result = NewCommodity;
	}
	else if(TypeIdentifier == "faction")
	{
		assert(ClassIdentifier.empty() == false);
		Result = new Faction();
	}
	else if(TypeIdentifier == "galaxy")
	{
		assert(ClassIdentifier.empty() == false);
		Result = new Galaxy();
	}
	else if(TypeIdentifier == "generator")
	{
		assert(_GeneratorClassManager != nullptr);
		
		auto GeneratorClass(_GeneratorClassManager->Get(ClassIdentifier));
		
		assert(GeneratorClass != nullptr);
		
		auto NewGenerator(new Generator());
		
		// set up type specific things
		NewGenerator->SetEnergyProvisionPerSecond(GeneratorClass->GetEnergyProvisionPerSecond());
		// set up aspects
		// set up accessory aspect
		assert(NewGenerator->GetAspectAccessory() != nullptr);
		NewGenerator->GetAspectAccessory()->SetSlotClassIdentifier(GeneratorClass->GetSlotClassIdentifier());
		// set up name aspect
		assert(NewGenerator->GetAspectName() != nullptr);
		NewGenerator->GetAspectName()->SetName(GeneratorClass->GetName());
		// set up physical aspect
		assert(NewGenerator->GetAspectPhysical() != nullptr);
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
		assert(_ShipClassManager != nullptr);
		
		auto ShipClass(_ShipClassManager->Get(ClassIdentifier));
		
		assert(ShipClass != nullptr);
		
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
		assert(g_Galaxy != nullptr);
		NewShip->SetFaction(g_Galaxy->GetFaction("neutral"));
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
		NewShip->GetAspectVisualization()->SetVisualizationPrototype(ShipClass->GetVisualizationPrototype());
		
		auto & PartStyles(NewShip->GetAspectVisualization()->GetVisualizationPrototype()->GetPartStyles());
		
		if(PartStyles.find("faction") != PartStyles.end())
		{
			PartStyles["faction"]->SetDiffuseColor(g_Galaxy->GetFaction("neutral")->GetColor());
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
		assert(ClassIdentifier.empty() == true);
		Result = new Shot();
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
	else if(TypeIdentifier == "turret")
	{
		assert(_TurretClassManager != nullptr);
		
		auto TurretClass(_TurretClassManager->Get(ClassIdentifier));
		
		assert(TurretClass != nullptr);
		
		auto NewTurret(new Turret());
		
		NewTurret->SetEnergyUsagePerShot(TurretClass->GetEnergyUsagePerShot());
		NewTurret->SetShotDamage(TurretClass->GetShotDamage());
		NewTurret->SetShotExitPosition(TurretClass->GetShotExitPosition());
		NewTurret->SetShotExitSpeed(TurretClass->GetShotExitSpeed());
		NewTurret->SetShotLifeTime(TurretClass->GetShotLifeTime());
		NewTurret->SetShotVisualizationPrototype(TurretClass->GetShotVisualizationPrototype());
		NewTurret->SetReloadTime(TurretClass->GetReloadTime());
		// set up accessory aspect
		assert(NewTurret->GetAspectAccessory() != nullptr);
		NewTurret->GetAspectAccessory()->SetSlotClassIdentifier(TurretClass->GetSlotClassIdentifier());
		// set up name aspect
		assert(NewTurret->GetAspectName() != nullptr);
		NewTurret->GetAspectName()->SetName(TurretClass->GetName());
		// set up physical aspect
		assert(NewTurret->GetAspectPhysical() != nullptr);
		NewTurret->GetAspectPhysical()->SetSpaceRequirement(TurretClass->GetSpaceRequirement());
		// set up position aspect
		assert(NewTurret->GetAspectPosition() != nullptr);
		NewTurret->GetAspectPosition()->SetOrientation(TurretClass->GetOrientation());
		// set up visualization aspect
		assert(NewTurret->GetAspectVisualization() != nullptr);
		NewTurret->GetAspectVisualization()->SetVisualizationPrototype(new VisualizationPrototype(TurretClass->GetTurretVisualizationPrototype()));
		Result = NewTurret;
	}
	else if(TypeIdentifier == "weapon")
	{
		assert(_WeaponClassManager != nullptr);
		
		auto WeaponClass(_WeaponClassManager->Get(ClassIdentifier));
		
		assert(WeaponClass != nullptr);
		
		auto NewWeapon(new Weapon());
		
		NewWeapon->SetEnergyUsagePerShot(WeaponClass->GetEnergyUsagePerShot());
		NewWeapon->SetShotDamage(WeaponClass->GetShotDamage());
		NewWeapon->SetShotExitPosition(WeaponClass->GetShotExitPosition());
		NewWeapon->SetShotExitSpeed(WeaponClass->GetShotExitSpeed());
		NewWeapon->SetShotLifeTime(WeaponClass->GetShotLifeTime());
		NewWeapon->SetShotVisualizationPrototype(WeaponClass->GetShotVisualizationPrototype());
		NewWeapon->SetReloadTime(WeaponClass->GetReloadTime());
		// set up accessory aspect
		assert(NewWeapon->GetAspectAccessory() != nullptr);
		NewWeapon->GetAspectAccessory()->SetSlotClassIdentifier(WeaponClass->GetSlotClassIdentifier());
		// set up name aspect
		assert(NewWeapon->GetAspectName() != nullptr);
		NewWeapon->GetAspectName()->SetName(WeaponClass->GetName());
		// set up physical aspect
		assert(NewWeapon->GetAspectPhysical() != nullptr);
		NewWeapon->GetAspectPhysical()->SetSpaceRequirement(WeaponClass->GetSpaceRequirement());
		// set up position aspect
		assert(NewWeapon->GetAspectPosition() != nullptr);
		NewWeapon->GetAspectPosition()->SetOrientation(WeaponClass->GetOrientation());
		// set up visualization aspect
		assert(NewWeapon->GetAspectVisualization() != nullptr);
		NewWeapon->GetAspectVisualization()->SetVisualizationPrototype(WeaponClass->GetWeaponVisualizationPrototype());
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
		assert(_BatteryClassManager != nullptr);
		
		auto BatteryClass(_BatteryClassManager->Get(ClassIdentifier));
		
		assert(BatteryClass != nullptr);
		
		return BatteryClass->GetSpaceRequirement();
	}
	else if(TypeIdentifier == "commodity")
	{
		assert(_CommodityClassManager != nullptr);
		
		auto CommodityClass(_CommodityClassManager->Get(ClassIdentifier));
		
		assert(CommodityClass != nullptr);
		
		return CommodityClass->GetSpaceRequirement();
	}
	else if(TypeIdentifier == "generator")
	{
		assert(_GeneratorClassManager != nullptr);
		
		auto GeneratorClass(_GeneratorClassManager->Get(ClassIdentifier));
		
		assert(GeneratorClass != nullptr);
		
		return GeneratorClass->GetSpaceRequirement();
	}
	else if(TypeIdentifier == "ship")
	{
		assert(_ShipClassManager != nullptr);
		
		auto ShipClass(_ShipClassManager->Get(ClassIdentifier));
		
		assert(ShipClass != nullptr);
		
		return ShipClass->GetSpaceRequirement();
	}
	else if(TypeIdentifier == "turret")
	{
		assert(_TurretClassManager != nullptr);
		
		auto TurretClass(_TurretClassManager->Get(ClassIdentifier));
		
		assert(TurretClass != nullptr);
		
		return TurretClass->GetSpaceRequirement();
	}
	else if(TypeIdentifier == "weapon")
	{
		assert(_WeaponClassManager != nullptr);
		
		auto WeaponClass(_WeaponClassManager->Get(ClassIdentifier));
		
		return WeaponClass->GetSpaceRequirement();
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
		assert(_BatteryClassManager != nullptr);
		
		auto BatteryClass(_BatteryClassManager->Get(ClassIdentifier));
		
		assert(BatteryClass != nullptr);
		
		return BatteryClass->GetVisualizationPrototype();
	}
	else if(TypeIdentifier == "commodity")
	{
		assert(_CommodityClassManager != nullptr);
		
		auto CommodityClass(_CommodityClassManager->Get(ClassIdentifier));
		
		assert(CommodityClass != nullptr);
		
		return CommodityClass->GetVisualizationPrototype();
	}
	else if(TypeIdentifier == "generator")
	{
		return nullptr;
	}
	else if(TypeIdentifier == "ship")
	{
		assert(_ShipClassManager != nullptr);
		
		auto ShipClass(_ShipClassManager->Get(ClassIdentifier));
		
		assert(ShipClass != nullptr);
		
		return ShipClass->GetVisualizationPrototype();
	}
	else if(TypeIdentifier == "turret")
	{
		assert(_TurretClassManager != nullptr);
		
		auto TurretClass(_TurretClassManager->Get(ClassIdentifier));
		
		assert(TurretClass != nullptr);
		
		return TurretClass->GetTurretVisualizationPrototype();
	}
	else if(TypeIdentifier == "weapon")
	{
		assert(_WeaponClassManager != nullptr);
		
		auto WeaponClass(_WeaponClassManager->Get(ClassIdentifier));
		
		return WeaponClass->GetWeaponVisualizationPrototype();
	}
	else
	{
		throw std::runtime_error("ObjectFactory::GetVisualizationPrototype(): Unknown object type '" + TypeIdentifier + "' (for object class '" + ClassIdentifier + "').");
	}
}
