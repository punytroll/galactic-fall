/**
 * galactic-fall
 * Copyright (C) 2007-2019  Hagen Möbius
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
#include "blueprint_manager.h"
#include "character.h"
#include "command_mind.h"
#include "commodity.h"
#include "blueprint.h"
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
#include "turret.h"
#include "weapon.h"
#include "visualization_prototype.h"

Graphics::ParticleSystem * CreateParticleSystem(const std::string & ParticleSystemSubTypeIdentifier);

Object * ObjectFactory::Create(const std::string & TypeIdentifier, const std::string & SubTypeIdentifier, bool CreateNestedObjects) const
{
	Object * Result{nullptr};
	
	if(TypeIdentifier == "ammunition")
	{
		assert(g_BlueprintManager != nullptr);
		
		auto AmmunitionBlueprint{g_BlueprintManager->Get(TypeIdentifier, SubTypeIdentifier)};
		
		assert(AmmunitionBlueprint != nullptr);
		
		auto NewAmmunition{new Ammunition{}};
		
		// set up type specific things
		NewAmmunition->SetAmount(AmmunitionBlueprint->GetValue< std::uint32_t >("cartridge-size"));
		NewAmmunition->SetCapacity(AmmunitionBlueprint->GetValue< std::uint32_t >("cartridge-size"));
		// set up aspects
		// set up name aspect
		assert(NewAmmunition->GetAspectName() != nullptr);
		NewAmmunition->GetAspectName()->SetName(AmmunitionBlueprint->GetValue< std::string >("name"));
		// set up physical aspect
		assert(NewAmmunition->GetAspectPhysical() != nullptr);
		NewAmmunition->GetAspectPhysical()->SetRadialSize(AmmunitionBlueprint->GetPropertyAsVisualizationPrototype("visualization-prototype").GetModel()->GetRadialSize());
		NewAmmunition->GetAspectPhysical()->SetSpaceRequirement(AmmunitionBlueprint->GetValue< std::uint32_t >("space-requirement"));
		Result = NewAmmunition;
	}
	else if(TypeIdentifier == "battery")
	{
		assert(g_BlueprintManager != nullptr);
		
		auto BatteryBlueprint{g_BlueprintManager->Get(TypeIdentifier, SubTypeIdentifier)};
		
		assert(BatteryBlueprint != nullptr);
		
		auto NewBattery{new Battery{}};
		
		// set up type specific things
		NewBattery->SetEnergyCapacity(BatteryBlueprint->GetValue< float >("energy-capacity"));
		NewBattery->SetEnergy(BatteryBlueprint->GetValue< float >("energy-capacity"));
		NewBattery->SetMaximumPowerInput(BatteryBlueprint->GetValue< float >("maximum-power-input"));
		NewBattery->SetMaximumPowerOutput(BatteryBlueprint->GetValue< float >("maximum-power-output"));
		// set up aspects
		// set up accessory aspect
		assert(NewBattery->GetAspectAccessory() != nullptr);
		NewBattery->GetAspectAccessory()->SetSlotClassIdentifier(BatteryBlueprint->GetValue< std::string >("slot-class-identifier"));
		// set up name aspect
		assert(NewBattery->GetAspectName() != nullptr);
		NewBattery->GetAspectName()->SetName(BatteryBlueprint->GetValue< std::string >("name"));
		// set up physical aspect
		assert(NewBattery->GetAspectPhysical() != nullptr);
		NewBattery->GetAspectPhysical()->SetSpaceRequirement(BatteryBlueprint->GetValue< std::uint32_t >("space-requirement"));
		Result = NewBattery;
	}
	else if(TypeIdentifier == "character")
	{
		assert(SubTypeIdentifier.empty() == true);
		Result = new Character{};
	}
	else if(TypeIdentifier == "commodity")
	{
		assert(g_BlueprintManager != nullptr);
		
		auto CommodityBlueprint{g_BlueprintManager->Get(TypeIdentifier, SubTypeIdentifier)};
		
		assert(CommodityBlueprint != nullptr);
		
		auto NewCommodity{new Commodity{}};
		
		// set up name aspect
		assert(NewCommodity->GetAspectName() != nullptr);
		NewCommodity->GetAspectName()->SetName(CommodityBlueprint->GetValue< std::string >("name"));
		// set up physical aspect
		assert(NewCommodity->GetAspectPhysical() != nullptr);
		NewCommodity->GetAspectPhysical()->SetRadialSize(CommodityBlueprint->GetPropertyAsVisualizationPrototype("visualization-prototype").GetModel()->GetRadialSize());
		NewCommodity->GetAspectPhysical()->SetSpaceRequirement(CommodityBlueprint->GetValue< std::uint32_t >("space-requirement"));
		// set up visualization aspect
		assert(NewCommodity->GetAspectVisualization() != nullptr);
		NewCommodity->GetAspectVisualization()->SetVisualizationPrototype(CommodityBlueprint->GetPropertyAsVisualizationPrototype("visualization-prototype"));
		Result = NewCommodity;
	}
	else if(TypeIdentifier == "faction")
	{
		assert(SubTypeIdentifier.empty() == false);
		Result = new Faction{};
	}
	else if(TypeIdentifier == "galaxy")
	{
		assert(SubTypeIdentifier.empty() == false);
		Result = new Galaxy{};
	}
	else if(TypeIdentifier == "generator")
	{
		assert(g_BlueprintManager != nullptr);
		
		auto GeneratorBlueprint{g_BlueprintManager->Get(TypeIdentifier, SubTypeIdentifier)};
		
		assert(GeneratorBlueprint != nullptr);
		
		auto NewGenerator{new Generator{}};
		
		// set up type specific things
		NewGenerator->SetMaximumPowerOutput(GeneratorBlueprint->GetValue< float >("maximum-power-output"));
		// set up aspects
		// set up accessory aspect
		assert(NewGenerator->GetAspectAccessory() != nullptr);
		NewGenerator->GetAspectAccessory()->SetSlotClassIdentifier(GeneratorBlueprint->GetValue< std::string >("slot-class-identifier"));
		// set up name aspect
		assert(NewGenerator->GetAspectName() != nullptr);
		NewGenerator->GetAspectName()->SetName(GeneratorBlueprint->GetValue< std::string >("name"));
		// set up physical aspect
		assert(NewGenerator->GetAspectPhysical() != nullptr);
		NewGenerator->GetAspectPhysical()->SetSpaceRequirement(GeneratorBlueprint->GetValue< std::uint32_t >("space-requirement"));
		Result = NewGenerator;
	}
	else if(TypeIdentifier == "hangar")
	{
		assert(SubTypeIdentifier.empty() == true);
		Result = new Hangar{};
	}
	else if(TypeIdentifier == "mind")
	{
		if(SubTypeIdentifier == "command_mind")
		{
			Result = new CommandMind{};
		}
		else
		{
			throw std::runtime_error("ObjectFactory::Create(): Unknown blueprint '" + TypeIdentifier + "'::'" + SubTypeIdentifier + ".");
		}
	}
	else if(TypeIdentifier == "planet")
	{
		assert(SubTypeIdentifier.empty() == false);
		Result = new Planet{};
	}
	else if(TypeIdentifier == "ship")
	{
		assert(g_BlueprintManager != nullptr);
		
		auto ShipBlueprint{g_BlueprintManager->Get(TypeIdentifier, SubTypeIdentifier)};
		
		assert(ShipBlueprint != nullptr);
		
		auto NewShip{new Ship{}};
		
		// set up type specific things
		NewShip->SetFuelCapacity(ShipBlueprint->GetValue< float >("fuel-capacity"));
		NewShip->SetFuelNeededToAccelerate(ShipBlueprint->GetValue< float >("forward-fuel"));
		NewShip->SetFuelNeededToJump(ShipBlueprint->GetValue< float >("jump-fuel"));
		NewShip->SetFuelNeededToTurn(ShipBlueprint->GetValue< float >("turn-fuel"));
		NewShip->SetHullCapacity(ShipBlueprint->GetValue< float >("hull"));
		NewShip->SetHull(ShipBlueprint->GetValue< float >("hull"));
		NewShip->SetMaximumForwardThrust(ShipBlueprint->GetValue< float >("forward-thrust"));
		NewShip->SetMaximumSpeed(ShipBlueprint->GetValue< float >("maximum-speed"));
		NewShip->SetMaximumTurnSpeed(ShipBlueprint->GetValue< float >("turn-speed"));
		assert(g_Galaxy != nullptr);
		NewShip->SetFaction(g_Galaxy->GetFaction("neutral"));
		
		auto & VisualizationPrototype{ShipBlueprint->GetPropertyAsVisualizationPrototype("visualization-prototype")};
		auto & ExhaustMarkerPartIdentifier{ShipBlueprint->GetValue< std::string >("exhaust-marker-part-identifier")};
		auto & ExhaustMarkerIdentifier{ShipBlueprint->GetValue< std::string >("exhaust-marker-identifier")};
		auto ExhaustRadius{VisualizationPrototype.GetMarkerLength(ExhaustMarkerPartIdentifier, ExhaustMarkerIdentifier)};
		
		assert(ExhaustRadius != nullptr);
		NewShip->SetExhaustRadius(*ExhaustRadius);
		
		auto ExhaustPosition{VisualizationPrototype.GetMarkerPosition(ExhaustMarkerPartIdentifier, ExhaustMarkerIdentifier)};
		
		assert(ExhaustPosition != nullptr);
		NewShip->SetExhaustPosition(*ExhaustPosition);
		
		auto ExhaustOrientation{VisualizationPrototype.GetMarkerOrientation(ExhaustMarkerPartIdentifier, ExhaustMarkerIdentifier)};
		
		assert(ExhaustOrientation != nullptr);
		NewShip->SetExhaustOrientation(*ExhaustOrientation);
		
		auto EngineGlowParticleSystem(CreateParticleSystem("engine_glow"));
		
		EngineGlowParticleSystem->SetPosition(*ExhaustPosition);
		NewShip->SetEngineGlowParticleSystem(EngineGlowParticleSystem);
		// set up aspects
		// set up name aspect
		assert(NewShip->GetAspectName() != nullptr);
		NewShip->GetAspectName()->SetName(ShipBlueprint->GetValue< std::string >("name"));
		// set up outfitting aspect
		assert(NewShip->GetAspectOutfitting() != nullptr);
		for(auto SlotProperties : ShipBlueprint->GetPropertyAsList("slots"))
		{
			auto NewSlot{new Slot{SlotProperties.GetPropertyAsSlotClass("class"), SlotProperties.GetValue< std::string >("identifier")}};
			
			NewSlot->SetName(SlotProperties.GetValue< std::string >("name"));
			NewSlot->SetPosition(SlotProperties.GetValue< Vector3f >("position"));
			NewSlot->SetOrientation(SlotProperties.GetValue< Quaternion >("orientation"));
			NewSlot->SetVisualizeAccessory(SlotProperties.GetValue< bool >("visualize-accessory"));
			NewShip->GetAspectOutfitting()->AddSlot(NewSlot);
		}
		// set up physical aspect
		assert(NewShip->GetAspectPhysical() != nullptr);
		NewShip->GetAspectPhysical()->SetRadialSize(VisualizationPrototype.GetModel()->GetRadialSize());
		NewShip->GetAspectPhysical()->SetSpaceRequirement(ShipBlueprint->GetValue< std::uint32_t >("space-requirement"));
		// set up visualization aspect
		assert(NewShip->GetAspectVisualization() != nullptr);
		NewShip->GetAspectVisualization()->SetVisualizationPrototype(VisualizationPrototype);
		
		auto & PartStyles{NewShip->GetAspectVisualization()->GetVisualizationPrototype()->GetPartStyles()};
		
		if(PartStyles.find("faction") != PartStyles.end())
		{
			PartStyles["faction"]->SetDiffuseColor(g_Galaxy->GetFaction("neutral")->GetColor());
		}
		// set up storage if required
		if(CreateNestedObjects == true)
		{
			auto NewStorage{dynamic_cast< Storage * >(Create("storage", "", CreateNestedObjects))};
			
			NewStorage->SetSpaceCapacity(ShipBlueprint->GetValue< std::uint32_t >("maximum-available-space"));
			assert(NewShip->GetAspectObjectContainer() != nullptr);
			NewShip->GetAspectObjectContainer()->AddContent(NewStorage);
		}
		Result = NewShip;
	}
	else if(TypeIdentifier == "shot")
	{
		assert(SubTypeIdentifier.empty() == true);
		Result = new Shot{};
	}
	else if(TypeIdentifier == "star")
	{
		assert(SubTypeIdentifier.empty() == false);
		Result = new Star{};
	}
	else if(TypeIdentifier == "storage")
	{
		assert(SubTypeIdentifier.empty() == true);
		Result = new Storage{};
	}
	else if(TypeIdentifier == "system")
	{
		assert(SubTypeIdentifier.empty() == false);
		Result = new System{};
	}
	else if(TypeIdentifier == "turret")
	{
		assert(g_BlueprintManager != nullptr);
		
		auto TurretBlueprint{g_BlueprintManager->Get(TypeIdentifier, SubTypeIdentifier)};
		
		assert(TurretBlueprint != nullptr);
		
		auto NewTurret{new Turret{}};
		
		NewTurret->SetEnergyUsagePerShot(TurretBlueprint->GetValue< float >("energy-usage-per-shot"));
		NewTurret->SetMuzzlePosition(TurretBlueprint->GetValue< Vector3f >("muzzle-position"));
		NewTurret->SetShotDamage(TurretBlueprint->GetValue< float >("shot-damage"));
		NewTurret->SetShotExitSpeed(TurretBlueprint->GetValue< float >("shot-exit-speed"));
		NewTurret->SetShotLifeTime(TurretBlueprint->GetValue< float >("shot-life-time"));
		NewTurret->SetShotVisualizationPrototype(TurretBlueprint->GetPropertyAsVisualizationPrototype("shot-visualization-prototype"));
		NewTurret->SetMaximumPowerInput(TurretBlueprint->GetValue< float >("maximum-power-input"));
		// set up accessory aspect
		assert(NewTurret->GetAspectAccessory() != nullptr);
		NewTurret->GetAspectAccessory()->SetSlotClassIdentifier(TurretBlueprint->GetValue< std::string >("slot-class-identifier"));
		// set up name aspect
		assert(NewTurret->GetAspectName() != nullptr);
		NewTurret->GetAspectName()->SetName(TurretBlueprint->GetValue< std::string >("name"));
		// set up physical aspect
		assert(NewTurret->GetAspectPhysical() != nullptr);
		NewTurret->GetAspectPhysical()->SetSpaceRequirement(TurretBlueprint->GetValue< std::uint32_t >("space-requirement"));
		// set up position aspect
		assert(NewTurret->GetAspectPosition() != nullptr);
		NewTurret->GetAspectPosition()->SetOrientation(TurretBlueprint->GetValue< Quaternion >("orientation"));
		// set up visualization aspect
		assert(NewTurret->GetAspectVisualization() != nullptr);
		NewTurret->GetAspectVisualization()->SetVisualizationPrototype(TurretBlueprint->GetPropertyAsVisualizationPrototype("visualization-prototype"));
		Result = NewTurret;
	}
	else if(TypeIdentifier == "weapon")
	{
		assert(g_BlueprintManager != nullptr);
		
		auto WeaponBlueprint{g_BlueprintManager->Get(TypeIdentifier, SubTypeIdentifier)};
		
		assert(WeaponBlueprint != nullptr);
		
		auto NewWeapon{new Weapon{}};
		
		NewWeapon->SetEnergyUsagePerShot(WeaponBlueprint->GetValue< float >("energy-usage-per-shot"));
		NewWeapon->SetMuzzlePosition(WeaponBlueprint->GetValue< Vector3f >("muzzle-position"));
		NewWeapon->SetShotDamage(WeaponBlueprint->GetValue< float >("shot-damage"));
		NewWeapon->SetShotExitSpeed(WeaponBlueprint->GetValue< float >("shot-exit-speed"));
		NewWeapon->SetShotLifeTime(WeaponBlueprint->GetValue< float >("shot-life-time"));
		NewWeapon->SetShotVisualizationPrototype(WeaponBlueprint->GetPropertyAsVisualizationPrototype("shot-visualization-prototype"));
		NewWeapon->SetMaximumPowerInput(WeaponBlueprint->GetValue< float >("maximum-power-input"));
		// set up accessory aspect
		assert(NewWeapon->GetAspectAccessory() != nullptr);
		NewWeapon->GetAspectAccessory()->SetSlotClassIdentifier(WeaponBlueprint->GetValue< std::string >("slot-class-identifier"));
		// set up name aspect
		assert(NewWeapon->GetAspectName() != nullptr);
		NewWeapon->GetAspectName()->SetName(WeaponBlueprint->GetValue< std::string >("name"));
		// set up physical aspect
		assert(NewWeapon->GetAspectPhysical() != nullptr);
		NewWeapon->GetAspectPhysical()->SetSpaceRequirement(WeaponBlueprint->GetValue< std::uint32_t >("space-requirement"));
		// set up position aspect
		assert(NewWeapon->GetAspectPosition() != nullptr);
		NewWeapon->GetAspectPosition()->SetOrientation(WeaponBlueprint->GetValue< Quaternion >("orientation"));
		// set up visualization aspect
		assert(NewWeapon->GetAspectVisualization() != nullptr);
		NewWeapon->GetAspectVisualization()->SetVisualizationPrototype(WeaponBlueprint->GetPropertyAsVisualizationPrototype("visualization-prototype"));
		Result = NewWeapon;
	}
	else
	{
		throw std::runtime_error("ObjectFactory::Create(): Unknown blueprint '" + TypeIdentifier + "'::'" + SubTypeIdentifier + ".");
	}
	Result->SetTypeIdentifier(TypeIdentifier);
	Result->SetSubTypeIdentifier(SubTypeIdentifier);
	
	return Result;
}
