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

#include <arxx/Item.h>
#include <arxx/Archive.h>


///////////////////////////////////////////////////////////////////////////////////////////////////
// Arxx::Item                                                                                    //
///////////////////////////////////////////////////////////////////////////////////////////////////
Arxx::Item * Arxx::Item::Create()
{
	return Arxx::Item::Create(0, g_u4InvalidID);
}

Arxx::Item * Arxx::Item::Create(Arxx::u4byte Identifier)
{
	return Arxx::Item::Create(0, Identifier);
}

Arxx::Item * Arxx::Item::Create(Arxx::Archive & Archive)
{
	return Arxx::Item::Create(&Archive);
}

Arxx::Item * Arxx::Item::Create(Arxx::Archive & Archive, Arxx::u4byte Identifier)
{
	return Arxx::Item::Create(&Archive, Identifier);
}

Arxx::Item * Arxx::Item::Create(Arxx::Archive * Archive)
{
	return Arxx::Item::Create(Archive, g_u4InvalidID);
}

Arxx::Item * Arxx::Item::Create(Arxx::Archive * Archive, Arxx::u4byte Identifier)
{
	Arxx::Item * Result(new Arxx::Item());
	
	Result->SetIdentifier(Identifier);
	if(Archive != 0)
	{
		Archive->Register(Result);
	}
	
	return Result;
}

void Arxx::Item::Delete(Arxx::Item * Item)
{
	if(Item->m_Archive != 0)
	{
		Item->m_Archive->Unregister(Item);
		Item->m_Archive = 0;
	}
	delete Item;
}

Arxx::Item::Item() :
	m_Identifier(0xFFFFFFFF),
	m_Type(0xFFFFFFFF),
	m_SubType(0xFFFFFFFF),
	m_MajorVersionNumber(0),
	m_MinorVersionNumber(0),
	m_RevisionNumber(0),
	m_CandidateNumber(0),
	m_Archive(0),
	m_Structure(*this)
{
}

Arxx::Item::~Item()
{
	assert(m_Archive == 0);
}

Arxx::u4byte Arxx::Item::GetIdentifier() const
{
	return m_Identifier;
}

void Arxx::Item::SetIdentifier(Arxx::u4byte Identifier)
{
	if(m_Archive == 0)
	{
		m_Identifier = Identifier;
	}
	else
	{
		// save the library as it will be set to null in vUnregisterItem.
		Arxx::Archive * Archive(m_Archive);
		
		try
		{
			Archive->Unregister(this);
		}
		catch(std::invalid_argument & Exception)
		{
		}
		
		Arxx::u4byte OldIdentifier = m_Identifier;
		
		m_Identifier = Identifier;
		try
		{
			Archive->Register(this);
		}
		catch(Arxx::id_not_unique & Exception)
		{
			m_Identifier = OldIdentifier;
			m_Archive->Register(this);
			throw;
		}
	}
}

const std::string & Arxx::Item::GetName() const
{
	return m_Name;
}

void Arxx::Item::SetName(const std::string & Name)
{
	m_Name = Name;
}

Arxx::u4byte Arxx::Item::GetType() const
{
	return m_Type;
}

void Arxx::Item::SetType(Arxx::u4byte Type)
{
	m_Type = Type;
}

Arxx::u4byte Arxx::Item::GetSubType() const
{
	return m_SubType;
}

void Arxx::Item::SetSubType(Arxx::u4byte SubType)
{
	m_SubType = SubType;
}

Arxx::u1byte Arxx::Item::GetMajorVersionNumber() const
{
	return m_MajorVersionNumber;
}

Arxx::u1byte Arxx::Item::GetMinorVersionNumber() const
{
	return m_MinorVersionNumber;
}

Arxx::u1byte Arxx::Item::GetRevisionNumber() const
{
	return m_RevisionNumber;
}

Arxx::u1byte Arxx::Item::GetCandidateNumber() const
{
	return m_CandidateNumber;
}

void Arxx::Item::SetVersionNumbers(Arxx::u1byte MajorVersionNumber, Arxx::u1byte MinorVersionNumber, Arxx::u1byte RevisionNumber, Arxx::u1byte CandidateNumber)
{
	m_MajorVersionNumber = MajorVersionNumber;
	m_MinorVersionNumber = MinorVersionNumber;
	m_RevisionNumber = RevisionNumber;
	m_CandidateNumber = CandidateNumber;
}

Arxx::Archive * Arxx::Item::GetArchive() const
{
	return m_Archive;
}

Arxx::Structure & Arxx::Item::GetStructure()
{
	return m_Structure;
}

const Arxx::Structure & Arxx::Item::GetStructure() const
{
	return m_Structure;
}

bool Arxx::Item::m_Fetch(Arxx::u4byte Offset, Arxx::u4byte Length)
{
	if(m_Archive != 0)
	{
		return m_Archive->Fetch(Offset, Length, this);
	}
	else
	{
		return Length == 0;
	}
}
