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

#include <Item.h>
#include <Archive.h>
#include <Reference.h>
#include <Structure.h>


///////////////////////////////////////////////////////////////////////////////////////////////////
// Arxx::Structure::Relation                                                                      //
///////////////////////////////////////////////////////////////////////////////////////////////////

Arxx::Structure::Relation::Relation(Arxx::Structure & Structure, const std::string & sName) :
	m_Structure(Structure),
	m_sName(sName)
{
}

void Arxx::Structure::Relation::vAdd(Arxx::u4byte u4UniqueID)
{
	if(m_Structure.m_Item.GetArchive() == 0)
	{
		m_References.insert(std::make_pair(u4UniqueID, Arxx::Reference(u4UniqueID)));
	}
	else
	{
		m_References.insert(std::make_pair(u4UniqueID, m_Structure.m_Item.GetArchive()->GetReference(u4UniqueID)));
	}
}

bool Arxx::Structure::Relation::bDelete(Arxx::u4byte u4UniqueID)
{
	std::multimap< Arxx::u4byte, Arxx::Reference >::iterator iReference(m_References.lower_bound(u4UniqueID));
	
	if(iReference == m_References.end())
	{
		return false;
	}
	m_References.erase(iReference);
	if(m_References.size() == 0)
	{
		m_Structure.vRemoveRelation(this);
	}
	
	return true;
}

std::list< Arxx::Item * > Arxx::Structure::Relation::GetItems(const std::string & Name)
{
	std::list< Arxx::Item * > Result;
	
	for(std::multimap< Arxx::u4byte, Arxx::Reference >::iterator ItemIterator = m_References.begin(); ItemIterator != m_References.end(); ++ItemIterator)
	{
		if((ItemIterator->second.pGetItem() != 0) && (ItemIterator->second.pGetItem()->GetName() == Name))
		{
			Result.push_back(ItemIterator->second.pGetItem());
		}
	}
	
	return Result;
}

Arxx::Structure::Relation::size_type Arxx::Structure::Relation::size(void) const
{
	return m_References.size();
}

Arxx::Structure::Relation::iterator Arxx::Structure::Relation::begin(void)
{
	return m_References.begin();
}

Arxx::Structure::Relation::iterator Arxx::Structure::Relation::end(void)
{
	return m_References.end();
}

Arxx::Structure::Relation::const_iterator Arxx::Structure::Relation::begin(void) const
{
	return m_References.begin();
}

Arxx::Structure::Relation::const_iterator Arxx::Structure::Relation::end(void) const
{
	return m_References.end();
}

const std::string & Arxx::Structure::Relation::sGetName(void) const
{
	return m_sName;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Arxx::Structure::Relation::iterator                                                            //
///////////////////////////////////////////////////////////////////////////////////////////////////

Arxx::Structure::Relation::iterator::iterator(std::multimap< Arxx::u4byte, Arxx::Reference >::iterator iReference) :
	m_iReference(iReference)
{
}

Arxx::Structure::Relation::iterator::~iterator(void)
{
}

Arxx::Structure::Relation::iterator & Arxx::Structure::Relation::iterator::operator++(void)
{
	++m_iReference;
	
	return *this;
}

Arxx::Reference & Arxx::Structure::Relation::iterator::operator*(void)
{
	return m_iReference->second;
}

const Arxx::Reference & Arxx::Structure::Relation::iterator::operator*(void) const
{
	return m_iReference->second;
}

Arxx::Reference * Arxx::Structure::Relation::iterator::operator->(void)
{
	return &(m_iReference->second);
}

bool Arxx::Structure::Relation::iterator::operator==(const Arxx::Structure::Relation::iterator & iReference) const
{
	return m_iReference == iReference.m_iReference;
}

bool Arxx::Structure::Relation::iterator::operator!=(const Arxx::Structure::Relation::iterator & iReference) const
{
	return m_iReference != iReference.m_iReference;
}

bool Arxx::Structure::Relation::iterator::operator==(const Arxx::Structure::Relation::const_iterator & iReference) const
{
	return m_iReference == iReference.m_iReference;
}

bool Arxx::Structure::Relation::iterator::operator!=(const Arxx::Structure::Relation::const_iterator & iReference) const
{
	return m_iReference != iReference.m_iReference;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Arxx::Structure::Relation::const_iterator                                                      //
///////////////////////////////////////////////////////////////////////////////////////////////////

Arxx::Structure::Relation::const_iterator::const_iterator(std::multimap< Arxx::u4byte, Arxx::Reference >::const_iterator iReference) :
	m_iReference(iReference)
{
}

Arxx::Structure::Relation::const_iterator::const_iterator(Arxx::Structure::Relation::iterator iIterator) :
	m_iReference(iIterator.m_iReference)
{
}

Arxx::Structure::Relation::const_iterator::~const_iterator(void)
{
}

Arxx::Structure::Relation::const_iterator & Arxx::Structure::Relation::const_iterator::operator++(void)
{
	++m_iReference;
	
	return *this;
}

const Arxx::Reference & Arxx::Structure::Relation::const_iterator::operator*(void) const
{
	return m_iReference->second;
}

const Arxx::Reference * Arxx::Structure::Relation::const_iterator::operator->(void)
{
	return &(m_iReference->second);
}

bool Arxx::Structure::Relation::const_iterator::operator==(const Arxx::Structure::Relation::const_iterator & iReference) const
{
	return m_iReference == iReference.m_iReference;
}

bool Arxx::Structure::Relation::const_iterator::operator!=(const Arxx::Structure::Relation::const_iterator & iReference) const
{
	return m_iReference != iReference.m_iReference;
}
