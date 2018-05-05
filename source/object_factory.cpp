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
#include "battery.h"
#include "character.h"
#include "class_manager.h"
#include "command_mind.h"
#include "commodity.h"
#include "faction.h"
#include "galaxy.h"
#include "game_time.h"
#include "generator.h"
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
#include "shot.h"
#include "slot.h"
#include "star.h"
#include "storage.h"
#include "system.h"
#include "template.h"
#include "turret.h"
#include "weapon.h"
#include "visualization_prototype.h"

Graphics::ParticleSystem * CreateParticleSystem(const std::string & ParticleSystemSubTypeIdentifier);

ObjectFactory::ObjectFactory(void) :
	_AmmunitionClassManager(new ClassManager< Template >()),
	_BatteryClassManager(new ClassManager< Template >()),
	_CommodityClassManager(new ClassManager< Template >()),
	_GeneratorClassManager(new ClassManager< Template >()),
	_ShipClassManager(new ClassManager< Template >()),
	_TurretClassManager(new ClassManager< Template >()),
	_WeaponClassManager(new ClassManager< Template >())
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
		NewAmmunition->SetAmount(AmmunitionClass->GetPropertyAsUnsignedInteger32Bit("cartridge-size"));
		NewAmmunition->SetCapacity(AmmunitionClass->GetPropertyAsUnsignedInteger32Bit("cartridge-size"));
		// set up aspects
		// set up name aspect
		assert(NewAmmunition->GetAspectName() != nullptr);
		NewAmmunition->GetAspectName()->SetName(AmmunitionClass->GetPropertyAsString("name"));
		// set up physical aspect
		assert(NewAmmunition->GetAspectPhysical() != nullptr);
		NewAmmunition->GetAspectPhysical()->SetRadialSize(AmmunitionClass->GetPropertyAsVisualizationPrototype("visualization-prototype").GetModel()->GetRadialSize());
		NewAmmunition->GetAspectPhysical()->SetSpaceRequirement(AmmunitionClass->GetPropertyAsUnsignedInteger32Bit("space-requirement"));
		Result = NewAmmunition;
	}
	else if(TypeIdentifier == "battery")
	{
		assert(_BatteryClassManager != nullptr);
		
		auto BatteryClass(_BatteryClassManager->Get(SubTypeIdentifier));
		
		assert(BatteryClass != nullptr);
		
		auto NewBattery(new Battery());
		
		// set up type specific things
		NewBattery->SetEnergy(BatteryClass->GetPropertyAsFloat("energy-capacity"));
		NewBattery->SetEnergyCapacity(BatteryClass->GetPropertyAsFloat("energy-capacity"));
		// set up aspects
		// set up accessory aspect
		assert(NewBattery->GetAspectAccessory() != nullptr);
		NewBattery->GetAspectAccessory()->SetSlotClassIdentifier(BatteryClass->GetPropertyAsString("slot-class-identifier"));
		// set up name aspect
		assert(NewBattery->GetAspectName() != nullptr);
		NewBattery->GetAspectName()->SetName(BatteryClass->GetPropertyAsString("name"));
		// set up physical aspect
		assert(NewBattery->GetAspectPhysical() != nullptr);
		NewBattery->GetAspectPhysical()->SetSpaceRequirement(BatteryClass->GetPropertyAsUnsignedInteger32Bit("space-requirement"));
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
		NewCommodity->GetAspectName()->SetName(CommodityClass->GetPropertyAsString("name"));
		// set up physical aspect
		assert(NewCommodity->GetAspectPhysical() != nullptr);
		NewCommodity->GetAspectPhysical()->SetRadialSize(CommodityClass->GetPropertyAsVisualizationPrototype("visualization-prototype").GetModel()->GetRadialSize());
		NewCommodity->GetAspectPhysical()->SetSpaceRequirement(CommodityClass->GetPropertyAsUnsignedInteger32Bit("space-requirement"));
		// set up visualization aspect
		assert(NewCommodity->GetAspectVisualization() != nullptr);
		NewCommodity->GetAspectVisualization()->SetVisualizationPrototype(CommodityClass->GetPropertyAsVisualizationPrototype("visualization-prototype"));
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
		NewGenerator->SetEnergyProvisionPerSecond(GeneratorClass->GetPropertyAsFloat("energy-provision-per-second"));
		// set up aspects
		// set up accessory aspect
		assert(NewGenerator->GetAspectAccessory() != nullptr);
		NewGenerator->GetAspectAccessory()->SetSlotClassIdentifier(GeneratorClass->GetPropertyAsString("slot-class-identifier"));
		// set up name aspect
		assert(NewGenerator->GetAspectName() != nullptr);
		NewGenerator->GetAspectName()->SetName(GeneratorClass->GetPropertyAsString("name"));
		// set up physical aspect
		assert(NewGenerator->GetAspectPhysical() != nullptr);
		NewGenerator->GetAspectPhysical()->SetSpaceRequirement(GeneratorClass->GetPropertyAsUnsignedInteger32Bit("space-requirement"));
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
		NewShip->SetExhaustOffset(ShipClass->GetPropertyAsVector3f("exhaust-offset"));
		NewShip->SetExhaustRadius(ShipClass->GetPropertyAsFloat("exhaust-radius"));
		NewShip->SetFuelCapacity(ShipClass->GetPropertyAsFloat("fuel-capacity"));
		NewShip->SetFuelNeededToAccelerate(ShipClass->GetPropertyAsFloat("forward-fuel"));
		NewShip->SetFuelNeededToJump(ShipClass->GetPropertyAsFloat("jump-fuel"));
		NewShip->SetFuelNeededToTurn(ShipClass->GetPropertyAsFloat("turn-fuel"));
		NewShip->SetHullCapacity(ShipClass->GetPropertyAsFloat("hull"));
		NewShip->SetHull(ShipClass->GetPropertyAsFloat("hull"));
		NewShip->SetMaximumForwardThrust(ShipClass->GetPropertyAsFloat("forward-thrust"));
		NewShip->SetMaximumSpeed(ShipClass->GetPropertyAsFloat("maximum-speed"));
		NewShip->SetMaximumTurnSpeed(ShipClass->GetPropertyAsFloat("turn-speed"));
		assert(g_Galaxy != nullptr);
		NewShip->SetFaction(g_Galaxy->GetFaction("neutral"));
		
		auto EngineGlowParticleSystem(CreateParticleSystem("engine_glow"));
		
		EngineGlowParticleSystem->SetPosition(ShipClass->GetPropertyAsVector3f("exhaust-offset"));
		NewShip->SetEngineGlowParticleSystem(EngineGlowParticleSystem);
		// set up aspects
		// set up name aspect
		assert(NewShip->GetAspectName() != nullptr);
		NewShip->GetAspectName()->SetName(ShipClass->GetPropertyAsString("name"));
		// set up outfitting aspect
		assert(NewShip->GetAspectOutfitting() != nullptr);
		for(auto SlotProperties : ShipClass->GetPropertyAsList("slots"))
		{
			auto NewSlot(new Slot(SlotProperties.GetPropertyAsSlotClass("class"), SlotProperties.GetPropertyAsString("identifier")));
			
			NewSlot->SetName(SlotProperties.GetPropertyAsString("name"));
			NewSlot->SetPosition(SlotProperties.GetPropertyAsVector3f("position"));
			NewSlot->SetOrientation(SlotProperties.GetPropertyAsQuaternion("orientation"));
			NewSlot->SetVisualizeAccessory(SlotProperties.GetPropertyAsBoolean("visualize-accessory"));
			NewShip->GetAspectOutfitting()->AddSlot(NewSlot);
		}
		// set up physical aspect
		assert(NewShip->GetAspectPhysical() != nullptr);
		NewShip->GetAspectPhysical()->SetRadialSize(ShipClass->GetPropertyAsVisualizationPrototype("visualization-prototype").GetModel()->GetRadialSize());
		NewShip->GetAspectPhysical()->SetSpaceRequirement(ShipClass->GetPropertyAsUnsignedInteger32Bit("space-requirement"));
		// set up visualization aspect
		assert(NewShip->GetAspectVisualization() != nullptr);
		NewShip->GetAspectVisualization()->SetVisualizationPrototype(ShipClass->GetPropertyAsVisualizationPrototype("visualization-prototype"));
		
		auto & PartStyles(NewShip->GetAspectVisualization()->GetVisualizationPrototype()->GetPartStyles());
		
		if(PartStyles.find("faction") != PartStyles.end())
		{
			PartStyles["faction"]->SetDiffuseColor(g_Galaxy->GetFaction("neutral")->GetColor());
		}
		// set up storage if required
		if(CreateNestedObjects == true)
		{
			auto NewStorage(dynamic_cast< Storage * >(Create("storage", "", CreateNestedObjects)));
			
			NewStorage->SetSpaceCapacity(ShipClass->GetPropertyAsUnsignedInteger32Bit("maximum-available-space"));
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
		
		NewTurret->SetEnergyUsagePerShot(TurretClass->GetPropertyAsFloat("energy-usage-per-shot"));
		NewTurret->SetMuzzlePosition(TurretClass->GetPropertyAsVector3f("muzzle-position"));
		NewTurret->SetShotDamage(TurretClass->GetPropertyAsFloat("shot-damage"));
		NewTurret->SetShotExitSpeed(TurretClass->GetPropertyAsFloat("shot-exit-speed"));
		NewTurret->SetShotLifeTime(TurretClass->GetPropertyAsFloat("shot-life-time"));
		NewTurret->SetShotVisualizationPrototype(TurretClass->GetPropertyAsVisualizationPrototype("shot-visualization-prototype"));
		NewTurret->SetReloadTime(TurretClass->GetPropertyAsFloat("reload-time"));
		// set up accessory aspect
		assert(NewTurret->GetAspectAccessory() != nullptr);
		NewTurret->GetAspectAccessory()->SetSlotClassIdentifier(TurretClass->GetPropertyAsString("slot-class-identifier"));
		// set up name aspect
		assert(NewTurret->GetAspectName() != nullptr);
		NewTurret->GetAspectName()->SetName(TurretClass->GetPropertyAsString("name"));
		// set up physical aspect
		assert(NewTurret->GetAspectPhysical() != nullptr);
		NewTurret->GetAspectPhysical()->SetSpaceRequirement(TurretClass->GetPropertyAsUnsignedInteger32Bit("space-requirement"));
		// set up position aspect
		assert(NewTurret->GetAspectPosition() != nullptr);
		NewTurret->GetAspectPosition()->SetOrientation(TurretClass->GetPropertyAsQuaternion("orientation"));
		// set up visualization aspect
		assert(NewTurret->GetAspectVisualization() != nullptr);
		NewTurret->GetAspectVisualization()->SetVisualizationPrototype(TurretClass->GetPropertyAsVisualizationPrototype("visualization-prototype"));
		Result = NewTurret;
	}
	else if(TypeIdentifier == "weapon")
	{
		assert(_WeaponClassManager != nullptr);
		
		auto WeaponClass(_WeaponClassManager->Get(SubTypeIdentifier));
		
		assert(WeaponClass != nullptr);
		
		auto NewWeapon(new Weapon());
		
		NewWeapon->SetEnergyUsagePerShot(WeaponClass->GetPropertyAsFloat("energy-usage-per-shot"));
		NewWeapon->SetMuzzlePosition(WeaponClass->GetPropertyAsVector3f("muzzle-position"));
		NewWeapon->SetShotDamage(WeaponClass->GetPropertyAsFloat("shot-damage"));
		NewWeapon->SetShotExitSpeed(WeaponClass->GetPropertyAsFloat("shot-exit-speed"));
		NewWeapon->SetShotLifeTime(WeaponClass->GetPropertyAsFloat("shot-life-time"));
		NewWeapon->SetShotVisualizationPrototype(WeaponClass->GetPropertyAsVisualizationPrototype("shot-visualization-prototype"));
		NewWeapon->SetReloadTime(WeaponClass->GetPropertyAsFloat("reload-time"));
		// set up accessory aspect
		assert(NewWeapon->GetAspectAccessory() != nullptr);
		NewWeapon->GetAspectAccessory()->SetSlotClassIdentifier(WeaponClass->GetPropertyAsString("slot-class-identifier"));
		// set up name aspect
		assert(NewWeapon->GetAspectName() != nullptr);
		NewWeapon->GetAspectName()->SetName(WeaponClass->GetPropertyAsString("name"));
		// set up physical aspect
		assert(NewWeapon->GetAspectPhysical() != nullptr);
		NewWeapon->GetAspectPhysical()->SetSpaceRequirement(WeaponClass->GetPropertyAsUnsignedInteger32Bit("space-requirement"));
		// set up position aspect
		assert(NewWeapon->GetAspectPosition() != nullptr);
		NewWeapon->GetAspectPosition()->SetOrientation(WeaponClass->GetPropertyAsQuaternion("orientation"));
		// set up visualization aspect
		assert(NewWeapon->GetAspectVisualization() != nullptr);
		NewWeapon->GetAspectVisualization()->SetVisualizationPrototype(WeaponClass->GetPropertyAsVisualizationPrototype("visualization-prototype"));
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
		
		return AmmunitionClass->GetPropertyAsUnsignedInteger32Bit("base-price");
	}
	else if(TypeIdentifier == "battery")
	{
		assert(_BatteryClassManager != nullptr);
		
		auto BatteryClass(_BatteryClassManager->Get(SubTypeIdentifier));
		
		assert(BatteryClass != nullptr);
		
		return BatteryClass->GetPropertyAsUnsignedInteger32Bit("base-price");
	}
	else if(TypeIdentifier == "commodity")
	{
		assert(_CommodityClassManager != nullptr);
		
		auto CommodityClass(_CommodityClassManager->Get(SubTypeIdentifier));
		
		assert(CommodityClass != nullptr);
		
		return CommodityClass->GetPropertyAsUnsignedInteger32Bit("base-price");
	}
	else if(TypeIdentifier == "generator")
	{
		assert(_GeneratorClassManager != nullptr);
		
		auto GeneratorClass(_GeneratorClassManager->Get(SubTypeIdentifier));
		
		assert(GeneratorClass != nullptr);
		
		return GeneratorClass->GetPropertyAsUnsignedInteger32Bit("base-price");
	}
	else if(TypeIdentifier == "ship")
	{
		assert(_ShipClassManager != nullptr);
		
		auto ShipClass(_ShipClassManager->Get(SubTypeIdentifier));
		
		assert(ShipClass != nullptr);
		
		return ShipClass->GetPropertyAsUnsignedInteger32Bit("base-price");
	}
	else if(TypeIdentifier == "turret")
	{
		assert(_TurretClassManager != nullptr);
		
		auto TurretClass(_TurretClassManager->Get(SubTypeIdentifier));
		
		assert(TurretClass != nullptr);
		
		return TurretClass->GetPropertyAsUnsignedInteger32Bit("base-price");
	}
	else if(TypeIdentifier == "weapon")
	{
		assert(_WeaponClassManager != nullptr);
		
		auto WeaponClass(_WeaponClassManager->Get(SubTypeIdentifier));
		
		return WeaponClass->GetPropertyAsUnsignedInteger32Bit("base-price");
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
		
		return AmmunitionClass->GetPropertyAsString("description");
	}
	else if(TypeIdentifier == "battery")
	{
		assert(_BatteryClassManager != nullptr);
		
		auto BatteryClass(_BatteryClassManager->Get(SubTypeIdentifier));
		
		assert(BatteryClass != nullptr);
		
		return BatteryClass->GetPropertyAsString("description");
	}
	else if(TypeIdentifier == "commodity")
	{
		assert(_CommodityClassManager != nullptr);
		
		auto CommodityClass(_CommodityClassManager->Get(SubTypeIdentifier));
		
		assert(CommodityClass != nullptr);
		
		return CommodityClass->GetPropertyAsString("description");
	}
	else if(TypeIdentifier == "generator")
	{
		assert(_GeneratorClassManager != nullptr);
		
		auto GeneratorClass(_GeneratorClassManager->Get(SubTypeIdentifier));
		
		assert(GeneratorClass != nullptr);
		
		return GeneratorClass->GetPropertyAsString("description");
	}
	else if(TypeIdentifier == "ship")
	{
		assert(_ShipClassManager != nullptr);
		
		auto ShipClass(_ShipClassManager->Get(SubTypeIdentifier));
		
		assert(ShipClass != nullptr);
		
		return ShipClass->GetPropertyAsString("description");
	}
	else if(TypeIdentifier == "turret")
	{
		assert(_TurretClassManager != nullptr);
		
		auto TurretClass(_TurretClassManager->Get(SubTypeIdentifier));
		
		assert(TurretClass != nullptr);
		
		return TurretClass->GetPropertyAsString("description");
	}
	else if(TypeIdentifier == "weapon")
	{
		assert(_WeaponClassManager != nullptr);
		
		auto WeaponClass(_WeaponClassManager->Get(SubTypeIdentifier));
		
		return WeaponClass->GetPropertyAsString("description");
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
		
		return AmmunitionClass->GetPropertyAsString("name");
	}
	else if(TypeIdentifier == "battery")
	{
		assert(_BatteryClassManager != nullptr);
		
		auto BatteryClass(_BatteryClassManager->Get(SubTypeIdentifier));
		
		assert(BatteryClass != nullptr);
		
		return BatteryClass->GetPropertyAsString("name");
	}
	else if(TypeIdentifier == "commodity")
	{
		assert(_CommodityClassManager != nullptr);
		
		auto CommodityClass(_CommodityClassManager->Get(SubTypeIdentifier));
		
		assert(CommodityClass != nullptr);
		
		return CommodityClass->GetPropertyAsString("name");
	}
	else if(TypeIdentifier == "generator")
	{
		assert(_GeneratorClassManager != nullptr);
		
		auto GeneratorClass(_GeneratorClassManager->Get(SubTypeIdentifier));
		
		assert(GeneratorClass != nullptr);
		
		return GeneratorClass->GetPropertyAsString("name");
	}
	else if(TypeIdentifier == "ship")
	{
		assert(_ShipClassManager != nullptr);
		
		auto ShipClass(_ShipClassManager->Get(SubTypeIdentifier));
		
		assert(ShipClass != nullptr);
		
		return ShipClass->GetPropertyAsString("name");
	}
	else if(TypeIdentifier == "turret")
	{
		assert(_TurretClassManager != nullptr);
		
		auto TurretClass(_TurretClassManager->Get(SubTypeIdentifier));
		
		assert(TurretClass != nullptr);
		
		return TurretClass->GetPropertyAsString("name");
	}
	else if(TypeIdentifier == "weapon")
	{
		assert(_WeaponClassManager != nullptr);
		
		auto WeaponClass(_WeaponClassManager->Get(SubTypeIdentifier));
		
		return WeaponClass->GetPropertyAsString("name");
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
		
		return AmmunitionClass->GetPropertyAsUnsignedInteger32Bit("space-requirement");
	}
	else if(TypeIdentifier == "battery")
	{
		assert(_BatteryClassManager != nullptr);
		
		auto BatteryClass(_BatteryClassManager->Get(SubTypeIdentifier));
		
		assert(BatteryClass != nullptr);
		
		return BatteryClass->GetPropertyAsUnsignedInteger32Bit("space-requirement");
	}
	else if(TypeIdentifier == "commodity")
	{
		assert(_CommodityClassManager != nullptr);
		
		auto CommodityClass(_CommodityClassManager->Get(SubTypeIdentifier));
		
		assert(CommodityClass != nullptr);
		
		return CommodityClass->GetPropertyAsUnsignedInteger32Bit("space-requirement");
	}
	else if(TypeIdentifier == "generator")
	{
		assert(_GeneratorClassManager != nullptr);
		
		auto GeneratorClass(_GeneratorClassManager->Get(SubTypeIdentifier));
		
		assert(GeneratorClass != nullptr);
		
		return GeneratorClass->GetPropertyAsUnsignedInteger32Bit("space-requirement");
	}
	else if(TypeIdentifier == "ship")
	{
		assert(_ShipClassManager != nullptr);
		
		auto ShipClass(_ShipClassManager->Get(SubTypeIdentifier));
		
		assert(ShipClass != nullptr);
		
		return ShipClass->GetPropertyAsUnsignedInteger32Bit("space-requirement");
	}
	else if(TypeIdentifier == "turret")
	{
		assert(_TurretClassManager != nullptr);
		
		auto TurretClass(_TurretClassManager->Get(SubTypeIdentifier));
		
		assert(TurretClass != nullptr);
		
		return TurretClass->GetPropertyAsUnsignedInteger32Bit("space-requirement");
	}
	else if(TypeIdentifier == "weapon")
	{
		assert(_WeaponClassManager != nullptr);
		
		auto WeaponClass(_WeaponClassManager->Get(SubTypeIdentifier));
		
		return WeaponClass->GetPropertyAsUnsignedInteger32Bit("space-requirement");
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
		
		return &(AmmunitionClass->GetPropertyAsVisualizationPrototype("visualization-prototype"));
	}
	else if(TypeIdentifier == "battery")
	{
		assert(_BatteryClassManager != nullptr);
		
		auto BatteryClass(_BatteryClassManager->Get(SubTypeIdentifier));
		
		assert(BatteryClass != nullptr);
		
		return &(BatteryClass->GetPropertyAsVisualizationPrototype("visualization-prototype"));
	}
	else if(TypeIdentifier == "commodity")
	{
		assert(_CommodityClassManager != nullptr);
		
		auto CommodityClass(_CommodityClassManager->Get(SubTypeIdentifier));
		
		assert(CommodityClass != nullptr);
		
		return &(CommodityClass->GetPropertyAsVisualizationPrototype("visualization-prototype"));
	}
	else if(TypeIdentifier == "generator")
	{
		assert(_GeneratorClassManager != nullptr);
		
		auto GeneratorClass(_GeneratorClassManager->Get(SubTypeIdentifier));
		
		assert(GeneratorClass != nullptr);
		
		return &(GeneratorClass->GetPropertyAsVisualizationPrototype("visualization-prototype"));
	}
	else if(TypeIdentifier == "ship")
	{
		assert(_ShipClassManager != nullptr);
		
		auto ShipClass(_ShipClassManager->Get(SubTypeIdentifier));
		
		assert(ShipClass != nullptr);
		
		return &(ShipClass->GetPropertyAsVisualizationPrototype("visualization-prototype"));
	}
	else if(TypeIdentifier == "turret")
	{
		assert(_TurretClassManager != nullptr);
		
		auto TurretClass(_TurretClassManager->Get(SubTypeIdentifier));
		
		assert(TurretClass != nullptr);
		
		return &(TurretClass->GetPropertyAsVisualizationPrototype("visualization-prototype"));
	}
	else if(TypeIdentifier == "weapon")
	{
		assert(_WeaponClassManager != nullptr);
		
		auto WeaponClass(_WeaponClassManager->Get(SubTypeIdentifier));
		
		return &(WeaponClass->GetPropertyAsVisualizationPrototype("visualization-prototype"));
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
