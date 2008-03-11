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
#include <DataRepository.h>
#include <Item.h>

#include "arx_reading.h"
#include "arx_resources.h"
#include "asset_class.h"
#include "asset_class_manager.h"
#include "buffer_reading.h"
#include "callbacks.h"
#include "commodity_class.h"
#include "commodity_class_manager.h"
#include "galaxy.h"
#include "globals.h"
#include "graphics_material.h"
#include "graphics_mesh.h"
#include "graphics_mesh_manager.h"
#include "graphics_model.h"
#include "graphics_model_manager.h"
#include "graphics_texture.h"
#include "graphics_texture_manager.h"
#include "label.h"
#include "local_file_data_channel.h"
#include "mini_map_display.h"
#include "object_aspect_name.h"
#include "object_aspect_object_container.h"
#include "object_aspect_position.h"
#include "planet.h"
#include "scanner_display.h"
#include "ship_class.h"
#include "ship_class_manager.h"
#include "slot.h"
#include "slot_class.h"
#include "slot_class_manager.h"
#include "star.h"
#include "string_cast.h"
#include "system.h"
#include "user_interface.h"
#include "weapon_class.h"
#include "weapon_class_manager.h"
#include "widget.h"

static Arxx::Item * Resolve(Arxx::Reference & Reference);

static void ReadAssetClass(Arxx::Reference & Reference);
static void ReadCommodityClass(Arxx::Reference & Reference);
static void ReadMesh(Arxx::Reference & Reference);
static void ReadModel(Arxx::Reference & Reference);
static void ReadShipClass(Arxx::Reference & Reference);
static void ReadSlotClass(Arxx::Reference & Reference);
static void ReadSystem(Arxx::Reference & Reference);
static void ReadSystemLink(Arxx::Reference & Reference);
static void ReadTexture(Arxx::Reference & Reference);
static void ReadWeaponClass(Arxx::Reference & Reference);
static void ReadWidget(Arxx::Reference & Reference);
static void ReadWidgetLabel(Arxx::BufferReader & Reader, Label * ReadLabel);
static void ReadWidgetMiniMapDisplay(Arxx::BufferReader & Reader, MiniMapDisplay * ReadMiniMapDisplay);
static void ReadWidgetScannerDisplay(Arxx::BufferReader & Reader, ScannerDisplay * ReadScannerDisplay);
static void ReadWidgetWidget(Arxx::BufferReader & Reader, Widget * ReadWidget);

static Arxx::Item * Resolve(Arxx::Reference & Reference)
{
	Arxx::Item * Item(Reference.pGetItem());
	
	if(Item == 0)
	{
		throw std::runtime_error("The unique identifier '" + to_string_cast(Reference.u4GetUniqueID()) + "' is referenced but could not be resolved.");
	}
	if(Item->bIsFetched() == false)
	{
		if(Item->bFetch() == false)
		{
			throw std::runtime_error("Could not fetch data for item '" + Item->sGetName() + "' [" + to_string_cast(Item->u4GetUniqueID()) + "] from URI '" + Item->GetURI().sGetURI() + "'.");
		}
		if(Item->bIsFetched() == false)
		{
			throw std::runtime_error("Could not fetch data for item '" + Item->sGetName() + "' [" + to_string_cast(Item->u4GetUniqueID()) + "] from URI '" + Item->GetURI().sGetURI() + "'.");
		}
	}
	if(Item->bIsCompressed() == true)
	{
		Item->vDecompress();
		if(Item->bIsCompressed() == true)
		{
			throw std::runtime_error("Could not decompress data for item '" + Item->sGetName() + "' [" + to_string_cast(Item->u4GetUniqueID()) + "].");
		}
	}
	
	return Item;
}

void ReadItems(Arxx::Archive * Archive, const std::string & Path, const Callback1< void, Arxx::Reference & > * Reader)
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
		(*Reader)(*CommodityIterator);
		++CommodityIterator;
	}
	delete Reader;
}

ResourceReader::ResourceReader(const std::string & DataDirectoryPath) :
	m_LocalFileDataChannel(new LocalFileDataChannel("file:", DataDirectoryPath)),
	m_Archive(0)
{
	if(Arxx::Repository.bRegisterDataChannel(m_LocalFileDataChannel) == false)
	{
		throw std::runtime_error("Could not register local file data channel.");
	}
}

