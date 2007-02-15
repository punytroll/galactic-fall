#include <iostream>

#include <Archive.h>
#include <BufferReader.h>
#include <Item.h>

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
		std::cerr << "Could not find item '" << UniqueIdentifier << "'." << std::endl;
		
		throw std::out_of_range("Finding the unique identifier failed.");
	}
	if(Item->bIsFetched() == false)
	{
		if(Item->bFetch() == false)
		{
			std::cerr << "Could not fetch data for item '" << UniqueIdentifier << "'." << std::endl;
			
			throw std::runtime_error("Fetching the data failed.");
		}
		if(Item->bIsFetched() == false)
		{
			std::cerr << "Could not fetch data for item '" << UniqueIdentifier << "'." << std::endl;
			
			throw std::runtime_error("Fetching the data failed.");
		}
	}
	if(Item->bIsCompressed() == true)
	{
		Item->vDecompress();
		if(Item->bIsCompressed() == true)
		{
			std::cerr << "Could not decompress data for item '" << UniqueIdentifier << "'." << std::endl;
			
			throw std::runtime_error("Decompressing the data failed.");
		}
	}
	
	return Item;
}

Commodity * ReadCommodity(CommodityManager * CommodityManager, Arxx::Item * Item)
{
	if(Item->u4GetType() != ARX_COMMODITY_TYPE)
	{
		std::cerr << "Item type for commodity '" << Item->sGetName() << "' should be '" << ARX_COMMODITY_TYPE << "' not '" << Item->u4GetType() << "'." << std::endl;
		
		throw std::out_of_range("Encountered invalid type.");
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
	if(Item->u4GetSubType() != 1)
	{
		std::cerr << "Item subtype for label should be '1' not '" << Item->u4GetSubType() << "'." << std::endl;
		
		throw std::out_of_range("Encountered unknown subtype.");
	}
	
	Arxx::BufferReader Reader(*Item);
	Label * NewLabel(new Label());
	
	ReadLabel(Reader, NewLabel);
	
	return NewLabel;
}

Model * ReadModel(ModelManager * ModelManager, Arxx::Item * Item)
{
	if(Item->u4GetType() != ARX_MODEL_TYPE)
	{
		std::cerr << "Item type for model '" << Item->sGetName() << "' should be '" << ARX_MODEL_TYPE << "' not '" << Item->u4GetType() << "'." << std::endl;
		
		throw std::out_of_range("Encountered invalid type.");
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
	if(Item->u4GetType() != ARX_SHIP_CLASS_TYPE)
	{
		std::cerr << "Item type for ship class '" << Item->sGetName() << "' should be '" << ARX_SHIP_CLASS_TYPE << "' not '" << Item->u4GetType() << "'." << std::endl;
		
		throw std::out_of_range("Encountered invalid type.");
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
	Color ModelColor;
	
	Reader >> ModelIdentifier >> ForwardThrust >> TurnSpeed >> MaximumSpeed >> CargoHoldSize >> FuelHoldSize >> JumpFuel >> ForwardFuel >> TurnFuel >> ModelColor;
	
	NewShipClass->SetCargoHoldSize(CargoHoldSize);
	NewShipClass->SetForwardThrust(ForwardThrust);
	NewShipClass->SetFuelHoldSize(FuelHoldSize);
	NewShipClass->SetJumpFuel(JumpFuel);
	NewShipClass->SetMaximumSpeed(MaximumSpeed);
	NewShipClass->SetModel(ShipClassManager->GetModelManager()->Get(ModelIdentifier));
	NewShipClass->SetTurnSpeed(TurnSpeed);
	NewShipClass->SetForwardFuel(ForwardFuel);
	NewShipClass->SetTurnFuel(TurnFuel);
	NewShipClass->SetColor(ModelColor);
	
	return NewShipClass;
}

System * ReadSystem(SystemManager * SystemManager, Arxx::Item * Item)
{
	if(Item->u4GetType() != ARX_SYSTEM_TYPE)
	{
		std::cerr << "Item type for system '" << Item->sGetName() << "' should be '" << ARX_SYSTEM_TYPE << "' not '" << Item->u4GetType() << "'." << std::endl;
		
		throw std::out_of_range("Encountered invalid type.");
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
	if(NewStar == 0)
	{
		std::cerr << "Error with star identifier '" << StarIdentifier << "'." << std::endl;
		
		throw std::runtime_error("Invalid identifier");
	}
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
	if(Item->u4GetType() != ARX_SYSTEM_LINK_TYPE)
	{
		std::cerr << "Item type for system '" << Item->sGetName() << "' should be '" << ARX_SYSTEM_LINK_TYPE << "' not '" << Item->u4GetType() << "'." << std::endl;
		
		throw std::out_of_range("Encountered invalid type.");
	}
	
	Arxx::BufferReader Reader(*Item);
	std::string System1Identifier;
	std::string System2Identifier;
	
	Reader >> System1Identifier >> System2Identifier;
	
	System * System1(SystemManager->Get(System1Identifier));
	System * System2(SystemManager->Get(System2Identifier));
	
	if((System1 == 0) || (System2 == 0))
	{
		std::cerr << "For system link '" << Item->sGetName() << "' both systems should be defined." << std::endl;
		
		throw std::out_of_range("Encountered invalid system identifier.");
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
			std::cerr << "Could not find the widget at '" << Path << "'." << std::endl;
			
			throw std::runtime_error("Unknown sup widget.");
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
	if(Item->u4GetSubType() != 0)
	{
		std::cerr << "Item subtype for widget should be '0' not '" << Item->u4GetSubType() << "'." << std::endl;
		
		throw std::out_of_range("Encountered unknown subtype.");
	}
	
	Arxx::BufferReader Reader(*Item);
	
	if(NewWidget == 0)
	{
		NewWidget = new Widget();
	}
	ReadWidget(Reader, NewWidget);
	
	return NewWidget;
}
