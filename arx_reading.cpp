#include <iostream>

#include <Archive.h>
#include <BufferReader.h>
#include <Item.h>

#include <math3d/vector3f.h>
#include <math3d/vector4f.h>

#include "arx_reading.h"
#include "arx_resources.h"
#include "buffer_reading.h"
#include "commodity.h"
#include "commodity_manager.h"
#include "globals.h"
#include "label.h"
#include "model.h"
#include "model_manager.h"
#include "planet.h"
#include "ship_class.h"
#include "ship_class_manager.h"
#include "star.h"
#include "string_cast.h"
#include "system.h"
#include "system_manager.h"
#include "user_interface.h"
#include "widget.h"

static void ReadWidget(Arxx::BufferReader & Reader, Widget * NewWidget);

Arxx::Item * GetItem(Arxx::Archive & Archive, Arxx::u4byte UniqueIdentifier)
{
	Arxx::Item * Item(Archive.pGetItem(UniqueIdentifier));
	
	if(Item == 0)
	{
		throw std::runtime_error("Could not find item '" + to_string_cast(UniqueIdentifier) + "'.");
	}
	if(Item->bIsFetched() == false)
	{
		if(Item->bFetch() == false)
		{
			throw std::runtime_error("Could not fetch data for item '" + to_string_cast(UniqueIdentifier) + "'.");
		}
		if(Item->bIsFetched() == false)
		{
			throw std::runtime_error("Could not fetch data for item '" + to_string_cast(UniqueIdentifier) + "'.");
		}
	}
	if(Item->bIsCompressed() == true)
	{
		Item->vDecompress();
		if(Item->bIsCompressed() == true)
		{
			throw std::runtime_error("Could not decompress data for item '" + to_string_cast(UniqueIdentifier) + "'.");
		}
	}
	
	return Item;
}

Commodity * ReadCommodity(CommodityManager * CommodityManager, Arxx::Item * Item)
{
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
		return 0;
	}
	
	std::string Name;
	float BasePrice;
	Color Color;
	
	Reader >> Name >> BasePrice >> Color;
	
	NewCommodity->SetName(Name);
	NewCommodity->SetBasePrice(BasePrice);
	NewCommodity->SetColor(Color);
	
	return NewCommodity;
}

static void ReadLabel(Arxx::BufferReader & Reader, Label * Label)
{
	ReadWidget(Reader, Label);
	
	bool UseForegroundColor;
	Color ForegroundColor;
	Arxx::u1byte HorizontalAlignment;
	Arxx::u1byte VerticalAlignment;
	
	Reader >> UseForegroundColor >> ForegroundColor >> HorizontalAlignment >> VerticalAlignment;
	if(UseForegroundColor == true)
	{
		Label->SetForegroundColor(ForegroundColor);
	}
	if(HorizontalAlignment == 0)
	{
		Label->SetHorizontalAlignment(Label::ALIGN_LEFT);
	}
	else if(HorizontalAlignment == 1)
	{
		Label->SetHorizontalAlignment(Label::ALIGN_RIGHT);
	}
	else if(HorizontalAlignment == 2)
	{
		Label->SetHorizontalAlignment(Label::ALIGN_HORIZONTAL_CENTER);
	}
	if(VerticalAlignment == 0)
	{
		Label->SetVerticalAlignment(Label::ALIGN_TOP);
	}
	else if(VerticalAlignment == 1)
	{
		Label->SetVerticalAlignment(Label::ALIGN_BOTTOM);
	}
	else if(VerticalAlignment == 2)
	{
		Label->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	}
}

Label * ReadLabel(Arxx::Item * Item)
{
	if(Item->u4GetType() != ARX_TYPE_WIDGET)
	{
		throw std::runtime_error("Item type for widget should be '" + to_string_cast(ARX_TYPE_WIDGET) + "' not '" + to_string_cast(Item->u4GetType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	Label * NewLabel(new Label());
	
	ReadLabel(Reader, NewLabel);
	
	return NewLabel;
}

Model * ReadModel(ModelManager * ModelManager, Arxx::Item * Item)
{
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
	
	return NewModel;
}

ShipClass * ReadShipClass(ShipClassManager * ShipClassManager, Arxx::Item * Item)
{
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
		return 0;
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
	
	return NewShipClass;
}

System * ReadSystem(SystemManager * SystemManager, Arxx::Item * Item)
{
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
	std::string StarIdentifier;
	math3d::vector2f StarPosition;
	Color StarColor;
	Arxx::u4byte PlanetCount;
	
	Reader >> Name >> Position >> StarIdentifier >> StarPosition >> StarColor >> PlanetCount;
	NewSystem->SetName(Name);
	NewSystem->SetPosition(Position);
	
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
			
			Commodity * ManagedCommodity(SystemManager->GetCommodityManager()->Get(CommodityIdentifier));
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
	
	return NewSystem;
}

void ReadSystemLink(SystemManager * SystemManager, Arxx::Item * Item)
{
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

static void ReadWidget(Arxx::BufferReader & Reader, Widget * NewWidget)
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
	NewWidget->SetName(Name);
	if((Path != "") && (NewWidget->GetSupWidget() == 0))
	{
		Widget * SupWidget(g_UserInterface.GetWidget(Path));
		
		if(SupWidget == 0)
		{
			throw std::runtime_error("For widget '" + Name + "' could not find the superior widget at path '" + Path + "'.");
		}
		SupWidget->AddSubWidget(NewWidget);
	}
	NewWidget->SetPosition(Position);
	if(UseSize == true)
	{
		NewWidget->SetSize(Size);
	}
	if(UseBackgroundColor == true)
	{
		NewWidget->SetBackgroundColor(BackgroundColor);
	}
	if(Visible == false)
	{
		NewWidget->Hide();
	}
}

Widget * ReadWidget(Arxx::Item * Item, Widget * NewWidget)
{
	if(Item->u4GetType() != ARX_TYPE_WIDGET)
	{
		throw std::runtime_error("Item type for widget should be '" + to_string_cast(ARX_TYPE_WIDGET) + "' not '" + to_string_cast(Item->u4GetType()) + "'.");
	}
	
	Arxx::BufferReader Reader(*Item);
	
	if(NewWidget == 0)
	{
		NewWidget = new Widget();
	}
	ReadWidget(Reader, NewWidget);
	
	return NewWidget;
}
