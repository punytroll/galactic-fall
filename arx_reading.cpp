#include <iostream>

#include <Archive.h>
#include <BufferReader.h>
#include <Item.h>

#include <math3d/vector4f.h>

#include "arx_reading.h"
#include "arxx_resources.h"
#include "buffer_reading.h"
#include "globals.h"
#include "label.h"
#include "model.h"
#include "model_manager.h"
#include "user_interface.h"
#include "widget.h"

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

void ReadWidget(Arxx::BufferReader & Reader, Widget * NewWidget)
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

void ReadLabel(Arxx::BufferReader & Reader, Label * Label)
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

Widget * ReadWidget(Arxx::Item * Item)
{
	if(Item->u4GetSubType() != 0)
	{
		std::cerr << "Item subtype for widget should be '0' not '" << Item->u4GetSubType() << "'." << std::endl;
		
		throw std::out_of_range("Encountered unknown subtype.");
	}
	
	Arxx::BufferReader Reader(*Item);
	Widget * NewWidget(new Widget());
	
	ReadWidget(Reader, NewWidget);
	
	return NewWidget;
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
