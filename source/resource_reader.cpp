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

static Arxx::Item * Resolve(Arxx::Reference & Reference);

static void ReadAmmunitionClass(Arxx::Reference & Reference, BlueprintManager * BlueprintManager);
static void ReadBatteryClass(Arxx::Reference & Reference, BlueprintManager * BlueprintManager);
static void ReadCommodityClass(Arxx::Reference & Reference, BlueprintManager * BlueprintManager);
static void ReadEnergyProjectileWeaponClass(Arxx::Reference & Reference, BlueprintManager * BlueprintManager);
static void ReadFaction(Arxx::Reference & Reference, Galaxy * Galaxy, std::list< std::tuple< std::string, std::string, float > > & FactionStandings);
static void ReadGeneratorClass(Arxx::Reference & Reference, BlueprintManager * BlueprintManager);
static void ReadMesh(Arxx::Reference & Reference);
static void ReadModel(Arxx::Reference & Reference);
static void ReadPlanet(Arxx::Reference & Reference, Galaxy * Galaxy, System * System);
static void ReadProgram(Arxx::Reference & Reference, Graphics::ShadingManager * ShadingManager);
static void ReadScenario(Arxx::Reference & Reference, ScenarioManager * ScenarioManager);
static void ReadShader(Arxx::Reference & Reference, Graphics::ShadingManager * ShadingManager);
static void ReadShipClass(Arxx::Reference & Reference, BlueprintManager * BlueprintManager, ClassManager< SlotClass > * SlotClassManager);
static void ReadSlotClass(Arxx::Reference & Reference, ClassManager< SlotClass > * SlotClassManager);
static void ReadStar(Arxx::Reference & Reference, System * System);
static void ReadSystemLink(Arxx::Reference & Reference, System * System, std::multimap< std::string, std::string > & SystemLinks);
static void ReadTexture(Arxx::Reference & Reference);
static void ReadTurretClass(Arxx::Reference & Reference, BlueprintManager * BlueprintManager);

static void MakeItemAvailable(Arxx::Item * Item)
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

static Arxx::Item * Resolve(Arxx::Reference & Reference)
{
	auto Item(Reference.GetItem());
	
	if(Item == nullptr)
	{
		throw std::runtime_error("The item identifier '" + to_string_cast(Reference.GetItemIdentifier()) + "' is referenced but could not be resolved.");
	}
	MakeItemAvailable(Item);
	
	return Item;
}

ResourceReader::ResourceReader(void) :
	_Archive(nullptr)
{
}

ResourceReader::~ResourceReader(void)
{
	delete _Archive;
	_Archive = nullptr;
}

bool ResourceReader::LoadArchive(const std::string & ArchivePath)
{
	assert(_Archive == nullptr);
	_Archive = new Arxx::Archive();
	if(_Archive->Load(ArchivePath) == false)
	{
		std::cerr << "Could not find or open \"" << ArchivePath << "\"." << std::endl;
		
		return false;
	}
	
	return true;
}

void ResourceReader::_ReadItems(const std::string & Path, std::function< void (Arxx::Reference &) > ReaderFunction)
{
	assert(_Archive != nullptr);
	
	auto Directory(_Archive->GetItem(Path));
	
	if(Directory == nullptr)
	{
		throw std::runtime_error("Could not find an item at the path '" + Path + "'.");
	}
	if(Directory->GetStructure().HasRelation("child") == false)
	{
		throw std::runtime_error("The item '" + Path + "' does not contain a 'child' relation.");
	}
	_ReadItems(Directory->GetStructure().GetRelation("child"), ReaderFunction);
}

void ResourceReader::_ReadItems(Arxx::Structure::Relation & Relation, std::function< void (Arxx::Reference &) > ReaderFunction)
{
	for(auto & Child : Relation)
	{
		ReaderFunction(Child);
	}
}

void ResourceReader::ReadAmmunitionClasses(BlueprintManager * BlueprintManager)
{
	_ReadItems("/Ammunition Classes", std::bind(ReadAmmunitionClass, std::placeholders::_1, BlueprintManager));
}

void ResourceReader::ReadBatteryClasses(BlueprintManager * BlueprintManager)
{
	_ReadItems("/Battery Classes", std::bind(ReadBatteryClass, std::placeholders::_1, BlueprintManager));
}

void ResourceReader::ReadCommodityClasses(BlueprintManager * BlueprintManager)
{
	_ReadItems("/Commodity Classes", std::bind(ReadCommodityClass, std::placeholders::_1, BlueprintManager));
}

void ResourceReader::ReadEnergyProjectileWeaponClasses(BlueprintManager * BlueprintManager)
{
	_ReadItems("/Energy Projectile Weapon Classes", std::bind(ReadEnergyProjectileWeaponClass, std::placeholders::_1, BlueprintManager));
}

