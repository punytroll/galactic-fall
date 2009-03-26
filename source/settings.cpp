/**
 * galactic-fall
 * Copyright (C) 2009  Hagen Möbius
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

#include <Archive.h>
#include <BufferReader.h>
#include <Item.h>

#include "arx_resources.h"
#include "buffer_reading.h"
#include "settings.h"
#include "string_cast.h"

static void MakeItemAvailable(Arxx::Item * Item)
{
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
}

Settings::Settings(void) :
	_WindowDimensions(0)
{
}

Settings::~Settings(void)
{
	delete _WindowDimensions;
	_WindowDimensions = 0;
}

void Settings::SetWindowDimensions(const Vector2f & WindowDimensions)
{
	delete _WindowDimensions;
	_WindowDimensions = new Vector2f(WindowDimensions);
}

void Settings::LoadFromItem(Arxx::Item * Item)
{
	if(Item->GetStructure().bHasRelation("child") == false)
	{
		throw std::runtime_error("The item '" + Item->sGetName() + "' does not contain a 'child' relation.");
	}
	
	Arxx::Structure::Relation & ChildRelation(Item->GetStructure().GetRelation("child"));
	
	// read window dimensions
	std::list< Arxx::Item * > WindowDimensionsItems(ChildRelation.GetItems("Window Dimensions"));
	
	if(WindowDimensionsItems.size() > 0)
	{
		Arxx::Item * WindowDimensionsItem(WindowDimensionsItems.front());
		
		if(WindowDimensionsItem->u4GetType() != ARX_TYPE_DIMENSIONS)
		{
			throw std::runtime_error("The item '" + WindowDimensionsItem->sGetName() + "' does not have the type '" + to_string_cast(ARX_TYPE_DIMENSIONS) + "' but the type '" + to_string_cast(WindowDimensionsItem->u4GetType()) + "'.");
		}
		if(WindowDimensionsItem->u4GetSubType() != ARX_TYPE_DIMENSIONS_SUB_TYPE_2_FLOATS)
		{
			throw std::runtime_error("The item '" + WindowDimensionsItem->sGetName() + "' does not have the sub type '" + to_string_cast(ARX_TYPE_DIMENSIONS_SUB_TYPE_2_FLOATS) + "' but the sub type '" + to_string_cast(WindowDimensionsItem->u4GetSubType()) + "'.");
		}
		MakeItemAvailable(WindowDimensionsItem);
		
		Arxx::BufferReader Reader(*WindowDimensionsItem);
		Vector2f WindowDimensions;
		
		Reader >> WindowDimensions;
		SetWindowDimensions(WindowDimensions);
	}
}
