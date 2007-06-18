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

#include <iostream>

#include <Archive.h>
#include <BufferReader.h>
#include <Item.h>

#include <math3d/vector3f.h>
#include <math3d/vector4f.h>

#include "arx_reading.h"
#include "arx_resources.h"
#include "buffer_reading.h"
#include "callbacks.h"
#include "commodity.h"
#include "commodity_manager.h"
#include "globals.h"
#include "label.h"
#include "mini_map.h"
#include "model.h"
#include "model_manager.h"
#include "planet.h"
#include "scanner_display.h"
#include "ship_class.h"
#include "ship_class_manager.h"
#include "star.h"
#include "string_cast.h"
#include "system.h"
#include "system_manager.h"
#include "user_interface.h"
#include "widget.h"

static Arxx::Item * Resolve(Arxx::Reference & Reference);

static void ReadCommodity(CommodityManager * CommodityManager, Arxx::Reference & Reference);
static void ReadModel(ModelManager * ModelManager, Arxx::Reference & Reference);
static void ReadShipClass(ShipClassManager * ShipClassManager, Arxx::Reference & Reference);
static void ReadSystem(SystemManager * SystemManager, Arxx::Reference & Reference);
static void ReadSystemLink(SystemManager * SystemManager, Arxx::Reference & Reference);
static void ReadWidget(UserInterface * UserInterface, Arxx::Reference & Reference);
static void ReadWidgetLabel(Arxx::BufferReader & Reader, Label * ReadLabel);
static void ReadWidgetMiniMap(Arxx::BufferReader & Reader, MiniMap * ReadMiniMap);
static void ReadWidgetScannerDisplay(Arxx::BufferReader & Reader, ScannerDisplay * ReadScannerDisplay);
static void ReadWidgetWidget(Arxx::BufferReader & Reader, UserInterface * UserInterface, Widget * ReadWidget);

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
			throw std::runtime_error("Could not fetch data for item '" + Item->sGetName() + "' [" + to_string_cast(Item->u4GetUniqueID()) + "].");
		}
		if(Item->bIsFetched() == false)
		{
			throw std::runtime_error("Could not fetch data for item '" + Item->sGetName() + "' [" + to_string_cast(Item->u4GetUniqueID()) + "].");
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

void ReadItems(Arxx::Archive & Archive, const std::string & Path, const Callback1< void, Arxx::Reference & > * Reader)
{
	Arxx::Item * Directory(Archive.GetItem(Path));
	
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

void ReadCommodities(Arxx::Archive & Archive, CommodityManager * Manager)
{
	ReadItems(Archive, "/Commodities", Bind1(Function(ReadCommodity), Manager));
}

void ReadModels(Arxx::Archive & Archive, ModelManager * Manager)
{
	ReadItems(Archive, "/Models", Bind1(Function(ReadModel), Manager));
}

void ReadShipClasses(Arxx::Archive & Archive, ShipClassManager * Manager)
{
	ReadItems(Archive, "/Ship Classes", Bind1(Function(ReadShipClass), Manager));
}

void ReadSystems(Arxx::Archive & Archive, SystemManager * Manager)
{
	ReadItems(Archive, "/Systems", Bind1(Function(ReadSystem), Manager));
}

void ReadSystemLinks(Arxx::Archive & Archive, SystemManager * Manager)
{
	ReadItems(Archive, "/System Links", Bind1(Function(ReadSystemLink), Manager));
}

void ReadUserInterface(Arxx::Archive & Archive, UserInterface * Manager)
{
	ReadItems(Archive, "/User Interface", Bind1(Function(ReadWidget), Manager));
}

static void ReadCommodity(CommodityManager * CommodityManager, Arxx::Reference & Reference)
{
	Arxx::Item * Item(Resolve(Reference));
	
	if(Item->u4GetType() != ARX_TYPE_COMMODITY)
	{
		throw std::runtime_error("Item type for commodity '" + Item->sGetName() + "' should be '" + to_string_cast(ARX_TYPE_COMMODITY) + "' not '" + to_string_cast(Item->u4GetType()) + "'.");
	}
	if(Item->u4GetSubType() != 0)
	{
		throw std::runtime_error("Item sub type for commodity '" + Item->sGetName() + "' should be '0' not '" + to_string_cast(Item->u4GetSubType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	std::string Identifier;
	
	Reader >> Identifier;
	
	Commodity * NewCommodity(CommodityManager->Create(Identifier));
	
	if(NewCommodity == 0)
	{
		throw std::runtime_error("Could not create commodity '" + Identifier + "'.");
	}
	
	std::string Name;
	float BasePrice;
	std::string ModelIdentifier;
	Color Color;
	
	Reader >> Name >> BasePrice >> ModelIdentifier >> Color;
	
	NewCommodity->SetName(Name);
	NewCommodity->SetBasePrice(BasePrice);
	
	Model * Model(g_ModelManager.Get(ModelIdentifier));
	
	if(Model == 0)
	{
		throw std::runtime_error("For the commodity '" + Name + "' could not find the model '" + ModelIdentifier + "'.");
	}
	NewCommodity->SetModel(Model);
	NewCommodity->SetColor(Color);
}

static void ReadModel(ModelManager * ModelManager, Arxx::Reference & Reference)
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
	
	Model * NewModel(ModelManager->Create(Identifier));
	
	if(NewModel == 0)
	{
		throw std::runtime_error("Could not create model '" + Identifier + "'.");
	}
	
	std::map< std::string, std::vector< math3d::vector4f >::size_type > Points;
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
		Points[PointIdentifier] = NewModel->AddPoint(math3d::vector4f(X, Y, Z, 0.0f));
	}
	
	std::map< std::string, std::pair< std::vector< math3d::vector4f >::size_type, math3d::vector4f > > TrianglePoints;
	
	Reader >> Count;
	for(Arxx::u4byte Number = 1; Number <= Count; ++Number)
	{
		std::string TrianglePointIdentifier;
		std::string PointIdentifier;
		float NormalX;
		float NormalY;
		float NormalZ;
		
		Reader >> TrianglePointIdentifier >> NormalX >> NormalY >> NormalZ >> PointIdentifier;
		TrianglePoints[TrianglePointIdentifier] = std::make_pair(Points[PointIdentifier], math3d::vector4f(NormalX, NormalY, NormalZ, 0.0f));
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
		NewModel->AddTriangle(TrianglePoints[TrianglePoint1Identifier].first, TrianglePoints[TrianglePoint1Identifier].second, TrianglePoints[TrianglePoint2Identifier].first, TrianglePoints[TrianglePoint2Identifier].second, TrianglePoints[TrianglePoint3Identifier].first, TrianglePoints[TrianglePoint3Identifier].second);
	}
}

static void ReadShipClass(ShipClassManager * ShipClassManager, Arxx::Reference & Reference)
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
	
	ShipClass * NewShipClass(ShipClassManager->Create(Identifier));
	
	if(NewShipClass == 0)
	{
		throw std::runtime_error("Could not create ship class '" + Identifier + "'.");
	}
	
	std::string ModelIdentifier;
	float ForwardThrust;
	float TurnSpeed;
	float MaximumSpeed;
	float CargoHoldSize;
	float FuelHoldSize;
	float JumpFuel;
	float ForwardFuel;
	float TurnFuel;
	float Hull;
	Color ModelColor;
	math3d::vector3f ExhaustOffset;
	
	Reader >> ModelIdentifier >> ForwardThrust >> TurnSpeed >> MaximumSpeed >> CargoHoldSize >> FuelHoldSize >> JumpFuel >> ForwardFuel >> TurnFuel >> Hull >> ModelColor >> ExhaustOffset;
	
	NewShipClass->SetCargoHoldSize(CargoHoldSize);
	NewShipClass->SetForwardThrust(ForwardThrust);
	NewShipClass->SetFuelHoldSize(FuelHoldSize);
	NewShipClass->SetJumpFuel(JumpFuel);
	NewShipClass->SetMaximumSpeed(MaximumSpeed);
	
	Model * Model(ShipClassManager->GetModelManager()->Get(ModelIdentifier));
	
	if(Model == 0)
	{
		throw std::runtime_error("For the ship class '" + Item->sGetName() + " could not find the model '" + ModelIdentifier + "'.");
	}
	NewShipClass->SetModel(Model);
	NewShipClass->SetTurnSpeed(TurnSpeed);
	NewShipClass->SetForwardFuel(ForwardFuel);
	NewShipClass->SetTurnFuel(TurnFuel);
	NewShipClass->SetHull(Hull);
	NewShipClass->SetColor(ModelColor);
	NewShipClass->SetExhaustOffset(ExhaustOffset);
}

static void ReadSystem(SystemManager * SystemManager, Arxx::Reference & Reference)
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
	
	System * NewSystem(SystemManager->Create(Identifier));
	
	NewSystem->SetObjectIdentifier("::system(" + NewSystem->GetIdentifier() + ")");
	
	std::string Name;
	math3d::vector2f Position;
	float TrafficDensity;
	std::string StarIdentifier;
	math3d::vector2f StarPosition;
	Color StarColor;
	Arxx::u4byte PlanetCount;
	
	Reader >> Name >> Position >> TrafficDensity >> StarIdentifier >> StarPosition >> StarColor >> PlanetCount;
	NewSystem->SetName(Name);
	NewSystem->SetPosition(Position);
	NewSystem->SetTrafficDensity(TrafficDensity);
	
	Star * NewStar(NewSystem->CreateStar());
	
	NewStar->SetObjectIdentifier("::system(" + NewSystem->GetIdentifier() + ")::star(" + StarIdentifier + ")");
	NewStar->SetPosition(StarPosition);
	NewStar->SetColor(StarColor);
	for(Arxx::u4byte Number = 1; Number <= PlanetCount; ++Number)
	{
		std::string PlanetIdentifier;
		
		Reader >> PlanetIdentifier;
		
		Planet * NewPlanet(NewSystem->CreatePlanet(PlanetIdentifier));
		
		NewPlanet->SetObjectIdentifier("::system(" + NewSystem->GetIdentifier() + ")::planet(" + NewPlanet->GetIdentifier() + ")");
		
		std::string Name;
		std::string Description;
		math3d::vector2f PlanetPosition;
		float Size;
		Color PlanetColor;
		Arxx::u4byte CommoditiesCount;
		
		Reader >> Name >> Description >> PlanetPosition >> Size >> PlanetColor >> CommoditiesCount;
		NewPlanet->SetName(Name);
		NewPlanet->SetDescription(Description);
		NewPlanet->SetPosition(PlanetPosition);
		NewPlanet->SetSize(Size);
		NewPlanet->SetColor(PlanetColor);
		for(Arxx::u4byte CommoditiesNumber = 1; CommoditiesNumber <= CommoditiesCount; ++CommoditiesNumber)
		{
			std::string CommodityIdentifier;
			float BasePriceModifier;
			
			Reader >> CommodityIdentifier >> BasePriceModifier;
			
			Commodity * ManagedCommodity(g_CommodityManager.Get(CommodityIdentifier));
			PlanetCommodity * NewPlanetCommodity(NewPlanet->CreateCommodity(ManagedCommodity));
			
			NewPlanetCommodity->SetBasePriceModifier(BasePriceModifier);
		}
		
		bool AllowRefuelling;
		float FuelPrice;
		float LandingFee;
		
		Reader >> AllowRefuelling >> FuelPrice >> LandingFee;
		NewPlanet->SetAllowRefuelling(AllowRefuelling);
		NewPlanet->SetFuelPrice(FuelPrice);
		NewPlanet->SetLandingFee(LandingFee);
	}
}

static void ReadSystemLink(SystemManager * SystemManager, Arxx::Reference & Reference)
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
	
	System * System1(SystemManager->Get(System1Identifier));
	System * System2(SystemManager->Get(System2Identifier));
	
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

static void ReadWidget(UserInterface * UserInterface, Arxx::Reference & Reference)
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
			
			ReadWidgetWidget(Reader, UserInterface, NewLabel);
			ReadWidgetLabel(Reader, NewLabel);
			
			break;
		}
	case ARX_TYPE_WIDGET_SUB_TYPE_MINI_MAP:
		{
			MiniMap * NewMiniMap(new MiniMap());
			
			ReadWidgetWidget(Reader, UserInterface, NewMiniMap);
			ReadWidgetMiniMap(Reader, NewMiniMap);
			
			break;
		}
	case ARX_TYPE_WIDGET_SUB_TYPE_SCANNER_DISPLAY:
		{
			ScannerDisplay * NewScannerDisplay(new ScannerDisplay());
			
			ReadWidgetWidget(Reader, UserInterface, NewScannerDisplay);
			ReadWidgetScannerDisplay(Reader, NewScannerDisplay);
			
			break;
		}
	case ARX_TYPE_WIDGET_SUB_TYPE_WIDGET:
		{
			Widget * NewWidget(new Widget());
			
			ReadWidgetWidget(Reader, UserInterface, NewWidget);
			
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

static void ReadWidgetMiniMap(Arxx::BufferReader & Reader, MiniMap * ReadMiniMap)
{
}

static void ReadWidgetScannerDisplay(Arxx::BufferReader & Reader, ScannerDisplay * ReadScannerDisplay)
{
}

static void ReadWidgetWidget(Arxx::BufferReader & Reader, UserInterface * UserInterface, Widget * ReadWidget)
{
	std::string Path;
	std::string Name;
	math3d::vector2f Position;
	bool UseSize;
	math3d::vector2f Size;
	bool UseBackgroundColor;
	Color BackgroundColor;
	bool Visible;
	
	Reader >> Path >> Name >> Position >> UseSize >> Size >> UseBackgroundColor >> BackgroundColor >> Visible;
	ReadWidget->SetName(Name);
	if((Path != "") && (ReadWidget->GetSupWidget() == 0))
	{
		Widget * SupWidget(UserInterface->GetWidget(Path));
		
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
	if(Visible == false)
	{
		ReadWidget->Hide();
	}
}
