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

#include <arxx/reference.h>

#include "reference_core.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
// Arxx::Reference                                                                                //
///////////////////////////////////////////////////////////////////////////////////////////////////

Arxx::Reference::Reference(Arxx::Item & Item) :
	m_ReferenceCore{Arxx::ReferenceCore::Create(Item)}
{
}

Arxx::Reference::Reference(Arxx::u4byte ItemIdentifier, Arxx::Archive * Archive) :
	m_ReferenceCore{Arxx::ReferenceCore::Create(ItemIdentifier, Archive)}
{
}

Arxx::Reference::Reference(Arxx::Reference const & Reference) :
	m_ReferenceCore{nullptr}
{
	m_SetCore(Reference.m_ReferenceCore);
}

Arxx::Reference::~Reference()
{
	m_SetCore(nullptr);
}

auto Arxx::Reference::GetItemIdentifier() const -> Arxx::u4byte
{
	return m_ReferenceCore->GetItemIdentifier();
}

auto Arxx::Reference::GetItem() -> Arxx::Item *
{
	return m_ReferenceCore->GetItem();
}

auto Arxx::Reference::GetItem() const -> Arxx::Item const *
{
	return m_ReferenceCore->GetItem();
}

auto Arxx::Reference::GetReferenceCount() const -> Arxx::u4byte
{
	return m_ReferenceCore->GetReferenceCount();
}

auto Arxx::Reference::Resolve(Arxx::Item & Item) -> void
{
	m_ReferenceCore->Resolve(Item);
}

auto Arxx::Reference::Unresolve() -> void
{
	m_ReferenceCore->Unresolve();
}

auto Arxx::Reference::Attach(Arxx::Reference & Reference) -> bool
{
	if((Reference.GetItemIdentifier() != GetItemIdentifier()) || (Reference.GetItem() != nullptr))
	{
		return false;
	}
	Reference.m_SetCore(m_ReferenceCore);
	
	return true;
}

auto Arxx::Reference::Detach() -> bool
{
	auto ReferenceCore = Arxx::ReferenceCore::Create(GetItemIdentifier());
	
	m_SetCore(ReferenceCore);
	// releasing the local pointer
	Arxx::ReferenceCore::Release(ReferenceCore);
	
	return true;
}

auto Arxx::Reference::DecoupleFromArchive() -> void
{
	m_ReferenceCore->DecoupleFromArchive();
}

auto Arxx::Reference::m_SetCore(Arxx::ReferenceCore * ReferenceCore) -> void
{
	if(m_ReferenceCore != nullptr)
	{
		Arxx::ReferenceCore::Release(m_ReferenceCore);
		m_ReferenceCore = nullptr;
	}
	if(ReferenceCore != nullptr)
	{
		m_ReferenceCore = Arxx::ReferenceCore::Create(ReferenceCore);
	}
}
