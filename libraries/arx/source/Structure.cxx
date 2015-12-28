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

#include <assert.h>
#include <netinet/in.h>

#include <iostream>

#include "../include/Buffer.h"
#include "../include/BufferWriter.h"
#include "../include/Reference.h"
#include "../include/Structure.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
// Arxx::Structure                                                                                //
///////////////////////////////////////////////////////////////////////////////////////////////////

Arxx::Structure::Structure(Arxx::Item & Item) :
	m_Item(Item)
{
}

bool Arxx::Structure::bAdd(Arxx::u4byte u4UniqueID, const std::string & sRelation)
{
	if((sRelation == "reference") || (u4UniqueID == g_u4InvalidID))
	{
		return false;
	}
	
	std::map< std::string, Arxx::Structure::Relation >::iterator iRelation(m_Relations.find(sRelation));
	
	if(iRelation == m_Relations.end())
	{
		iRelation = m_Relations.insert(std::make_pair(sRelation, Arxx::Structure::Relation(*this, sRelation))).first;
	}
	iRelation->second.vAdd(u4UniqueID);
	
	return true;
}

bool Arxx::Structure::bDelete(Arxx::u4byte u4UniqueID, const std::string & sRelation)
{
	if((sRelation == "reference") || (u4UniqueID == g_u4InvalidID))
	{
		return false;
	}
	
	std::map< std::string, Arxx::Structure::Relation >::iterator iRelation(m_Relations.find(sRelation));
	
	if(iRelation == m_Relations.end())
	{
		return false;
	}
	
	return iRelation->second.bDelete(u4UniqueID);
}

const Arxx::Structure::Relation & Arxx::Structure::GetRelation(const std::string & sRelation) const
{
	std::map< std::string, Arxx::Structure::Relation >::const_iterator iRelation(m_Relations.find(sRelation));
	
	if(iRelation == m_Relations.end())
	{
		throw std::runtime_error("Relation \"" + sRelation + "\" not found.");
	}
	else
	{
		return iRelation->second;
	}
}

Arxx::Structure::Relation & Arxx::Structure::GetRelation(const std::string & sRelation)
{
	std::map< std::string, Arxx::Structure::Relation >::iterator iRelation(m_Relations.find(sRelation));
	
	if(iRelation == m_Relations.end())
	{
		throw std::runtime_error("Relation \"" + sRelation + "\" not found.");
	}
	else
	{
		return iRelation->second;
	}
}

Arxx::Structure::size_type Arxx::Structure::size(void) const
{
	return m_Relations.size();
}

Arxx::Structure::iterator Arxx::Structure::begin(void)
{
	return m_Relations.begin();
}

Arxx::Structure::iterator Arxx::Structure::end(void)
{
	return m_Relations.end();
}

Arxx::Structure::const_iterator Arxx::Structure::begin(void) const
{
	return m_Relations.begin();
}

Arxx::Structure::const_iterator Arxx::Structure::end(void) const
{
	return m_Relations.end();
}

static Arxx::u4byte u4GetU4Byte(std::istream & IStream)
{
	Arxx::u4byte u4Result(0);
	
	IStream.read(reinterpret_cast< std::istream::char_type * >(&u4Result), sizeof(Arxx::u4byte));
	u4Result = ntohl(u4Result);
	
	return u4Result;
}

static std::string sGetString(std::istream & IStream)
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

void Arxx::Structure::vRemoveRelation(Arxx::Structure::Relation * pRelation)
{
	std::map< std::string, Arxx::Structure::Relation >::iterator iRelation(m_Relations.begin());
	
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

bool Arxx::Structure::bHasRelation(const std::string & sRelation) const
{
	return m_Relations.find(sRelation) != m_Relations.end();
}

void Arxx::Structure::vReadFromStream(Arxx::u4byte u4StructureDataLength, std::istream & IStream)
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

void Arxx::Structure::vWriteToBuffer(Arxx::Buffer & Buffer) const
{
	Arxx::BufferWriter BufferWriter(Buffer);
	std::map< std::string, Arxx::Structure::Relation >::const_iterator iRelation(m_Relations.begin());
	
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

Arxx::Structure::iterator::iterator(std::map< std::string, Arxx::Structure::Relation >::iterator iRelation) :
	m_iRelation(iRelation)
{
}

Arxx::Structure::iterator::~iterator(void)
{
}

Arxx::Structure::iterator & Arxx::Structure::iterator::operator++(void)
{
	++m_iRelation;
	
	return *this;
}

Arxx::Structure::Relation & Arxx::Structure::iterator::operator*(void)
{
	return m_iRelation->second;
}

const Arxx::Structure::Relation & Arxx::Structure::iterator::operator*(void) const
{
	return m_iRelation->second;
}

Arxx::Structure::Relation * Arxx::Structure::iterator::operator->(void)
{
	return &(m_iRelation->second);
}

bool Arxx::Structure::iterator::operator==(const Arxx::Structure::iterator & iRelation) const
{
	return m_iRelation == iRelation.m_iRelation;
}

bool Arxx::Structure::iterator::operator!=(const Arxx::Structure::iterator & iRelation) const
{
	return m_iRelation != iRelation.m_iRelation;
}

bool Arxx::Structure::iterator::operator==(const Arxx::Structure::const_iterator & iRelation) const
{
	return m_iRelation == iRelation.m_iRelation;
}

bool Arxx::Structure::iterator::operator!=(const Arxx::Structure::const_iterator & iRelation) const
{
	return m_iRelation != iRelation.m_iRelation;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Arxx::Structure::const_iterator                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////

Arxx::Structure::const_iterator::const_iterator(std::map< std::string, Arxx::Structure::Relation >::const_iterator iRelation) :
	m_iRelation(iRelation)
{
}

Arxx::Structure::const_iterator::const_iterator(Arxx::Structure::iterator iRelation) :
	m_iRelation(iRelation.m_iRelation)
{
}

Arxx::Structure::const_iterator::~const_iterator(void)
{
}

Arxx::Structure::const_iterator & Arxx::Structure::const_iterator::operator++(void)
{
	++m_iRelation;
	
	return *this;
}

const Arxx::Structure::Relation & Arxx::Structure::const_iterator::operator*(void) const
{
	return m_iRelation->second;
}

const Arxx::Structure::Relation * Arxx::Structure::const_iterator::operator->(void)
{
	return &(m_iRelation->second);
}

bool Arxx::Structure::const_iterator::operator==(const Arxx::Structure::const_iterator & iRelation) const
{
	return m_iRelation == iRelation.m_iRelation;
}

bool Arxx::Structure::const_iterator::operator!=(const Arxx::Structure::const_iterator & iRelation) const
{
	return m_iRelation != iRelation.m_iRelation;
}

Arxx::Buffer & Arxx::operator<<(Arxx::Buffer & Buffer, const Arxx::Structure & Structure)
{
	Structure.vWriteToBuffer(Buffer);
	
	return Buffer;
}
