/**
 * libarxx - Advanced Resource files in C++
 * Copyright (C) 2005  Hagen Möbius
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

#include <Item.h>
#include <Archive.h>

#include "ReferenceImplementation.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
// Arxx::ReferenceImplementation                                                                  //
///////////////////////////////////////////////////////////////////////////////////////////////////

Arxx::ReferenceImplementation::ReferenceImplementation(void) :
	m_u4UniqueID(0),
	m_pItem(0),
	m_u4ReferenceCount(1),
	m_pArchive(0)
{
}

Arxx::ReferenceImplementation::~ReferenceImplementation(void)
{
	assert(m_u4ReferenceCount == 0);
}

Arxx::ReferenceImplementation * Arxx::ReferenceImplementation::pGetReference(Arxx::Item & Item)
{
	Arxx::ReferenceImplementation * pReference(new Arxx::ReferenceImplementation);
	
	pReference->m_pItem = &Item;
	pReference->m_u4UniqueID = Item.GetIdentifier();
	pReference->m_u4ReferenceCount = 1;
	pReference->m_pArchive = Item.GetArchive();
	
	return pReference;
}

Arxx::ReferenceImplementation * Arxx::ReferenceImplementation::pGetReference(Arxx::u4byte u4UniqueID, Arxx::Archive * pArchive)
{
	Arxx::ReferenceImplementation * pReference(new Arxx::ReferenceImplementation);
	
	pReference->m_pItem = 0;
	pReference->m_u4UniqueID = u4UniqueID;
	pReference->m_u4ReferenceCount = 1;
	pReference->m_pArchive = pArchive;
	
	return pReference;
}

Arxx::ReferenceImplementation * Arxx::ReferenceImplementation::pGetReference(Arxx::ReferenceImplementation * pReference)
{
	pReference->m_u4ReferenceCount++;
	
	return pReference;
}

bool Arxx::ReferenceImplementation::bRelease(Arxx::ReferenceImplementation * pReference)
{
	assert(pReference->m_u4ReferenceCount != 0);
	pReference->m_u4ReferenceCount--;
	if(pReference->m_u4ReferenceCount == 0)
	{
		delete pReference;
		
		return true;
	}
	else
	{
		if((pReference->m_pItem == 0) && (pReference->m_pArchive != 0) && (pReference->m_u4ReferenceCount == 1))
		{
			pReference->m_pArchive->ReleaseReference(pReference);
			
			return true;
		}
		else
		{
			return false;
		}
	}
}

Arxx::u4byte Arxx::ReferenceImplementation::u4GetUniqueID(void) const
{
	return m_u4UniqueID;
}

Arxx::Item * Arxx::ReferenceImplementation::pGetItem(void)
{
	return m_pItem;
}

const Arxx::Item * Arxx::ReferenceImplementation::pGetItem(void) const
{
	return m_pItem;
}

Arxx::u4byte Arxx::ReferenceImplementation::u4GetReferenceCount(void) const
{
	return m_u4ReferenceCount;
}

void Arxx::ReferenceImplementation::vResolve(Arxx::Item & Item)
{
	if(pGetItem() != 0)
	{
		throw std::runtime_error("Arxx::ReferenceImplementation::Resolve: Trying to resolve a resolved reference.");
	}
	if(Item.GetIdentifier() != u4GetUniqueID())
	{
		throw std::runtime_error("Arxx::ReferenceImplementation::Resolve: Trying to resolve a reference with different unique ID.");
	}
	m_pItem = &Item;
}

void Arxx::ReferenceImplementation::vUnresolve(void)
{
	if(pGetItem() == 0)
	{
		throw std::runtime_error("Arxx::ReferenceImplementation::Unresolve: Trying to unresolve an unresolved reference.");
	}
	m_pItem = 0;
}

void Arxx::ReferenceImplementation::vDecoupleFromArchive(void)
{
	m_pArchive = 0;
}
