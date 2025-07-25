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

#include <arxx/item.h>
#include <arxx/archive.h>


///////////////////////////////////////////////////////////////////////////////////////////////////
// Arxx::Item                                                                                    //
///////////////////////////////////////////////////////////////////////////////////////////////////
auto Arxx::Item::Create() -> Arxx::Item *
{
	return Arxx::Item::Create(nullptr, g_InvalidItemIdentifier);
}

auto Arxx::Item::Create(Arxx::u4byte Identifier) -> Arxx::Item *
{
	return Arxx::Item::Create(nullptr, Identifier);
}

auto Arxx::Item::Create(Arxx::Archive & Archive) -> Arxx::Item *
{
	return Arxx::Item::Create(&Archive);
}

auto Arxx::Item::Create(Arxx::Archive & Archive, Arxx::u4byte Identifier) -> Arxx::Item *
{
	return Arxx::Item::Create(&Archive, Identifier);
}

auto Arxx::Item::Create(Arxx::Archive * Archive) -> Arxx::Item *
{
	return Arxx::Item::Create(Archive, g_InvalidItemIdentifier);
}

auto Arxx::Item::Create(Arxx::Archive * Archive, Arxx::u4byte Identifier) -> Arxx::Item *
{
	auto Result = new Arxx::Item();
	
	Result->SetIdentifier(Identifier);
	if(Archive != nullptr)
	{
		Archive->Register(Result);
	}
	
	return Result;
}

auto Arxx::Item::Delete(Arxx::Item * Item) -> void
{
	if(Item->m_Archive != nullptr)
	{
		Item->m_Archive->Unregister(Item);
		Item->m_Archive = nullptr;
	}
	delete Item;
}

Arxx::Item::Item() :
	m_Structure{*this}
{
}

Arxx::Item::~Item()
{
	assert(m_Archive == nullptr);
}

auto Arxx::Item::GetIdentifier() const -> Arxx::u4byte
{
	return m_Identifier;
}

auto Arxx::Item::SetIdentifier(Arxx::u4byte Identifier) -> void
{
	if(m_Archive == nullptr)
	{
		m_Identifier = Identifier;
	}
	else
	{
		// save the archive as it will be set to null in UnregisterItem.
		auto Archive = m_Archive;
		
		try
		{
			Archive->Unregister(this);
		}
		catch(std::invalid_argument & Exception)
		{
		}
		
		auto OldIdentifier = m_Identifier;
		
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

auto Arxx::Item::GetName() const -> std::string const &
{
	return m_Name;
}

auto Arxx::Item::SetName(std::string const & Name) -> void
{
	m_Name = Name;
}

auto Arxx::Item::GetType() const -> Arxx::u4byte
{
	return m_Type;
}

auto Arxx::Item::SetType(Arxx::u4byte Type) -> void
{
	m_Type = Type;
}

auto Arxx::Item::GetSubType() const -> Arxx::u4byte
{
	return m_SubType;
}

auto Arxx::Item::SetSubType(Arxx::u4byte SubType) -> void
{
	m_SubType = SubType;
}

auto Arxx::Item::GetMajorVersionNumber() const -> Arxx::u1byte
{
	return m_MajorVersionNumber;
}

auto Arxx::Item::GetMinorVersionNumber() const -> Arxx::u1byte
{
	return m_MinorVersionNumber;
}

auto Arxx::Item::GetRevisionNumber() const -> Arxx::u1byte
{
	return m_RevisionNumber;
}

auto Arxx::Item::GetCandidateNumber() const -> Arxx::u1byte
{
	return m_CandidateNumber;
}

auto Arxx::Item::SetVersionNumbers(Arxx::u1byte MajorVersionNumber, Arxx::u1byte MinorVersionNumber, Arxx::u1byte RevisionNumber, Arxx::u1byte CandidateNumber) -> void
{
	m_MajorVersionNumber = MajorVersionNumber;
	m_MinorVersionNumber = MinorVersionNumber;
	m_RevisionNumber = RevisionNumber;
	m_CandidateNumber = CandidateNumber;
}

auto Arxx::Item::GetArchive() const -> Arxx::Archive *
{
	return m_Archive;
}

auto Arxx::Item::GetStructure() -> Arxx::Structure &
{
	return m_Structure;
}

auto Arxx::Item::GetStructure() const -> Arxx::Structure const &
{
	return m_Structure;
}

auto Arxx::Item::m_Fetch(Arxx::u4byte Offset, Arxx::u4byte Length) -> bool
{
	if(m_Archive != nullptr)
	{
		return m_Archive->Fetch(Offset, Length, this);
	}
	else
	{
		return Length == 0;
	}
}
