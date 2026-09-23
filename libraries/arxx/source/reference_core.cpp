/**
 * ARX - Advanced Resource Archives
 * Copyright (C) 2005-2026  Hagen Möbius
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

#include "reference_core.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
// ARX::ReferenceCore                                                                           //
///////////////////////////////////////////////////////////////////////////////////////////////////
ARX::ReferenceCore::~ReferenceCore()
{
	assert(m_ReferenceCount == 0);
}

auto ARX::ReferenceCore::Create(ARX::Item & Item) -> ARX::ReferenceCore *
{
	auto ReferenceCore = new ARX::ReferenceCore{};
	
	ReferenceCore->m_Item = &Item;
	ReferenceCore->m_ItemIdentifier = Item.GetIdentifier();
	ReferenceCore->m_ReferenceCount = 1;
	ReferenceCore->m_Archive = Item.GetArchive();
	
	return ReferenceCore;
}

auto ARX::ReferenceCore::Create(std::uint32_t ItemIdentifier, ARX::Archive * Archive) -> ARX::ReferenceCore *
{
	auto ReferenceCore = new ARX::ReferenceCore{};
	
	ReferenceCore->m_Item = nullptr;
	ReferenceCore->m_ItemIdentifier = ItemIdentifier;
	ReferenceCore->m_ReferenceCount = 1;
	ReferenceCore->m_Archive = Archive;
	
	return ReferenceCore;
}

auto ARX::ReferenceCore::Create(ARX::ReferenceCore * ReferenceCore) -> ARX::ReferenceCore *
{
	ReferenceCore->m_ReferenceCount++;
	
	return ReferenceCore;
}

auto ARX::ReferenceCore::Release(ARX::ReferenceCore * ReferenceCore) -> bool
{
	assert(ReferenceCore->m_ReferenceCount != 0);
	ReferenceCore->m_ReferenceCount--;
	if(ReferenceCore->m_ReferenceCount == 0)
	{
		delete ReferenceCore;
		
		return true;
	}
	else
	{
		if((ReferenceCore->m_Item == nullptr) && (ReferenceCore->m_Archive != nullptr) && (ReferenceCore->m_ReferenceCount == 1))
		{
			ReferenceCore->m_Archive->ReleaseReferenceCore(ReferenceCore);
			
			return true;
		}
		else
		{
			return false;
		}
	}
}

auto ARX::ReferenceCore::GetItemIdentifier() const -> std::uint32_t
{
	return m_ItemIdentifier;
}

auto ARX::ReferenceCore::GetItem() -> ARX::Item *
{
	return m_Item;
}

auto ARX::ReferenceCore::GetItem() const -> ARX::Item const *
{
	return m_Item;
}

auto ARX::ReferenceCore::GetReferenceCount() const -> std::uint32_t
{
	return m_ReferenceCount;
}

auto ARX::ReferenceCore::Resolve(ARX::Item & Item) -> void
{
	if(GetItem() != nullptr)
	{
		throw std::runtime_error{"ARX::ReferenceCore::Resolve: Trying to resolve a resolved reference."};
	}
	if(Item.GetIdentifier() != GetItemIdentifier())
	{
		throw std::runtime_error{"ARX::ReferenceCore::Resolve: Trying to resolve a reference with different unique ID."};
	}
	m_Item = std::addressof(Item);
}

auto ARX::ReferenceCore::Unresolve() -> void
{
	if(GetItem() == nullptr)
	{
		throw std::runtime_error{"ARX::ReferenceCore::Unresolve: Trying to unresolve an unresolved reference."};
	}
	m_Item = nullptr;
}

auto ARX::ReferenceCore::DecoupleFromArchive() -> void
{
	m_Archive = nullptr;
}
