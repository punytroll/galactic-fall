/**
 * libarxx - Advanced Resource files in C++
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
#include <iostream>

#include <arxx/buffer.h>
#include <arxx/buffer_writer.h>
#include <arxx/reference.h>
#include <arxx/structure.h>

#include "io.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
// Arxx::Structure                                                                                //
///////////////////////////////////////////////////////////////////////////////////////////////////

Arxx::Structure::Structure(Arxx::Item & Item) :
	m_Item{Item}
{
}

auto Arxx::Structure::Add(std::uint32_t ItemIdentifier, std::string const & Relation) -> bool
{
	if((Relation == "reference") || (ItemIdentifier == g_InvalidItemIdentifier))
	{
		return false;
	}
	
	auto RelationIterator = m_Relations.find(Relation);
	
	if(RelationIterator == m_Relations.end())
	{
		RelationIterator = m_Relations.insert(std::make_pair(Relation, Arxx::Structure::Relation(*this, Relation))).first;
	}
	RelationIterator->second.Add(ItemIdentifier);
	
	return true;
}

auto Arxx::Structure::Delete(std::uint32_t ItemIdentifier, std::string const & Relation) -> bool
{
	if((Relation == "reference") || (ItemIdentifier == g_InvalidItemIdentifier))
	{
		return false;
	}
	
	auto RelationIterator = m_Relations.find(Relation);
	
	if(RelationIterator == m_Relations.end())
	{
		return false;
	}
	
	return RelationIterator->second.Delete(ItemIdentifier);
}

auto Arxx::Structure::GetRelation(std::string const & Relation) const -> Arxx::Structure::Relation const &
{
	auto RelationIterator = m_Relations.find(Relation);
	
	if(RelationIterator == m_Relations.end())
	{
		throw std::runtime_error{"Relation \"" + Relation + "\" not found."};
	}
	else
	{
		return RelationIterator->second;
	}
}

auto Arxx::Structure::GetRelation(std::string const & Relation) -> Arxx::Structure::Relation &
{
	auto RelationIterator = m_Relations.find(Relation);
	
	if(RelationIterator == m_Relations.end())
	{
		throw std::runtime_error{"Relation \"" + Relation + "\" not found."};
	}
	else
	{
		return RelationIterator->second;
	}
}

auto Arxx::Structure::size() const -> Arxx::Structure::size_type
{
	return m_Relations.size();
}

auto Arxx::Structure::begin() -> Arxx::Structure::iterator
{
	return m_Relations.begin();
}

auto Arxx::Structure::end() -> Arxx::Structure::iterator
{
	return m_Relations.end();
}

auto Arxx::Structure::begin() const -> Arxx::Structure::const_iterator
{
	return m_Relations.begin();
}

auto Arxx::Structure::end() const -> Arxx::Structure::const_iterator
{
	return m_Relations.end();
}

auto Arxx::Structure::m_RemoveRelation(Arxx::Structure::Relation * Relation) -> void
{
    std::erase_if(m_Relations, [Relation](auto const & Item)
                               {
                                   return std::addressof(Item.second) == Relation;
                               });
}

auto Arxx::Structure::HasRelation(std::string const & Relation) const -> bool
{
	return m_Relations.find(Relation) != m_Relations.end();
}

auto Arxx::Structure::m_ReadFromStream(std::uint32_t StructureDataLength, std::istream & IStream) -> void
{
	while(StructureDataLength > 0)
	{
		auto Relation = ARX::ReadStringWithTermination(IStream);
		
		assert(IStream);
		StructureDataLength -= Relation.length() + 1;
		
		auto ItemCount = ARX::ReadUnsignedInteger32Bit(IStream);
		
		assert(IStream);
		StructureDataLength -= 4;
		while(ItemCount > 0)
		{
			auto ItemIdentifier = ARX::ReadUnsignedInteger32Bit(IStream);
			
			assert(IStream);
			StructureDataLength -= 4;
			Add(ItemIdentifier, Relation);
			--ItemCount;
		}
	}
}

auto Arxx::Structure::m_WriteToBuffer(Arxx::Buffer & Buffer) const -> void
{
	auto BufferWriter = Arxx::BufferWriter{Buffer};
    
    for(auto & [RelationName, Relation] : m_Relations)
    {
		BufferWriter << RelationName;
		BufferWriter << ARX::ToNetworkByteOrder(Relation.size());
		for(auto Reference : Relation)
        {
			BufferWriter << ARX::ToNetworkByteOrder(Reference.GetItemIdentifier());
        }
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Arxx::Structure::iterator                                                                     //
///////////////////////////////////////////////////////////////////////////////////////////////////

Arxx::Structure::iterator::iterator(std::map<std::string, Arxx::Structure::Relation>::iterator Iterator) :
	m_Iterator{Iterator}
{
}

auto Arxx::Structure::iterator::operator++() -> Arxx::Structure::iterator &
{
	++m_Iterator;
	
	return *this;
}

auto Arxx::Structure::iterator::operator*() -> Arxx::Structure::Relation &
{
	return m_Iterator->second;
}

auto Arxx::Structure::iterator::operator*() const -> Arxx::Structure::Relation const &
{
	return m_Iterator->second;
}

auto Arxx::Structure::iterator::operator->() -> Arxx::Structure::Relation *
{
	return std::addressof(m_Iterator->second);
}

auto Arxx::Structure::iterator::operator==(Arxx::Structure::iterator const & Other) const -> bool
{
	return m_Iterator == Other.m_Iterator;
}

auto Arxx::Structure::iterator::operator!=(Arxx::Structure::iterator const & Other) const -> bool
{
	return m_Iterator != Other.m_Iterator;
}

auto Arxx::Structure::iterator::operator==(Arxx::Structure::const_iterator const & Other) const -> bool
{
	return m_Iterator == Other.m_Iterator;
}

auto Arxx::Structure::iterator::operator!=(Arxx::Structure::const_iterator const & Other) const -> bool
{
	return m_Iterator != Other.m_Iterator;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Arxx::Structure::const_iterator                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////

Arxx::Structure::const_iterator::const_iterator(std::map<std::string, Arxx::Structure::Relation>::const_iterator Iterator) :
	m_Iterator{Iterator}
{
}

Arxx::Structure::const_iterator::const_iterator(Arxx::Structure::iterator Iterator) :
	m_Iterator{Iterator.m_Iterator}
{
}

auto Arxx::Structure::const_iterator::operator++() -> Arxx::Structure::const_iterator &
{
	++m_Iterator;
	
	return *this;
}

auto Arxx::Structure::const_iterator::operator*() const -> Arxx::Structure::Relation const &
{
	return m_Iterator->second;
}

auto Arxx::Structure::const_iterator::operator->() -> Arxx::Structure::Relation const *
{
	return std::addressof(m_Iterator->second);
}

auto Arxx::Structure::const_iterator::operator==(Arxx::Structure::const_iterator const & Other) const -> bool
{
	return m_Iterator == Other.m_Iterator;
}

auto Arxx::Structure::const_iterator::operator!=(Arxx::Structure::const_iterator const & Other) const -> bool
{
	return m_Iterator != Other.m_Iterator;
}

auto Arxx::operator<<(Arxx::Buffer & Buffer, Arxx::Structure const & Structure) -> Arxx::Buffer &
{
	Structure.m_WriteToBuffer(Buffer);
	
	return Buffer;
}
