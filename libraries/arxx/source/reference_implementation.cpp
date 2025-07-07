/**
 * libarxx - Advanced Resource files in C++
 * Copyright (C) 2005-2025  Hagen Möbius
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

#include <cassert>

#include <arxx/item.h>
#include <arxx/archive.h>

#include "reference_implementation.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
// Arxx::ReferenceImplementation                                                                 //
///////////////////////////////////////////////////////////////////////////////////////////////////
Arxx::ReferenceImplementation::~ReferenceImplementation()
{
	assert(m_ReferenceCount == 0);
}

auto Arxx::ReferenceImplementation::Create(Arxx::Item & Item) -> Arxx::ReferenceImplementation *
{
	Arxx::ReferenceImplementation * ReferenceImplementation(new Arxx::ReferenceImplementation{});
	
	ReferenceImplementation->m_Item = &Item;
	ReferenceImplementation->m_ItemIdentifier = Item.GetIdentifier();
	ReferenceImplementation->m_ReferenceCount = 1;
	ReferenceImplementation->m_Archive = Item.GetArchive();
	
	return ReferenceImplementation;
}

auto Arxx::ReferenceImplementation::Create(Arxx::u4byte ItemIdentifier, Arxx::Archive * Archive) -> Arxx::ReferenceImplementation *
{
	Arxx::ReferenceImplementation * ReferenceImplementation(new Arxx::ReferenceImplementation{});
	
	ReferenceImplementation->m_Item = nullptr;
	ReferenceImplementation->m_ItemIdentifier = ItemIdentifier;
	ReferenceImplementation->m_ReferenceCount = 1;
	ReferenceImplementation->m_Archive = Archive;
	
	return ReferenceImplementation;
}

auto Arxx::ReferenceImplementation::Create(Arxx::ReferenceImplementation * ReferenceImplementation) -> Arxx::ReferenceImplementation *
{
	ReferenceImplementation->m_ReferenceCount++;
	
	return ReferenceImplementation;
}

auto Arxx::ReferenceImplementation::Release(Arxx::ReferenceImplementation * ReferenceImplementation) -> bool
{
	assert(ReferenceImplementation->m_ReferenceCount != 0);
	ReferenceImplementation->m_ReferenceCount--;
	if(ReferenceImplementation->m_ReferenceCount == 0)
	{
		delete ReferenceImplementation;
		
		return true;
	}
	else
	{
		if((ReferenceImplementation->m_Item == nullptr) && (ReferenceImplementation->m_Archive != nullptr) && (ReferenceImplementation->m_ReferenceCount == 1))
		{
			ReferenceImplementation->m_Archive->ReleaseReference(ReferenceImplementation);
			
			return true;
		}
		else
		{
			return false;
		}
	}
}

auto Arxx::ReferenceImplementation::GetItemIdentifier() const -> Arxx::u4byte
{
	return m_ItemIdentifier;
}

auto Arxx::ReferenceImplementation::GetItem() -> Arxx::Item *
{
	return m_Item;
}

auto Arxx::ReferenceImplementation::GetItem() const -> Arxx::Item const *
{
	return m_Item;
}

auto Arxx::ReferenceImplementation::GetReferenceCount() const -> Arxx::u4byte
{
	return m_ReferenceCount;
}

auto Arxx::ReferenceImplementation::Resolve(Arxx::Item & Item) -> void
{
	if(GetItem() != nullptr)
	{
		throw std::runtime_error("Arxx::ReferenceImplementation::Resolve: Trying to resolve a resolved reference.");
	}
	if(Item.GetIdentifier() != GetItemIdentifier())
	{
		throw std::runtime_error("Arxx::ReferenceImplementation::Resolve: Trying to resolve a reference with different unique ID.");
	}
	m_Item = std::addressof(Item);
}

auto Arxx::ReferenceImplementation::Unresolve() -> void
{
	if(GetItem() == nullptr)
	{
		throw std::runtime_error("Arxx::ReferenceImplementation::Unresolve: Trying to unresolve an unresolved reference.");
	}
	m_Item = nullptr;
}

auto Arxx::ReferenceImplementation::DecoupleFromArchive() -> void
{
	m_Archive = nullptr;
}
