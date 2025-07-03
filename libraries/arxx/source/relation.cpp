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

#include <arxx/archive.h>
#include <arxx/item.h>
#include <arxx/reference.h>
#include <arxx/structure.h>


///////////////////////////////////////////////////////////////////////////////////////////////////
// Arxx::Structure::Relation                                                                      //
///////////////////////////////////////////////////////////////////////////////////////////////////

Arxx::Structure::Relation::Relation(Arxx::Structure & Structure, std::string const & sName) :
	m_Structure(Structure),
	m_sName(sName)
{
}

auto Arxx::Structure::Relation::vAdd(Arxx::u4byte u4UniqueID) -> void
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

auto Arxx::Structure::Relation::bDelete(Arxx::u4byte u4UniqueID) -> bool
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

auto Arxx::Structure::Relation::GetItems(std::string const & Name) -> std::list<Arxx::Item *>
{
	std::list<Arxx::Item *> Result;
	
	for(std::multimap< Arxx::u4byte, Arxx::Reference >::iterator ItemIterator = m_References.begin(); ItemIterator != m_References.end(); ++ItemIterator)
	{
		if((ItemIterator->second.pGetItem() != 0) && (ItemIterator->second.pGetItem()->GetName() == Name))
		{
			Result.push_back(ItemIterator->second.pGetItem());
		}
	}
	
	return Result;
}

auto Arxx::Structure::Relation::size() const -> Arxx::Structure::Relation::size_type
{
	return m_References.size();
}

auto Arxx::Structure::Relation::begin() -> Arxx::Structure::Relation::iterator
{
	return m_References.begin();
}

auto Arxx::Structure::Relation::end() -> Arxx::Structure::Relation::iterator
{
	return m_References.end();
}

auto Arxx::Structure::Relation::begin() const -> Arxx::Structure::Relation::const_iterator
{
	return m_References.begin();
}

auto Arxx::Structure::Relation::end() const -> Arxx::Structure::Relation::const_iterator
{
	return m_References.end();
}

auto Arxx::Structure::Relation::sGetName() const -> std::string const &
{
	return m_sName;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Arxx::Structure::Relation::iterator                                                            //
///////////////////////////////////////////////////////////////////////////////////////////////////

Arxx::Structure::Relation::iterator::iterator(std::multimap<Arxx::u4byte, Arxx::Reference>::iterator iReference) :
	m_iReference(iReference)
{
}

Arxx::Structure::Relation::iterator::~iterator()
{
}

auto Arxx::Structure::Relation::iterator::operator++() -> Arxx::Structure::Relation::iterator &
{
	++m_iReference;
	
	return *this;
}

auto Arxx::Structure::Relation::iterator::operator*() -> Arxx::Reference &
{
	return m_iReference->second;
}

auto Arxx::Structure::Relation::iterator::operator*() const -> Arxx::Reference const &
{
	return m_iReference->second;
}

auto Arxx::Structure::Relation::iterator::operator->() -> Arxx::Reference *
{
	return &(m_iReference->second);
}

auto Arxx::Structure::Relation::iterator::operator==(Arxx::Structure::Relation::iterator const & Other) const -> bool
{
	return m_iReference == Other.m_iReference;
}

auto Arxx::Structure::Relation::iterator::operator!=(Arxx::Structure::Relation::iterator const & Other) const -> bool
{
	return m_iReference != Other.m_iReference;
}

auto Arxx::Structure::Relation::iterator::operator==(Arxx::Structure::Relation::const_iterator const & Other) const -> bool
{
	return m_iReference == Other.m_iReference;
}

auto Arxx::Structure::Relation::iterator::operator!=(Arxx::Structure::Relation::const_iterator const & Other) const -> bool
{
	return m_iReference != Other.m_iReference;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Arxx::Structure::Relation::const_iterator                                                      //
///////////////////////////////////////////////////////////////////////////////////////////////////

Arxx::Structure::Relation::const_iterator::const_iterator(std::multimap<Arxx::u4byte, Arxx::Reference>::const_iterator iReference) :
	m_iReference(iReference)
{
}

Arxx::Structure::Relation::const_iterator::const_iterator(Arxx::Structure::Relation::iterator iIterator) :
	m_iReference(iIterator.m_iReference)
{
}

Arxx::Structure::Relation::const_iterator::~const_iterator()
{
}

auto Arxx::Structure::Relation::const_iterator::operator++() -> Arxx::Structure::Relation::const_iterator &
{
	++m_iReference;
	
	return *this;
}

auto Arxx::Structure::Relation::const_iterator::operator*() const -> Arxx::Reference const &
{
	return m_iReference->second;
}

auto Arxx::Structure::Relation::const_iterator::operator->() -> Arxx::Reference const *
{
	return std::addressof(m_iReference->second);
}

auto Arxx::Structure::Relation::const_iterator::operator==(Arxx::Structure::Relation::const_iterator const & Other) const -> bool
{
	return m_iReference == Other.m_iReference;
}

auto Arxx::Structure::Relation::const_iterator::operator!=(Arxx::Structure::Relation::const_iterator const & Other) const -> bool
{
	return m_iReference != Other.m_iReference;
}