ResourceReader::~ResourceReader(void)
{
	if(Arxx::Repository.bUnregisterDataChannel(m_LocalFileDataChannel) == false)
	{
		throw std::runtime_error("Could not unregister the local file data channel.");
	}
	delete m_LocalFileDataChannel;
	m_LocalFileDataChannel = 0;
	delete m_Archive;
	m_Archive = 0;
}

bool ResourceReader::LoadArchive(const std::string & ArchivePath)
{
	assert(m_Archive == 0);
	m_Archive = new Arxx::Archive();
	
	Arxx::URI URI(ArchivePath);
	
	if(m_Archive->bLoad(URI) == false)
	{
		std::cerr << "Could not find or open " << URI << "." << std::endl;
		
		return false;
	}
	
	return true;
}

void ResourceReader::ReadAssetClasses(void)
{
	ReadItems(m_Archive, "/Asset Classes", Function(ReadAssetClass));
}

void ResourceReader::ReadCommodityClasses(void)
{
	ReadItems(m_Archive, "/Commodity Classes", Function(ReadCommodityClass));
}

void ResourceReader::ReadMeshes(void)
{
	ReadItems(m_Archive, "/Meshes", Function(ReadMesh));
}

void ResourceReader::ReadModels(void)
{
	ReadItems(m_Archive, "/Models", Function(ReadModel));
}

void ResourceReader::ReadShipClasses(void)
{
	ReadItems(m_Archive, "/Ship Classes", Function(ReadShipClass));
}

void ResourceReader::ReadSlotClasses(void)
{
	ReadItems(m_Archive, "/Slot Classes", Function(ReadSlotClass));
}

void ResourceReader::ReadSystems(void)
{
	ReadItems(m_Archive, "/Systems", Function(ReadSystem));
}

void ResourceReader::ReadSystemLinks(void)
{
	ReadItems(m_Archive, "/System Links", Function(ReadSystemLink));
}

void ResourceReader::ReadTextures(void)
{
	ReadItems(m_Archive, "/Textures", Function(ReadTexture));
}

void ResourceReader::ReadUserInterface(void)
{
	ReadItems(m_Archive, "/User Interface", Function(ReadWidget));
}

void ResourceReader::ReadWeaponClasses(void)
{
	ReadItems(m_Archive, "/Weapon Classes", Function(ReadWeaponClass));
}

