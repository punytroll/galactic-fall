/**
 * galactic-fall
 * Copyright (C) 2007-2018  Hagen Möbius
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

#include "ammunition.h"
#include "ammunition_class.h"
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

Graphics::ParticleSystem * CreateParticleSystem(const std::string & ParticleSystemSubTypeIdentifier);

ObjectFactory::ObjectFactory(void) :
	_AmmunitionClassManager(new ClassManager< AmmunitionClass >()),
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
	delete _AmmunitionClassManager;
	_AmmunitionClassManager = nullptr;
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

Object * ObjectFactory::Create(const std::string & TypeIdentifier, const std::string & SubTypeIdentifier, bool CreateNestedObjects) const
{
	Object * Result(nullptr);
	
	if(TypeIdentifier == "ammunition")
	{
		assert(_AmmunitionClassManager != nullptr);
		
		auto AmmunitionClass(_AmmunitionClassManager->Get(SubTypeIdentifier));
		
		assert(AmmunitionClass != nullptr);
		
		auto NewAmmunition{new Ammunition{}};
		
		// set up type specific things
		NewAmmunition->SetAmount(AmmunitionClass->GetFieldAsUnsignedInteger32Bit("cartridge-size"));
		NewAmmunition->SetCapacity(AmmunitionClass->GetFieldAsUnsignedInteger32Bit("cartridge-size"));
		// set up aspects
		// set up name aspect
		assert(NewAmmunition->GetAspectName() != nullptr);
		NewAmmunition->GetAspectName()->SetName(AmmunitionClass->GetFieldAsString("name"));
		// set up physical aspect
		assert(NewAmmunition->GetAspectPhysical() != nullptr);
		NewAmmunition->GetAspectPhysical()->SetRadialSize(AmmunitionClass->GetVisualizationPrototype()->GetModel()->GetRadialSize());
		NewAmmunition->GetAspectPhysical()->SetSpaceRequirement(AmmunitionClass->GetFieldAsUnsignedInteger32Bit("space-requirement"));
		Result = NewAmmunition;
	}
	else if(TypeIdentifier == "battery")
	{
		assert(_BatteryClassManager != nullptr);
		
		auto BatteryClass(_BatteryClassManager->Get(SubTypeIdentifier));
		
		assert(BatteryClass != nullptr);
		
		auto NewBattery(new Battery());
		
		// set up type specific things
		NewBattery->SetEnergy(BatteryClass->GetFieldAsFloat("energy-capacity"));
		NewBattery->SetEnergyCapacity(BatteryClass->GetFieldAsFloat("energy-capacity"));
		// set up aspects
		// set up accessory aspect
		assert(NewBattery->GetAspectAccessory() != nullptr);
		NewBattery->GetAspectAccessory()->SetSlotClassIdentifier(BatteryClass->GetFieldAsString("slot-class-identifier"));
		// set up name aspect
		assert(NewBattery->GetAspectName() != nullptr);
		NewBattery->GetAspectName()->SetName(BatteryClass->GetFieldAsString("name"));
		// set up physical aspect
		assert(NewBattery->GetAspectPhysical() != nullptr);
		NewBattery->GetAspectPhysical()->SetSpaceRequirement(BatteryClass->GetFieldAsUnsignedInteger32Bit("space-requirement"));
		Result = NewBattery;
	}
	else if(TypeIdentifier == "character")
	{
		assert(SubTypeIdentifier.empty() == true);
		Result = new Character();
	}
	else if(TypeIdentifier == "commodity")
	{
		assert(_CommodityClassManager != nullptr);
		
		auto CommodityClass(_CommodityClassManager->Get(SubTypeIdentifier));
		
		assert(CommodityClass != nullptr);
		
		auto NewCommodity(new Commodity());
		
		// set up name aspect
		assert(NewCommodity->GetAspectName() != nullptr);
		NewCommodity->GetAspectName()->SetName(CommodityClass->GetFieldAsString("name"));
		// set up physical aspect
		assert(NewCommodity->GetAspectPhysical() != nullptr);
		NewCommodity->GetAspectPhysical()->SetRadialSize(CommodityClass->GetVisualizationPrototype()->GetModel()->GetRadialSize());
		NewCommodity->GetAspectPhysical()->SetSpaceRequirement(CommodityClass->GetFieldAsUnsignedInteger32Bit("space-requirement"));
		// set up visualization aspect
		assert(NewCommodity->GetAspectVisualization() != nullptr);
		NewCommodity->GetAspectVisualization()->SetVisualizationPrototype(CommodityClass->GetVisualizationPrototype());
		Result = NewCommodity;
	}
	else if(TypeIdentifier == "faction")
	{
		assert(SubTypeIdentifier.empty() == false);
		Result = new Faction();
	}
	else if(TypeIdentifier == "galaxy")
	{
		assert(SubTypeIdentifier.empty() == false);
		Result = new Galaxy();
	}
	else if(TypeIdentifier == "generator")
	{
		assert(_GeneratorClassManager != nullptr);
		
		auto GeneratorClass(_GeneratorClassManager->Get(SubTypeIdentifier));
		
		assert(GeneratorClass != nullptr);
		
		auto NewGenerator(new Generator());
		
		// set up type specific things
		NewGenerator->SetEnergyProvisionPerSecond(GeneratorClass->GetFieldAsFloat("energy-provision-per-second"));
		// set up aspects
		// set up accessory aspect
		assert(NewGenerator->GetAspectAccessory() != nullptr);
		NewGenerator->GetAspectAccessory()->SetSlotClassIdentifier(GeneratorClass->GetFieldAsString("slot-class-identifier"));
		// set up name aspect
		assert(NewGenerator->GetAspectName() != nullptr);
		NewGenerator->GetAspectName()->SetName(GeneratorClass->GetFieldAsString("name"));
		// set up physical aspect
		assert(NewGenerator->GetAspectPhysical() != nullptr);
		NewGenerator->GetAspectPhysical()->SetSpaceRequirement(GeneratorClass->GetFieldAsUnsignedInteger32Bit("space-requirement"));
		Result = NewGenerator;
	}
	else if(TypeIdentifier == "hangar")
	{
		assert(SubTypeIdentifier.empty() == true);
		Result = new Hangar();
	}
	else if(TypeIdentifier == "mind")
	{
		if(SubTypeIdentifier == "command_mind")
		{
			Result = new CommandMind();
		}
		else
		{
			throw std::runtime_error("ObjectFactory::Create(): Unknown class '" + SubTypeIdentifier + "' (for object type '" + TypeIdentifier + "').");
		}
	}
	else if(TypeIdentifier == "planet")
	{
		assert(SubTypeIdentifier.empty() == false);
		Result = new Planet();
	}
	else if(TypeIdentifier == "ship")
	{
		assert(_ShipClassManager != nullptr);
		
		auto ShipClass(_ShipClassManager->Get(SubTypeIdentifier));
		
		assert(ShipClass != nullptr);
		
		auto NewShip(new Ship());
		
		// set up type specific things
		NewShip->SetExhaustOffset(ShipClass->GetFieldAsVector3f("exhaust-offset"));
		NewShip->SetExhaustRadius(ShipClass->GetFieldAsFloat("exhaust-radius"));
		NewShip->SetFuelCapacity(ShipClass->GetFieldAsFloat("fuel-capacity"));
		NewShip->SetFuelNeededToAccelerate(ShipClass->GetFieldAsFloat("forward-fuel"));
		NewShip->SetFuelNeededToJump(ShipClass->GetFieldAsFloat("jump-fuel"));
		NewShip->SetFuelNeededToTurn(ShipClass->GetFieldAsFloat("turn-fuel"));
		NewShip->SetHullCapacity(ShipClass->GetFieldAsFloat("hull"));
		NewShip->SetHull(ShipClass->GetFieldAsFloat("hull"));
		NewShip->SetMaximumForwardThrust(ShipClass->GetFieldAsFloat("forward-thrust"));
		NewShip->SetMaximumSpeed(ShipClass->GetFieldAsFloat("maximum-speed"));
		NewShip->SetMaximumTurnSpeed(ShipClass->GetFieldAsFloat("turn-speed"));
		assert(g_Galaxy != nullptr);
		NewShip->SetFaction(g_Galaxy->GetFaction("neutral"));
		
		auto EngineGlowParticleSystem(CreateParticleSystem("engine_glow"));
		
		EngineGlowParticleSystem->SetPosition(ShipClass->GetFieldAsVector3f("exhaust-offset"));
		NewShip->SetEngineGlowParticleSystem(EngineGlowParticleSystem);
		// set up aspects
		// set up name aspect
		assert(NewShip->GetAspectName() != nullptr);
		NewShip->GetAspectName()->SetName(ShipClass->GetFieldAsString("name"));
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
		NewShip->GetAspectPhysical()->SetSpaceRequirement(ShipClass->GetFieldAsUnsignedInteger32Bit("space-requirement"));
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
			
			NewStorage->SetSpaceCapacity(ShipClass->GetFieldAsUnsignedInteger32Bit("maximum-available-space"));
			assert(NewShip->GetAspectObjectContainer() != nullptr);
			NewShip->GetAspectObjectContainer()->AddContent(NewStorage);
		}
		Result = NewShip;
	}
	else if(TypeIdentifier == "shot")
	{
		assert(SubTypeIdentifier.empty() == true);
		Result = new Shot();
	}
	else if(TypeIdentifier == "star")
	{
		assert(SubTypeIdentifier.empty() == false);
		Result = new Star();
	}
	else if(TypeIdentifier == "storage")
	{
		assert(SubTypeIdentifier.empty() == true);
		Result = new Storage();
	}
	else if(TypeIdentifier == "system")
	{
		assert(SubTypeIdentifier.empty() == false);
		Result = new System();
	}
	else if(TypeIdentifier == "turret")
	{
		assert(_TurretClassManager != nullptr);
		
		auto TurretClass(_TurretClassManager->Get(SubTypeIdentifier));
		
		assert(TurretClass != nullptr);
		
		auto NewTurret(new Turret());
		
		NewTurret->SetEnergyUsagePerShot(TurretClass->GetFieldAsFloat("energy-usage-per-shot"));
		NewTurret->SetMuzzlePosition(TurretClass->GetMuzzlePosition());
		NewTurret->SetShotDamage(TurretClass->GetFieldAsFloat("shot-damage"));
		NewTurret->SetShotExitSpeed(TurretClass->GetFieldAsFloat("shot-exit-speed"));
		NewTurret->SetShotLifeTime(TurretClass->GetFieldAsFloat("shot-life-time"));
		NewTurret->SetShotVisualizationPrototype(TurretClass->GetShotVisualizationPrototype());
		NewTurret->SetReloadTime(TurretClass->GetFieldAsFloat("reload-time"));
		// set up accessory aspect
		assert(NewTurret->GetAspectAccessory() != nullptr);
		NewTurret->GetAspectAccessory()->SetSlotClassIdentifier(TurretClass->GetFieldAsString("slot-class-identifier"));
		// set up name aspect
		assert(NewTurret->GetAspectName() != nullptr);
		NewTurret->GetAspectName()->SetName(TurretClass->GetFieldAsString("name"));
		// set up physical aspect
		assert(NewTurret->GetAspectPhysical() != nullptr);
		NewTurret->GetAspectPhysical()->SetSpaceRequirement(TurretClass->GetFieldAsUnsignedInteger32Bit("space-requirement"));
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
		
		auto WeaponClass(_WeaponClassManager->Get(SubTypeIdentifier));
		
		assert(WeaponClass != nullptr);
		
		auto NewWeapon(new Weapon());
		
		NewWeapon->SetEnergyUsagePerShot(WeaponClass->GetFieldAsFloat("energy-usage-per-shot"));
		NewWeapon->SetMuzzlePosition(WeaponClass->GetMuzzlePosition());
		NewWeapon->SetShotDamage(WeaponClass->GetFieldAsFloat("shot-damage"));
		NewWeapon->SetShotExitSpeed(WeaponClass->GetFieldAsFloat("shot-exit-speed"));
		NewWeapon->SetShotLifeTime(WeaponClass->GetFieldAsFloat("shot-life-time"));
		NewWeapon->SetShotVisualizationPrototype(WeaponClass->GetShotVisualizationPrototype());
		NewWeapon->SetReloadTime(WeaponClass->GetFieldAsFloat("reload-time"));
		// set up accessory aspect
		assert(NewWeapon->GetAspectAccessory() != nullptr);
		NewWeapon->GetAspectAccessory()->SetSlotClassIdentifier(WeaponClass->GetFieldAsString("slot-class-identifier"));
		// set up name aspect
		assert(NewWeapon->GetAspectName() != nullptr);
		NewWeapon->GetAspectName()->SetName(WeaponClass->GetFieldAsString("name"));
		// set up physical aspect
		assert(NewWeapon->GetAspectPhysical() != nullptr);
		NewWeapon->GetAspectPhysical()->SetSpaceRequirement(WeaponClass->GetFieldAsUnsignedInteger32Bit("space-requirement"));
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
		throw std::runtime_error("ObjectFactory::Create(): Unknown object type '" + TypeIdentifier + "', sub type '" + SubTypeIdentifier + "'.");
	}
	Result->SetTypeIdentifier(TypeIdentifier);
	Result->SetSubTypeIdentifier(SubTypeIdentifier);
	
	return Result;
}

std::uint32_t ObjectFactory::GetBasePrice(const std::string & TypeIdentifier, const std::string & SubTypeIdentifier) const
{
	if(TypeIdentifier == "ammunition")
	{
		assert(_AmmunitionClassManager != nullptr);
		
		auto AmmunitionClass{_AmmunitionClassManager->Get(SubTypeIdentifier)};
		
		assert(AmmunitionClass != nullptr);
		
		return AmmunitionClass->GetFieldAsUnsignedInteger32Bit("base-price");
	}
	else if(TypeIdentifier == "battery")
	{
		assert(_BatteryClassManager != nullptr);
		
		auto BatteryClass(_BatteryClassManager->Get(SubTypeIdentifier));
		
		assert(BatteryClass != nullptr);
		
		return BatteryClass->GetFieldAsUnsignedInteger32Bit("base-price");
	}
	else if(TypeIdentifier == "commodity")
	{
		assert(_CommodityClassManager != nullptr);
		
		auto CommodityClass(_CommodityClassManager->Get(SubTypeIdentifier));
		
		assert(CommodityClass != nullptr);
		
		return CommodityClass->GetFieldAsUnsignedInteger32Bit("base-price");
	}
	else if(TypeIdentifier == "generator")
	{
		assert(_GeneratorClassManager != nullptr);
		
		auto GeneratorClass(_GeneratorClassManager->Get(SubTypeIdentifier));
		
		assert(GeneratorClass != nullptr);
		
		return GeneratorClass->GetFieldAsUnsignedInteger32Bit("base-price");
	}
	else if(TypeIdentifier == "ship")
	{
		assert(_ShipClassManager != nullptr);
		
		auto ShipClass(_ShipClassManager->Get(SubTypeIdentifier));
		
		assert(ShipClass != nullptr);
		
		return ShipClass->GetFieldAsUnsignedInteger32Bit("base-price");
	}
	else if(TypeIdentifier == "turret")
	{
		assert(_TurretClassManager != nullptr);
		
		auto TurretClass(_TurretClassManager->Get(SubTypeIdentifier));
		
		assert(TurretClass != nullptr);
		
		return TurretClass->GetFieldAsUnsignedInteger32Bit("base-price");
	}
	else if(TypeIdentifier == "weapon")
	{
		assert(_WeaponClassManager != nullptr);
		
		auto WeaponClass(_WeaponClassManager->Get(SubTypeIdentifier));
		
		return WeaponClass->GetFieldAsUnsignedInteger32Bit("base-price");
	}
	else
	{
		throw std::runtime_error("ObjectFactory::GetBasePrice(): Unknown object type '" + TypeIdentifier + "', sub type '" + SubTypeIdentifier + "'.");
	}
}

const std::string & ObjectFactory::GetDescription(const std::string & TypeIdentifier, const std::string & SubTypeIdentifier) const
{
	if(TypeIdentifier == "ammunition")
	{
		assert(_AmmunitionClassManager != nullptr);
		
		auto AmmunitionClass{_AmmunitionClassManager->Get(SubTypeIdentifier)};
		
		assert(AmmunitionClass != nullptr);
		
		return AmmunitionClass->GetFieldAsString("description");
	}
	else if(TypeIdentifier == "battery")
	{
		assert(_BatteryClassManager != nullptr);
		
		auto BatteryClass(_BatteryClassManager->Get(SubTypeIdentifier));
		
		assert(BatteryClass != nullptr);
		
		return BatteryClass->GetFieldAsString("description");
	}
	else if(TypeIdentifier == "commodity")
	{
		assert(_CommodityClassManager != nullptr);
		
		auto CommodityClass(_CommodityClassManager->Get(SubTypeIdentifier));
		
		assert(CommodityClass != nullptr);
		
		return CommodityClass->GetFieldAsString("description");
	}
	else if(TypeIdentifier == "generator")
	{
		assert(_GeneratorClassManager != nullptr);
		
		auto GeneratorClass(_GeneratorClassManager->Get(SubTypeIdentifier));
		
		assert(GeneratorClass != nullptr);
		
		return GeneratorClass->GetFieldAsString("description");
	}
	else if(TypeIdentifier == "ship")
	{
		assert(_ShipClassManager != nullptr);
		
		auto ShipClass(_ShipClassManager->Get(SubTypeIdentifier));
		
		assert(ShipClass != nullptr);
		
		return ShipClass->GetFieldAsString("description");
	}
	else if(TypeIdentifier == "turret")
	{
		assert(_TurretClassManager != nullptr);
		
		auto TurretClass(_TurretClassManager->Get(SubTypeIdentifier));
		
		assert(TurretClass != nullptr);
		
		return TurretClass->GetFieldAsString("description");
	}
	else if(TypeIdentifier == "weapon")
	{
		assert(_WeaponClassManager != nullptr);
		
		auto WeaponClass(_WeaponClassManager->Get(SubTypeIdentifier));
		
		return WeaponClass->GetFieldAsString("description");
	}
	else
	{
		throw std::runtime_error("ObjectFactory::GetDescription(): Unknown object type '" + TypeIdentifier + "', sub type '" + SubTypeIdentifier + "'.");
	}
}

const std::string & ObjectFactory::GetName(const std::string & TypeIdentifier, const std::string & SubTypeIdentifier) const
{
	if(TypeIdentifier == "ammunition")
	{
		assert(_AmmunitionClassManager != nullptr);
		
		auto AmmunitionClass{_AmmunitionClassManager->Get(SubTypeIdentifier)};
		
		assert(AmmunitionClass != nullptr);
		
		return AmmunitionClass->GetFieldAsString("name");
	}
	else if(TypeIdentifier == "battery")
	{
		assert(_BatteryClassManager != nullptr);
		
		auto BatteryClass(_BatteryClassManager->Get(SubTypeIdentifier));
		
		assert(BatteryClass != nullptr);
		
		return BatteryClass->GetFieldAsString("name");
	}
	else if(TypeIdentifier == "commodity")
	{
		assert(_CommodityClassManager != nullptr);
		
		auto CommodityClass(_CommodityClassManager->Get(SubTypeIdentifier));
		
		assert(CommodityClass != nullptr);
		
		return CommodityClass->GetFieldAsString("name");
	}
	else if(TypeIdentifier == "generator")
	{
		assert(_GeneratorClassManager != nullptr);
		
		auto GeneratorClass(_GeneratorClassManager->Get(SubTypeIdentifier));
		
		assert(GeneratorClass != nullptr);
		
		return GeneratorClass->GetFieldAsString("name");
	}
	else if(TypeIdentifier == "ship")
	{
		assert(_ShipClassManager != nullptr);
		
		auto ShipClass(_ShipClassManager->Get(SubTypeIdentifier));
		
		assert(ShipClass != nullptr);
		
		return ShipClass->GetFieldAsString("name");
	}
	else if(TypeIdentifier == "turret")
	{
		assert(_TurretClassManager != nullptr);
		
		auto TurretClass(_TurretClassManager->Get(SubTypeIdentifier));
		
		assert(TurretClass != nullptr);
		
		return TurretClass->GetFieldAsString("name");
	}
	else if(TypeIdentifier == "weapon")
	{
		assert(_WeaponClassManager != nullptr);
		
		auto WeaponClass(_WeaponClassManager->Get(SubTypeIdentifier));
		
		return WeaponClass->GetFieldAsString("name");
	}
	else
	{
		throw std::runtime_error("ObjectFactory::GetName(): Unknown object type '" + TypeIdentifier + "', sub type '" + SubTypeIdentifier + "'.");
	}
}

std::uint32_t ObjectFactory::GetSpaceRequirement(const std::string & TypeIdentifier, const std::string & SubTypeIdentifier) const
{
	if(TypeIdentifier == "ammunition")
	{
		assert(_AmmunitionClassManager != nullptr);
		
		auto AmmunitionClass{_AmmunitionClassManager->Get(SubTypeIdentifier)};
		
		assert(AmmunitionClass != nullptr);
		
		return AmmunitionClass->GetFieldAsUnsignedInteger32Bit("space-requirement");
	}
	else if(TypeIdentifier == "battery")
	{
		assert(_BatteryClassManager != nullptr);
		
		auto BatteryClass(_BatteryClassManager->Get(SubTypeIdentifier));
		
		assert(BatteryClass != nullptr);
		
		return BatteryClass->GetFieldAsUnsignedInteger32Bit("space-requirement");
	}
	else if(TypeIdentifier == "commodity")
	{
		assert(_CommodityClassManager != nullptr);
		
		auto CommodityClass(_CommodityClassManager->Get(SubTypeIdentifier));
		
		assert(CommodityClass != nullptr);
		
		return CommodityClass->GetFieldAsUnsignedInteger32Bit("space-requirement");
	}
	else if(TypeIdentifier == "generator")
	{
		assert(_GeneratorClassManager != nullptr);
		
		auto GeneratorClass(_GeneratorClassManager->Get(SubTypeIdentifier));
		
		assert(GeneratorClass != nullptr);
		
		return GeneratorClass->GetFieldAsUnsignedInteger32Bit("space-requirement");
	}
	else if(TypeIdentifier == "ship")
	{
		assert(_ShipClassManager != nullptr);
		
		auto ShipClass(_ShipClassManager->Get(SubTypeIdentifier));
		
		assert(ShipClass != nullptr);
		
		return ShipClass->GetFieldAsUnsignedInteger32Bit("space-requirement");
	}
	else if(TypeIdentifier == "turret")
	{
		assert(_TurretClassManager != nullptr);
		
		auto TurretClass(_TurretClassManager->Get(SubTypeIdentifier));
		
		assert(TurretClass != nullptr);
		
		return TurretClass->GetFieldAsUnsignedInteger32Bit("space-requirement");
	}
	else if(TypeIdentifier == "weapon")
	{
		assert(_WeaponClassManager != nullptr);
		
		auto WeaponClass(_WeaponClassManager->Get(SubTypeIdentifier));
		
		return WeaponClass->GetFieldAsUnsignedInteger32Bit("space-requirement");
	}
	else
	{
		throw std::runtime_error("ObjectFactory::GetSpaceRequirement(): Unknown object type '" + TypeIdentifier + "', sub type '" + SubTypeIdentifier + "'.");
	}
}

const VisualizationPrototype * ObjectFactory::GetVisualizationPrototype(const std::string & TypeIdentifier, const std::string & SubTypeIdentifier) const
{
	if(TypeIdentifier == "ammunition")
	{
		assert(_AmmunitionClassManager != nullptr);
		
		auto AmmunitionClass{_AmmunitionClassManager->Get(SubTypeIdentifier)};
		
		assert(AmmunitionClass != nullptr);
		
		return AmmunitionClass->GetVisualizationPrototype();
	}
	else if(TypeIdentifier == "battery")
	{
		assert(_BatteryClassManager != nullptr);
		
		auto BatteryClass(_BatteryClassManager->Get(SubTypeIdentifier));
		
		assert(BatteryClass != nullptr);
		
		return BatteryClass->GetVisualizationPrototype();
	}
	else if(TypeIdentifier == "commodity")
	{
		assert(_CommodityClassManager != nullptr);
		
		auto CommodityClass(_CommodityClassManager->Get(SubTypeIdentifier));
		
		assert(CommodityClass != nullptr);
		
		return CommodityClass->GetVisualizationPrototype();
	}
	else if(TypeIdentifier == "generator")
	{
		assert(_GeneratorClassManager != nullptr);
		
		auto GeneratorClass(_GeneratorClassManager->Get(SubTypeIdentifier));
		
		assert(GeneratorClass != nullptr);
		
		return GeneratorClass->GetVisualizationPrototype();
	}
	else if(TypeIdentifier == "ship")
	{
		assert(_ShipClassManager != nullptr);
		
		auto ShipClass(_ShipClassManager->Get(SubTypeIdentifier));
		
		assert(ShipClass != nullptr);
		
		return ShipClass->GetVisualizationPrototype();
	}
	else if(TypeIdentifier == "turret")
	{
		assert(_TurretClassManager != nullptr);
		
		auto TurretClass(_TurretClassManager->Get(SubTypeIdentifier));
		
		assert(TurretClass != nullptr);
		
		return TurretClass->GetTurretVisualizationPrototype();
	}
	else if(TypeIdentifier == "weapon")
	{
		assert(_WeaponClassManager != nullptr);
		
		auto WeaponClass(_WeaponClassManager->Get(SubTypeIdentifier));
		
		return WeaponClass->GetWeaponVisualizationPrototype();
	}
	else
	{
		throw std::runtime_error("ObjectFactory::GetVisualizationPrototype(): Unknown object type '" + TypeIdentifier + "', sub type '" + SubTypeIdentifier + "'.");
	}
}

bool ObjectFactory::HasClass(const std::string & TypeIdentifier, const std::string & SubTypeIdentifier) const
{
	if(TypeIdentifier == "ammunition")
	{
		assert(_AmmunitionClassManager != nullptr);
		
		return _AmmunitionClassManager->Get(SubTypeIdentifier) != nullptr;
	}
	else if(TypeIdentifier == "battery")
	{
		assert(_BatteryClassManager != nullptr);
		
		return _BatteryClassManager->Get(SubTypeIdentifier) != nullptr;
	}
	else if(TypeIdentifier == "commodity")
	{
		assert(_CommodityClassManager != nullptr);
		
		return _CommodityClassManager->Get(SubTypeIdentifier) != nullptr;
	}
	else if(TypeIdentifier == "generator")
	{
		assert(_GeneratorClassManager != nullptr);
		
		return _GeneratorClassManager->Get(SubTypeIdentifier) != nullptr;
	}
	else if(TypeIdentifier == "ship")
	{
		assert(_ShipClassManager != nullptr);
		
		return _ShipClassManager->Get(SubTypeIdentifier) != nullptr;
	}
	else if(TypeIdentifier == "turret")
	{
		assert(_TurretClassManager != nullptr);
		
		return _TurretClassManager->Get(SubTypeIdentifier) != nullptr;
	}
	else if(TypeIdentifier == "weapon")
	{
		assert(_WeaponClassManager != nullptr);
		
		return _WeaponClassManager->Get(SubTypeIdentifier) != nullptr;
	}
	else
	{
		throw std::runtime_error("ObjectFactory::HasClass(): Unknown object type '" + TypeIdentifier + "', sub type '" + SubTypeIdentifier + "'.");
	}
}