Galaxy * ResourceReader::ReadGalaxy(const std::string & GalaxyIdentifier)
{
	assert(_Archive != nullptr);
	
	std::string Path("/Galaxies");
	auto Directory(_Archive->GetItem(Path));
	
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
		auto GalaxyItem(Resolve(Child));
		
		if(GalaxyItem->GetType() != DATA_TYPE_GALAXY)
		{
			throw std::runtime_error("Item type for galaxy '" + GalaxyItem->GetName() + "' should be '" + to_string_cast(DATA_TYPE_GALAXY) + "' not '" + to_string_cast(GalaxyItem->GetType()) + "'.");
		}
		if(GalaxyItem->GetSubType() != 0)
		{
			throw std::runtime_error("Item sub type for galaxy '" + GalaxyItem->GetName() + "' should be '0' not '" + to_string_cast(GalaxyItem->GetSubType()) + "'.");
		}
		
		Arxx::BufferReader Reader(*GalaxyItem);
		std::string Identifier;
		
		Reader >> Identifier;
		if(Identifier == GalaxyIdentifier)
		{
			auto NewGalaxy(dynamic_cast< Galaxy * >(g_ObjectFactory->Create("galaxy", Identifier, false)));
			std::string Name;
			
			Reader >> Name;
			assert(NewGalaxy->GetAspectName() != nullptr);
			NewGalaxy->GetAspectName()->SetName(Name);
			if(GalaxyItem->GetStructure().HasRelation("factions") == false)
			{
				throw std::runtime_error("The item '" + Path + "/" + GalaxyItem->GetName() + "' does not contain a 'factions' relation.");
			}
			
			std::list< std::tuple< std::string, std::string, float > > FactionStandings;
			
			_ReadItems(GalaxyItem->GetStructure().GetRelation("factions"), std::bind(ReadFaction, std::placeholders::_1, NewGalaxy, std::ref(FactionStandings)));
			for(auto & FactionStanding : FactionStandings)
			{
				NewGalaxy->GetFaction(std::get<0>(FactionStanding))->AddFactionStanding(NewGalaxy->GetFaction(std::get<1>(FactionStanding)), std::get<2>(FactionStanding));
			}
			
			std::multimap< std::string, std::string > SystemLinks;
			
			_ReadItems(GalaxyItem->GetStructure().GetRelation("systems"), std::bind(&ResourceReader::_ReadSystem, this, std::placeholders::_1, NewGalaxy, std::ref(SystemLinks)));
			for(auto & SystemLink : SystemLinks)
			{
				NewGalaxy->GetSystem(SystemLink.first)->AddLinkedSystem(NewGalaxy->GetSystem(SystemLink.second));
			}
			
			return NewGalaxy;
		}
	}
	
	return nullptr;
}

void ResourceReader::ReadGeneratorClasses(BlueprintManager * BlueprintManager)
{
	_ReadItems("/Generator Classes", std::bind(ReadGeneratorClass, std::placeholders::_1, BlueprintManager));
}

void ResourceReader::ReadMeshes(void)
{
	_ReadItems("/Meshes", ReadMesh);
}

void ResourceReader::ReadModels(void)
{
	_ReadItems("/Models", ReadModel);
}

void ResourceReader::ReadScenarios(ScenarioManager * ScenarioManager)
{
	_ReadItems("/Scenarios", std::bind(ReadScenario, std::placeholders::_1, ScenarioManager));
}

void ResourceReader::ReadSettings(Settings * Settings)
{
	auto Item(_Archive->GetItem("/Settings"));
	
	if(Item == nullptr)
	{
		throw std::runtime_error("Could not find an item at the path '/Settings'.");
	}
	Settings->LoadFromItem(Item);
}

void ResourceReader::ReadShadersAndPrograms(Graphics::ShadingManager * ShadingManager)
{
	_ReadItems("/Shaders", std::bind(ReadShader, std::placeholders::_1, ShadingManager));
	_ReadItems("/Programs", std::bind(ReadProgram, std::placeholders::_1, ShadingManager));
}

void ResourceReader::ReadShipClasses(BlueprintManager * BlueprintManager, ClassManager< SlotClass > * SlotClassManager)
{
	_ReadItems("/Ship Classes", std::bind(ReadShipClass, std::placeholders::_1, BlueprintManager, SlotClassManager));
}

void ResourceReader::ReadSlotClasses(ClassManager< SlotClass > * SlotClassManager)
{
	_ReadItems("/Slot Classes", std::bind(ReadSlotClass, std::placeholders::_1, SlotClassManager));
}