static void ReadAssetClass(Arxx::Reference & Reference)
{
	Arxx::Item * Item(Resolve(Reference));
	
	if(Item->u4GetType() != ARX_TYPE_ASSET_CLASS)
	{
		throw std::runtime_error("Item type for asset class '" + Item->sGetName() + "' should be '" + to_string_cast(ARX_TYPE_ASSET_CLASS) + "' not '" + to_string_cast(Item->u4GetType()) + "'.");
	}
	if(Item->u4GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for asset class '" + Item->sGetName() + "' should be '0' not '" + to_string_cast(Item->u4GetSubType()) + "'.");
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

static void ReadCommodityClass(Arxx::Reference & Reference)
{
	Arxx::Item * Item(Resolve(Reference));
	
	if(Item->u4GetType() != ARX_TYPE_COMMODITY_CLASS)
	{
		throw std::runtime_error("Item type for commodity class '" + Item->sGetName() + "' should be '" + to_string_cast(ARX_TYPE_COMMODITY_CLASS) + "' not '" + to_string_cast(Item->u4GetType()) + "'.");
	}
	if(Item->u4GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for commodity class '" + Item->sGetName() + "' should be '0' not '" + to_string_cast(Item->u4GetSubType()) + "'.");
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
	std::string ModelIdentifier;
	Color Color;
	float SpaceRequirement;
	
	Reader >> Name >> ModelIdentifier >> Color >> SpaceRequirement;
	
	NewCommodityClass->SetName(Name);
	
	const Graphics::Model * Model(g_ModelManager->Get(ModelIdentifier));
	
	if(Model == 0)
	{
		throw std::runtime_error("For the commodity class '" + Name + "' could not find the model '" + ModelIdentifier + "'.");
	}
	NewCommodityClass->SetModel(Model);
	NewCommodityClass->SetColor(Color);
	NewCommodityClass->SetSpaceRequirement(static_cast< unsigned_numeric >(1000 * SpaceRequirement));
}

static void ReadMesh(Arxx::Reference & Reference)
{
	Arxx::Item * Item(Resolve(Reference));
	
	if(Item->u4GetType() != ARX_TYPE_MESH)
	{
		throw std::runtime_error("Item type for mesh '" + Item->sGetName() + "' should be '" + to_string_cast(ARX_TYPE_MESH) + "' not '" + to_string_cast(Item->u4GetType()) + "'.");
	}
	if(Item->u4GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for mesh '" + Item->sGetName() + "' should be '0' not '" + to_string_cast(Item->u4GetSubType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	std::string Identifier;
	
	Reader >> Identifier;
	
	Graphics::Mesh * NewMesh(g_MeshManager->Create(Identifier));
	
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
	
	if(Item->u4GetType() != ARX_TYPE_MODEL)
	{
		throw std::runtime_error("Item type for model '" + Item->sGetName() + "' should be '" + to_string_cast(ARX_TYPE_MODEL) + "' not '" + to_string_cast(Item->u4GetType()) + "'.");
	}
	if(Item->u4GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for model '" + Item->sGetName() + "' should be '0' not '" + to_string_cast(Item->u4GetSubType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	std::string Identifier;
	
	Reader >> Identifier;
	
	Graphics::Model * NewModel(g_ModelManager->Create(Identifier));
	
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
		
		const Graphics::Mesh * Mesh(g_MeshManager->Get(MeshIdentifier));
		
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
	
	if(Item->u4GetType() != ARX_TYPE_SHIP_CLASS)
	{
		throw std::runtime_error("Item type for ship class '" + Item->sGetName() + "' should be '" + to_string_cast(ARX_TYPE_SHIP_CLASS) + "' not '" + to_string_cast(Item->u4GetType()) + "'.");
	}
	if(Item->u4GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for ship class '" + Item->sGetName() + "' should be '0' not '" + to_string_cast(Item->u4GetSubType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	std::string Identifier;
	
	Reader >> Identifier;
	
	ShipClass * NewShipClass(g_ShipClassManager->Create(Identifier));
	
	if(NewShipClass == 0)
	{
		throw std::runtime_error("Could not create ship class '" + Identifier + "'.");
	}
	
	std::string ModelIdentifier;
	
	Reader >> ModelIdentifier;
	
	const Graphics::Model * Model(g_ModelManager->Get(ModelIdentifier));
	
	if(Model == 0)
	{
		throw std::runtime_error("For the ship class '" + Item->sGetName() + " could not find the model '" + ModelIdentifier + "'.");
	}
	NewShipClass->SetModel(Model);
	
	Arxx::u4byte VisualizationCount;
	
	Reader >> VisualizationCount;
	for(Arxx::u4byte VisualizationNumber = 1; VisualizationNumber <= VisualizationCount; ++VisualizationNumber)
	{
		std::string PartIdentifier;
		Color PartDiffuseColor;
		Color PartSpecularColor;
		float PartShininess;
		
		Reader >> PartIdentifier >> PartDiffuseColor >> PartSpecularColor >> PartShininess;
		
		Graphics::Material * PartMaterial(new Graphics::Material());
		
		PartMaterial->SetDiffuseColor(PartDiffuseColor);
		PartMaterial->SetSpecularColor(PartSpecularColor);
		PartMaterial->SetShininess(PartShininess);
		if(NewShipClass->AddPartMaterial(PartIdentifier, PartMaterial) == false)
		{
			throw std::runtime_error("For the ship class '" + Identifier + "' could not add the material for part '" + PartIdentifier + "'.");
		}
	}
	
	float ForwardThrust;
	float TurnSpeed;
	float MaximumSpeed;
	float MaximumAvailableSpace;
	float FuelHoldSize;
	float JumpFuel;
	float ForwardFuel;
	float TurnFuel;
	float Hull;
	Vector3f ExhaustOffset;
	float ExhaustRadius;
	Arxx::u4byte SlotCount;
	
	Reader >> ForwardThrust >> TurnSpeed >> MaximumSpeed >> MaximumAvailableSpace >> FuelHoldSize >> JumpFuel >> ForwardFuel >> TurnFuel >> Hull >> ExhaustOffset >> ExhaustRadius >> SlotCount;
	NewShipClass->SetForwardThrust(ForwardThrust);
	NewShipClass->SetFuelHoldSize(FuelHoldSize);
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
		
		Reader >> Name >> SlotPosition >> SlotOrientation;
		
		Slot * NewSlot(new Slot(SlotClass, SlotIdentifier));
		
		NewSlot->SetName(Name);
		NewSlot->SetPosition(SlotPosition);
		NewSlot->SetOrientation(SlotOrientation);
		if(NewShipClass->AddSlot(SlotIdentifier, NewSlot) == false)
		{
			throw std::runtime_error("Could not add slot '" + SlotIdentifier + "' to ship class '" + Identifier + "'.");
		}
	}
}

static void ReadSlotClass(Arxx::Reference & Reference)
{
	Arxx::Item * Item(Resolve(Reference));
	
	if(Item->u4GetType() != ARX_TYPE_SLOT_CLASS)
	{
		throw std::runtime_error("Item type for slot class '" + Item->sGetName() + "' should be '" + to_string_cast(ARX_TYPE_SLOT_CLASS) + "' not '" + to_string_cast(Item->u4GetType()) + "'.");
	}
	if(Item->u4GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for slot class '" + Item->sGetName() + "' should be '0' not '" + to_string_cast(Item->u4GetSubType()) + "'.");
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
	
	if(Item->u4GetType() != ARX_TYPE_SYSTEM)
	{
		throw std::runtime_error("Item type for system '" + Item->sGetName() + "' should be '" + to_string_cast(ARX_TYPE_SYSTEM) + "' not '" + to_string_cast(Item->u4GetType()) + "'.");
	}
	if(Item->u4GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for system '" + Item->sGetName() + "' should be '0' not '" + to_string_cast(Item->u4GetSubType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	std::string Identifier;
	
	Reader >> Identifier;
	
	System * NewSystem(new System(Identifier));
	
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
		
		Planet * NewPlanet(new Planet(PlanetIdentifier));
		
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
		
		float LandingFee;
		
		Reader >> LandingFee;
		NewPlanet->SetLandingFee(LandingFee);
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
	
	if(Item->u4GetType() != ARX_TYPE_SYSTEM_LINK)
	{
		throw std::runtime_error("Item type for system link '" + Item->sGetName() + "' should be '" + to_string_cast(ARX_TYPE_SYSTEM_LINK) + "' not '" + to_string_cast(Item->u4GetType()) + "'.");
	}
	if(Item->u4GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for system link '" + Item->sGetName() + "' should be '0' not '" + to_string_cast(Item->u4GetSubType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	std::string System1Identifier;
	std::string System2Identifier;
	
	Reader >> System1Identifier >> System2Identifier;
	
	System * System1(g_Galaxy->GetSystem(System1Identifier));
	System * System2(g_Galaxy->GetSystem(System2Identifier));
	
	if(System1 == 0)
	{
		throw std::runtime_error("For the system link '" + Item->sGetName() + "' the first system '" + System1Identifier + "' must be defined.");
	}		
	if(System2 == 0)
	{
		throw std::runtime_error("For the system link '" + Item->sGetName() + "' the second system '" + System2Identifier + "' must be defined.");
	}
	System1->AddLinkedSystem(System2);
	System2->AddLinkedSystem(System1);
}

static void ReadTexture(Arxx::Reference & Reference)
{
	Arxx::Item * Item(Resolve(Reference));
	
	if(Item->u4GetType() != ARX_TYPE_TEXTURE)
	{
		throw std::runtime_error("Item type for texture '" + Item->sGetName() + "' should be '" + to_string_cast(ARX_TYPE_TEXTURE) + "' not '" + to_string_cast(Item->u4GetType()) + "'.");
	}
	if(Item->u4GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for texture '" + Item->sGetName() + "' should be '0' not '" + to_string_cast(Item->u4GetSubType()) + "'.");
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
	
	Graphics::Texture * Texture(g_TextureManager->Create(Identifier));
	
	if(Texture == 0)
	{
		throw std::runtime_error("Could not create texture '" + Identifier + "'.");
	}
	Texture->SetData(Width, Height, Format, Reader.GetBuffer().GetBegin() + Reader.stGetPosition());
}

static void ReadWeaponClass(Arxx::Reference & Reference)
{
	Arxx::Item * Item(Resolve(Reference));
	
	if(Item->u4GetType() != ARX_TYPE_WEAPON_CLASS)
	{
		throw std::runtime_error("Item type for weapon class '" + Item->sGetName() + "' should be '" + to_string_cast(ARX_TYPE_WEAPON_CLASS) + "' not '" + to_string_cast(Item->u4GetType()) + "'.");
	}
	if(Item->u4GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for weapon class '" + Item->sGetName() + "' should be '0' not '" + to_string_cast(Item->u4GetSubType()) + "'.");
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
	std::string ModelIdentifier;
	Color ModelColor;
	std::string SlotClassIdentifier;
	Quaternion Orientation;
	float ReloadTime;
	float SpaceRequirement;
	Vector3f ParticleExitPosition;
	float ParticleExitSpeed;
	float ParticleDamage;
	float ParticleLifeTime;
	std::string ParticleModelIdentifier;
	Color ParticleColor;
	
	Reader >> Name >> ModelColor >> ModelIdentifier >> SlotClassIdentifier >> Orientation >> ReloadTime >> SpaceRequirement >> ParticleExitPosition >> ParticleExitSpeed >> ParticleDamage >> ParticleLifeTime >> ParticleModelIdentifier >> ParticleColor;
	NewWeaponClass->SetName(Name);
	
	const Graphics::Model * WeaponClassModel(g_ModelManager->Get(ModelIdentifier));
	
	if(WeaponClassModel == 0)
	{
		throw std::runtime_error("For the weapon class '" + Item->sGetName() + " could not find the model '" + ModelIdentifier + "'.");
	}
	NewWeaponClass->SetModel(WeaponClassModel);
	NewWeaponClass->SetModelColor(ModelColor);
	NewWeaponClass->SetSlotClassIdentifier(SlotClassIdentifier);
	NewWeaponClass->SetOrientation(Orientation);
	NewWeaponClass->SetReloadTime(ReloadTime);
	NewWeaponClass->SetSpaceRequirement(static_cast< unsigned_numeric >(1000 * SpaceRequirement));
	NewWeaponClass->SetParticleExitPosition(ParticleExitPosition);
	NewWeaponClass->SetParticleExitSpeed(ParticleExitSpeed);
	NewWeaponClass->SetParticleDamage(ParticleDamage);
	NewWeaponClass->SetParticleLifeTime(ParticleLifeTime);
	
	const Graphics::Model * ParticleModel(g_ModelManager->Get(ParticleModelIdentifier));
	
	if(ParticleModel == 0)
	{
		throw std::runtime_error("For the weapon class '" + Item->sGetName() + " could not find the particle model '" + ParticleModelIdentifier + "'.");
	}
	NewWeaponClass->SetParticleModel(ParticleModel);
	NewWeaponClass->SetParticleColor(ParticleColor);
}

static void ReadWidget(Arxx::Reference & Reference)
{
	Arxx::Item * Item(Resolve(Reference));
	
	if(Item->u4GetType() != ARX_TYPE_WIDGET)
	{
		throw std::runtime_error("Item type for widget '" + Item->sGetName() + "' should be '" + to_string_cast(ARX_TYPE_WIDGET) + "' not '" + to_string_cast(Item->u4GetType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	
	switch(Item->u4GetSubType())
	{
	case ARX_TYPE_WIDGET_SUB_TYPE_LABEL:
		{
			Label * NewLabel(new Label());
			
			ReadWidgetWidget(Reader, NewLabel);
			ReadWidgetLabel(Reader, NewLabel);
			
			break;
		}
	case ARX_TYPE_WIDGET_SUB_TYPE_MINI_MAP_DISPLAY:
		{
			MiniMapDisplay * NewMiniMapDisplay(new MiniMapDisplay());
			
			ReadWidgetWidget(Reader, NewMiniMapDisplay);
			ReadWidgetMiniMapDisplay(Reader, NewMiniMapDisplay);
			
			break;
		}
	case ARX_TYPE_WIDGET_SUB_TYPE_SCANNER_DISPLAY:
		{
			ScannerDisplay * NewScannerDisplay(new ScannerDisplay());
			
			ReadWidgetWidget(Reader, NewScannerDisplay);
			ReadWidgetScannerDisplay(Reader, NewScannerDisplay);
			
			break;
		}
	case ARX_TYPE_WIDGET_SUB_TYPE_WIDGET:
		{
			Widget * NewWidget(new Widget());
			
			ReadWidgetWidget(Reader, NewWidget);
			
			break;
		}
	default:
		{
			throw std::runtime_error("Unknown item sub type for widget '" + Item->sGetName() + "' which is '" + to_string_cast(Item->u4GetSubType()) + "'.");
		}
	}
}

static void ReadWidgetLabel(Arxx::BufferReader & Reader, Label * ReadLabel)
{
	bool UseForegroundColor;
	Color ForegroundColor;
	Arxx::u1byte HorizontalAlignment;
	Arxx::u1byte VerticalAlignment;
	
	Reader >> UseForegroundColor >> ForegroundColor >> HorizontalAlignment >> VerticalAlignment;
	if(UseForegroundColor == true)
	{
		ReadLabel->SetForegroundColor(ForegroundColor);
	}
	if(HorizontalAlignment == 0)
	{
		ReadLabel->SetHorizontalAlignment(Label::ALIGN_LEFT);
	}
	else if(HorizontalAlignment == 1)
	{
		ReadLabel->SetHorizontalAlignment(Label::ALIGN_RIGHT);
	}
	else if(HorizontalAlignment == 2)
	{
		ReadLabel->SetHorizontalAlignment(Label::ALIGN_HORIZONTAL_CENTER);
	}
	if(VerticalAlignment == 0)
	{
		ReadLabel->SetVerticalAlignment(Label::ALIGN_TOP);
	}
	else if(VerticalAlignment == 1)
	{
		ReadLabel->SetVerticalAlignment(Label::ALIGN_BOTTOM);
	}
	else if(VerticalAlignment == 2)
	{
		ReadLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	}
}

static void ReadWidgetMiniMapDisplay(Arxx::BufferReader & Reader, MiniMapDisplay * ReadMiniMapDisplay)
{
}

static void ReadWidgetScannerDisplay(Arxx::BufferReader & Reader, ScannerDisplay * ReadScannerDisplay)
{
}

static void ReadWidgetWidget(Arxx::BufferReader & Reader, Widget * ReadWidget)
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
	
	Reader >> Path >> Name >> Position >> UseSize >> Size >> UseBackgroundColor >> BackgroundColor >> Visible >> AnchorBottom >> AnchorLeft >> AnchorRight >> AnchorTop;
	ReadWidget->SetName(Name);
	if((Path != "") && (ReadWidget->GetSupWidget() == 0))
	{
		Widget * SupWidget(g_UserInterface->GetWidget(Path));
		
		if(SupWidget == 0)
		{
			throw std::runtime_error("For widget '" + Name + "' could not find the superior widget at path '" + Path + "'.");
		}
		SupWidget->AddSubWidget(ReadWidget);
	}
	ReadWidget->SetPosition(Position);
	if(UseSize == true)
	{
		ReadWidget->SetSize(Size);
	}
	if(UseBackgroundColor == true)
	{
		ReadWidget->SetBackgroundColor(BackgroundColor);
	}
	ReadWidget->SetVisible(Visible);
	ReadWidget->SetAnchorBottom(AnchorBottom);
	ReadWidget->SetAnchorLeft(AnchorLeft);
	ReadWidget->SetAnchorRight(AnchorRight);
	ReadWidget->SetAnchorTop(AnchorTop);
}
