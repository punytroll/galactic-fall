/**
 * galactic-fall
 * Copyright (C) 2015-2015  Hagen Möbius
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

#include <arpa/inet.h>

#include <functional>
#include <iostream>

#include <arxx/archive.h>
#include <arxx/buffer_reader.h>
#include <arxx/item.h>

#include <graphics/engine.h>
#include <graphics/mesh.h>
#include <graphics/mesh_manager.h>
#include <graphics/model.h>
#include <graphics/model_manager.h>
#include <graphics/program.h>
#include <graphics/shader.h>
#include <graphics/shading_manager.h>
#include <graphics/style.h>
#include <graphics/texture.h>
#include <graphics/texture_manager.h>

#include <string_cast/string_cast.h>

#include "arx_types.h"
#include "blueprint.h"
#include "blueprint_manager.h"
#include "buffer_reading.h"
#include "class_manager.h"
#include "faction.h"
#include "galaxy.h"
#include "globals.h"
#include "object_aspect_name.h"
#include "object_aspect_object_container.h"
#include "object_aspect_position.h"
#include "object_aspect_visualization.h"
#include "object_factory.h"
#include "planet.h"
#include "planet_assets.h"
#include "resource_reader.h"
#include "scenario.h"
#include "scenario_manager.h"
#include "settings.h"
#include "slot.h"
#include "slot_class.h"
#include "star.h"
#include "system.h"
#include "visualization_prototype.h"

static auto Resolve(Arxx::Reference & Reference) -> Arxx::Item *;
static auto ReadAmmunitionClass(Arxx::Reference & Reference, BlueprintManager * BlueprintManager) -> void;
static auto ReadBatteryClass(Arxx::Reference & Reference, BlueprintManager * BlueprintManager) -> void;
static auto ReadCommodityClass(Arxx::Reference & Reference, BlueprintManager * BlueprintManager) -> void;
static auto ReadEnergyProjectileWeaponClass(Arxx::Reference & Reference, BlueprintManager * BlueprintManager) -> void;
static auto ReadFaction(Arxx::Reference & Reference, Galaxy * Galaxy, std::list<std::tuple<std::string, std::string, float>> & FactionStandings) -> void;
static auto ReadGeneratorClass(Arxx::Reference & Reference, BlueprintManager * BlueprintManager) -> void;
static auto ReadMesh(Arxx::Reference & Reference) -> void;
static auto ReadModel(Arxx::Reference & Reference) -> void;
static auto ReadPlanet(Arxx::Reference & Reference, Galaxy * Galaxy, System * System) -> void;
static auto ReadProgram(Arxx::Reference & Reference, Graphics::ShadingManager * ShadingManager) -> void;
static auto ReadScenario(Arxx::Reference & Reference, ScenarioManager * ScenarioManager) -> void;
static auto ReadShader(Arxx::Reference & Reference, Graphics::ShadingManager * ShadingManager) -> void;
static auto ReadShipClass(Arxx::Reference & Reference, BlueprintManager * BlueprintManager, ClassManager<SlotClass> * SlotClassManager) -> void;
static auto ReadSlotClass(Arxx::Reference & Reference, ClassManager<SlotClass> * SlotClassManager) -> void;
static auto ReadStar(Arxx::Reference & Reference, System * System) -> void;
static auto ReadSystemLink(Arxx::Reference & Reference, System * System, std::multimap<std::string, std::string> & SystemLinks) -> void;
static auto ReadTexture(Arxx::Reference & Reference) -> void;
static auto ReadTurretClass(Arxx::Reference & Reference, BlueprintManager * BlueprintManager) -> void;

static auto MakeItemAvailable(Arxx::Item * Item) -> void
{
	assert(Item != nullptr);
	if(Item->IsFetched() == false)
	{
		if(Item->Fetch() == false)
		{
			throw std::runtime_error("Could not fetch data for item '" + Item->GetName() + "' [" + to_string_cast(Item->GetIdentifier()) + "].");
		}
		if(Item->IsFetched() == false)
		{
			throw std::runtime_error("Could not fetch data for item '" + Item->GetName() + "' [" + to_string_cast(Item->GetIdentifier()) + "].");
		}
	}
	if(Item->IsCompressed() == true)
	{
		Item->Decompress();
		if(Item->IsCompressed() == true)
		{
			throw std::runtime_error("Could not decompress data for item '" + Item->GetName() + "' [" + to_string_cast(Item->GetIdentifier()) + "].");
		}
	}
}

static auto Resolve(Arxx::Reference & Reference) -> Arxx::Item *
{
	auto Item(Reference.GetItem());
	
	if(Item == nullptr)
	{
		throw std::runtime_error("The item identifier '" + to_string_cast(Reference.GetItemIdentifier()) + "' is referenced but could not be resolved.");
	}
	MakeItemAvailable(Item);
	
	return Item;
}

auto ResourceReader::LoadArchive(std::string const & ArchivePath) -> bool
{
	assert(m_Archive == nullptr);
	m_Archive = std::make_unique<Arxx::Archive>();
	if(m_Archive->Load(ArchivePath) == false)
	{
		std::cerr << "Could not find or open \"" << ArchivePath << "\"." << std::endl;
		
		return false;
	}
    else
    {
        return true;
    }
}

auto ResourceReader::m_ReadItems(std::string const & Path, std::function<void(Arxx::Reference &)> ReaderFunction) -> void
{
	assert(m_Archive != nullptr);
	
	auto Directory(m_Archive->GetItem(Path));
	
	if(Directory == nullptr)
	{
		throw std::runtime_error("Could not find an item at the path '" + Path + "'.");
	}
	if(Directory->GetStructure().HasRelation("child") == false)
	{
		throw std::runtime_error("The item '" + Path + "' does not contain a 'child' relation.");
	}
	m_ReadItems(Directory->GetStructure().GetRelation("child"), ReaderFunction);
}

auto ResourceReader::m_ReadItems(Arxx::Structure::Relation & Relation, std::function<void(Arxx::Reference &)> ReaderFunction) -> void
{
	for(auto & Child : Relation)
	{
		ReaderFunction(Child);
	}
}

auto ResourceReader::ReadAmmunitionClasses(BlueprintManager * BlueprintManager) -> void
{
	m_ReadItems("/Ammunition Classes", std::bind(ReadAmmunitionClass, std::placeholders::_1, BlueprintManager));
}

auto ResourceReader::ReadBatteryClasses(BlueprintManager * BlueprintManager) -> void
{
	m_ReadItems("/Battery Classes", std::bind(ReadBatteryClass, std::placeholders::_1, BlueprintManager));
}

auto ResourceReader::ReadCommodityClasses(BlueprintManager * BlueprintManager) -> void
{
	m_ReadItems("/Commodity Classes", std::bind(ReadCommodityClass, std::placeholders::_1, BlueprintManager));
}

auto ResourceReader::ReadEnergyProjectileWeaponClasses(BlueprintManager * BlueprintManager) -> void
{
	m_ReadItems("/Energy Projectile Weapon Classes", std::bind(ReadEnergyProjectileWeaponClass, std::placeholders::_1, BlueprintManager));
}

auto ResourceReader::ReadGalaxy(std::string const & GalaxyIdentifier) -> Galaxy *
{
	assert(m_Archive != nullptr);
	
	auto Path = std::string{"/Galaxies"};
	auto Directory = m_Archive->GetItem(Path);
	
	if(Directory == nullptr)
	{
		throw std::runtime_error("Could not find an item at the path '" + Path + "'.");
	}
	if(Directory->GetStructure().HasRelation("child") == false)
	{
		throw std::runtime_error("The item '" + Path + "' does not contain a 'child' relation.");
	}
	for(auto & Child : Directory->GetStructure().GetRelation("child"))
	{
		auto GalaxyItem = Resolve(Child);
		
		if(GalaxyItem->GetType() != DATA_TYPE_GALAXY)
		{
			throw std::runtime_error("Item type for galaxy '" + GalaxyItem->GetName() + "' should be '" + to_string_cast(DATA_TYPE_GALAXY) + "' not '" + to_string_cast(GalaxyItem->GetType()) + "'.");
		}
		if(GalaxyItem->GetSubType() != 0)
		{
			throw std::runtime_error("Item sub type for galaxy '" + GalaxyItem->GetName() + "' should be '0' not '" + to_string_cast(GalaxyItem->GetSubType()) + "'.");
		}
		
		auto Reader = Arxx::BufferReader{*GalaxyItem};
		auto Identifier = std::string{};
		
		Reader >> Identifier;
		if(Identifier == GalaxyIdentifier)
		{
			auto NewGalaxy = dynamic_cast<Galaxy *>(g_ObjectFactory->Create("galaxy", Identifier, false));
			auto Name = std::string{};
			
			Reader >> Name;
			assert(NewGalaxy->GetAspectName() != nullptr);
			NewGalaxy->GetAspectName()->SetName(Name);
			if(GalaxyItem->GetStructure().HasRelation("factions") == false)
			{
				throw std::runtime_error("The item '" + Path + "/" + GalaxyItem->GetName() + "' does not contain a 'factions' relation.");
			}
			
			auto FactionStandings = std::list<std::tuple<std::string, std::string, float>>{};
			
			m_ReadItems(GalaxyItem->GetStructure().GetRelation("factions"), std::bind(ReadFaction, std::placeholders::_1, NewGalaxy, std::ref(FactionStandings)));
			for(auto & FactionStanding : FactionStandings)
			{
				NewGalaxy->GetFaction(std::get<0>(FactionStanding))->AddFactionStanding(NewGalaxy->GetFaction(std::get<1>(FactionStanding)), std::get<2>(FactionStanding));
			}
			
			auto SystemLinks = std::multimap<std::string, std::string>{};
			
			m_ReadItems(GalaxyItem->GetStructure().GetRelation("systems"), std::bind(&ResourceReader::m_ReadSystem, this, std::placeholders::_1, NewGalaxy, std::ref(SystemLinks)));
			for(auto & SystemLink : SystemLinks)
			{
				NewGalaxy->GetSystem(SystemLink.first)->AddLinkedSystem(NewGalaxy->GetSystem(SystemLink.second));
			}
			
			return NewGalaxy;
		}
	}
	
	return nullptr;
}

auto ResourceReader::ReadGeneratorClasses(BlueprintManager * BlueprintManager) -> void
{
	m_ReadItems("/Generator Classes", std::bind(ReadGeneratorClass, std::placeholders::_1, BlueprintManager));
}

auto ResourceReader::ReadMeshes() -> void
{
	m_ReadItems("/Meshes", ReadMesh);
}

auto ResourceReader::ReadModels() -> void
{
	m_ReadItems("/Models", ReadModel);
}

auto ResourceReader::ReadScenarios(ScenarioManager * ScenarioManager) -> void
{
	m_ReadItems("/Scenarios", std::bind(ReadScenario, std::placeholders::_1, ScenarioManager));
}

auto ResourceReader::ReadSettings(Settings * Settings) -> void
{
	auto Item = m_Archive->GetItem("/Settings");
	
	if(Item == nullptr)
	{
		throw std::runtime_error("Could not find an item at the path '/Settings'.");
	}
	Settings->LoadFromItem(Item);
}

auto ResourceReader::ReadShadersAndPrograms(Graphics::ShadingManager * ShadingManager) -> void
{
	m_ReadItems("/Shaders", std::bind(ReadShader, std::placeholders::_1, ShadingManager));
	m_ReadItems("/Programs", std::bind(ReadProgram, std::placeholders::_1, ShadingManager));
}

auto ResourceReader::ReadShipClasses(BlueprintManager * BlueprintManager, ClassManager<SlotClass> * SlotClassManager) -> void
{
	m_ReadItems("/Ship Classes", std::bind(ReadShipClass, std::placeholders::_1, BlueprintManager, SlotClassManager));
}

auto ResourceReader::ReadSlotClasses(ClassManager<SlotClass> * SlotClassManager) -> void
{
	m_ReadItems("/Slot Classes", std::bind(ReadSlotClass, std::placeholders::_1, SlotClassManager));
}

auto ResourceReader::m_ReadSystem(Arxx::Reference & Reference, Galaxy * Galaxy, std::multimap<std::string, std::string> & SystemLinks) -> void
{
	auto Item = Resolve(Reference);
	
	if(Item->GetType() != DATA_TYPE_SYSTEM)
	{
		throw std::runtime_error("Item type for system '" + Item->GetName() + "' should be '" + to_string_cast(DATA_TYPE_SYSTEM) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for system '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	auto Reader = Arxx::BufferReader{*Item};
	auto Identifier = std::string{};
	
	Reader >> Identifier;
	
	auto NewSystem = dynamic_cast<System *>(g_ObjectFactory->Create("system", Identifier, false));
	
	if(NewSystem == nullptr)
	{
		throw std::runtime_error("Could not create system '" + Identifier + "'.");
	}
	
	auto Name = std::string{};
	auto Position = Vector2f{};
	auto TrafficDensity = float{};
	auto FactionInfluencesCount = std::uint32_t{};
	
	Reader >> Name >> Position >> TrafficDensity >> FactionInfluencesCount;
	NewSystem->GetAspectName()->SetName(Name);
	NewSystem->GetAspectPosition()->SetPosition(Vector3f::CreateFromComponents(Position[0], Position[1], 0.0f));
	NewSystem->SetTrafficDensity(TrafficDensity);
	for(auto FactionInfluenceIndex = std::uint32_t{0}; FactionInfluenceIndex < FactionInfluencesCount; ++FactionInfluenceIndex)
	{
		auto FactionIdentifier = std::string{};
		auto FactionInfluence = float{};
		
		Reader >> FactionIdentifier >> FactionInfluence;
		
		auto Faction{Galaxy->GetFaction(FactionIdentifier)};
		
		if(Faction == nullptr)
		{
			throw std::runtime_error("Could not find faction '" + FactionIdentifier + "'.");
		}
		NewSystem->AddFactionInfluence(Faction, FactionInfluence);
	}
	Galaxy->GetAspectObjectContainer()->AddContent(NewSystem);
	if(Item->GetStructure().HasRelation("stars") == true)
	{
		m_ReadItems(Item->GetStructure().GetRelation("stars"), std::bind(ReadStar, std::placeholders::_1, NewSystem));
	}
	if(Item->GetStructure().HasRelation("planets") == true)
	{
		m_ReadItems(Item->GetStructure().GetRelation("planets"), std::bind(ReadPlanet, std::placeholders::_1, Galaxy, NewSystem));
	}
	if(Item->GetStructure().HasRelation("linked-systems") == true)
	{
		m_ReadItems(Item->GetStructure().GetRelation("linked-systems"), std::bind(ReadSystemLink, std::placeholders::_1, NewSystem, std::ref(SystemLinks)));
	}
}

auto ResourceReader::ReadTextures() -> void
{
	m_ReadItems("/Textures", ReadTexture);
}

auto ResourceReader::ReadTurretClasses(BlueprintManager * BlueprintManager) -> void
{
	m_ReadItems("/Turret Classes", std::bind(ReadTurretClass, std::placeholders::_1, BlueprintManager));
}

auto ResourceReader::ReadSavegameFromScenarioPath(std::string const & ScenarioPath) -> std::string
{
	auto Item = m_Archive->GetItem(ScenarioPath);
	auto Result = std::string{};
	
	if(Item != nullptr)
	{
		MakeItemAvailable(Item);
		
		auto Reader = Arxx::BufferReader{*Item};
		auto Identifier = std::string{};
		auto Name = std::string{};
		auto Description = std::string{};
		
		Reader >> Identifier >> Name >> Description >> Result;
	}
	
	return Result;
}

static auto ReadAmmunitionClass(Arxx::Reference & Reference, BlueprintManager * BlueprintManager) -> void
{
	auto Item = Resolve(Reference);
	
	if(Item->GetType() != DATA_TYPE_AMMUNITION_CLASS)
	{
		throw std::runtime_error("Item type for ammunition class '" + Item->GetName() + "' should be '" + to_string_cast(DATA_TYPE_AMMUNITION_CLASS) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for ammunition class '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	auto Reader = Arxx::BufferReader{*Item};
	auto Identifier = std::string{};
	
	Reader >> Identifier;
	
	auto NewAmmunitionClass = BlueprintManager->Create("ammunition", Identifier);
	
	if(NewAmmunitionClass == nullptr)
	{
		throw std::runtime_error("Could not create ammunition class '" + Identifier + "'.");
	}
	
	auto Name = std::string{};
	auto Description = std::string{};
	auto BasePrice = std::uint32_t{};
	auto SpaceRequirement = std::uint32_t{};
	VisualizationPrototype VisualizationPrototype{};
	auto CartridgeSize = std::uint32_t{};
	
	Reader >> Name >> Description >> BasePrice >> SpaceRequirement >> VisualizationPrototype >> CartridgeSize;
	
	NewAmmunitionClass->AddProperty("name", Name);
	NewAmmunitionClass->AddProperty("description", Description);
	NewAmmunitionClass->AddProperty("base-price", BasePrice);
	NewAmmunitionClass->AddProperty("space-requirement", SpaceRequirement);
	NewAmmunitionClass->AddProperty("visualization-prototype", VisualizationPrototype);
	NewAmmunitionClass->AddProperty("cartridge-size", CartridgeSize);
}

static auto ReadBatteryClass(Arxx::Reference & Reference, BlueprintManager * BlueprintManager) -> void
{
	auto Item = Resolve(Reference);
	
	if(Item->GetType() != DATA_TYPE_BATTERY_CLASS)
	{
		throw std::runtime_error("Item type for battery class '" + Item->GetName() + "' should be '" + to_string_cast(DATA_TYPE_BATTERY_CLASS) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for battery class '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	auto Reader = Arxx::BufferReader{*Item};
	auto Identifier = std::string{};
	
	Reader >> Identifier;
	
	auto NewBatteryClass = BlueprintManager->Create("battery", Identifier);
	
	if(NewBatteryClass == nullptr)
	{
		throw std::runtime_error("Could not create battery class '" + Identifier + "'.");
	}
	
	auto Name = std::string{};
	auto Description = std::string{};
	auto BasePrice = std::uint32_t{};
	auto SpaceRequirement = std::uint32_t{};
	VisualizationPrototype VisualizationPrototype{};
	auto EnergyCapacity = float{};
	auto MaximumPowerInput = float{};
	auto MaximumPowerOutput = float{};
	auto SlotClassIdentifier = std::string{};
	
	Reader >> Name >> Description >> BasePrice >> SpaceRequirement >> VisualizationPrototype >> EnergyCapacity >> MaximumPowerInput >> MaximumPowerOutput >> SlotClassIdentifier;
	NewBatteryClass->AddProperty("name", Name);
	NewBatteryClass->AddProperty("description", Description);
	NewBatteryClass->AddProperty("base-price", BasePrice);
	NewBatteryClass->AddProperty("space-requirement", SpaceRequirement);
	NewBatteryClass->AddProperty("visualization-prototype", VisualizationPrototype);
	NewBatteryClass->AddProperty("energy-capacity", EnergyCapacity);
	NewBatteryClass->AddProperty("maximum-power-input", MaximumPowerInput);
	NewBatteryClass->AddProperty("maximum-power-output", MaximumPowerOutput);
	NewBatteryClass->AddProperty("slot-class-identifier", SlotClassIdentifier);
}

static auto ReadCommodityClass(Arxx::Reference & Reference, BlueprintManager * BlueprintManager) -> void
{
	auto Item = Resolve(Reference);
	
	if(Item->GetType() != DATA_TYPE_COMMODITY_CLASS)
	{
		throw std::runtime_error("Item type for commodity class '" + Item->GetName() + "' should be '" + to_string_cast(DATA_TYPE_COMMODITY_CLASS) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for commodity class '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	auto Reader = Arxx::BufferReader{*Item};
	auto Identifier = std::string{};
	
	Reader >> Identifier;
	
	auto NewCommodityClass = BlueprintManager->Create("commodity", Identifier);
	
	if(NewCommodityClass == nullptr)
	{
		throw std::runtime_error("Could not create commodity class '" + Identifier + "'.");
	}
	
	auto Name = std::string{};
	auto Description = std::string{};
	auto BasePrice = std::uint32_t{};
	auto SpaceRequirement = std::uint32_t{};
	VisualizationPrototype VisualizationPrototype{};
	
	Reader >> Name >> Description >> BasePrice >> SpaceRequirement >> VisualizationPrototype;
	NewCommodityClass->AddProperty("name", Name);
	NewCommodityClass->AddProperty("description", Description);
	NewCommodityClass->AddProperty("base-price", BasePrice);
	NewCommodityClass->AddProperty("space-requirement", SpaceRequirement);
	NewCommodityClass->AddProperty("visualization-prototype", VisualizationPrototype);
}

static auto ReadEnergyProjectileWeaponClass(Arxx::Reference & Reference, BlueprintManager * BlueprintManager) -> void
{
	auto Item = Resolve(Reference);
	
	if(Item->GetType() != DATA_TYPE_ENERGY_PROJECTILE_WEAPON_CLASS)
	{
		throw std::runtime_error("Item type for energy projectile weapon class '" + Item->GetName() + "' should be '" + to_string_cast(DATA_TYPE_ENERGY_PROJECTILE_WEAPON_CLASS) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for energy projectile weapon class '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	auto Reader = Arxx::BufferReader{*Item};
	auto Identifier = std::string{};
	
	Reader >> Identifier;
	
	auto NewEnergyProjectileWeaponBlueprint = BlueprintManager->Create("energy-projectile-weapon", Identifier);
	
	if(NewEnergyProjectileWeaponBlueprint == nullptr)
	{
		throw std::runtime_error("Could not create energy projectile weapon blueprint '" + Identifier + "'.");
	}
	
	auto Name = std::string{};
	auto Description = std::string{};
	auto BasePrice = std::uint32_t{};
	auto SpaceRequirement = std::uint32_t{};
	auto EnergyProjectileWeaponVisualizationPrototype = VisualizationPrototype{};
	auto SlotClassIdentifier = std::string{};
	auto Orientation = Quaternion{};
	auto MaximumPowerInput = float{};
	auto MaximumPowerOutput = float{};
	auto EnergyUsagePerShot = float{};
	auto MuzzlePositionPartIdentifier = std::string{};
	auto MuzzlePositionMarkerIdentifier = std::string{};
	auto ShotExitSpeed = float{};
	auto ShotDamage = float{};
	auto ShotLifeTime = float{};
	auto ShotVisualizationPrototype = VisualizationPrototype{};
	
	Reader >> Name >> Description >> BasePrice >> SpaceRequirement >> EnergyProjectileWeaponVisualizationPrototype >> SlotClassIdentifier >> Orientation >> MaximumPowerInput >> MaximumPowerOutput >> EnergyUsagePerShot >> MuzzlePositionPartIdentifier >> MuzzlePositionMarkerIdentifier >> ShotExitSpeed >> ShotDamage >> ShotLifeTime >> ShotVisualizationPrototype;
	NewEnergyProjectileWeaponBlueprint->AddProperty("name", Name);
	NewEnergyProjectileWeaponBlueprint->AddProperty("description", Description);
	NewEnergyProjectileWeaponBlueprint->AddProperty("base-price", BasePrice);
	NewEnergyProjectileWeaponBlueprint->AddProperty("visualization-prototype", EnergyProjectileWeaponVisualizationPrototype);
	NewEnergyProjectileWeaponBlueprint->AddProperty("slot-class-identifier", SlotClassIdentifier);
	NewEnergyProjectileWeaponBlueprint->AddProperty("orientation", Orientation);
	NewEnergyProjectileWeaponBlueprint->AddProperty("maximum-power-input", MaximumPowerInput);
	NewEnergyProjectileWeaponBlueprint->AddProperty("maximum-power-output", MaximumPowerOutput);
	NewEnergyProjectileWeaponBlueprint->AddProperty("space-requirement", SpaceRequirement);
	NewEnergyProjectileWeaponBlueprint->AddProperty("energy-usage-per-shot", EnergyUsagePerShot);
	
	auto MuzzlePosition = EnergyProjectileWeaponVisualizationPrototype.GetMarkerPosition(MuzzlePositionPartIdentifier, MuzzlePositionMarkerIdentifier);
	
	if(MuzzlePosition == nullptr)
	{
		throw std::runtime_error("For the energy projectile weapon '" + Identifier + "', could not find a marker or its position for the muzzle position '" + MuzzlePositionMarkerIdentifier + "' on the part '" + MuzzlePositionPartIdentifier + "'.");
	}
	NewEnergyProjectileWeaponBlueprint->AddProperty("muzzle-position", *MuzzlePosition);
	NewEnergyProjectileWeaponBlueprint->AddProperty("shot-exit-speed", ShotExitSpeed);
	NewEnergyProjectileWeaponBlueprint->AddProperty("shot-damage", ShotDamage);
	NewEnergyProjectileWeaponBlueprint->AddProperty("shot-life-time", ShotLifeTime);
	NewEnergyProjectileWeaponBlueprint->AddProperty("shot-visualization-prototype", ShotVisualizationPrototype);
}

static auto ReadFaction(Arxx::Reference & Reference, Galaxy * Galaxy, std::list<std::tuple<std::string, std::string, float>> & FactionStandings) -> void
{
	auto Item = Resolve(Reference);
	
	if(Item->GetType() != DATA_TYPE_FACTION)
	{
		throw std::runtime_error("Item type for faction '" + Item->GetName() + "' should be '" + to_string_cast(DATA_TYPE_FACTION) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for faction '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	auto Reader = Arxx::BufferReader{*Item};
	auto Identifier = std::string{};
	
	Reader >> Identifier;
	
	auto NewFaction = dynamic_cast<Faction *>(g_ObjectFactory->Create("faction", Identifier, false));
	
	if(NewFaction == nullptr)
	{
		throw std::runtime_error("Could not create faction '" + Identifier + "'.");
	}
	
	auto Name = std::string{};
	auto FactionColor = Graphics::ColorRGBO{};
	auto StandingsCount = std::uint32_t{};
	
	Reader >> Name >> FactionColor >> StandingsCount;
	NewFaction->GetAspectName()->SetName(Name);
	NewFaction->SetColor(FactionColor);
	for(auto StandingsIndex = std::uint32_t{0}; StandingsIndex < StandingsCount; ++StandingsIndex)
	{
		auto FactionIdentifier = std::string{};
		auto FactionStanding = float{};
		
		Reader >> FactionIdentifier >> FactionStanding;
		FactionStandings.push_back(std::make_tuple(Identifier, FactionIdentifier, FactionStanding));
	}
	Galaxy->GetAspectObjectContainer()->AddContent(NewFaction);
}

static auto ReadGeneratorClass(Arxx::Reference & Reference, BlueprintManager * BlueprintManager) -> void
{
	auto Item = Resolve(Reference);
	
	if(Item->GetType() != DATA_TYPE_GENERATOR_CLASS)
	{
		throw std::runtime_error("Item type for generator class '" + Item->GetName() + "' should be '" + to_string_cast(DATA_TYPE_GENERATOR_CLASS) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for generator class '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	auto Reader = Arxx::BufferReader{*Item};
	auto Identifier = std::string{};
	
	Reader >> Identifier;
	
	auto NewGeneratorClass = BlueprintManager->Create("generator", Identifier);
	
	if(NewGeneratorClass == nullptr)
	{
		throw std::runtime_error("Could not create generator class '" + Identifier + "'.");
	}
	
	auto Name = std::string{};
	auto Description = std::string{};
	auto BasePrice = std::uint32_t{};
	auto SpaceRequirement = std::uint32_t{};
	VisualizationPrototype VisualizationPrototype{};
	auto MaximumPowerOutput = float{};
	auto SlotClassIdentifier = std::string{};
	
	Reader >> Name >> Description >> BasePrice >> SpaceRequirement >> VisualizationPrototype >> MaximumPowerOutput >> SlotClassIdentifier;
	NewGeneratorClass->AddProperty("name", Name);
	NewGeneratorClass->AddProperty("description", Description);
	NewGeneratorClass->AddProperty("base-price", BasePrice);
	NewGeneratorClass->AddProperty("space-requirement", SpaceRequirement);
	NewGeneratorClass->AddProperty("visualization-prototype", VisualizationPrototype);
	NewGeneratorClass->AddProperty("maximum-power-output", MaximumPowerOutput);
	NewGeneratorClass->AddProperty("slot-class-identifier", SlotClassIdentifier);
}

static auto ReadMesh(Arxx::Reference & Reference) -> void
{
	auto Item = Resolve(Reference);
	
	if(Item->GetType() != DATA_TYPE_MESH)
	{
		throw std::runtime_error("Item type for mesh '" + Item->GetName() + "' should be '" + to_string_cast(DATA_TYPE_MESH) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for mesh '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	auto Reader = Arxx::BufferReader{*Item};
	auto Identifier = std::string{};
	
	Reader >> Identifier;
	
	auto NewMesh = g_GraphicsEngine->GetMeshManager()->Create(Identifier);
	
	if(NewMesh == nullptr)
	{
		throw std::runtime_error("Could not create mesh '" + Identifier + "'.");
	}
	
	auto Points = std::map<std::string, std::vector<Vector3f>::size_type>{};
	auto PointCount = std::uint32_t{};
	
	Reader >> PointCount;
	for(auto PointIndex = std::uint32_t{0}; PointIndex < PointCount; ++PointIndex)
	{
		auto PointIdentifier = std::string{};
		auto PointName = std::string{};
		auto PointCoordinates = Vector3f{};
		
		Reader >> PointIdentifier >> PointName >> PointCoordinates;
		Points[PointIdentifier] = NewMesh->AddPoint(PointCoordinates);
	}
	
	auto TrianglePoints = std::map<std::string, std::pair<std::vector<Vector3f >::size_type, Vector3f>>{};
	auto TrianglePointCount = std::uint32_t{};
	
	Reader >> TrianglePointCount;
	for(auto TrianglePointIndex = std::uint32_t{0}; TrianglePointIndex < TrianglePointCount; ++TrianglePointIndex)
	{
		auto TrianglePointIdentifier = std::string{};
		auto TrianglePointNormal = Vector3f{};
		auto PointIdentifier = std::string{};
		
		Reader >> TrianglePointIdentifier >> TrianglePointNormal >> PointIdentifier;
		TrianglePoints[TrianglePointIdentifier] = std::make_pair(Points[PointIdentifier], TrianglePointNormal);
	}
	
	auto TriangleCount = std::uint32_t{};
    
	Reader >> TriangleCount;
	for(auto TriangleIndex = std::uint32_t{0}; TriangleIndex < TriangleCount; ++TriangleIndex)
	{
		auto TriangleIdentifier = std::string{};
		auto TriangleName = std::string{};
		auto TrianglePoint1Identifier = std::string{};
		auto TrianglePoint2Identifier = std::string{};
		auto TrianglePoint3Identifier = std::string{};
		
		Reader >> TriangleIdentifier >> TriangleName >> TrianglePoint1Identifier >> TrianglePoint2Identifier >> TrianglePoint3Identifier;
		NewMesh->AddTriangle(TrianglePoints[TrianglePoint1Identifier].first, TrianglePoints[TrianglePoint1Identifier].second, TrianglePoints[TrianglePoint2Identifier].first, TrianglePoints[TrianglePoint2Identifier].second, TrianglePoints[TrianglePoint3Identifier].first, TrianglePoints[TrianglePoint3Identifier].second);
	}
	
	auto MarkerCount = std::uint32_t{};
	
	Reader >> MarkerCount;
	for(auto MarkerIndex = std::uint32_t{0}; MarkerIndex < MarkerCount; ++MarkerIndex)
	{
		auto MarkerIdentifier = std::string{};
		auto MarkerLengthValid = bool{};
		auto MarkerLength = float{};
		auto MarkerPositionValid = bool{};
		auto MarkerPosition = Vector3f{};
		auto MarkerOrientationValid = bool{};
		auto MarkerOrientation = Quaternion{};
		
		Reader >> MarkerIdentifier >> MarkerLengthValid >> MarkerLength >> MarkerPositionValid >> MarkerPosition >> MarkerOrientationValid >> MarkerOrientation;
		NewMesh->AddMarker(MarkerIdentifier);
		if(MarkerLengthValid == true)
		{
			NewMesh->SetMarkerLength(MarkerIdentifier, MarkerLength);
		}
		if(MarkerPositionValid == true)
		{
			NewMesh->SetMarkerPosition(MarkerIdentifier, MarkerPosition);
		}
		if(MarkerOrientationValid == true)
		{
			NewMesh->SetMarkerOrientation(MarkerIdentifier, MarkerOrientation);
		}
	}
	NewMesh->BuildVertexArray();
}

static auto ReadModel(Arxx::Reference & Reference) -> void
{
	auto Item = Resolve(Reference);
	
	if(Item->GetType() != DATA_TYPE_MODEL)
	{
		throw std::runtime_error("Item type for model '" + Item->GetName() + "' should be '" + to_string_cast(DATA_TYPE_MODEL) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for model '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	auto Reader = Arxx::BufferReader{*Item};
	auto Identifier = std::string{};
	
	Reader >> Identifier;
    
	auto NewModel = g_GraphicsEngine->GetModelManager()->Create(Identifier);
	
	if(NewModel == nullptr)
	{
		throw std::runtime_error("Could not create model '" + Identifier + "'.");
	}
	
	auto PartCount = std::uint32_t{};
	
	Reader >> PartCount;
	for(auto PartIndex = std::uint32_t{0}; PartIndex < PartCount; ++PartIndex)
	{
		auto PartIdentifier = std::string{};
		auto PartMeshIdentifier = std::string{};
		auto PartPosition = Vector3f{};
		auto PartOrientation = Quaternion{};
		
		Reader >> PartIdentifier >> PartMeshIdentifier >> PartPosition >> PartOrientation;
		
		auto PartMesh = g_GraphicsEngine->GetMeshManager()->Get(PartMeshIdentifier);
		
		if(PartMesh == nullptr)
		{
			throw std::runtime_error("For model '" + Identifier + "' could not find the mesh '" + PartMeshIdentifier + "' for part '" + PartIdentifier + "'.");
		}
		NewModel->AddPart(PartIdentifier, PartMesh, PartPosition, PartOrientation);
	}
}

static auto ReadPlanet(Arxx::Reference & Reference, Galaxy * Galaxy, System * System) -> void
{
	auto Item = Resolve(Reference);
	
	if(Item->GetType() != DATA_TYPE_PLANET)
	{
		throw std::runtime_error("Item type for planet '" + Item->GetName() + "' should be '" + to_string_cast(DATA_TYPE_PLANET) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for planet '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	auto Reader = Arxx::BufferReader{*Item};
	auto Identifier = std::string{};
	
	Reader >> Identifier;
	
	auto NewPlanet = dynamic_cast<Planet *>(g_ObjectFactory->Create("planet", Identifier, false));
	
	if(NewPlanet == nullptr)
	{
		throw std::runtime_error("Could not create planet '" + Identifier + "'.");
	}
	
	auto Name = std::string{};
	auto Description = std::string{};
	VisualizationPrototype VisualizationPrototype{};
	
	Reader >> Name >> Description >> VisualizationPrototype;
	NewPlanet->GetAspectName()->SetName(Name);
	NewPlanet->SetDescription(Description);
	assert(NewPlanet->GetAspectVisualization() != nullptr);
	assert(NewPlanet->GetAspectVisualization()->GetVisualizationPrototype() == nullptr);
	NewPlanet->GetAspectVisualization()->SetVisualizationPrototype(VisualizationPrototype);
	
	auto PlanetPosition = Vector2f{};
	auto Size = float{};
	auto PlanetAssetsCount = std::uint32_t{};
	
	Reader >> PlanetPosition >> Size >> PlanetAssetsCount;
	NewPlanet->GetAspectPosition()->SetPosition(Vector3f::CreateFromComponents(PlanetPosition[0], PlanetPosition[1], 0.0f));
	NewPlanet->SetSize(Size);
	for(auto PlanetAssetsIndex = std::uint32_t{0}; PlanetAssetsIndex < PlanetAssetsCount; ++PlanetAssetsIndex)
	{
		auto AssetsTypeIdentifier = std::string{};
		auto AssetsSubTypeIdentifier = std::string{};
		auto BasePriceModifier = float{};
		
		Reader >> AssetsTypeIdentifier >> AssetsSubTypeIdentifier >> BasePriceModifier;
		if(g_BlueprintManager->Has(AssetsTypeIdentifier, AssetsSubTypeIdentifier) == true)
		{
			auto NewPlanetAssets = NewPlanet->CreatePlanetAssets(AssetsTypeIdentifier, AssetsSubTypeIdentifier);
			
			NewPlanetAssets->SetBasePriceModifier(BasePriceModifier);
		}
		else
		{
			throw std::runtime_error("For the assets of planet \"" + Name + "\", don't know a class of type '" + AssetsTypeIdentifier + "' and sub type '" + AssetsSubTypeIdentifier + "'.");
		}
	}
	
	auto LandingFeePerSpace = float{};
	auto OffersRecharging = bool{};
	auto RechargingFeePerEnergy = float{};
	auto OffersRepairing = bool{};
	auto RepairingFeePerHull = float{};
	auto FactionIdentifier = std::string{};
	
	Reader >> LandingFeePerSpace >> OffersRecharging >> RechargingFeePerEnergy >> OffersRepairing >> RepairingFeePerHull >> FactionIdentifier;
	NewPlanet->SetLandingFeePerSpace(LandingFeePerSpace / 1000.0f);
	NewPlanet->SetOffersRecharging(OffersRecharging);
	NewPlanet->SetRechargingFeePerEnergy(RechargingFeePerEnergy);
	NewPlanet->SetOffersRepairing(OffersRepairing);
	NewPlanet->SetRepairingFeePerHull(RepairingFeePerHull);
	
	auto Faction = Galaxy->GetFaction(FactionIdentifier);
	
	if(Faction == nullptr)
	{
		throw std::runtime_error("Could not find faction '" + FactionIdentifier + "' for planet '" + Identifier + "' in system '" + System->GetSubTypeIdentifier() + "'.");
	}
	NewPlanet->SetFaction(Faction);
	System->GetAspectObjectContainer()->AddContent(NewPlanet);
}

static auto ReadProgram(Arxx::Reference & Reference, Graphics::ShadingManager * ShadingManager) -> void
{
	auto Item = Resolve(Reference);
	
	if(Item->GetType() != DATA_TYPE_SHADER_PROGRAM)
	{
		throw std::runtime_error("Item type for shader '" + Item->GetName() + "' should be '" + to_string_cast(DATA_TYPE_SHADER_PROGRAM) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	
	auto Reader = Arxx::BufferReader{*Item};
	auto Identifier = std::string{};
	
	Reader >> Identifier;
	
	auto Program = ShadingManager->CreateProgram(Identifier);
	
	if(Program == nullptr)
	{
		throw std::runtime_error("Could not create program '" + Identifier + "'.");
	}
	
	auto NumberOfShaders = std::uint32_t{};
	
	Reader >> NumberOfShaders;
	for(auto ShaderNumber = std::uint32_t{1}; ShaderNumber <= NumberOfShaders; ++ShaderNumber)
	{
		auto ShaderIdentifier = std::string{};
		
		Reader >> ShaderIdentifier;
		Program->AddShaderIdentifier(ShaderIdentifier);
	}
}

static auto ReadScenario(Arxx::Reference & Reference, ScenarioManager * ScenarioManager) -> void
{
	auto Item = Resolve(Reference);
	
	if(Item->GetType() != DATA_TYPE_SCENARIO)
	{
		throw std::runtime_error("Item type for scenario '" + Item->GetName() + "' should be '" + to_string_cast(DATA_TYPE_SCENARIO) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for scenario '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	auto Reader = Arxx::BufferReader{*Item};
	auto Identifier = std::string{};
	
	Reader >> Identifier;
	
	auto NewScenario = ScenarioManager->Create(Identifier);
	
	if(NewScenario == nullptr)
	{
		throw std::runtime_error("Could not create scenario '" + Identifier + "'.");
	}
	NewScenario->SetResourcePath("/Scenarios/" + Item->GetName());
	
	auto Name = std::string{};
	auto Description = std::string{};
	
	Reader >> Name >> Description;
	NewScenario->SetName(Name);
	NewScenario->SetDescription(Description);
}

static auto ReadShader(Arxx::Reference & Reference, Graphics::ShadingManager * ShadingManager) -> void
{
	auto Item = Resolve(Reference);
	
	if(Item->GetType() != DATA_TYPE_SHADER)
	{
		throw std::runtime_error("Item type for shader '" + Item->GetName() + "' should be '" + to_string_cast(DATA_TYPE_SHADER) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	
	auto Reader = Arxx::BufferReader{*Item};
	auto Identifier = std::string{};
	
	Reader >> Identifier;
	
	auto Shader = ShadingManager->CreateShader(Identifier);
	
	if(Shader == nullptr)
	{
		throw std::runtime_error("Could not create shader '" + Identifier + "'.");
	}
	if(Item->GetSubType() == DATA_TYPE_SHADER_SUB_TYPE_VERTEX_SHADER)
	{
		Shader->SetType(Graphics::Shader::Type::VertexShader);
	}
	else if(Item->GetSubType() == DATA_TYPE_SHADER_SUB_TYPE_FRAGMENT_SHADER)
	{
		Shader->SetType(Graphics::Shader::Type::FragmentShader);
	}
	else
	{
		throw std::runtime_error("Unknown sub type '" + to_string_cast(Item->GetType()) + "' for shader '" + Item->GetName() + ".");
	}
	
	auto Source = std::string{};
	
	Reader >> Source;
	Shader->SetSource(Source);
}

static auto ReadShipClass(Arxx::Reference & Reference, BlueprintManager * BlueprintManager, ClassManager<SlotClass> * SlotClassManager) -> void
{
	auto Item = Resolve(Reference);
	
	if(Item->GetType() != DATA_TYPE_SHIP_CLASS)
	{
		throw std::runtime_error("Item type for ship class '" + Item->GetName() + "' should be '" + to_string_cast(DATA_TYPE_SHIP_CLASS) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for ship class '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	auto Reader = Arxx::BufferReader{*Item};
	auto Identifier = std::string{};
	
	Reader >> Identifier;
	
	auto NewShipClass = BlueprintManager->Create("ship", Identifier);
	
	if(NewShipClass == nullptr)
	{
		throw std::runtime_error("Could not create ship class '" + Identifier + "'.");
	}
    
	auto Name = std::string{};
	auto Description = std::string{};
	auto BasePrice = std::uint32_t{};
	auto SpaceRequirement = std::uint32_t{};
	VisualizationPrototype VisualizationPrototype{};
	auto FuelCapacity = float{};
	auto ForwardThrust = float{};
	auto TurnSpeed = float{};
	auto MaximumSpeed = float{};
	auto MaximumAvailableSpace = std::uint32_t{};
	auto JumpFuel = float{};
	auto ForwardFuel = float{};
	auto TurnFuel = float{};
	auto Hull = float{};
	auto ExhaustMarkerPartIdentifier = std::string{};
	auto ExhaustMarkerIdentifier = std::string{};
	auto SlotCount = std::uint32_t{};
	
	Reader >> Name >> Description >> BasePrice >> SpaceRequirement >> VisualizationPrototype >> ForwardThrust >> TurnSpeed >> MaximumSpeed >> MaximumAvailableSpace >> FuelCapacity >> JumpFuel >> ForwardFuel >> TurnFuel >> Hull >> ExhaustMarkerPartIdentifier >> ExhaustMarkerIdentifier >> SlotCount;
	NewShipClass->AddProperty("name", Name);
	NewShipClass->AddProperty("description", Description);
	NewShipClass->AddProperty("base-price", BasePrice);
	NewShipClass->AddProperty("space-requirement", SpaceRequirement);
	NewShipClass->AddProperty("visualization-prototype", VisualizationPrototype);
	NewShipClass->AddProperty("fuel-capacity", FuelCapacity);
	NewShipClass->AddProperty("forward-thrust", ForwardThrust);
	NewShipClass->AddProperty("jump-fuel", JumpFuel);
	NewShipClass->AddProperty("maximum-speed", MaximumSpeed);
	NewShipClass->AddProperty("maximum-available-space", MaximumAvailableSpace);
	NewShipClass->AddProperty("turn-speed", TurnSpeed);
	NewShipClass->AddProperty("forward-fuel", ForwardFuel);
	NewShipClass->AddProperty("turn-fuel", TurnFuel);
	NewShipClass->AddProperty("hull", Hull);
	NewShipClass->AddProperty("exhaust-marker-part-identifier", ExhaustMarkerPartIdentifier);
	NewShipClass->AddProperty("exhaust-marker-identifier", ExhaustMarkerIdentifier);
	
	auto Slots = std::list<Properties>{};
	
	for(auto SlotIndex = std::uint32_t{0}; SlotIndex < SlotCount; ++SlotIndex)
	{
		auto NewSlot = Properties{};
		auto SlotIdentifier = std::string{};
		auto SlotClassIdentifier = std::string{};
		
		Reader >> SlotIdentifier >> SlotClassIdentifier;
		NewSlot.AddProperty("identifier", SlotIdentifier);
		
		auto SlotClass = SlotClassManager->Get(SlotClassIdentifier);
		
		if(SlotClass == nullptr)
		{
			throw std::runtime_error("Could not get slot class '" + SlotClassIdentifier + "' for slot '" + SlotIdentifier + "' of ship class '" + Identifier + "'.");
		}
		NewSlot.AddProperty("class", SlotClass);
		
		auto SlotName = std::string{};
		auto SlotMarkerPartIdentifier = std::string{};
		auto SlotMarkerIdentifier = std::string{};
		auto VisualizeAccessory = bool{};
		
		Reader >> SlotName >> SlotMarkerPartIdentifier >> SlotMarkerIdentifier >> VisualizeAccessory;
		NewSlot.AddProperty("name", SlotName);
	
		auto SlotPosition = VisualizationPrototype.GetMarkerPosition(SlotMarkerPartIdentifier, SlotMarkerIdentifier);

		if(SlotPosition == nullptr)
		{
			throw std::runtime_error("For the ship '" + Identifier + "' and slot '" + SlotName + "', could not find the marker or its position for the slot position '" + SlotMarkerIdentifier + "' on the part '" + SlotMarkerPartIdentifier + "'.");
		}
		NewSlot.AddProperty("position", *SlotPosition);
	
		auto SlotOrientation = VisualizationPrototype.GetMarkerOrientation(SlotMarkerPartIdentifier, SlotMarkerIdentifier);

		if(SlotOrientation == nullptr)
		{
			throw std::runtime_error("For the ship '" + Identifier + "' and slot '" + SlotName + "', could not find the marker or its orientation for the slot position '" + SlotMarkerIdentifier + "' on the part '" + SlotMarkerPartIdentifier + "'.");
		}
		NewSlot.AddProperty("orientation", *SlotOrientation);
		NewSlot.AddProperty("visualize-accessory", VisualizeAccessory);
		Slots.push_back(NewSlot);
	}
	NewShipClass->AddProperty("slots", Slots);
}

static auto ReadSlotClass(Arxx::Reference & Reference, ClassManager<SlotClass> * SlotClassManager) -> void
{
	auto Item = Resolve(Reference);
	
	if(Item->GetType() != DATA_TYPE_SLOT_CLASS)
	{
		throw std::runtime_error("Item type for slot class '" + Item->GetName() + "' should be '" + to_string_cast(DATA_TYPE_SLOT_CLASS) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for slot class '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	auto Reader = Arxx::BufferReader{*Item};
	auto Identifier = std::string{};
	
	Reader >> Identifier;
	
	auto NewSlotClass = SlotClassManager->Create(Identifier);
	
	if(NewSlotClass == nullptr)
	{
		throw std::runtime_error("Could not create slot class '" + Identifier + "'.");
	}
	
	auto Name = std::string{};
	auto AcceptedSlotClassIdentifierCount = std::uint32_t{};
	
	Reader >> Name >> AcceptedSlotClassIdentifierCount;
	NewSlotClass->SetName(Name);
	for(auto AcceptedSlotClassIdentifierIndex = std::uint32_t{0}; AcceptedSlotClassIdentifierIndex < AcceptedSlotClassIdentifierCount; ++AcceptedSlotClassIdentifierIndex)
	{
		auto AcceptedSlotClassIdentifier = std::string{};
		
		Reader >> AcceptedSlotClassIdentifier;
		NewSlotClass->AddAcceptedSlotClassIdentifier(AcceptedSlotClassIdentifier);
	}
}

static auto ReadStar(Arxx::Reference & Reference, System * System) -> void
{
	auto Item = Resolve(Reference);
	
	if(Item->GetType() != DATA_TYPE_STAR)
	{
		throw std::runtime_error("Item type for star '" + Item->GetName() + "' should be '" + to_string_cast(DATA_TYPE_STAR) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for star '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	auto Reader = Arxx::BufferReader{*Item};
	auto Identifier = std::string{};
	auto Position = Vector2f{};
	auto Color = Graphics::ColorRGB{};
	
	Reader >> Identifier >> Position >> Color;
	
	auto NewStar = dynamic_cast<Star *>(g_ObjectFactory->Create("star", Identifier, false));
	
	NewStar->GetAspectPosition()->SetPosition(Vector3f::CreateFromComponents(Position[0], Position[1], 0.0f));
	NewStar->SetColor(Color);
	System->GetAspectObjectContainer()->AddContent(NewStar);
}

static auto ReadSystemLink(Arxx::Reference & Reference, System * System, std::multimap<std::string, std::string> & SystemLinks) -> void
{
	auto Item = Resolve(Reference);
	
	if(Item->GetType() != DATA_TYPE_SYSTEM)
	{
		throw std::runtime_error("Item type for system '" + Item->GetName() + "' should be '" + to_string_cast(DATA_TYPE_SYSTEM) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for system '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	auto Reader = Arxx::BufferReader{*Item};
	auto Identifier = std::string{};
	
	Reader >> Identifier;
	SystemLinks.insert(std::make_pair(System->GetSubTypeIdentifier(), Identifier));
}

static auto ReadTexture(Arxx::Reference & Reference) -> void
{
	auto Item = Resolve(Reference);
	
	if(Item->GetType() != DATA_TYPE_TEXTURE)
	{
		throw std::runtime_error("Item type for texture '" + Item->GetName() + "' should be '" + to_string_cast(DATA_TYPE_TEXTURE) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for texture '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	auto Reader = Arxx::BufferReader{*Item};
	auto Identifier = std::string{};
	auto Width = std::uint32_t{};
	auto Height = std::uint32_t{};
	auto Format = std::uint32_t{};
	
	Reader >> Identifier >> Width >> Height >> Format;
	Width = ntohl(Width);
	Height = ntohl(Height);
	Format = ntohl(Format);
	
	auto Texture = g_GraphicsEngine->GetTextureManager()->Create(Identifier);
	
	if(Texture == nullptr)
	{
		throw std::runtime_error("Could not create texture '" + Identifier + "'.");
	}
	Texture->SetData(Width, Height, Format, Reader.GetBuffer().GetBegin() + Reader.GetPosition());
}

static auto ReadTurretClass(Arxx::Reference & Reference, BlueprintManager * BlueprintManager) -> void
{
	auto Item = Resolve(Reference);
	
	if(Item->GetType() != DATA_TYPE_TURRET_CLASS)
	{
		throw std::runtime_error("Item type for turret class '" + Item->GetName() + "' should be '" + to_string_cast(DATA_TYPE_TURRET_CLASS) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for turret class '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	auto Reader = Arxx::BufferReader{*Item};
	auto Identifier = std::string{};
	
	Reader >> Identifier;
	
	auto NewTurretClass = BlueprintManager->Create("turret", Identifier);
	
	if(NewTurretClass == nullptr)
	{
		throw std::runtime_error("Could not create turret class '" + Identifier + "'.");
	}
	
	auto Name = std::string{};
	auto Description = std::string{};
	auto BasePrice = std::uint32_t{};
	auto SpaceRequirement = std::uint32_t{};
	VisualizationPrototype TurretVisualizationPrototype{};
	auto SlotClassIdentifier = std::string{};
	auto Orientation = Quaternion{};
	auto MaximumPowerInput = float{};
	auto MaximumPowerOutput = float{};
	auto EnergyUsagePerShot = float{};
	auto MuzzlePositionPartIdentifier = std::string{};
	auto MuzzlePositionMarkerIdentifier = std::string{};
	auto ShotExitSpeed = float{};
	auto ShotDamage = float{};
	auto ShotLifeTime = float{};
	VisualizationPrototype ShotVisualizationPrototype{};
	
	Reader >> Name >> Description >> BasePrice >> SpaceRequirement >> TurretVisualizationPrototype >> SlotClassIdentifier >> Orientation >> MaximumPowerInput >> MaximumPowerOutput >> EnergyUsagePerShot >> MuzzlePositionPartIdentifier >> MuzzlePositionMarkerIdentifier >> ShotExitSpeed >> ShotDamage >> ShotLifeTime >> ShotVisualizationPrototype;
	NewTurretClass->AddProperty("name", Name);
	NewTurretClass->AddProperty("description", Description);
	NewTurretClass->AddProperty("base-price", BasePrice);
	NewTurretClass->AddProperty("space-requirement", SpaceRequirement);
	NewTurretClass->AddProperty("visualization-prototype", TurretVisualizationPrototype);
	NewTurretClass->AddProperty("slot-class-identifier", SlotClassIdentifier);
	NewTurretClass->AddProperty("orientation", Orientation);
	NewTurretClass->AddProperty("maximum-power-input", MaximumPowerInput);
	NewTurretClass->AddProperty("maximum-power-output", MaximumPowerOutput);
	NewTurretClass->AddProperty("energy-usage-per-shot", EnergyUsagePerShot);
	
	auto MuzzlePosition = TurretVisualizationPrototype.GetMarkerPosition(MuzzlePositionPartIdentifier, MuzzlePositionMarkerIdentifier);
	
	if(MuzzlePosition == nullptr)
	{
		throw std::runtime_error("For the turret '" + Identifier + "', could not find a marker or its position for the muzzle position '" + MuzzlePositionMarkerIdentifier + "' on the part '" + MuzzlePositionPartIdentifier + "'.");
	}
	NewTurretClass->AddProperty("muzzle-position", *MuzzlePosition);
	NewTurretClass->AddProperty("shot-exit-speed", ShotExitSpeed);
	NewTurretClass->AddProperty("shot-damage", ShotDamage);
	NewTurretClass->AddProperty("shot-life-time", ShotLifeTime);
	NewTurretClass->AddProperty("shot-visualization-prototype", ShotVisualizationPrototype);
}
