/**
 * galactic-fall
 * Copyright (C) 2009-2025  Hagen Möbius
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

#include <arxx/Archive.h>
#include <arxx/BufferReader.h>
#include <arxx/Item.h>

#include <string_cast/string_cast.h>

#include "arx_types.h"
#include "buffer_reading.h"
#include "settings.h"

static void MakeItemAvailable(Arxx::Item * Item)
{
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

Settings::KeyBinding::KeyBinding(int Code, const std::string & Event, const std::string & Action) :
	Action(Action),
	Code(Code),
	Event(Event)
{
}

Settings::Settings(void) :
	_KeyBindings(nullptr),
	_WindowDimensions(nullptr)
{
}

Settings::~Settings(void)
{
	delete _KeyBindings;
	_KeyBindings = nullptr;
	delete _WindowDimensions;
	_WindowDimensions = nullptr;
}

void Settings::SetKeyBindings(const std::list< KeyBinding > & KeyBindings)
{
	delete _KeyBindings;
	_KeyBindings = new std::list< KeyBinding >(KeyBindings);
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
		throw std::runtime_error("The item '" + Item->GetName() + "' does not contain a 'child' relation.");
	}
	
	Arxx::Structure::Relation & ChildRelation(Item->GetStructure().GetRelation("child"));
	
	// read key binding profile
	std::list< Arxx::Item * > KeyBindingProfileItems(ChildRelation.GetItems("Key Binding Profile"));
	
	if(KeyBindingProfileItems.size() > 0)
	{
		Arxx::Item * KeyBindingProfileItem(KeyBindingProfileItems.front());
		
		if(KeyBindingProfileItem->GetType() != DATA_TYPE_KEY_BINDING_PROFILE)
		{
			throw std::runtime_error("The item '" + KeyBindingProfileItem->GetName() + "' does not have the type '" + to_string_cast(DATA_TYPE_KEY_BINDING_PROFILE) + "' but the type '" + to_string_cast(KeyBindingProfileItem->GetType()) + "'.");
		}
		if(KeyBindingProfileItem->GetSubType() != 0)
		{
			throw std::runtime_error("The item '" + KeyBindingProfileItem->GetName() + "' does not have the sub type '" + to_string_cast(0) + "' but the sub type '" + to_string_cast(KeyBindingProfileItem->GetSubType()) + "'.");
		}
		MakeItemAvailable(KeyBindingProfileItem);
		
		Arxx::BufferReader Reader(*KeyBindingProfileItem);
		std::uint32_t NumberOfKeyBindings;
		std::list< KeyBinding > KeyBindings;
		
		Reader >> NumberOfKeyBindings;
		for(auto KeyBindingIndex = 0ul; KeyBindingIndex < NumberOfKeyBindings; ++KeyBindingIndex)
		{
			std::uint32_t Code;
			std::string Event;
			std::string Action;
			
			Reader >> Code >> Event >> Action;
			KeyBindings.push_back(KeyBinding(Code, Event, Action));
		}
		SetKeyBindings(KeyBindings);
	}
	
	// read window dimensions
	std::list< Arxx::Item * > WindowDimensionsItems(ChildRelation.GetItems("Window Dimensions"));
	
	if(WindowDimensionsItems.size() > 0)
	{
		Arxx::Item * WindowDimensionsItem(WindowDimensionsItems.front());
		
		if(WindowDimensionsItem->GetType() != DATA_TYPE_DIMENSIONS)
		{
			throw std::runtime_error("The item '" + WindowDimensionsItem->GetName() + "' does not have the type '" + to_string_cast(DATA_TYPE_DIMENSIONS) + "' but the type '" + to_string_cast(WindowDimensionsItem->GetType()) + "'.");
		}
		if(WindowDimensionsItem->GetSubType() != DATA_TYPE_DIMENSIONS_SUB_TYPE_DIMENSIONS2F)
		{
			throw std::runtime_error("The item '" + WindowDimensionsItem->GetName() + "' does not have the sub type '" + to_string_cast(DATA_TYPE_DIMENSIONS_SUB_TYPE_DIMENSIONS2F) + "' but the sub type '" + to_string_cast(WindowDimensionsItem->GetSubType()) + "'.");
		}
		MakeItemAvailable(WindowDimensionsItem);
		
		Arxx::BufferReader Reader(*WindowDimensionsItem);
		Vector2f WindowDimensions;
		
		Reader >> WindowDimensions;
		SetWindowDimensions(WindowDimensions);
	}
}
