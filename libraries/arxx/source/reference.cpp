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

#include "reference_implementation.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
// Arxx::Reference                                                                                //
///////////////////////////////////////////////////////////////////////////////////////////////////

Arxx::Reference::Reference(Arxx::Item & Item) :
	m_ReferenceImplementation{Arxx::ReferenceImplementation::Create(Item)}
{
}

Arxx::Reference::Reference(Arxx::u4byte ItemIdentifier, Arxx::Archive * Archive) :
	m_ReferenceImplementation{Arxx::ReferenceImplementation::Create(ItemIdentifier, Archive)}
{
}

Arxx::Reference::Reference(Arxx::Reference const & Reference) :
	m_ReferenceImplementation{nullptr}
{
	ChangeReferenceImplementation(Reference.m_ReferenceImplementation);
}

Arxx::Reference::~Reference()
{
	ChangeReferenceImplementation(nullptr);
}

auto Arxx::Reference::GetItemIdentifier() const -> Arxx::u4byte
{
	return m_ReferenceImplementation->GetItemIdentifier();
}

auto Arxx::Reference::GetItem() -> Arxx::Item *
{
	return m_ReferenceImplementation->GetItem();
}

auto Arxx::Reference::GetItem() const -> Arxx::Item const *
{
	return m_ReferenceImplementation->GetItem();
}

auto Arxx::Reference::GetReferenceCount() const -> Arxx::u4byte
{
	return m_ReferenceImplementation->GetReferenceCount();
}

auto Arxx::Reference::Resolve(Arxx::Item & Item) -> void
{
	m_ReferenceImplementation->Resolve(Item);
}

auto Arxx::Reference::Unresolve() -> void
{
	m_ReferenceImplementation->Unresolve();
}

auto Arxx::Reference::Attach(Arxx::Reference & Reference) -> bool
{
	if((Reference.GetItemIdentifier() != GetItemIdentifier()) || (Reference.GetItem() != nullptr))
	{
		return false;
	}
	Reference.ChangeReferenceImplementation(m_ReferenceImplementation);
	
	return true;
}

auto Arxx::Reference::Detach() -> bool
{
	Arxx::ReferenceImplementation * ReferenceImplementation(Arxx::ReferenceImplementation::Create(GetItemIdentifier()));
	
	ChangeReferenceImplementation(ReferenceImplementation);
	// releasing the local pointer
	Arxx::ReferenceImplementation::Release(ReferenceImplementation);
	
	return true;
}

auto Arxx::Reference::DecoupleFromArchive() -> void
{
	m_ReferenceImplementation->DecoupleFromArchive();
}

auto Arxx::Reference::ChangeReferenceImplementation(Arxx::ReferenceImplementation * ReferenceImplementation) -> void
{
	if(m_ReferenceImplementation != nullptr)
	{
		Arxx::ReferenceImplementation::Release(m_ReferenceImplementation);
		m_ReferenceImplementation = nullptr;
	}
	if(ReferenceImplementation != nullptr)
	{
		m_ReferenceImplementation = Arxx::ReferenceImplementation::Create(ReferenceImplementation);
	}
}
