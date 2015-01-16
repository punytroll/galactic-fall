/**
 * galactic-fall
 * Copyright (C) 2015  Hagen Möbius
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

#include <Archive.h>
#include <Item.h>

#include <string_cast/string_cast.h>

#include "arx_types.h"
#include "asset_class.h"
#include "battery_class.h"
#include "buffer_reading.h"
#include "class_manager.h"
#include "commodity_class.h"
#include "faction.h"
#include "galaxy.h"
#include "generator_class.h"
#include "globals.h"
#include "graphics/engine.h"
#include "graphics/mesh.h"
#include "graphics/mesh_manager.h"
#include "graphics/model.h"
#include "graphics/model_manager.h"
#include "graphics/program.h"
#include "graphics/shader.h"
#include "graphics/shading_manager.h"
#include "graphics/style.h"
#include "graphics/texture.h"
#include "graphics/texture_manager.h"
#include "object_aspect_name.h"
#include "object_aspect_object_container.h"
#include "object_aspect_position.h"
#include "object_aspect_visualization.h"
#include "object_factory.h"
#include "planet.h"
#include "resource_reader.h"
#include "scenario.h"
#include "scenario_manager.h"
#include "settings.h"
#include "ship_class.h"
#include "slot.h"
#include "slot_class.h"
#include "star.h"
#include "system.h"
#include "visualization_prototype.h"
#include "weapon_class.h"

static Arxx::Item * Resolve(Arxx::Reference & Reference);

static void ReadAssetClass(Arxx::Reference & Reference, ClassManager< AssetClass > * AssetClassManager);
static void ReadBatteryClass(Arxx::Reference & Reference, ClassManager< BatteryClass > * BatteryClassManager);
static void ReadCommodityClass(Arxx::Reference & Reference, ClassManager< CommodityClass > * CommodityClassManager);
static void ReadFaction(Arxx::Reference & Reference);
static void ReadGeneratorClass(Arxx::Reference & Reference, ClassManager< GeneratorClass > * GeneratorClassManager);
static void ReadMesh(Arxx::Reference & Reference);
static void ReadModel(Arxx::Reference & Reference);
static void ReadProgram(Arxx::Reference & Reference, Graphics::ShadingManager * ShadingManager);
static void ReadScenario(Arxx::Reference & Reference, ScenarioManager * ScenarioManager);
static void ReadShader(Arxx::Reference & Reference, Graphics::ShadingManager * ShadingManager);
static void ReadShipClass(Arxx::Reference & Reference, ClassManager< ShipClass > * ShipClassManager, ClassManager< SlotClass > * SlotClassManager);
static void ReadSlotClass(Arxx::Reference & Reference, ClassManager< SlotClass > * SlotClassManager);
static void ReadSystem(Arxx::Reference & Reference, ClassManager< AssetClass > * AssetClassManager);
static void ReadSystemLink(Arxx::Reference & Reference);
static void ReadTexture(Arxx::Reference & Reference);
static void ReadWeaponClass(Arxx::Reference & Reference, ClassManager< WeaponClass > * WeaponClassManager);

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
	auto Item(Reference.pGetItem());
	
	if(Item == nullptr)
	{
		throw std::runtime_error("The unique identifier '" + to_string_cast(Reference.u4GetUniqueID()) + "' is referenced but could not be resolved.");
	}
	MakeItemAvailable(Item);
	
	return Item;
}

ResourceReader::ResourceReader(const std::string & DataDirectoryPath) :
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
	if(Directory->GetStructure().bHasRelation("child") == false)
	{
		throw std::runtime_error("The directory '" + Path + "' does not contain a 'child' relation.");
	}
	for(auto & Child : Directory->GetStructure().GetRelation("child"))
	{
		ReaderFunction(Child);
	}
}

void ResourceReader::ReadAssetClasses(ClassManager< AssetClass > * AssetClassManager)
{
	_ReadItems("/Asset Classes", std::bind(ReadAssetClass, std::placeholders::_1, AssetClassManager));
}

void ResourceReader::ReadBatteryClasses(ClassManager< BatteryClass > * BatteryClassManager)
{
	_ReadItems("/Battery Classes", std::bind(ReadBatteryClass, std::placeholders::_1, BatteryClassManager));
}

void ResourceReader::ReadCommodityClasses(ClassManager< CommodityClass > * CommodityClassManager)
{
	_ReadItems("/Commodity Classes", std::bind(ReadCommodityClass, std::placeholders::_1, CommodityClassManager));
}

void ResourceReader::ReadFactions(void)
{
	_ReadItems("/Factions", ReadFaction);
}

void ResourceReader::ReadGeneratorClasses(ClassManager< GeneratorClass > * GeneratorClassManager)
{
	_ReadItems("/Generator Classes", std::bind(ReadGeneratorClass, std::placeholders::_1, GeneratorClassManager));
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

void ResourceReader::ReadShipClasses(ClassManager< ShipClass > * ShipClassManager, ClassManager< SlotClass > * SlotClassManager)
{
	_ReadItems("/Ship Classes", std::bind(ReadShipClass, std::placeholders::_1, ShipClassManager, SlotClassManager));
}

void ResourceReader::ReadSlotClasses(ClassManager< SlotClass > * SlotClassManager)
{
	_ReadItems("/Slot Classes", std::bind(ReadSlotClass, std::placeholders::_1, SlotClassManager));
}

void ResourceReader::ReadSystems(ClassManager< AssetClass > * AssetClassManager)
{
	_ReadItems("/Systems", std::bind(ReadSystem, std::placeholders::_1, AssetClassManager));
}

void ResourceReader::ReadSystemLinks(void)
{
	_ReadItems("/System Links", ReadSystemLink);
}

void ResourceReader::ReadTextures(void)
{
	_ReadItems("/Textures", ReadTexture);
}

void ResourceReader::ReadWeaponClasses(ClassManager< WeaponClass > * WeaponClassManager)
{
	_ReadItems("/Weapon Classes", std::bind(ReadWeaponClass, std::placeholders::_1, WeaponClassManager));
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

static void ReadAssetClass(Arxx::Reference & Reference, ClassManager< AssetClass > * AssetClassManager)
{
	auto Item(Resolve(Reference));
	
	if(Item->GetType() != DATA_TYPE_ASSET_CLASS)
	{
		throw std::runtime_error("Item type for asset class '" + Item->GetName() + "' should be '" + to_string_cast(DATA_TYPE_ASSET_CLASS) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for asset class '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	std::string Identifier;
	
	Reader >> Identifier;
	
	auto NewAssetClass(AssetClassManager->Create(Identifier));
	
	if(NewAssetClass == nullptr)
	{
		throw std::runtime_error("Could not create asset class '" + Identifier + "'.");
	}
	
	std::string Name;
	float BasePrice;
	std::string Description;
	std::string ObjectTypeIdentifier;
	std::string ObjectClassIdentifier;
	
	Reader >> Name >> BasePrice >> Description >> ObjectTypeIdentifier >> ObjectClassIdentifier;
	
	NewAssetClass->SetName(Name);
	NewAssetClass->SetBasePrice(BasePrice);
	NewAssetClass->SetDescription(Description);
	NewAssetClass->SetObjectTypeIdentifier(ObjectTypeIdentifier);
	NewAssetClass->SetObjectClassIdentifier(ObjectClassIdentifier);
}

static void ReadBatteryClass(Arxx::Reference & Reference, ClassManager< BatteryClass > * BatteryClassManager)
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
	
	auto NewBatteryClass(BatteryClassManager->Create(Identifier));
	
	if(NewBatteryClass == nullptr)
	{
		throw std::runtime_error("Could not create battery class '" + Identifier + "'.");
	}
	
	std::string Name;
	VisualizationPrototype VisualizationPrototype;
	Arxx::u4byte SpaceRequirement;
	float EnergyCapacity;
	std::string SlotClassIdentifier;
	
	Reader >> Name >> VisualizationPrototype >> SpaceRequirement >> EnergyCapacity >> SlotClassIdentifier;
	NewBatteryClass->SetName(Name);
	NewBatteryClass->SetVisualizationPrototype(VisualizationPrototype);
	NewBatteryClass->SetSpaceRequirement(SpaceRequirement);
	NewBatteryClass->SetEnergyCapacity(EnergyCapacity);
	NewBatteryClass->SetSlotClassIdentifier(SlotClassIdentifier);
}

static void ReadCommodityClass(Arxx::Reference & Reference, ClassManager< CommodityClass > * CommodityClassManager)
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
	
	auto NewCommodityClass(CommodityClassManager->Create(Identifier));
	
	if(NewCommodityClass == nullptr)
	{
		throw std::runtime_error("Could not create commodity class '" + Identifier + "'.");
	}
	
	std::string Name;
	Arxx::u4byte SpaceRequirement;
	VisualizationPrototype VisualizationPrototype;
	
	Reader >> Name >> SpaceRequirement >> VisualizationPrototype;
	
	NewCommodityClass->SetName(Name);
	NewCommodityClass->SetSpaceRequirement(SpaceRequirement);
	NewCommodityClass->SetVisualizationPrototype(VisualizationPrototype);
}

static void ReadFaction(Arxx::Reference & Reference)
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
	NewFaction->SetObjectIdentifier("::faction(" + NewFaction->GetClassIdentifier() + ")");
	
	std::string Name;
	Color FactionColor;
	
	Reader >> Name >> FactionColor;
	NewFaction->GetAspectName()->SetName(Name);
	NewFaction->SetColor(FactionColor);
	g_Galaxy->GetAspectObjectContainer()->AddContent(NewFaction);
}

static void ReadGeneratorClass(Arxx::Reference & Reference, ClassManager< GeneratorClass > * GeneratorClassManager)
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
	
	auto NewGeneratorClass(GeneratorClassManager->Create(Identifier));
	
	if(NewGeneratorClass == nullptr)
	{
		throw std::runtime_error("Could not create generator class '" + Identifier + "'.");
	}
	
	std::string Name;
	Arxx::u4byte SpaceRequirement;
	float EnergyProvisionPerSecond;
	std::string SlotClassIdentifier;
	
	Reader >> Name >> SpaceRequirement >> EnergyProvisionPerSecond >> SlotClassIdentifier;
	
	NewGeneratorClass->SetName(Name);
	NewGeneratorClass->SetSpaceRequirement(SpaceRequirement);
	NewGeneratorClass->SetEnergyProvisionPerSecond(EnergyProvisionPerSecond);
	NewGeneratorClass->SetSlotClassIdentifier(SlotClassIdentifier);
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
	Arxx::u4byte Count;
	
	Reader >> Count;
	for(Arxx::u4byte Number = 1; Number <= Count; ++Number)
	{
		std::string PointIdentifier;
		std::string PointName;
		Vector3f PointCoordinates;
		
		Reader >> PointIdentifier >> PointName >> PointCoordinates;
		Points[PointIdentifier] = NewMesh->AddPoint(PointCoordinates);
	}
	
	std::map< std::string, std::pair< std::vector< Vector3f >::size_type, Vector3f > > TrianglePoints;
	
	Reader >> Count;
	for(Arxx::u4byte Number = 1; Number <= Count; ++Number)
	{
		std::string TrianglePointIdentifier;
		Vector3f TrianglePointNormal;
		std::string PointIdentifier;
		
		Reader >> TrianglePointIdentifier >> TrianglePointNormal >> PointIdentifier;
		TrianglePoints[TrianglePointIdentifier] = std::make_pair(Points[PointIdentifier], TrianglePointNormal);
	}
	
	Reader >> Count;
	for(Arxx::u4byte Number = 1; Number <= Count; ++Number)
	{
		std::string TriangleIdentifier;
		std::string TriangleName;
		std::string TrianglePoint1Identifier;
		std::string TrianglePoint2Identifier;
		std::string TrianglePoint3Identifier;
		
		Reader >> TriangleIdentifier >> TriangleName >> TrianglePoint1Identifier >> TrianglePoint2Identifier >> TrianglePoint3Identifier;
		NewMesh->AddTriangle(TrianglePoints[TrianglePoint1Identifier].first, TrianglePoints[TrianglePoint1Identifier].second, TrianglePoints[TrianglePoint2Identifier].first, TrianglePoints[TrianglePoint2Identifier].second, TrianglePoints[TrianglePoint3Identifier].first, TrianglePoints[TrianglePoint3Identifier].second);
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
	
	Arxx::u4byte Count;
	
	Reader >> Count;
	for(Arxx::u4byte Number = 1; Number <= Count; ++Number)
	{
		std::string PartIdentifier;
		std::string MeshIdentifier;
		
		Reader >> PartIdentifier >> MeshIdentifier;
		
		const Graphics::Mesh * Mesh(g_GraphicsEngine->GetMeshManager()->Get(MeshIdentifier));
		
		if(Mesh == nullptr)
		{
			throw std::runtime_error("For model '" + Identifier + "' could not find the mesh '" + MeshIdentifier + "' for part '" + PartIdentifier + "'.");
		}
		NewModel->AddMesh(PartIdentifier, Mesh);
	}
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

static void ReadShipClass(Arxx::Reference & Reference, ClassManager< ShipClass > * ShipClassManager, ClassManager< SlotClass > * SlotClassManager)
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
	
	auto NewShipClass(ShipClassManager->Create(Identifier));
	
	if(NewShipClass == nullptr)
	{
		throw std::runtime_error("Could not create ship class '" + Identifier + "'.");
	}
	// read the name aspect
	std::string Name;
	Arxx::u4byte SpaceRequirement;
	VisualizationPrototype VisualizationPrototype;
	float FuelCapacity;
	float ForwardThrust;
	float TurnSpeed;
	float MaximumSpeed;
	float MaximumAvailableSpace;
	float JumpFuel;
	float ForwardFuel;
	float TurnFuel;
	float Hull;
	Vector3f ExhaustOffset;
	float ExhaustRadius;
	Arxx::u4byte SlotCount;
	
	Reader >> Name >> SpaceRequirement >> VisualizationPrototype >> ForwardThrust >> TurnSpeed >> MaximumSpeed >> MaximumAvailableSpace >> FuelCapacity >> JumpFuel >> ForwardFuel >> TurnFuel >> Hull >> ExhaustOffset >> ExhaustRadius >> SlotCount;
	NewShipClass->SetName(Name);
	NewShipClass->SetSpaceRequirement(SpaceRequirement);
	NewShipClass->SetVisualizationPrototype(VisualizationPrototype);
	NewShipClass->SetFuelCapacity(FuelCapacity);
	NewShipClass->SetForwardThrust(ForwardThrust);
	NewShipClass->SetJumpFuel(JumpFuel);
	NewShipClass->SetMaximumSpeed(MaximumSpeed);
	NewShipClass->SetMaximumAvailableSpace(static_cast< unsigned_numeric >(1000 * MaximumAvailableSpace));
	NewShipClass->SetTurnSpeed(TurnSpeed);
	NewShipClass->SetForwardFuel(ForwardFuel);
	NewShipClass->SetTurnFuel(TurnFuel);
	NewShipClass->SetHull(Hull);
	NewShipClass->SetExhaustOffset(ExhaustOffset);
	NewShipClass->SetExhaustRadius(ExhaustRadius);
	for(Arxx::u4byte SlotNumber = 1; SlotNumber <= SlotCount; ++SlotNumber)
	{
		std::string SlotIdentifier;
		std::string SlotClassIdentifier;
		
		Reader >> SlotIdentifier >> SlotClassIdentifier;
		
		auto SlotClass(SlotClassManager->Get(SlotClassIdentifier));
		
		if(SlotClass == nullptr)
		{
			throw std::runtime_error("Could not get slot class '" + SlotClassIdentifier + "' for slot '" + SlotIdentifier + "' of ship class '" + Identifier + "'.");
		}
		
		std::string Name;
		Vector3f SlotPosition;
		Quaternion SlotOrientation;
		bool VisualizeAccessory;
		
		Reader >> Name >> SlotPosition >> SlotOrientation >> VisualizeAccessory;
		
		auto NewSlot(new Slot(SlotClass, SlotIdentifier));
		
		NewSlot->SetName(Name);
		NewSlot->SetPosition(SlotPosition);
		NewSlot->SetOrientation(SlotOrientation);
		NewSlot->SetVisualizeAccessory(VisualizeAccessory);
		if(NewShipClass->AddSlot(SlotIdentifier, NewSlot) == false)
		{
			throw std::runtime_error("Could not add slot '" + SlotIdentifier + "' to ship class '" + Identifier + "'.");
		}
	}
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
	Arxx::u4byte AcceptedSlotClassIdentifierCount;
	
	Reader >> Name >> AcceptedSlotClassIdentifierCount;
	NewSlotClass->SetName(Name);
	for(Arxx::u4byte AcceptedSlotClassIdentifierNumber = 1; AcceptedSlotClassIdentifierNumber <= AcceptedSlotClassIdentifierCount; ++AcceptedSlotClassIdentifierNumber)
	{
		std::string AcceptedSlotClassIdentifier;
		
		Reader >> AcceptedSlotClassIdentifier;
		NewSlotClass->AddAcceptedSlotClassIdentifier(AcceptedSlotClassIdentifier);
	}
}

static void ReadSystem(Arxx::Reference & Reference, ClassManager< AssetClass > * AssetClassManager)
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
	NewSystem->SetObjectIdentifier("::system(" + NewSystem->GetClassIdentifier() + ")");
	
	std::string Name;
	Vector2f Position;
	float TrafficDensity;
	std::string StarIdentifier;
	Vector2f StarPosition;
	Color StarColor;
	Arxx::u4byte PlanetCount;
	
	Reader >> Name >> Position >> TrafficDensity >> StarIdentifier >> StarPosition >> StarColor >> PlanetCount;
	NewSystem->GetAspectName()->SetName(Name);
	NewSystem->GetAspectPosition()->SetPosition(Vector3f(Position[0], Position[1], 0.0f));
	NewSystem->SetTrafficDensity(TrafficDensity);
	
	Star * NewStar(dynamic_cast< Star * >(g_ObjectFactory->Create("star", StarIdentifier, false)));
	
	NewStar->SetObjectIdentifier("::star(" + StarIdentifier + ")::in_system(" + NewSystem->GetClassIdentifier() + ")");
	NewStar->GetAspectPosition()->SetPosition(Vector3f(StarPosition[0], StarPosition[1], 0.0f));
	NewStar->SetColor(StarColor);
	NewSystem->GetAspectObjectContainer()->AddContent(NewStar);
	for(Arxx::u4byte Number = 1; Number <= PlanetCount; ++Number)
	{
		std::string PlanetIdentifier;
		
		Reader >> PlanetIdentifier;
		
		auto NewPlanet(dynamic_cast< Planet * >(g_ObjectFactory->Create("planet", PlanetIdentifier, false)));
		
		if(NewPlanet == nullptr)
		{
			throw std::runtime_error("Could not create planet '" + PlanetIdentifier + "'.");
		}
		NewPlanet->SetObjectIdentifier("::planet(" + NewPlanet->GetClassIdentifier() + ")::in_system(" + NewSystem->GetClassIdentifier() + ")");
		
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
		Arxx::u4byte OfferedAssetsCount;
		
		Reader >> PlanetPosition >> Size >> OfferedAssetsCount;
		NewPlanet->GetAspectPosition()->SetPosition(Vector3f(PlanetPosition[0], PlanetPosition[1], 0.0f));
		NewPlanet->SetSize(Size);
		for(Arxx::u4byte OfferedAssetNumber = 1; OfferedAssetNumber <= OfferedAssetsCount; ++OfferedAssetNumber)
		{
			std::string AssetClassIdentifier;
			float BasePriceModifier;
			
			Reader >> AssetClassIdentifier >> BasePriceModifier;
			
			auto AssetClass(AssetClassManager->Get(AssetClassIdentifier));
			
			if(AssetClass == nullptr)
			{
				throw std::runtime_error("Could not find asset class '" + AssetClassIdentifier + "' for planet '" + PlanetIdentifier + "' in system '" + Identifier + "'.");
			}
			
			auto NewPlanetAssetClass(NewPlanet->CreatePlanetAssetClass(AssetClass));
			
			NewPlanetAssetClass->SetBasePriceModifier(BasePriceModifier);
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
		
		auto Faction(g_Galaxy->GetFaction(FactionIdentifier));
		
		if(Faction == nullptr)
		{
			throw std::runtime_error("Could not find faction '" + FactionIdentifier + "' for planet '" + PlanetIdentifier + "' in system '" + Identifier + "'.");
		}
		NewPlanet->SetFaction(Faction->GetReference());
		NewSystem->GetAspectObjectContainer()->AddContent(NewPlanet);
	}
	g_Galaxy->GetAspectObjectContainer()->AddContent(NewSystem);
}

static void ReadSystemLink(Arxx::Reference & Reference)
{
	auto Item(Resolve(Reference));
	
	if(Item->GetType() != DATA_TYPE_SYSTEM_LINK)
	{
		throw std::runtime_error("Item type for system link '" + Item->GetName() + "' should be '" + to_string_cast(DATA_TYPE_SYSTEM_LINK) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for system link '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	std::string System1Identifier;
	std::string System2Identifier;
	
	Reader >> System1Identifier >> System2Identifier;
	
	auto System1(g_Galaxy->GetSystem(System1Identifier));
	auto System2(g_Galaxy->GetSystem(System2Identifier));
	
	if(System1 == nullptr)
	{
		throw std::runtime_error("For the system link '" + Item->GetName() + "' the first system '" + System1Identifier + "' must be defined.");
	}		
	if(System2 == nullptr)
	{
		throw std::runtime_error("For the system link '" + Item->GetName() + "' the second system '" + System2Identifier + "' must be defined.");
	}
	System1->AddLinkedSystem(System2);
	System2->AddLinkedSystem(System1);
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
	Arxx::u4byte Width;
	Arxx::u4byte Height;
	Arxx::u4byte Format;
	
	Reader >> Identifier >> Width >> Height >> Format;
	Width = ntohl(Width);
	Height = ntohl(Height);
	Format = ntohl(Format);
	
	auto Texture(g_GraphicsEngine->GetTextureManager()->Create(Identifier));
	
	if(Texture == nullptr)
	{
		throw std::runtime_error("Could not create texture '" + Identifier + "'.");
	}
	Texture->SetData(Width, Height, Format, Reader.GetBuffer().GetBegin() + Reader.stGetPosition());
}

static void ReadWeaponClass(Arxx::Reference & Reference, ClassManager< WeaponClass > * WeaponClassManager)
{
	auto Item(Resolve(Reference));
	
	if(Item->GetType() != DATA_TYPE_WEAPON_CLASS)
	{
		throw std::runtime_error("Item type for weapon class '" + Item->GetName() + "' should be '" + to_string_cast(DATA_TYPE_WEAPON_CLASS) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for weapon class '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	std::string Identifier;
	
	Reader >> Identifier;
	
	auto NewWeaponClass(WeaponClassManager->Create(Identifier));
	
	if(NewWeaponClass == nullptr)
	{
		throw std::runtime_error("Could not create weapon class '" + Identifier + "'.");
	}
	
	std::string Name;
	VisualizationPrototype WeaponVisualizationPrototype;
	
	Reader >> Name >> WeaponVisualizationPrototype;
	NewWeaponClass->SetName(Name);
	NewWeaponClass->SetWeaponVisualizationPrototype(WeaponVisualizationPrototype);
	
	std::string SlotClassIdentifier;
	Quaternion Orientation;
	float ReloadTime;
	Arxx::u4byte SpaceRequirement;
	float EnergyUsagePerShot;
	Vector3f ShotExitPosition;
	float ShotExitSpeed;
	float ShotDamage;
	float ShotLifeTime;
	VisualizationPrototype ShotVisualizationPrototype;
	
	Reader >> SlotClassIdentifier >> Orientation >> ReloadTime >> SpaceRequirement >> EnergyUsagePerShot >> ShotExitPosition >> ShotExitSpeed >> ShotDamage >> ShotLifeTime >> ShotVisualizationPrototype;
	NewWeaponClass->SetSlotClassIdentifier(SlotClassIdentifier);
	NewWeaponClass->SetOrientation(Orientation);
	NewWeaponClass->SetReloadTime(ReloadTime);
	NewWeaponClass->SetSpaceRequirement(static_cast< unsigned_numeric >(SpaceRequirement));
	NewWeaponClass->SetEnergyUsagePerShot(EnergyUsagePerShot);
	NewWeaponClass->SetShotExitPosition(ShotExitPosition);
	NewWeaponClass->SetShotExitSpeed(ShotExitSpeed);
	NewWeaponClass->SetShotDamage(ShotDamage);
	NewWeaponClass->SetShotLifeTime(ShotLifeTime);
	NewWeaponClass->SetShotVisualizationPrototype(ShotVisualizationPrototype);
}
