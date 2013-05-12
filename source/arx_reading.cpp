/**
 * galactic-fall
 * Copyright (C) 2006  Hagen Möbius
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

#include <iostream>

#include <Archive.h>
#include <Item.h>

#include "arx_reading.h"
#include "arx_resources.h"
#include "asset_class.h"
#include "battery_class.h"
#include "buffer_reading.h"
#include "callbacks/callbacks.h"
#include "class_manager.h"
#include "commodity_class.h"
#include "faction.h"
#include "galaxy.h"
#include "generator_class.h"
#include "globals.h"
#include "graphics/engine.h"
#include "graphics/material.h"
#include "graphics/mesh.h"
#include "graphics/mesh_manager.h"
#include "graphics/model.h"
#include "graphics/model_manager.h"
#include "graphics/texture.h"
#include "graphics/texture_manager.h"
#include "object_aspect_name.h"
#include "object_aspect_object_container.h"
#include "object_aspect_position.h"
#include "object_factory.h"
#include "planet.h"
#include "settings.h"
#include "ship_class.h"
#include "slot.h"
#include "slot_class.h"
#include "star.h"
#include "string_cast.h"
#include "system.h"
#include "ui/label.h"
#include "ui/mini_map_display.h"
#include "ui/scanner_display.h"
#include "ui/user_interface.h"
#include "ui/widget.h"
#include "visualization_prototype.h"
#include "weapon_class.h"

static Arxx::Item * Resolve(Arxx::Reference & Reference);

static void ReadAssetClass(Arxx::Reference & Reference);
static void ReadBatteryClass(Arxx::Reference & Reference);
static void ReadCommodityClass(Arxx::Reference & Reference);
static void ReadFaction(Arxx::Reference & Reference);
static void ReadGeneratorClass(Arxx::Reference & Reference);
static void ReadMesh(Arxx::Reference & Reference);
static void ReadModel(Arxx::Reference & Reference);
static void ReadShipClass(Arxx::Reference & Reference);
static void ReadSlotClass(Arxx::Reference & Reference);
static void ReadSystem(Arxx::Reference & Reference);
static void ReadSystemLink(Arxx::Reference & Reference);
static void ReadTexture(Arxx::Reference & Reference);
static void ReadVisualizationPrototype(Arxx::BufferReader & Reader, VisualizationPrototype * VisualizationPrototype);
static void ReadWeaponClass(Arxx::Reference & Reference);
static void ReadWidget(Arxx::Reference & Reference);
static UI::Widget * ReadWidget(Arxx::BufferReader & Reader, Arxx::u4byte Type, Arxx::u4byte SubType);
static void ReadWidgetLabel(Arxx::BufferReader & Reader, UI::Label * ReadLabel);
static void ReadWidgetMiniMapDisplay(Arxx::BufferReader & Reader, UI::MiniMapDisplay * ReadMiniMapDisplay);
static void ReadWidgetScannerDisplay(Arxx::BufferReader & Reader, UI::ScannerDisplay * ReadScannerDisplay);
static void ReadWidgetWidget(Arxx::BufferReader & Reader, UI::Widget * Widget);

static void MakeItemAvailable(Arxx::Item * Item)
{
	assert(Item != 0);
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
	Arxx::Item * Item(Reference.pGetItem());
	
	if(Item == 0)
	{
		throw std::runtime_error("The unique identifier '" + to_string_cast(Reference.u4GetUniqueID()) + "' is referenced but could not be resolved.");
	}
	MakeItemAvailable(Item);
	
	return Item;
}

void ReadItems(Arxx::Archive * Archive, const std::string & Path, const Callback1< void, Arxx::Reference & > Reader)
{
	Arxx::Item * Directory(Archive->GetItem(Path));
	
	if(Directory == 0)
	{
		throw std::runtime_error("Could not find an item at the path '" + Path + "'.");
	}
	if(Directory->GetStructure().bHasRelation("child") == false)
	{
		throw std::runtime_error("The directory '" + Path + "' does not contain a 'child' relation.");
	}
	
	Arxx::Structure::Relation & Relation(Directory->GetStructure().GetRelation("child"));
	Arxx::Structure::Relation::iterator CommodityIterator(Relation.begin());
	
	while(CommodityIterator != Relation.end())
	{
		Reader(*CommodityIterator);
		++CommodityIterator;
	}
}

ResourceReader::ResourceReader(const std::string & DataDirectoryPath) :
	m_Archive(0)
{
}

ResourceReader::~ResourceReader(void)
{
	delete m_Archive;
	m_Archive = 0;
}

bool ResourceReader::LoadArchive(const std::string & ArchivePath)
{
	assert(m_Archive == 0);
	m_Archive = new Arxx::Archive();
	if(m_Archive->Load(ArchivePath) == false)
	{
		std::cerr << "Could not find or open \"" << ArchivePath << "\"." << std::endl;
		
		return false;
	}
	
	return true;
}

void ResourceReader::ReadAssetClasses(void)
{
	ReadItems(m_Archive, "/Asset Classes", Callback(ReadAssetClass));
}

void ResourceReader::ReadBatteryClasses(void)
{
	ReadItems(m_Archive, "/Battery Classes", Callback(ReadBatteryClass));
}

void ResourceReader::ReadCommodityClasses(void)
{
	ReadItems(m_Archive, "/Commodity Classes", Callback(ReadCommodityClass));
}

void ResourceReader::ReadFactions(void)
{
	ReadItems(m_Archive, "/Factions", Callback(ReadFaction));
}

void ResourceReader::ReadGeneratorClasses(void)
{
	ReadItems(m_Archive, "/Generator Classes", Callback(ReadGeneratorClass));
}

void ResourceReader::ReadMeshes(void)
{
	ReadItems(m_Archive, "/Meshes", Callback(ReadMesh));
}

void ResourceReader::ReadModels(void)
{
	ReadItems(m_Archive, "/Models", Callback(ReadModel));
}

void ResourceReader::ReadSettings(Settings * Settings)
{
	Arxx::Item * Item(m_Archive->GetItem("/Settings"));
	
	if(Item == 0)
	{
		throw std::runtime_error("Could not find an item at the path '/Settings'.");
	}
	Settings->LoadFromItem(Item);
}

void ResourceReader::ReadShipClasses(void)
{
	ReadItems(m_Archive, "/Ship Classes", Callback(ReadShipClass));
}

void ResourceReader::ReadSlotClasses(void)
{
	ReadItems(m_Archive, "/Slot Classes", Callback(ReadSlotClass));
}

void ResourceReader::ReadSystems(void)
{
	ReadItems(m_Archive, "/Systems", Callback(ReadSystem));
}

void ResourceReader::ReadSystemLinks(void)
{
	ReadItems(m_Archive, "/System Links", Callback(ReadSystemLink));
}

void ResourceReader::ReadTextures(void)
{
	ReadItems(m_Archive, "/Textures", Callback(ReadTexture));
}

void ResourceReader::ReadUserInterface(void)
{
	ReadItems(m_Archive, "/User Interface", Callback(ReadWidget));
}

void ResourceReader::ReadWeaponClasses(void)
{
	ReadItems(m_Archive, "/Weapon Classes", Callback(ReadWeaponClass));
}

std::string ResourceReader::GetContentStringFromResourcePath(const std::string & ResourcePath)
{
	Arxx::Item * Item(m_Archive->GetItem(ResourcePath));
	std::string Result;
	
	if(Item != 0)
	{
		MakeItemAvailable(Item);
		
		Arxx::BufferReader Reader(*Item);
		
		Reader >> Result;
	}
	
	return Result;
}

static void ReadAssetClass(Arxx::Reference & Reference)
{
	Arxx::Item * Item(Resolve(Reference));
	
	if(Item->GetType() != ARX_TYPE_ASSET_CLASS)
	{
		throw std::runtime_error("Item type for asset class '" + Item->GetName() + "' should be '" + to_string_cast(ARX_TYPE_ASSET_CLASS) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for asset class '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	std::string Identifier;
	
	Reader >> Identifier;
	
	AssetClass * NewAssetClass(g_AssetClassManager->Create(Identifier));
	
	if(NewAssetClass == 0)
	{
		throw std::runtime_error("Could not create asset class '" + Identifier + "'.");
	}
	
	std::string Name;
	float BasePrice;
	std::string ObjectType;
	std::string ObjectClass;
	
	Reader >> Name >> BasePrice >> ObjectType >> ObjectClass;
	
	NewAssetClass->SetName(Name);
	NewAssetClass->SetBasePrice(BasePrice);
	NewAssetClass->SetObjectType(ObjectType);
	NewAssetClass->SetObjectClass(ObjectClass);
}

static void ReadBatteryClass(Arxx::Reference & Reference)
{
	Arxx::Item * Item(Resolve(Reference));
	
	if(Item->GetType() != ARX_TYPE_BATTERY_CLASS)
	{
		throw std::runtime_error("Item type for battery class '" + Item->GetName() + "' should be '" + to_string_cast(ARX_TYPE_BATTERY_CLASS) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for battery class '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	std::string Identifier;
	
	Reader >> Identifier;
	
	BatteryClass * NewBatteryClass(g_BatteryClassManager->Create(Identifier));
	
	if(NewBatteryClass == 0)
	{
		throw std::runtime_error("Could not create battery class '" + Identifier + "'.");
	}
	
	std::string Name;
	float SpaceRequirement;
	float EnergyCapacity;
	std::string SlotClassIdentifier;
	
	Reader >> Name >> SpaceRequirement >> EnergyCapacity >> SlotClassIdentifier;
	
	NewBatteryClass->SetName(Name);
	NewBatteryClass->SetSpaceRequirement(1000 * SpaceRequirement);
	NewBatteryClass->SetEnergyCapacity(EnergyCapacity);
	NewBatteryClass->SetSlotClassIdentifier(SlotClassIdentifier);
}

static void ReadCommodityClass(Arxx::Reference & Reference)
{
	Arxx::Item * Item(Resolve(Reference));
	
	if(Item->GetType() != ARX_TYPE_COMMODITY_CLASS)
	{
		throw std::runtime_error("Item type for commodity class '" + Item->GetName() + "' should be '" + to_string_cast(ARX_TYPE_COMMODITY_CLASS) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for commodity class '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	std::string Identifier;
	
	Reader >> Identifier;
	
	CommodityClass * NewCommodityClass(g_CommodityClassManager->Create(Identifier));
	
	if(NewCommodityClass == 0)
	{
		throw std::runtime_error("Could not create commodity class '" + Identifier + "'.");
	}
	
	std::string Name;
	float SpaceRequirement;
	
	Reader >> Name >> SpaceRequirement;
	
	NewCommodityClass->SetName(Name);
	NewCommodityClass->SetSpaceRequirement(1000 * SpaceRequirement);
	
	// read visualization prototype
	NewCommodityClass->AddVisualizationPrototype();
	ReadVisualizationPrototype(Reader, NewCommodityClass->GetVisualizationPrototype());
}

static void ReadFaction(Arxx::Reference & Reference)
{
	Arxx::Item * Item(Resolve(Reference));
	
	if(Item->GetType() != ARX_TYPE_FACTION)
	{
		throw std::runtime_error("Item type for faction '" + Item->GetName() + "' should be '" + to_string_cast(ARX_TYPE_FACTION) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for faction '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	std::string Identifier;
	
	Reader >> Identifier;
	
	Faction * NewFaction(dynamic_cast< Faction * >(g_ObjectFactory->Create("faction", Identifier)));
	
	if(NewFaction == 0)
	{
		throw std::runtime_error("Could not create faction '" + Identifier + "'.");
	}
	NewFaction->SetObjectIdentifier("::faction(" + NewFaction->GetClassIdentifier() + ")");
	
	std::string Name;
	Color FactionColor;
	
	Reader >> Name >> FactionColor;
	NewFaction->GetAspectName()->SetName(Name);
	NewFaction->SetColor(new Color(&FactionColor));
	if(g_Galaxy->GetAspectObjectContainer()->AddContent(NewFaction) == false)
	{
		throw std::runtime_error("Could not add faction '" + Identifier + "' to galaxy.");
	}
}

static void ReadGeneratorClass(Arxx::Reference & Reference)
{
	Arxx::Item * Item(Resolve(Reference));
	
	if(Item->GetType() != ARX_TYPE_GENERATOR_CLASS)
	{
		throw std::runtime_error("Item type for generator class '" + Item->GetName() + "' should be '" + to_string_cast(ARX_TYPE_GENERATOR_CLASS) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for generator class '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	std::string Identifier;
	
	Reader >> Identifier;
	
	GeneratorClass * NewGeneratorClass(g_GeneratorClassManager->Create(Identifier));
	
	if(NewGeneratorClass == 0)
	{
		throw std::runtime_error("Could not create generator class '" + Identifier + "'.");
	}
	
	std::string Name;
	float SpaceRequirement;
	float EnergyProvisionPerSecond;
	std::string SlotClassIdentifier;
	
	Reader >> Name >> SpaceRequirement >> EnergyProvisionPerSecond >> SlotClassIdentifier;
	
	NewGeneratorClass->SetName(Name);
	NewGeneratorClass->SetSpaceRequirement(1000 * SpaceRequirement);
	NewGeneratorClass->SetEnergyProvisionPerSecond(EnergyProvisionPerSecond);
	NewGeneratorClass->SetSlotClassIdentifier(SlotClassIdentifier);
}

static void ReadMesh(Arxx::Reference & Reference)
{
	Arxx::Item * Item(Resolve(Reference));
	
	if(Item->GetType() != ARX_TYPE_MESH)
	{
		throw std::runtime_error("Item type for mesh '" + Item->GetName() + "' should be '" + to_string_cast(ARX_TYPE_MESH) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for mesh '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	std::string Identifier;
	
	Reader >> Identifier;
	
	Graphics::Mesh * NewMesh(g_GraphicsEngine->GetMeshManager()->Create(Identifier));
	
	if(NewMesh == 0)
	{
		throw std::runtime_error("Could not create model '" + Identifier + "'.");
	}
	
	std::map< std::string, std::vector< Vector4f >::size_type > Points;
	Arxx::u4byte Count;
	
	Reader >> Count;
	for(Arxx::u4byte Number = 1; Number <= Count; ++Number)
	{
		std::string PointIdentifier;
		std::string PointName;
		float X;
		float Y;
		float Z;
		
		Reader >> PointIdentifier >> PointName >> X >> Y >> Z;
		Points[PointIdentifier] = NewMesh->AddPoint(Vector4f(X, Y, Z, 0.0f));
	}
	
	std::map< std::string, std::pair< std::vector< Vector4f >::size_type, Vector4f > > TrianglePoints;
	
	Reader >> Count;
	for(Arxx::u4byte Number = 1; Number <= Count; ++Number)
	{
		std::string TrianglePointIdentifier;
		std::string PointIdentifier;
		float NormalX;
		float NormalY;
		float NormalZ;
		
		Reader >> TrianglePointIdentifier >> NormalX >> NormalY >> NormalZ >> PointIdentifier;
		TrianglePoints[TrianglePointIdentifier] = std::make_pair(Points[PointIdentifier], Vector4f(NormalX, NormalY, NormalZ, 0.0f));
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
}

static void ReadModel(Arxx::Reference & Reference)
{
	Arxx::Item * Item(Resolve(Reference));
	
	if(Item->GetType() != ARX_TYPE_MODEL)
	{
		throw std::runtime_error("Item type for model '" + Item->GetName() + "' should be '" + to_string_cast(ARX_TYPE_MODEL) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for model '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	std::string Identifier;
	
	Reader >> Identifier;
	
	Graphics::Model * NewModel(g_GraphicsEngine->GetModelManager()->Create(Identifier));
	
	if(NewModel == 0)
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
		
		if(Mesh == 0)
		{
			throw std::runtime_error("For model '" + Identifier + "' could not find the mesh '" + MeshIdentifier + "' for part '" + PartIdentifier + "'.");
		}
		NewModel->AddMesh(PartIdentifier, Mesh);
	}
}

static void ReadShipClass(Arxx::Reference & Reference)
{
	Arxx::Item * Item(Resolve(Reference));
	
	if(Item->GetType() != ARX_TYPE_SHIP_CLASS)
	{
		throw std::runtime_error("Item type for ship class '" + Item->GetName() + "' should be '" + to_string_cast(ARX_TYPE_SHIP_CLASS) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for ship class '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	std::string Identifier;
	
	Reader >> Identifier;
	
	ShipClass * NewShipClass(g_ShipClassManager->Create(Identifier));
	
	if(NewShipClass == 0)
	{
		throw std::runtime_error("Could not create ship class '" + Identifier + "'.");
	}
	// read the physical aspect
	float SpaceRequirement;
	
	Reader >> SpaceRequirement;
	NewShipClass->SetSpaceRequirement(1000 * SpaceRequirement);
	
	// read the visualization
	NewShipClass->AddVisualizationPrototype();
	ReadVisualizationPrototype(Reader, NewShipClass->GetVisualizationPrototype());
	
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
	
	Reader >> ForwardThrust >> TurnSpeed >> MaximumSpeed >> MaximumAvailableSpace >> FuelCapacity >> JumpFuel >> ForwardFuel >> TurnFuel >> Hull >> ExhaustOffset >> ExhaustRadius >> SlotCount;
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
		
		const SlotClass * SlotClass(g_SlotClassManager->Get(SlotClassIdentifier));
		
		if(SlotClass == 0)
		{
			throw std::runtime_error("Could not get slot class '" + SlotClassIdentifier + "' for slot '" + SlotIdentifier + "' of ship class '" + Identifier + "'.");
		}
		
		std::string Name;
		Vector3f SlotPosition;
		Quaternion SlotOrientation;
		bool VisualizeAccessory;
		
		Reader >> Name >> SlotPosition >> SlotOrientation >> VisualizeAccessory;
		
		Slot * NewSlot(new Slot(SlotClass, SlotIdentifier));
		
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

static void ReadSlotClass(Arxx::Reference & Reference)
{
	Arxx::Item * Item(Resolve(Reference));
	
	if(Item->GetType() != ARX_TYPE_SLOT_CLASS)
	{
		throw std::runtime_error("Item type for slot class '" + Item->GetName() + "' should be '" + to_string_cast(ARX_TYPE_SLOT_CLASS) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for slot class '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	std::string Identifier;
	
	Reader >> Identifier;
	
	SlotClass * NewSlotClass(g_SlotClassManager->Create(Identifier));
	
	if(NewSlotClass == 0)
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

static void ReadSystem(Arxx::Reference & Reference)
{
	Arxx::Item * Item(Resolve(Reference));
	
	if(Item->GetType() != ARX_TYPE_SYSTEM)
	{
		throw std::runtime_error("Item type for system '" + Item->GetName() + "' should be '" + to_string_cast(ARX_TYPE_SYSTEM) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for system '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	std::string Identifier;
	
	Reader >> Identifier;
	
	System * NewSystem(new System(Identifier));
	
	NewSystem->SetTypeIdentifier("system");
	NewSystem->SetClassIdentifier(Identifier);
	NewSystem->SetObjectIdentifier("::system(" + NewSystem->GetIdentifier() + ")");
	
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
	
	Star * NewStar(new Star());
	
	NewStar->SetObjectIdentifier("::star(" + StarIdentifier + ")::in_system(" + NewSystem->GetIdentifier() + ")");
	NewStar->GetAspectPosition()->SetPosition(Vector3f(StarPosition[0], StarPosition[1], 0.0f));
	NewStar->SetColor(StarColor);
	if(NewSystem->GetAspectObjectContainer()->AddContent(NewStar) == false)
	{
		throw std::runtime_error("Could not add star '" + StarIdentifier + "' to system '" + Identifier + "'.");
	}
	for(Arxx::u4byte Number = 1; Number <= PlanetCount; ++Number)
	{
		std::string PlanetIdentifier;
		
		Reader >> PlanetIdentifier;
		
		Planet * NewPlanet(dynamic_cast< Planet * >(g_ObjectFactory->Create("planet", PlanetIdentifier)));
		
		NewPlanet->SetObjectIdentifier("::planet(" + NewPlanet->GetIdentifier() + ")::in_system(" + NewSystem->GetIdentifier() + ")");
		
		std::string Name;
		std::string Description;
		Vector2f PlanetPosition;
		float Size;
		Color PlanetColor;
		Arxx::u4byte OfferedAssetsCount;
		
		Reader >> Name >> Description >> PlanetPosition >> Size >> PlanetColor >> OfferedAssetsCount;
		NewPlanet->GetAspectName()->SetName(Name);
		NewPlanet->SetDescription(Description);
		NewPlanet->GetAspectPosition()->SetPosition(Vector3f(PlanetPosition[0], PlanetPosition[1], 0.0f));
		NewPlanet->SetSize(Size);
		NewPlanet->SetColor(PlanetColor);
		for(Arxx::u4byte OfferedAssetNumber = 1; OfferedAssetNumber <= OfferedAssetsCount; ++OfferedAssetNumber)
		{
			std::string AssetClassIdentifier;
			float BasePriceModifier;
			
			Reader >> AssetClassIdentifier >> BasePriceModifier;
			
			const AssetClass * ManagedAssetClass(g_AssetClassManager->Get(AssetClassIdentifier));
			
			if(ManagedAssetClass == 0)
			{
				throw std::runtime_error("Could not find asset class '" + AssetClassIdentifier + "' for planet '" + PlanetIdentifier + "' in system '" + Identifier + "'.");
			}
			
			PlanetAssetClass * NewPlanetAssetClass(NewPlanet->CreatePlanetAssetClass(ManagedAssetClass));
			
			NewPlanetAssetClass->SetBasePriceModifier(BasePriceModifier);
		}
		
		float LandingFeePerSpace;
		std::string FactionIdentifier;
		
		Reader >> LandingFeePerSpace >> FactionIdentifier;
		NewPlanet->SetLandingFeePerSpace(LandingFeePerSpace / 1000.0f);
		
		Faction * Faction(g_Galaxy->GetFaction(FactionIdentifier));
		
		if(Faction == 0)
		{
			throw std::runtime_error("Could not find faction '" + FactionIdentifier + "' for planet '" + PlanetIdentifier + "' in system '" + Identifier + "'.");
		}
		NewPlanet->SetFaction(Faction->GetReference());
		if(NewSystem->GetAspectObjectContainer()->AddContent(NewPlanet) == false)
		{
			throw std::runtime_error("Could not add planet '" + PlanetIdentifier + "' to system '" + Identifier + "'.");
		}
	}
	if(g_Galaxy->GetAspectObjectContainer()->AddContent(NewSystem) == false)
	{
		throw std::runtime_error("Could not add system '" + Identifier + "' to galaxy.");
	}
}

static void ReadSystemLink(Arxx::Reference & Reference)
{
	Arxx::Item * Item(Resolve(Reference));
	
	if(Item->GetType() != ARX_TYPE_SYSTEM_LINK)
	{
		throw std::runtime_error("Item type for system link '" + Item->GetName() + "' should be '" + to_string_cast(ARX_TYPE_SYSTEM_LINK) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for system link '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	std::string System1Identifier;
	std::string System2Identifier;
	
	Reader >> System1Identifier >> System2Identifier;
	
	System * System1(g_Galaxy->GetSystem(System1Identifier));
	System * System2(g_Galaxy->GetSystem(System2Identifier));
	
	if(System1 == 0)
	{
		throw std::runtime_error("For the system link '" + Item->GetName() + "' the first system '" + System1Identifier + "' must be defined.");
	}		
	if(System2 == 0)
	{
		throw std::runtime_error("For the system link '" + Item->GetName() + "' the second system '" + System2Identifier + "' must be defined.");
	}
	System1->AddLinkedSystem(System2);
	System2->AddLinkedSystem(System1);
}

static void ReadTexture(Arxx::Reference & Reference)
{
	Arxx::Item * Item(Resolve(Reference));
	
	if(Item->GetType() != ARX_TYPE_TEXTURE)
	{
		throw std::runtime_error("Item type for texture '" + Item->GetName() + "' should be '" + to_string_cast(ARX_TYPE_TEXTURE) + "' not '" + to_string_cast(Item->GetType()) + "'.");
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
	
	Graphics::Texture * Texture(g_GraphicsEngine->GetTextureManager()->Create(Identifier));
	
	if(Texture == 0)
	{
		throw std::runtime_error("Could not create texture '" + Identifier + "'.");
	}
	Texture->SetData(Width, Height, Format, Reader.GetBuffer().GetBegin() + Reader.stGetPosition());
}

static void ReadVisualizationPrototype(Arxx::BufferReader & Reader, VisualizationPrototype * VisualizationPrototype)
{
	std::string ModelIdentifier;
	
	Reader >> ModelIdentifier;
	VisualizationPrototype->SetModel(g_GraphicsEngine->GetModelManager()->Get(ModelIdentifier));
	
	Arxx::u4byte VisualizationPartCount;
	
	Reader >> VisualizationPartCount;
	for(Arxx::u4byte VisualizationPartNumber = 1; VisualizationPartNumber <= VisualizationPartCount; ++VisualizationPartNumber)
	{
		std::string PartIdentifier;
		bool PartUseSpecularColor;
		Color PartDiffuseColor;
		Color PartSpecularColor;
		float PartShininess;
		
		Reader >> PartIdentifier >> PartUseSpecularColor >> PartDiffuseColor >> PartSpecularColor >> PartShininess;
		
		Graphics::Material * PartMaterial(new Graphics::Material());
		
		PartMaterial->SetDiffuseColor(new Color(PartDiffuseColor));
		if(PartUseSpecularColor == true)
		{
			PartMaterial->SetSpecularColor(new Color(PartSpecularColor));
		}
		PartMaterial->SetShininess(PartShininess);
		VisualizationPrototype->SetPartMaterial(PartIdentifier, PartMaterial);
	}
}

static void ReadWeaponClass(Arxx::Reference & Reference)
{
	Arxx::Item * Item(Resolve(Reference));
	
	if(Item->GetType() != ARX_TYPE_WEAPON_CLASS)
	{
		throw std::runtime_error("Item type for weapon class '" + Item->GetName() + "' should be '" + to_string_cast(ARX_TYPE_WEAPON_CLASS) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	if(Item->GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for weapon class '" + Item->GetName() + "' should be '0' not '" + to_string_cast(Item->GetSubType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	std::string Identifier;
	
	Reader >> Identifier;
	
	WeaponClass * NewWeaponClass(g_WeaponClassManager->Create(Identifier));
	
	if(NewWeaponClass == 0)
	{
		throw std::runtime_error("Could not create weapon class '" + Identifier + "'.");
	}
	
	std::string Name;
	
	Reader >> Name;
	NewWeaponClass->SetName(Name);
	
	// read the visualization
	NewWeaponClass->AddVisualizationPrototype();
	ReadVisualizationPrototype(Reader, NewWeaponClass->GetVisualizationPrototype());
	
	std::string SlotClassIdentifier;
	Quaternion Orientation;
	float ReloadTime;
	float SpaceRequirement;
	float EnergyUsagePerShot;
	Vector3f ParticleExitPosition;
	float ParticleExitSpeed;
	float ParticleDamage;
	float ParticleLifeTime;
	
	Reader >> SlotClassIdentifier >> Orientation >> ReloadTime >> SpaceRequirement >> EnergyUsagePerShot >> ParticleExitPosition >> ParticleExitSpeed >> ParticleDamage >> ParticleLifeTime;
	NewWeaponClass->SetSlotClassIdentifier(SlotClassIdentifier);
	NewWeaponClass->SetOrientation(Orientation);
	NewWeaponClass->SetReloadTime(ReloadTime);
	NewWeaponClass->SetSpaceRequirement(static_cast< unsigned_numeric >(1000 * SpaceRequirement));
	NewWeaponClass->SetEnergyUsagePerShot(EnergyUsagePerShot);
	NewWeaponClass->SetParticleExitPosition(ParticleExitPosition);
	NewWeaponClass->SetParticleExitSpeed(ParticleExitSpeed);
	NewWeaponClass->SetParticleDamage(ParticleDamage);
	NewWeaponClass->SetParticleLifeTime(ParticleLifeTime);
	NewWeaponClass->AddParticleVisualizationPrototype();
	ReadVisualizationPrototype(Reader, NewWeaponClass->GetParticleVisualizationPrototype());
}

static void ReadWidget(Arxx::Reference & Reference)
{
	Arxx::Item * Item(Resolve(Reference));
	
	if(Item->GetType() != ARX_TYPE_WIDGET)
	{
		throw std::runtime_error("Item type for widget '" + Item->GetName() + "' should be '" + to_string_cast(ARX_TYPE_WIDGET) + "' not '" + to_string_cast(Item->GetType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	
	ReadWidget(Reader, Item->GetType(), Item->GetSubType());
	if(Reader.stGetPosition() != Item->GetDecompressedLength())
	{
		throw std::runtime_error("For the widget '" + Item->GetName() + "' the reader functions did not read the expected amount of data. Should be '" + to_string_cast(Item->GetDecompressedLength()) + "' not '" + to_string_cast(Reader.stGetPosition()) + "'.");
	}
}
	
static UI::Widget * ReadWidget(Arxx::BufferReader & Reader, Arxx::u4byte Type, Arxx::u4byte SubType)
{
	UI::Widget * Result = 0;
	
	switch(SubType)
	{
	case ARX_TYPE_WIDGET_SUB_TYPE_LABEL:
		{
			UI::Label * Label(new UI::Label());
			
			ReadWidgetWidget(Reader, Label);
			ReadWidgetLabel(Reader, Label);
			Result = Label;
			
			break;
		}
	case ARX_TYPE_WIDGET_SUB_TYPE_MINI_MAP_DISPLAY:
		{
			UI::MiniMapDisplay * MiniMapDisplay(new UI::MiniMapDisplay());
			
			ReadWidgetWidget(Reader, MiniMapDisplay);
			ReadWidgetMiniMapDisplay(Reader, MiniMapDisplay);
			Result = MiniMapDisplay;
			
			break;
		}
	case ARX_TYPE_WIDGET_SUB_TYPE_SCANNER_DISPLAY:
		{
			UI::ScannerDisplay * ScannerDisplay(new UI::ScannerDisplay());
			
			ReadWidgetWidget(Reader, ScannerDisplay);
			ReadWidgetScannerDisplay(Reader, ScannerDisplay);
			Result = ScannerDisplay;
			
			break;
		}
	case ARX_TYPE_WIDGET_SUB_TYPE_WIDGET:
		{
			UI::Widget * Widget(new UI::Widget());
			
			ReadWidgetWidget(Reader, Widget);
			Result = Widget;
			
			break;
		}
	default:
		{
			throw std::runtime_error("Unknown item sub type for widget '" + to_string_cast(SubType) + "'.");
		}
	}
	
	return Result;
}

static void ReadWidgetLabel(Arxx::BufferReader & Reader, UI::Label * Label)
{
	std::string Text;
	bool UseTextColor;
	Color TextColor;
	Arxx::u1byte HorizontalAlignment;
	Arxx::u1byte VerticalAlignment;
	
	Reader >> Text >> UseTextColor >> TextColor >> HorizontalAlignment >> VerticalAlignment;
	Label->SetText(Text);
	if(UseTextColor == true)
	{
		Label->SetTextColor(TextColor);
	}
	if(HorizontalAlignment == 0)
	{
		Label->SetHorizontalAlignment(UI::Label::ALIGN_LEFT);
	}
	else if(HorizontalAlignment == 1)
	{
		Label->SetHorizontalAlignment(UI::Label::ALIGN_RIGHT);
	}
	else if(HorizontalAlignment == 2)
	{
		Label->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	}
	if(VerticalAlignment == 0)
	{
		Label->SetVerticalAlignment(UI::Label::ALIGN_TOP);
	}
	else if(VerticalAlignment == 1)
	{
		Label->SetVerticalAlignment(UI::Label::ALIGN_BOTTOM);
	}
	else if(VerticalAlignment == 2)
	{
		Label->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	}
}

static void ReadWidgetMiniMapDisplay(Arxx::BufferReader & Reader, UI::MiniMapDisplay * MiniMapDisplay)
{
}

static void ReadWidgetScannerDisplay(Arxx::BufferReader & Reader, UI::ScannerDisplay * ScannerDisplay)
{
}

static void ReadWidgetWidget(Arxx::BufferReader & Reader, UI::Widget * Widget)
{
	std::string Path;
	std::string Name;
	Vector2f Position;
	bool UseSize;
	Vector2f Size;
	bool UseBackgroundColor;
	Color BackgroundColor;
	bool Visible;
	bool AnchorBottom;
	bool AnchorLeft;
	bool AnchorRight;
	bool AnchorTop;
	Arxx::u4byte SubWidgetCount;
	
	Reader >> Path >> Name >> Position >> UseSize >> Size >> UseBackgroundColor >> BackgroundColor >> Visible >> AnchorBottom >> AnchorLeft >> AnchorRight >> AnchorTop >> SubWidgetCount;
	Widget->SetName(Name);
	if((Path != "") && (Widget->GetSupWidget() == 0))
	{
		UI::Widget * SupWidget(g_UserInterface->GetWidget(Path));
		
		if(SupWidget == 0)
		{
			throw std::runtime_error("For widget '" + Name + "' could not find the superior widget at path '" + Path + "'.");
		}
		SupWidget->AddSubWidget(Widget);
	}
	Widget->SetPosition(Position);
	if(UseSize == true)
	{
		Widget->SetSize(Size);
	}
	if(UseBackgroundColor == true)
	{
		Widget->SetBackgroundColor(BackgroundColor);
	}
	Widget->SetVisible(Visible);
	Widget->SetAnchorBottom(AnchorBottom);
	Widget->SetAnchorLeft(AnchorLeft);
	Widget->SetAnchorRight(AnchorRight);
	Widget->SetAnchorTop(AnchorTop);
	for(Arxx::u4byte SubWidgetNumber = 0; SubWidgetNumber < SubWidgetCount; ++SubWidgetNumber)
	{
		Arxx::u4byte Type;
		Arxx::u4byte SubType;
		
		Reader >> Type >> SubType;
		
		UI::Widget * SubWidget(ReadWidget(Reader, Type, SubType));
		
		Widget->AddSubWidget(SubWidget);
	}
}
