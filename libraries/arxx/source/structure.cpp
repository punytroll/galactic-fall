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

#include <netinet/in.h>

#include <cassert>
#include <iostream>

#include <arxx/buffer.h>
#include <arxx/buffer_writer.h>
#include <arxx/reference.h>
#include <arxx/structure.h>


///////////////////////////////////////////////////////////////////////////////////////////////////
// Arxx::Structure                                                                                //
///////////////////////////////////////////////////////////////////////////////////////////////////

Arxx::Structure::Structure(Arxx::Item & Item) :
	m_Item(Item)
{
}

auto Arxx::Structure::bAdd(Arxx::u4byte u4UniqueID, std::string const & sRelation) -> bool
{
	if((sRelation == "reference") || (u4UniqueID == g_u4InvalidID))
	{
		return false;
	}
	
	std::map<std::string, Arxx::Structure::Relation>::iterator iRelation(m_Relations.find(sRelation));
	
	if(iRelation == m_Relations.end())
	{
		iRelation = m_Relations.insert(std::make_pair(sRelation, Arxx::Structure::Relation(*this, sRelation))).first;
	}
	iRelation->second.vAdd(u4UniqueID);
	
	return true;
}

auto Arxx::Structure::bDelete(Arxx::u4byte u4UniqueID, std::string const & sRelation) -> bool
{
	if((sRelation == "reference") || (u4UniqueID == g_u4InvalidID))
	{
		return false;
	}
	
	std::map<std::string, Arxx::Structure::Relation>::iterator iRelation(m_Relations.find(sRelation));
	
	if(iRelation == m_Relations.end())
	{
		return false;
	}
	
	return iRelation->second.bDelete(u4UniqueID);
}

auto Arxx::Structure::GetRelation(std::string const & sRelation) const -> Arxx::Structure::Relation const &
{
	std::map<std::string, Arxx::Structure::Relation>::const_iterator iRelation(m_Relations.find(sRelation));
	
	if(iRelation == m_Relations.end())
	{
		throw std::runtime_error("Relation \"" + sRelation + "\" not found.");
	}
	else
	{
		return iRelation->second;
	}
}

