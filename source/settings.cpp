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

#include <arxx/archive.h>
#include <arxx/buffer_reader.h>
#include <arxx/item.h>

#include <string_cast/string_cast.h>

#include "arx_types.h"
#include "buffer_reading.h"
#include "settings.h"

static void MakeItemAvailable(ARX::Item * Item)
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

Settings::KeyBinding::KeyBinding(int Code, std::string Event, std::string Action) :
	Action{std::move(Action)},
	Code(Code),
	Event{std::move(Event)}
{
}

auto Settings::LoadFromItem(ARX::Item * Item) -> void
{
	if(Item->GetStructure().HasRelation("child") == false)
	{
		throw std::runtime_error("The item '" + Item->GetName() + "' does not contain a 'child' relation.");
	}
	
	auto & ChildRelation = Item->GetStructure().GetRelation("child");
	// read key binding profile
	auto KeyBindingProfileItems = ChildRelation.GetItems("Key Binding Profile");
	
	if(KeyBindingProfileItems.size() > 0)
	{
		auto KeyBindingProfileItem = KeyBindingProfileItems.front();
		
		if(KeyBindingProfileItem->GetType() != DATA_TYPE_KEY_BINDING_PROFILE)
		{
			throw std::runtime_error("The item '" + KeyBindingProfileItem->GetName() + "' does not have the type '" + to_string_cast(DATA_TYPE_KEY_BINDING_PROFILE) + "' but the type '" + to_string_cast(KeyBindingProfileItem->GetType()) + "'.");
		}
		if(KeyBindingProfileItem->GetSubType() != 0)
		{
			throw std::runtime_error("The item '" + KeyBindingProfileItem->GetName() + "' does not have the sub type '" + to_string_cast(0) + "' but the sub type '" + to_string_cast(KeyBindingProfileItem->GetSubType()) + "'.");
		}
		MakeItemAvailable(KeyBindingProfileItem);
		
		auto Reader = ARX::BufferReader{*KeyBindingProfileItem};
		auto NumberOfKeyBindings = std::uint32_t{0};
		auto KeyBindings = std::list<Settings::KeyBinding>{};
		
		Reader >> NumberOfKeyBindings;
		for(auto KeyBindingIndex = 0ul; KeyBindingIndex < NumberOfKeyBindings; ++KeyBindingIndex)
		{
			auto Code = std::uint32_t{0};
			auto Event = std::string{};
			auto Action = std::string{};
			
			Reader >> Code >> Event >> Action;
			KeyBindings.push_back(KeyBinding(Code, Event, Action));
		}
		m_KeyBindings = KeyBindings;
	}
	
	// read window dimensions
	auto WindowDimensionsItems = ChildRelation.GetItems("Window Dimensions");
	
	if(WindowDimensionsItems.size() > 0)
	{
		auto WindowDimensionsItem = WindowDimensionsItems.front();
		
		if(WindowDimensionsItem->GetType() != DATA_TYPE_DIMENSIONS)
		{
			throw std::runtime_error("The item '" + WindowDimensionsItem->GetName() + "' does not have the type '" + to_string_cast(DATA_TYPE_DIMENSIONS) + "' but the type '" + to_string_cast(WindowDimensionsItem->GetType()) + "'.");
		}
		if(WindowDimensionsItem->GetSubType() != DATA_TYPE_DIMENSIONS_SUB_TYPE_DIMENSIONS2F)
		{
			throw std::runtime_error("The item '" + WindowDimensionsItem->GetName() + "' does not have the sub type '" + to_string_cast(DATA_TYPE_DIMENSIONS_SUB_TYPE_DIMENSIONS2F) + "' but the sub type '" + to_string_cast(WindowDimensionsItem->GetSubType()) + "'.");
		}
		MakeItemAvailable(WindowDimensionsItem);
		
		auto Reader = ARX::BufferReader{*WindowDimensionsItem};
		auto WindowDimensions = Vector2f{};
		
		Reader >> WindowDimensions;
		m_WindowDimensions = WindowDimensions;
	}
}
