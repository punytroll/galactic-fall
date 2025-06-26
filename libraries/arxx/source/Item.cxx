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
Arxx::Item * Arxx::Item::Create(void)
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
	if(Item->_Archive != 0)
	{
		Item->_Archive->Unregister(Item);
		Item->_Archive = 0;
	}
	delete Item;
}

Arxx::Item::Item(void) :
	_Identifier(0xFFFFFFFF),
	_Type(0xFFFFFFFF),
	_SubType(0xFFFFFFFF),
	_MajorVersionNumber(0),
	_MinorVersionNumber(0),
	_RevisionNumber(0),
	_CandidateNumber(0),
	_Archive(0),
	_Structure(*this)
{
}

Arxx::Item::~Item(void)
{
	assert(_Archive == 0);
}

Arxx::u4byte Arxx::Item::GetIdentifier(void) const
{
	return _Identifier;
}

void Arxx::Item::SetIdentifier(Arxx::u4byte Identifier)
{
	if(_Archive == 0)
	{
		_Identifier = Identifier;
	}
	else
	{
		// save the library as it will be set to null in vUnregisterItem.
		Arxx::Archive * Archive(_Archive);
		
		try
		{
			Archive->Unregister(this);
		}
		catch(std::invalid_argument & Exception)
		{
		}
		
		Arxx::u4byte OldIdentifier = _Identifier;
		
		_Identifier = Identifier;
		try
		{
			Archive->Register(this);
		}
		catch(Arxx::id_not_unique & Exception)
		{
			_Identifier = OldIdentifier;
			_Archive->Register(this);
			throw;
		}
	}
}

const std::string & Arxx::Item::GetName(void) const
{
	return _Name;
}

void Arxx::Item::SetName(const std::string & Name)
{
	_Name = Name;
}

Arxx::u4byte Arxx::Item::GetType(void) const
{
	return _Type;
}

void Arxx::Item::SetType(Arxx::u4byte Type)
{
	_Type = Type;
}

Arxx::u4byte Arxx::Item::GetSubType(void) const
{
	return _SubType;
}

void Arxx::Item::SetSubType(Arxx::u4byte SubType)
{
	_SubType = SubType;
}

Arxx::u1byte Arxx::Item::GetMajorVersionNumber(void) const
{
	return _MajorVersionNumber;
}

Arxx::u1byte Arxx::Item::GetMinorVersionNumber(void) const
{
	return _MinorVersionNumber;
}

Arxx::u1byte Arxx::Item::GetRevisionNumber(void) const
{
	return _RevisionNumber;
}

Arxx::u1byte Arxx::Item::GetCandidateNumber(void) const
{
	return _CandidateNumber;
}

void Arxx::Item::SetVersionNumbers(Arxx::u1byte MajorVersionNumber, Arxx::u1byte MinorVersionNumber, Arxx::u1byte RevisionNumber, Arxx::u1byte CandidateNumber)
{
	_MajorVersionNumber = MajorVersionNumber;
	_MinorVersionNumber = MinorVersionNumber;
	_RevisionNumber = RevisionNumber;
	_CandidateNumber = CandidateNumber;
}

Arxx::Archive * Arxx::Item::GetArchive(void) const
{
	return _Archive;
}

Arxx::Structure & Arxx::Item::GetStructure(void)
{
	return _Structure;
}

const Arxx::Structure & Arxx::Item::GetStructure(void) const
{
	return _Structure;
}

bool Arxx::Item::_Fetch(Arxx::u4byte Offset, Arxx::u4byte Length)
{
	if(_Archive != 0)
	{
		return _Archive->Fetch(Offset, Length, this);
	}
	else
	{
		return Length == 0;
	}
}
