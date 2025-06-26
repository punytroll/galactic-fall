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

#include <arxx/Reference.h>

#include "ReferenceImplementation.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
// Arxx::Reference                                                                                //
///////////////////////////////////////////////////////////////////////////////////////////////////

Arxx::Reference::Reference(Arxx::Item & Item) :
	m_pReference(Arxx::ReferenceImplementation::pGetReference(Item))
{
}

Arxx::Reference::Reference(Arxx::u4byte u4UniqueID, Arxx::Archive * pArchive) :
	m_pReference(Arxx::ReferenceImplementation::pGetReference(u4UniqueID, pArchive))
{
}

Arxx::Reference::Reference(const Arxx::Reference & Reference) :
	m_pReference(0)
{
	vChangeReference(Reference.m_pReference);
}

Arxx::Reference::~Reference(void)
{
	vChangeReference(0);
}

Arxx::u4byte Arxx::Reference::u4GetUniqueID(void) const
{
	return m_pReference->u4GetUniqueID();
}

Arxx::Item * Arxx::Reference::pGetItem(void)
{
	return m_pReference->pGetItem();
}

const Arxx::Item * Arxx::Reference::pGetItem(void) const
{
	return m_pReference->pGetItem();
}

Arxx::u4byte Arxx::Reference::u4GetReferenceCount(void) const
{
	return m_pReference->u4GetReferenceCount();
}

void Arxx::Reference::vResolve(Arxx::Item & Item)
{
	m_pReference->vResolve(Item);
}

void Arxx::Reference::vUnresolve(void)
{
	m_pReference->vUnresolve();
}

bool Arxx::Reference::bAttach(Arxx::Reference & Reference)
{
	if((Reference.u4GetUniqueID() != u4GetUniqueID()) || (Reference.pGetItem() != 0))
	{
		return false;
	}
	Reference.vChangeReference(m_pReference);
	
	return true;
}

bool Arxx::Reference::bDetach(void)
{
	Arxx::ReferenceImplementation * pReference(Arxx::ReferenceImplementation::pGetReference(u4GetUniqueID()));
	
	vChangeReference(pReference);
	// releasing the local pointer
	Arxx::ReferenceImplementation::bRelease(pReference);
	
	return true;
}

void Arxx::Reference::vDecoupleFromArchive(void)
{
	m_pReference->vDecoupleFromArchive();
}

void Arxx::Reference::vChangeReference(Arxx::ReferenceImplementation * pReference)
{
	if(m_pReference != 0)
	{
		Arxx::ReferenceImplementation::bRelease(m_pReference);
		m_pReference = 0;
	}
	if(pReference != 0)
	{
		m_pReference = Arxx::ReferenceImplementation::pGetReference(pReference);
	}
}