auto Arxx::Structure::GetRelation(std::string const & sRelation) -> Arxx::Structure::Relation &
{
	std::map<std::string, Arxx::Structure::Relation>::iterator iRelation(m_Relations.find(sRelation));
	
	if(iRelation == m_Relations.end())
	{
		throw std::runtime_error("Relation \"" + sRelation + "\" not found.");
	}
	else
	{
		return iRelation->second;
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

static auto u4GetU4Byte(std::istream & IStream) -> Arxx::u4byte
{
	Arxx::u4byte u4Result(0);
	
	IStream.read(reinterpret_cast< std::istream::char_type * >(&u4Result), sizeof(Arxx::u4byte));
	u4Result = ntohl(u4Result);
	
	return u4Result;
}

static auto sGetString(std::istream & IStream) -> std::string
{
	std::string sResult;
	char cChar(0);
	
	while(IStream.get(cChar))
	{
		if(cChar != '\0')
		{
			sResult += cChar;
		}
		else
		{
			break;
		}
	}
	
	return sResult;
}

auto Arxx::Structure::vRemoveRelation(Arxx::Structure::Relation * pRelation) -> void
{
	std::map<std::string, Arxx::Structure::Relation>::iterator iRelation(m_Relations.begin());
	
	while(iRelation != m_Relations.end())
	{
		if(&(iRelation->second) == pRelation)
		{
			m_Relations.erase(iRelation);
			
			return;
		}
		++iRelation;
	}
}

auto Arxx::Structure::bHasRelation(std::string const & sRelation) const -> bool
{
	return m_Relations.find(sRelation) != m_Relations.end();
}

auto Arxx::Structure::vReadFromStream(Arxx::u4byte u4StructureDataLength, std::istream & IStream) -> void
{
	while(u4StructureDataLength > 0)
	{
		std::string sRelation(sGetString(IStream));
		
		assert(IStream);
		u4StructureDataLength -= sRelation.length() + 1;
		
		Arxx::u4byte u4ItemCount(u4GetU4Byte(IStream));
		
		assert(IStream);
		u4StructureDataLength -= 4;
		while(u4ItemCount > 0)
		{
			Arxx::u4byte u4UniqueID(u4GetU4Byte(IStream));
			
			assert(IStream);
			u4StructureDataLength -= 4;
			bAdd(u4UniqueID, sRelation);
			--u4ItemCount;
		}
	}
}

auto Arxx::Structure::vWriteToBuffer(Arxx::Buffer & Buffer) const -> void
{
	Arxx::BufferWriter BufferWriter(Buffer);
	std::map<std::string, Arxx::Structure::Relation>::const_iterator iRelation(m_Relations.begin());
	
	while(iRelation != m_Relations.end())
	{
		BufferWriter << iRelation->first;
		
		const Arxx::Structure::Relation & Relation(iRelation->second);
		
		BufferWriter << static_cast< Arxx::u4byte >(htonl(Relation.size()));
		
		Arxx::Structure::Relation::const_iterator iReference(Relation.begin());
		
		while(iReference != Relation.end())
		{
			BufferWriter << static_cast< Arxx::u4byte >(htonl(iReference->u4GetUniqueID()));
			++iReference;
		}
		++iRelation;
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Arxx::Structure::iterator                                                                     //
///////////////////////////////////////////////////////////////////////////////////////////////////

Arxx::Structure::iterator::iterator(std::map<std::string, Arxx::Structure::Relation>::iterator iRelation) :
	m_iRelation(iRelation)
{
}

Arxx::Structure::iterator::~iterator()
{
}

auto Arxx::Structure::iterator::operator++() -> Arxx::Structure::iterator &
{
	++m_iRelation;
	
	return *this;
}

auto Arxx::Structure::iterator::operator*() -> Arxx::Structure::Relation &
{
	return m_iRelation->second;
}

auto Arxx::Structure::iterator::operator*() const -> Arxx::Structure::Relation const &
{
	return m_iRelation->second;
}

auto Arxx::Structure::iterator::operator->() -> Arxx::Structure::Relation *
{
	return &(m_iRelation->second);
}

auto Arxx::Structure::iterator::operator==(Arxx::Structure::iterator const & iRelation) const -> bool
{
	return m_iRelation == iRelation.m_iRelation;
}

auto Arxx::Structure::iterator::operator!=(Arxx::Structure::iterator const & iRelation) const -> bool
{
	return m_iRelation != iRelation.m_iRelation;
}

auto Arxx::Structure::iterator::operator==(Arxx::Structure::const_iterator const & iRelation) const -> bool
{
	return m_iRelation == iRelation.m_iRelation;
}

auto Arxx::Structure::iterator::operator!=(Arxx::Structure::const_iterator const & iRelation) const -> bool
{
	return m_iRelation != iRelation.m_iRelation;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Arxx::Structure::const_iterator                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////

Arxx::Structure::const_iterator::const_iterator(std::map<std::string, Arxx::Structure::Relation>::const_iterator iRelation) :
	m_iRelation(iRelation)
{
}

Arxx::Structure::const_iterator::const_iterator(Arxx::Structure::iterator iRelation) :
	m_iRelation(iRelation.m_iRelation)
{
}

Arxx::Structure::const_iterator::~const_iterator()
{
}

auto Arxx::Structure::const_iterator::operator++() -> Arxx::Structure::const_iterator &
{
	++m_iRelation;
	
	return *this;
}

auto Arxx::Structure::const_iterator::operator*() const -> Arxx::Structure::Relation const &
{
	return m_iRelation->second;
}

auto Arxx::Structure::const_iterator::operator->() -> Arxx::Structure::Relation const *
{
	return std::addressof(m_iRelation->second);
}

auto Arxx::Structure::const_iterator::operator==(Arxx::Structure::const_iterator const & Other) const -> bool
{
	return m_iRelation == Other.m_iRelation;
}

auto Arxx::Structure::const_iterator::operator!=(Arxx::Structure::const_iterator const & Other) const -> bool
{
	return m_iRelation != Other.m_iRelation;
}

auto Arxx::operator<<(Arxx::Buffer & Buffer, Arxx::Structure const & Structure) -> Arxx::Buffer &
{
	Structure.vWriteToBuffer(Buffer);
	
	return Buffer;
}