void ResourceReader::_ReadSystem(Arxx::Reference & Reference, Galaxy * Galaxy, std::multimap< std::string, std::string > & SystemLinks)
{
	Arxx::Item * Item(Resolve(Reference));
	
	if(Item->GetType() != DATA_TYPE_SYSTEM)
	{
		throw std::runtime_error("Item type for system '" + Item->GetName() + "' should be '" + to_string_cast(DATA_TYPE_SYSTEM) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for system '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	std::string Identifier;
	
	Reader >> Identifier;
	
	auto NewSystem(dynamic_cast< System * >(g_ObjectFactory->Create("system", Identifier, false)));
	
	if(NewSystem == nullptr)
	{
		throw std::runtime_error("Could not create system '" + Identifier + "'.");
	}
	
	std::string Name;
	Vector2f Position;
	float TrafficDensity;
	std::uint32_t FactionInfluencesCount;
	
	Reader >> Name >> Position >> TrafficDensity >> FactionInfluencesCount;
	NewSystem->GetAspectName()->SetName(Name);
	NewSystem->GetAspectPosition()->SetPosition(Vector3f::CreateFromComponents(Position[0], Position[1], 0.0f));
	NewSystem->SetTrafficDensity(TrafficDensity);
	for(auto FactionInfluenceIndex = 0ul; FactionInfluenceIndex < FactionInfluencesCount; ++FactionInfluenceIndex)
	{
		std::string FactionIdentifier;
		float FactionInfluence;
		
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
		_ReadItems(Item->GetStructure().GetRelation("stars"), std::bind(ReadStar, std::placeholders::_1, NewSystem));
	}
	if(Item->GetStructure().HasRelation("planets") == true)
	{
		_ReadItems(Item->GetStructure().GetRelation("planets"), std::bind(ReadPlanet, std::placeholders::_1, Galaxy, NewSystem));
	}
	if(Item->GetStructure().HasRelation("linked-systems") == true)
	{
		_ReadItems(Item->GetStructure().GetRelation("linked-systems"), std::bind(ReadSystemLink, std::placeholders::_1, NewSystem, std::ref(SystemLinks)));
	}
}

void ResourceReader::ReadTextures(void)
{
	_ReadItems("/Textures", ReadTexture);
}

void ResourceReader::ReadTurretClasses(BlueprintManager * BlueprintManager)
{
	_ReadItems("/Turret Classes", std::bind(ReadTurretClass, std::placeholders::_1, BlueprintManager));
}

std::string ResourceReader::ReadSavegameFromScenarioPath(const std::string & ScenarioPath)
{
	auto Item(_Archive->GetItem(ScenarioPath));
	std::string Result;
	
	if(Item != nullptr)
	{
		MakeItemAvailable(Item);
		
		Arxx::BufferReader Reader(*Item);
		std::string Identifier;
		std::string Name;
		std::string Description;
		
		Reader >> Identifier >> Name >> Description >> Result;
	}
	
	return Result;
}

static void ReadAmmunitionClass(Arxx::Reference & Reference, BlueprintManager * BlueprintManager)
{
	auto Item(Resolve(Reference));
	
	if(Item->GetType() != DATA_TYPE_AMMUNITION_CLASS)
	{
		throw std::runtime_error("Item type for ammunition class '" + Item->GetName() + "' should be '" + to_string_cast(DATA_TYPE_AMMUNITION_CLASS) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for ammunition class '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	std::string Identifier;
	
	Reader >> Identifier;
	
	auto NewAmmunitionClass(BlueprintManager->Create("ammunition", Identifier));
	
	if(NewAmmunitionClass == nullptr)
	{
		throw std::runtime_error("Could not create ammunition class '" + Identifier + "'.");
	}
	
	std::string Name;
	std::string Description;
	std::uint32_t BasePrice;
	std::uint32_t SpaceRequirement;
	VisualizationPrototype VisualizationPrototype;
	std::uint32_t CartridgeSize;
	
	Reader >> Name >> Description >> BasePrice >> SpaceRequirement >> VisualizationPrototype >> CartridgeSize;
	
	NewAmmunitionClass->AddProperty("name", Name);
	NewAmmunitionClass->AddProperty("description", Description);
	NewAmmunitionClass->AddProperty("base-price", BasePrice);
	NewAmmunitionClass->AddProperty("space-requirement", SpaceRequirement);
	NewAmmunitionClass->AddProperty("visualization-prototype", VisualizationPrototype);
	NewAmmunitionClass->AddProperty("cartridge-size", CartridgeSize);
}

static void ReadBatteryClass(Arxx::Reference & Reference, BlueprintManager * BlueprintManager)
{
	auto Item(Resolve(Reference));
	
	if(Item->GetType() != DATA_TYPE_BATTERY_CLASS)
	{
		throw std::runtime_error("Item type for battery class '" + Item->GetName() + "' should be '" + to_string_cast(DATA_TYPE_BATTERY_CLASS) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for battery class '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	std::string Identifier;
	
	Reader >> Identifier;
	
	auto NewBatteryClass(BlueprintManager->Create("battery", Identifier));
	
	if(NewBatteryClass == nullptr)
	{
		throw std::runtime_error("Could not create battery class '" + Identifier + "'.");
	}
	
	std::string Name;
	std::string Description;
	std::uint32_t BasePrice;
	std::uint32_t SpaceRequirement;
	VisualizationPrototype VisualizationPrototype;
	float EnergyCapacity;
	float MaximumPowerInput;
	float MaximumPowerOutput;
	std::string SlotClassIdentifier;
	
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

static void ReadCommodityClass(Arxx::Reference & Reference, BlueprintManager * BlueprintManager)
{
	auto Item(Resolve(Reference));
	
	if(Item->GetType() != DATA_TYPE_COMMODITY_CLASS)
	{
		throw std::runtime_error("Item type for commodity class '" + Item->GetName() + "' should be '" + to_string_cast(DATA_TYPE_COMMODITY_CLASS) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for commodity class '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	std::string Identifier;
	
	Reader >> Identifier;
	
	auto NewCommodityClass(BlueprintManager->Create("commodity", Identifier));
	
	if(NewCommodityClass == nullptr)
	{
		throw std::runtime_error("Could not create commodity class '" + Identifier + "'.");
	}
	
	std::string Name;
	std::string Description;
	std::uint32_t BasePrice;
	std::uint32_t SpaceRequirement;
	VisualizationPrototype VisualizationPrototype;
	
	Reader >> Name >> Description >> BasePrice >> SpaceRequirement >> VisualizationPrototype;
	
	NewCommodityClass->AddProperty("name", Name);
	NewCommodityClass->AddProperty("description", Description);
	NewCommodityClass->AddProperty("base-price", BasePrice);
	NewCommodityClass->AddProperty("space-requirement", SpaceRequirement);
	NewCommodityClass->AddProperty("visualization-prototype", VisualizationPrototype);
}

static void ReadEnergyProjectileWeaponClass(Arxx::Reference & Reference, BlueprintManager * BlueprintManager)
{
	auto Item(Resolve(Reference));
	
	if(Item->GetType() != DATA_TYPE_ENERGY_PROJECTILE_WEAPON_CLASS)
	{
		throw std::runtime_error("Item type for energy projectile weapon class '" + Item->GetName() + "' should be '" + to_string_cast(DATA_TYPE_ENERGY_PROJECTILE_WEAPON_CLASS) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for energy projectile weapon class '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	std::string Identifier;
	
	Reader >> Identifier;
	
	auto NewEnergyProjectileWeaponBlueprint(BlueprintManager->Create("energy-projectile-weapon", Identifier));
	
	if(NewEnergyProjectileWeaponBlueprint == nullptr)
	{
		throw std::runtime_error("Could not create energy projectile weapon blueprint '" + Identifier + "'.");
	}
	
	std::string Name;
	std::string Description;
	std::uint32_t BasePrice;
	std::uint32_t SpaceRequirement;
	VisualizationPrototype EnergyProjectileWeaponVisualizationPrototype;
	std::string SlotClassIdentifier;
	Quaternion Orientation;
	float MaximumPowerInput;
	float MaximumPowerOutput;
	float EnergyUsagePerShot;
	std::string MuzzlePositionPartIdentifier;
	std::string MuzzlePositionMarkerIdentifier;
	float ShotExitSpeed;
	float ShotDamage;
	float ShotLifeTime;
	VisualizationPrototype ShotVisualizationPrototype;
	
	
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
	
	auto MuzzlePosition{EnergyProjectileWeaponVisualizationPrototype.GetMarkerPosition(MuzzlePositionPartIdentifier, MuzzlePositionMarkerIdentifier)};
	
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

static void ReadFaction(Arxx::Reference & Reference, Galaxy * Galaxy, std::list< std::tuple< std::string, std::string, float > > & FactionStandings)
{
	auto Item(Resolve(Reference));
	
	if(Item->GetType() != DATA_TYPE_FACTION)
	{
		throw std::runtime_error("Item type for faction '" + Item->GetName() + "' should be '" + to_string_cast(DATA_TYPE_FACTION) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for faction '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	std::string Identifier;
	
	Reader >> Identifier;
	
	auto NewFaction(dynamic_cast< Faction * >(g_ObjectFactory->Create("faction", Identifier, false)));
	
	if(NewFaction == nullptr)
	{
		throw std::runtime_error("Could not create faction '" + Identifier + "'.");
	}
	
	std::string Name;
	Graphics::ColorRGBO FactionColor;
	std::uint32_t StandingsCount;
	
	Reader >> Name >> FactionColor >> StandingsCount;
	NewFaction->GetAspectName()->SetName(Name);
	NewFaction->SetColor(FactionColor);
	for(auto StandingsIndex = 0ul; StandingsIndex < StandingsCount; ++StandingsIndex)
	{
		std::string FactionIdentifier;
		float FactionStanding;
		
		Reader >> FactionIdentifier >> FactionStanding;
		FactionStandings.push_back(std::make_tuple(Identifier, FactionIdentifier, FactionStanding));
	}
	Galaxy->GetAspectObjectContainer()->AddContent(NewFaction);
}

static void ReadGeneratorClass(Arxx::Reference & Reference, BlueprintManager * BlueprintManager)
{
	auto Item(Resolve(Reference));
	
	if(Item->GetType() != DATA_TYPE_GENERATOR_CLASS)
	{
		throw std::runtime_error("Item type for generator class '" + Item->GetName() + "' should be '" + to_string_cast(DATA_TYPE_GENERATOR_CLASS) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for generator class '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	std::string Identifier;
	
	Reader >> Identifier;
	
	auto NewGeneratorClass(BlueprintManager->Create("generator", Identifier));
	
	if(NewGeneratorClass == nullptr)
	{
		throw std::runtime_error("Could not create generator class '" + Identifier + "'.");
	}
	
	std::string Name;
	std::string Description;
	std::uint32_t BasePrice;
	std::uint32_t SpaceRequirement;
	VisualizationPrototype VisualizationPrototype;
	float MaximumPowerOutput;
	std::string SlotClassIdentifier;
	
	Reader >> Name >> Description >> BasePrice >> SpaceRequirement >> VisualizationPrototype >> MaximumPowerOutput >> SlotClassIdentifier;
	
	NewGeneratorClass->AddProperty("name", Name);
	NewGeneratorClass->AddProperty("description", Description);
	NewGeneratorClass->AddProperty("base-price", BasePrice);
	NewGeneratorClass->AddProperty("space-requirement", SpaceRequirement);
	NewGeneratorClass->AddProperty("visualization-prototype", VisualizationPrototype);
	NewGeneratorClass->AddProperty("maximum-power-output", MaximumPowerOutput);
	NewGeneratorClass->AddProperty("slot-class-identifier", SlotClassIdentifier);
}

static void ReadMesh(Arxx::Reference & Reference)
{
	auto Item(Resolve(Reference));
	
	if(Item->GetType() != DATA_TYPE_MESH)
	{
		throw std::runtime_error("Item type for mesh '" + Item->GetName() + "' should be '" + to_string_cast(DATA_TYPE_MESH) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for mesh '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	std::string Identifier;
	
	Reader >> Identifier;
	
	auto NewMesh(g_GraphicsEngine->GetMeshManager()->Create(Identifier));
	
	if(NewMesh == nullptr)
	{
		throw std::runtime_error("Could not create mesh '" + Identifier + "'.");
	}
	
	std::map< std::string, std::vector< Vector3f >::size_type > Points;
	std::uint32_t PrimitiveCount;
	
	Reader >> PrimitiveCount;
	for(auto PointIndex = 0ul; PointIndex < PrimitiveCount; ++PointIndex)
	{
		std::string PointIdentifier;
		std::string PointName;
		Vector3f PointCoordinates;
		
		Reader >> PointIdentifier >> PointName >> PointCoordinates;
		Points[PointIdentifier] = NewMesh->AddPoint(PointCoordinates);
	}
	
	std::map< std::string, std::pair< std::vector< Vector3f >::size_type, Vector3f > > TrianglePoints;
	
	Reader >> PrimitiveCount;
	for(auto TrianglePointIndex = 0ul; TrianglePointIndex < PrimitiveCount; ++TrianglePointIndex)
	{
		std::string TrianglePointIdentifier;
		Vector3f TrianglePointNormal;
		std::string PointIdentifier;
		
		Reader >> TrianglePointIdentifier >> TrianglePointNormal >> PointIdentifier;
		TrianglePoints[TrianglePointIdentifier] = std::make_pair(Points[PointIdentifier], TrianglePointNormal);
	}
	
	Reader >> PrimitiveCount;
	for(auto TriangleIndex = 0ul; TriangleIndex < PrimitiveCount; ++TriangleIndex)
	{
		std::string TriangleIdentifier;
		std::string TriangleName;
		std::string TrianglePoint1Identifier;
		std::string TrianglePoint2Identifier;
		std::string TrianglePoint3Identifier;
		
		Reader >> TriangleIdentifier >> TriangleName >> TrianglePoint1Identifier >> TrianglePoint2Identifier >> TrianglePoint3Identifier;
		NewMesh->AddTriangle(TrianglePoints[TrianglePoint1Identifier].first, TrianglePoints[TrianglePoint1Identifier].second, TrianglePoints[TrianglePoint2Identifier].first, TrianglePoints[TrianglePoint2Identifier].second, TrianglePoints[TrianglePoint3Identifier].first, TrianglePoints[TrianglePoint3Identifier].second);
	}
	
	std::uint32_t MarkerCount;
	
	Reader >> MarkerCount;
	for(auto MarkerIndex = 0ul; MarkerIndex < MarkerCount; ++MarkerIndex)
	{
		std::string MarkerIdentifier;
		bool MarkerLengthValid;
		float MarkerLength;
		bool MarkerPositionValid;
		Vector3f MarkerPosition;
		bool MarkerOrientationValid;
		Quaternion MarkerOrientation;
		
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

static void ReadModel(Arxx::Reference & Reference)
{
	auto Item(Resolve(Reference));
	
	if(Item->GetType() != DATA_TYPE_MODEL)
	{
		throw std::runtime_error("Item type for model '" + Item->GetName() + "' should be '" + to_string_cast(DATA_TYPE_MODEL) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for model '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	std::string Identifier;
	
	Reader >> Identifier;
	
	Graphics::Model * NewModel(g_GraphicsEngine->GetModelManager()->Create(Identifier));
	
	if(NewModel == nullptr)
	{
		throw std::runtime_error("Could not create model '" + Identifier + "'.");
	}
	
	std::uint32_t PartCount;
	
	Reader >> PartCount;
	for(auto PartIndex = 0ul; PartIndex < PartCount; ++PartIndex)
	{
		std::string PartIdentifier;
		std::string PartMeshIdentifier;
		Vector3f PartPosition;
		Quaternion PartOrientation;
		
		Reader >> PartIdentifier >> PartMeshIdentifier >> PartPosition >> PartOrientation;
		
		auto PartMesh(g_GraphicsEngine->GetMeshManager()->Get(PartMeshIdentifier));
		
		if(PartMesh == nullptr)
		{
			throw std::runtime_error("For model '" + Identifier + "' could not find the mesh '" + PartMeshIdentifier + "' for part '" + PartIdentifier + "'.");
		}
		NewModel->AddPart(PartIdentifier, PartMesh, PartPosition, PartOrientation);
	}
}

static void ReadPlanet(Arxx::Reference & Reference, Galaxy * Galaxy, System * System)
{
	Arxx::Item * Item(Resolve(Reference));
	
	if(Item->GetType() != DATA_TYPE_PLANET)
	{
		throw std::runtime_error("Item type for planet '" + Item->GetName() + "' should be '" + to_string_cast(DATA_TYPE_PLANET) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for planet '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	std::string Identifier;
	
	Reader >> Identifier;
	
	auto NewPlanet(dynamic_cast< Planet * >(g_ObjectFactory->Create("planet", Identifier, false)));
	
	if(NewPlanet == nullptr)
	{
		throw std::runtime_error("Could not create planet '" + Identifier + "'.");
	}
	
	std::string Name;
	std::string Description;
	VisualizationPrototype VisualizationPrototype;
	
	Reader >> Name >> Description >> VisualizationPrototype;
	NewPlanet->GetAspectName()->SetName(Name);
	NewPlanet->SetDescription(Description);
	assert(NewPlanet->GetAspectVisualization() != nullptr);
	assert(NewPlanet->GetAspectVisualization()->GetVisualizationPrototype() == nullptr);
	NewPlanet->GetAspectVisualization()->SetVisualizationPrototype(VisualizationPrototype);
	
	Vector2f PlanetPosition;
	float Size;
	std::uint32_t PlanetAssetsCount;
	
	Reader >> PlanetPosition >> Size >> PlanetAssetsCount;
	NewPlanet->GetAspectPosition()->SetPosition(Vector3f::CreateFromComponents(PlanetPosition[0], PlanetPosition[1], 0.0f));
	NewPlanet->SetSize(Size);
	for(auto PlanetAssetsIndex = 0ul; PlanetAssetsIndex < PlanetAssetsCount; ++PlanetAssetsIndex)
	{
		std::string AssetsTypeIdentifier;
		std::string AssetsSubTypeIdentifier;
		float BasePriceModifier;
		
		Reader >> AssetsTypeIdentifier >> AssetsSubTypeIdentifier >> BasePriceModifier;
		if(g_BlueprintManager->Has(AssetsTypeIdentifier, AssetsSubTypeIdentifier) == true)
		{
			auto NewPlanetAssets{NewPlanet->CreatePlanetAssets(AssetsTypeIdentifier, AssetsSubTypeIdentifier)};
			
			NewPlanetAssets->SetBasePriceModifier(BasePriceModifier);
		}
		else
		{
			throw std::runtime_error("For the assets of planet \"" + Name + "\", don't know a class of type '" + AssetsTypeIdentifier + "' and sub type '" + AssetsSubTypeIdentifier + "'.");
		}
	}
	
	float LandingFeePerSpace;
	bool OffersRecharging;
	float RechargingFeePerEnergy;
	bool OffersRepairing;
	float RepairingFeePerHull;
	std::string FactionIdentifier;
	
	Reader >> LandingFeePerSpace >> OffersRecharging >> RechargingFeePerEnergy >> OffersRepairing >> RepairingFeePerHull >> FactionIdentifier;
	NewPlanet->SetLandingFeePerSpace(LandingFeePerSpace / 1000.0f);
	NewPlanet->SetOffersRecharging(OffersRecharging);
	NewPlanet->SetRechargingFeePerEnergy(RechargingFeePerEnergy);
	NewPlanet->SetOffersRepairing(OffersRepairing);
	NewPlanet->SetRepairingFeePerHull(RepairingFeePerHull);
	
	auto Faction(Galaxy->GetFaction(FactionIdentifier));
	
	if(Faction == nullptr)
	{
		throw std::runtime_error("Could not find faction '" + FactionIdentifier + "' for planet '" + Identifier + "' in system '" + System->GetSubTypeIdentifier() + "'.");
	}
	NewPlanet->SetFaction(Faction);
	System->GetAspectObjectContainer()->AddContent(NewPlanet);
}

static void ReadProgram(Arxx::Reference & Reference, Graphics::ShadingManager * ShadingManager)
{
	auto Item(Resolve(Reference));
	
	if(Item->GetType() != DATA_TYPE_SHADER_PROGRAM)
	{
		throw std::runtime_error("Item type for shader '" + Item->GetName() + "' should be '" + to_string_cast(DATA_TYPE_SHADER_PROGRAM) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	std::string Identifier;
	
	Reader >> Identifier;
	
	auto Program(ShadingManager->CreateProgram(Identifier));
	
	if(Program == nullptr)
	{
		throw std::runtime_error("Could not create program '" + Identifier + "'.");
	}
	
	std::uint32_t NumberOfShaders;
	
	Reader >> NumberOfShaders;
	for(std::uint32_t ShaderNumber = 1; ShaderNumber <= NumberOfShaders; ++ShaderNumber)
	{
		std::string ShaderIdentifier;
		
		Reader >> ShaderIdentifier;
		Program->AddShaderIdentifier(ShaderIdentifier);
	}
}

static void ReadScenario(Arxx::Reference & Reference, ScenarioManager * ScenarioManager)
{
	auto Item(Resolve(Reference));
	
	if(Item->GetType() != DATA_TYPE_SCENARIO)
	{
		throw std::runtime_error("Item type for scenario '" + Item->GetName() + "' should be '" + to_string_cast(DATA_TYPE_SCENARIO) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for scenario '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	std::string Identifier;
	
	Reader >> Identifier;
	
	auto NewScenario(ScenarioManager->Create(Identifier));
	
	if(NewScenario == nullptr)
	{
		throw std::runtime_error("Could not create scenario '" + Identifier + "'.");
	}
	NewScenario->SetResourcePath("/Scenarios/" + Item->GetName());
	
	std::string Name;
	std::string Description;
	
	Reader >> Name >> Description;
	NewScenario->SetName(Name);
	NewScenario->SetDescription(Description);
}

static void ReadShader(Arxx::Reference & Reference, Graphics::ShadingManager * ShadingManager)
{
	auto Item(Resolve(Reference));
	
	if(Item->GetType() != DATA_TYPE_SHADER)
	{
		throw std::runtime_error("Item type for shader '" + Item->GetName() + "' should be '" + to_string_cast(DATA_TYPE_SHADER) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	std::string Identifier;
	
	Reader >> Identifier;
	
	auto Shader(ShadingManager->CreateShader(Identifier));
	
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
	
	std::string Source;
	
	Reader >> Source;
	Shader->SetSource(Source);
}

static void ReadShipClass(Arxx::Reference & Reference, BlueprintManager * BlueprintManager, ClassManager< SlotClass > * SlotClassManager)
{
	auto Item(Resolve(Reference));
	
	if(Item->GetType() != DATA_TYPE_SHIP_CLASS)
	{
		throw std::runtime_error("Item type for ship class '" + Item->GetName() + "' should be '" + to_string_cast(DATA_TYPE_SHIP_CLASS) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for ship class '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	std::string Identifier;
	
	Reader >> Identifier;
	
	auto NewShipClass(BlueprintManager->Create("ship", Identifier));
	
	if(NewShipClass == nullptr)
	{
		throw std::runtime_error("Could not create ship class '" + Identifier + "'.");
	}
	// read the name aspect
	std::string Name;
	std::string Description;
	std::uint32_t BasePrice;
	std::uint32_t SpaceRequirement;
	VisualizationPrototype VisualizationPrototype;
	float FuelCapacity;
	float ForwardThrust;
	float TurnSpeed;
	float MaximumSpeed;
	std::uint32_t MaximumAvailableSpace;
	float JumpFuel;
	float ForwardFuel;
	float TurnFuel;
	float Hull;
	std::string ExhaustMarkerPartIdentifier;
	std::string ExhaustMarkerIdentifier;
	std::uint32_t SlotCount;
	
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
	
	std::list< Properties > Slots;
	
	for(auto SlotIndex = 0ul; SlotIndex < SlotCount; ++SlotIndex)
	{
		Properties NewSlot;
		std::string SlotIdentifier;
		std::string SlotClassIdentifier;
		
		Reader >> SlotIdentifier >> SlotClassIdentifier;
		
		NewSlot.AddProperty("identifier", SlotIdentifier);
		
		auto SlotClass(SlotClassManager->Get(SlotClassIdentifier));
		
		if(SlotClass == nullptr)
		{
			throw std::runtime_error("Could not get slot class '" + SlotClassIdentifier + "' for slot '" + SlotIdentifier + "' of ship class '" + Identifier + "'.");
		}
		NewSlot.AddProperty("class", SlotClass);
		
		std::string SlotName;
		std::string SlotMarkerPartIdentifier;
		std::string SlotMarkerIdentifier;
		bool VisualizeAccessory;
		
		Reader >> SlotName >> SlotMarkerPartIdentifier >> SlotMarkerIdentifier >> VisualizeAccessory;
		NewSlot.AddProperty("name", SlotName);
	
		auto SlotPosition{VisualizationPrototype.GetMarkerPosition(SlotMarkerPartIdentifier, SlotMarkerIdentifier)};

		if(SlotPosition == nullptr)
		{
			throw std::runtime_error("For the ship '" + Identifier + "' and slot '" + SlotName + "', could not find the marker or its position for the slot position '" + SlotMarkerIdentifier + "' on the part '" + SlotMarkerPartIdentifier + "'.");
		}
		NewSlot.AddProperty("position", *SlotPosition);
	
		auto SlotOrientation{VisualizationPrototype.GetMarkerOrientation(SlotMarkerPartIdentifier, SlotMarkerIdentifier)};

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

static void ReadSlotClass(Arxx::Reference & Reference, ClassManager< SlotClass > * SlotClassManager)
{
	auto Item(Resolve(Reference));
	
	if(Item->GetType() != DATA_TYPE_SLOT_CLASS)
	{
		throw std::runtime_error("Item type for slot class '" + Item->GetName() + "' should be '" + to_string_cast(DATA_TYPE_SLOT_CLASS) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for slot class '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	std::string Identifier;
	
	Reader >> Identifier;
	
	auto NewSlotClass(SlotClassManager->Create(Identifier));
	
	if(NewSlotClass == nullptr)
	{
		throw std::runtime_error("Could not create slot class '" + Identifier + "'.");
	}
	
	std::string Name;
	std::uint32_t AcceptedSlotClassIdentifierCount;
	
	Reader >> Name >> AcceptedSlotClassIdentifierCount;
	NewSlotClass->SetName(Name);
	for(auto AcceptedSlotClassIdentifierIndex = 0ul; AcceptedSlotClassIdentifierIndex < AcceptedSlotClassIdentifierCount; ++AcceptedSlotClassIdentifierIndex)
	{
		std::string AcceptedSlotClassIdentifier;
		
		Reader >> AcceptedSlotClassIdentifier;
		NewSlotClass->AddAcceptedSlotClassIdentifier(AcceptedSlotClassIdentifier);
	}
}

static void ReadStar(Arxx::Reference & Reference, System * System)
{
	Arxx::Item * Item(Resolve(Reference));
	
	if(Item->GetType() != DATA_TYPE_STAR)
	{
		throw std::runtime_error("Item type for star '" + Item->GetName() + "' should be '" + to_string_cast(DATA_TYPE_STAR) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for star '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	std::string Identifier;
	Vector2f Position;
	Graphics::ColorRGB Color;
	
	Reader >> Identifier >> Position >> Color;
	
	auto NewStar(dynamic_cast< Star * >(g_ObjectFactory->Create("star", Identifier, false)));
	
	NewStar->GetAspectPosition()->SetPosition(Vector3f::CreateFromComponents(Position[0], Position[1], 0.0f));
	NewStar->SetColor(Color);
	System->GetAspectObjectContainer()->AddContent(NewStar);
}

static void ReadSystemLink(Arxx::Reference & Reference, System * System, std::multimap< std::string, std::string > & SystemLinks)
{
	Arxx::Item * Item(Resolve(Reference));
	
	if(Item->GetType() != DATA_TYPE_SYSTEM)
	{
		throw std::runtime_error("Item type for system '" + Item->GetName() + "' should be '" + to_string_cast(DATA_TYPE_SYSTEM) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for system '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	std::string Identifier;
	
	Reader >> Identifier;
	SystemLinks.insert(std::make_pair(System->GetSubTypeIdentifier(), Identifier));
}

static void ReadTexture(Arxx::Reference & Reference)
{
	auto Item(Resolve(Reference));
	
	if(Item->GetType() != DATA_TYPE_TEXTURE)
	{
		throw std::runtime_error("Item type for texture '" + Item->GetName() + "' should be '" + to_string_cast(DATA_TYPE_TEXTURE) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for texture '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	std::string Identifier;
	std::uint32_t Width;
	std::uint32_t Height;
	std::uint32_t Format;
	
	Reader >> Identifier >> Width >> Height >> Format;
	Width = ntohl(Width);
	Height = ntohl(Height);
	Format = ntohl(Format);
	
	auto Texture(g_GraphicsEngine->GetTextureManager()->Create(Identifier));
	
	if(Texture == nullptr)
	{
		throw std::runtime_error("Could not create texture '" + Identifier + "'.");
	}
	Texture->SetData(Width, Height, Format, Reader.GetBuffer().GetBegin() + Reader.GetPosition());
}

static void ReadTurretClass(Arxx::Reference & Reference, BlueprintManager * BlueprintManager)
{
	Arxx::Item * Item(Resolve(Reference));
	
	if(Item->GetType() != DATA_TYPE_TURRET_CLASS)
	{
		throw std::runtime_error("Item type for turret class '" + Item->GetName() + "' should be '" + to_string_cast(DATA_TYPE_TURRET_CLASS) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for turret class '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	std::string Identifier;
	
	Reader >> Identifier;
	
	auto NewTurretClass(BlueprintManager->Create("turret", Identifier));
	
	if(NewTurretClass == nullptr)
	{
		throw std::runtime_error("Could not create turret class '" + Identifier + "'.");
	}
	
	std::string Name;
	std::string Description;
	std::uint32_t BasePrice;
	std::uint32_t SpaceRequirement;
	VisualizationPrototype TurretVisualizationPrototype;
	std::string SlotClassIdentifier;
	Quaternion Orientation;
	float MaximumPowerInput;
	float MaximumPowerOutput;
	float EnergyUsagePerShot;
	std::string MuzzlePositionPartIdentifier;
	std::string MuzzlePositionMarkerIdentifier;
	float ShotExitSpeed;
	float ShotDamage;
	float ShotLifeTime;
	VisualizationPrototype ShotVisualizationPrototype;
	
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
	
	auto MuzzlePosition{TurretVisualizationPrototype.GetMarkerPosition(MuzzlePositionPartIdentifier, MuzzlePositionMarkerIdentifier)};
	
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
