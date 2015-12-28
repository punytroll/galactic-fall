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

#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <sstream>

#include "ArchiveFile.h"
#include "ReferenceImplementation.h"
#include "../include/Archive.h"
#include "../include/BufferWriter.h"
#include "../include/Item.h"
#include "../include/Reference.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
// Arxx::Archive                                                                                  //
///////////////////////////////////////////////////////////////////////////////////////////////////

Arxx::Archive::Archive(void) :
	_RootItem(0),
	_IStream(0)
{
	srand(time(0));
}

Arxx::Archive::~Archive(void)
{
	Close();
}

bool Arxx::Archive::Load(const std::string & FilePath)
{
	_IStream = new std::ifstream(FilePath.c_str());
	if(_IStream->fail())
	{
		return false;
	}
	// this position I chose after only a few looks at the file => prove me wrong ;)
	Close();
	
	Arxx::ArchiveHeader ArchiveHeader;
	
	(*_IStream) >> ArchiveHeader;
	if((ArchiveHeader.MajorVersionNumber == 2) && (ArchiveHeader.MinorVersionNumber == 1) && (ArchiveHeader.RevisionNumber == 0) && (ArchiveHeader.CandidateNumber == 0))
	{
		Read_2_1_0_0(ArchiveHeader.NumberOfItems);
		_RootItem = GetItem(ArchiveHeader.RootItemIdentifier);
	}
	else
	{
		std::cerr << "Unknown ARX archive format: version = " << static_cast< Arxx::u4byte >(ArchiveHeader.MajorVersionNumber) << '.' << static_cast< Arxx::u4byte >(ArchiveHeader.MinorVersionNumber) << '.' << static_cast< Arxx::u4byte >(ArchiveHeader.RevisionNumber) << '.' << static_cast< Arxx::u4byte >(ArchiveHeader.CandidateNumber) << '.' << std::endl;
		throw bad_file_format(FilePath);
	}
	
	return true;
}

void Arxx::Archive::Close(void)
{
	while(_Items.begin() != _Items.end())
	{
		Arxx::Item * pItem(_Items.begin()->second);
		
		Unregister(pItem);
		Arxx::Item::Delete(pItem);
	}
	_RootItem = 0;
	
	std::map< Arxx::u4byte, Arxx::Reference >::iterator iReference(_References.begin());
	
	while(iReference != _References.end())
	{
		iReference->second.vUnresolve();
		_References.erase(iReference);
		iReference = _References.begin();
	}
}

void Arxx::Archive::Register(Arxx::Item * Item)
{
	if(Item == 0)
	{
		throw std::invalid_argument("Item");
	}
	if(Item->_Archive == 0)
	{
		Item->_Archive = this;
	}
	if(Item->_Archive != this)
	{
		throw std::invalid_argument("Item");
	}
	if(Item->GetIdentifier() == g_u4InvalidID)
	{
		std::map< Arxx::u4byte, Arxx::Item * >::iterator iIterator;
		Arxx::u4byte NewIdentifier;
		
		do
		{
			NewIdentifier = rand();
			iIterator = _Items.find(NewIdentifier);
		} while((iIterator != _Items.end()) || (NewIdentifier == g_u4InvalidID));
		Item->SetIdentifier(NewIdentifier);
	}
	else
	{
		std::map< Arxx::u4byte, Arxx::Item * >::iterator iIterator = _Items.find(Item->GetIdentifier());
		
		if(iIterator != _Items.end())
		{
			std::stringstream ssID;
			
			ssID << Item->GetIdentifier();
			
			throw id_not_unique(ssID.str());
		}
		_Items[Item->GetIdentifier()] = Item;
		
		std::map< Arxx::u4byte, Arxx::Reference >::iterator iReference(_References.find(Item->GetIdentifier()));
		
		if(iReference == _References.end())
		{
			_References.insert(std::make_pair(Item->GetIdentifier(), Arxx::Reference(*Item)));
		}
		else
		{
			iReference->second.vResolve(*Item);
		}
		
		/** @todo the following is the brute force implementation. It can be done much faster with a merge sort **/
		Arxx::Structure::iterator iRelation(Item->GetStructure().begin());
		Arxx::Structure::iterator iEndRelation(Item->GetStructure().end());
		
		while(iRelation != iEndRelation)
		{
			Arxx::Structure::Relation::iterator iReference(iRelation->begin());
			Arxx::Structure::Relation::iterator iEndReference(iRelation->end());
			
			while(iReference != iEndReference)
			{
				// use GetReference() to create or retrieve the Reference in the archive's Reference map and then attach the Item's Reference to it
				Arxx::Reference Reference(GetReference(iReference->u4GetUniqueID()));
				
				Reference.bAttach(*iReference);
				++iReference;
			}
			++iRelation;
		}
	}
}

void Arxx::Archive::Unregister(Item * Item)
{
	if(Item->_Archive != this)
	{
		throw std::invalid_argument("Item");
	}
	
	std::map< Arxx::u4byte, Arxx::Item * >::iterator iIterator(_Items.find(Item->GetIdentifier()));
	std::map< Arxx::u4byte, Arxx::Reference >::iterator iReference(_References.find(Item->GetIdentifier()));
	
	if((iIterator == _Items.end()) || (iReference == _References.end()))
	{
		throw std::invalid_argument("Item");
	}
	
	// before removing the Item from the Archive we correct its references
	Arxx::Structure::iterator iRelation(Item->GetStructure().begin());
	Arxx::Structure::iterator iEndRelation(Item->GetStructure().end());
	
	while(iRelation != iEndRelation)
	{
		Arxx::Structure::Relation::iterator iReference(iRelation->begin());
		Arxx::Structure::Relation::iterator iEndReference(iRelation->end());
		
		while(iReference != iEndReference)
		{
			iReference->bDetach();
			++iReference;
		}
		++iRelation;
	}
	// now we can remove the Item
	_Items.erase(iIterator);
	Item->_Archive = 0;
	iReference->second.vUnresolve();
	iReference->second.vDecoupleFromArchive();
	_References.erase(iReference);
}

void Arxx::Archive::SetRootItem(Item * Item)
{
	if((Item != 0) && (Item->_Archive != this))
	{
		throw std::invalid_argument("pItem");
	}
	_RootItem = Item;
}

const Arxx::Item * Arxx::Archive::GetItem(Arxx::u4byte ItemIdentifier) const
{
	if(ItemIdentifier == g_u4InvalidID)
	{
		return 0;
	}
	
	std::map< Arxx::u4byte, Arxx::Item * >::const_iterator iIterator(_Items.find(ItemIdentifier));
	
	if(iIterator == _Items.end())
	{
		return 0;
	}
	
	return (*iIterator).second;
}

Arxx::Item * Arxx::Archive::GetItem(Arxx::u4byte ItemIdentifier)
{
	if(ItemIdentifier == g_u4InvalidID)
	{
		return 0;
	}
	
	std::map< Arxx::u4byte, Arxx::Item * >::iterator iIterator(_Items.find(ItemIdentifier));
	
	if(iIterator == _Items.end())
	{
		return 0;
	}
	
	return (*iIterator).second;
}

Arxx::Item * Arxx::Archive::GetItem(std::string Path)
{
	if(Path[0] != '/')
	{
		return 0;
	}
	
	std::string ChildRelation;
	Arxx::Item * Item(GetRootItem());
	
	while((Item != 0) && ((Path != "/") || (Path == "")))
	{
		std::string::size_type PathSeparatorIndex(Path.find('/', 1));
		std::string ChildName(Path.substr(1, PathSeparatorIndex - 1));
		
		if(PathSeparatorIndex == std::string::npos)
		{
			Path = "/";
		}
		else
		{
			Path = Path.substr(PathSeparatorIndex);
		}
		
		std::string::size_type RelationSeparatorIndex(ChildName.find("::"));
		
		if(RelationSeparatorIndex != std::string::npos)
		{
			ChildRelation = ChildName.substr(0, RelationSeparatorIndex);
			ChildName = ChildName.substr(RelationSeparatorIndex + 2);
		}
		else
		{
			ChildRelation = "child";
		}
		if(Item->GetStructure().bHasRelation(ChildRelation) == false)
		{
			return 0;
		}
		else
		{
			Arxx::Structure::Relation & Relation(Item->GetStructure().GetRelation(ChildRelation));
			Arxx::Structure::Relation::iterator ReferenceIterator(Relation.begin());
			
			for(; ReferenceIterator != Relation.end(); ++ReferenceIterator)
			{
				if((ReferenceIterator->pGetItem() != 0) && (ReferenceIterator->pGetItem()->GetName() == ChildName))
				{
					Item = ReferenceIterator->pGetItem();
					
					break;
				}
			}
			if(ReferenceIterator == Relation.end())
			{
				return 0;
			}
		}
	}
	
	return Item;
}

const Arxx::Item * Arxx::Archive::GetItem(std::string Path) const
{
	if(Path[0] != '/')
	{
		return 0;
	}
	
	std::string ChildRelation;
	const Arxx::Item * Item(GetRootItem());
	
	while((Item != 0) && ((Path != "/") || (Path == "")))
	{
		std::string::size_type PathSeparatorIndex(Path.find('/', 1));
		std::string ChildName(Path.substr(1, PathSeparatorIndex - 1));
		
		if(PathSeparatorIndex == std::string::npos)
		{
			Path = "/";
		}
		else
		{
			Path = Path.substr(PathSeparatorIndex);
		}
		
		std::string::size_type RelationSeparatorIndex(ChildName.find("::"));
		
		if(RelationSeparatorIndex != std::string::npos)
		{
			ChildRelation = ChildName.substr(0, RelationSeparatorIndex);
			ChildName = ChildName.substr(RelationSeparatorIndex + 2);
		}
		else
		{
			ChildRelation = "child";
		}
		if(Item->GetStructure().bHasRelation(ChildRelation) == false)
		{
			return 0;
		}
		else
		{
			const Arxx::Structure::Relation & Relation(Item->GetStructure().GetRelation(ChildRelation));
			Arxx::Structure::Relation::const_iterator ReferenceIterator(Relation.begin());
			
			for(; ReferenceIterator != Relation.end(); ++ReferenceIterator)
			{
				if((ReferenceIterator->pGetItem() != 0) && (ReferenceIterator->pGetItem()->GetName() == ChildName))
				{
					Item = ReferenceIterator->pGetItem();
					
					break;
				}
			}
			if(ReferenceIterator == Relation.end())
			{
				return 0;
			}
		}
	}
	
	return Item;
}

Arxx::Item * Arxx::Archive::GetRootItem(void)
{
	return _RootItem;
}

const Arxx::Item * Arxx::Archive::GetRootItem(void) const
{
	return _RootItem;
}

Arxx::Reference Arxx::Archive::GetReference(Arxx::u4byte ItemIdentifier)
{
	std::map< Arxx::u4byte, Arxx::Reference >::iterator iReference(_References.find(ItemIdentifier));
	
	if(iReference == _References.end())
	{
		return _References.insert(std::make_pair(ItemIdentifier, Arxx::Reference(ItemIdentifier, this))).first->second;
	}
	else
	{
		return iReference->second;
	}
}

void Arxx::Archive::ReleaseReference(Arxx::ReferenceImplementation * pReference)
{
	std::map< Arxx::u4byte, Arxx::Reference >::iterator iReference(_References.find(pReference->u4GetUniqueID()));
	
	if(iReference == _References.end())
	{
		throw std::runtime_error("Arxx::Archive::vReleaseReference: Reference not found in the reference map.");
	}
	_References.erase(iReference);
}

void Arxx::Archive::Read_2_1_0_0(Arxx::u4byte u4ItemCount)
{
	Arxx::ItemHeader ItemHeader;
	Arxx::u4byte u4I(0);
	
	while(++u4I <= u4ItemCount)
	{
		(*_IStream) >> ItemHeader;
		
		Item * pItem(Arxx::Item::Create(this, ItemHeader.Identifier));
		
		pItem->SetType(ItemHeader.Type);
		pItem->SetSubType(ItemHeader.SubType);
		pItem->SetVersionNumbers(ItemHeader.MajorVersionNumber, ItemHeader.MinorVersionNumber, ItemHeader.RevisionNumber, ItemHeader.CandidateNumber);
		
		char * pcName = new char[ItemHeader.NameLength + 1];
		
		_IStream->read(pcName, ItemHeader.NameLength + 1);
		pcName[ItemHeader.NameLength] = '\0';
		pItem->SetName(pcName);
		delete[] pcName;
		pItem->GetStructure().vReadFromStream(ItemHeader.StructureLength, *_IStream);
		pItem->SetFetchInformation(_IStream->tellg(), static_cast< Arxx::Data::Compression >(ItemHeader.DataCompressionType), ItemHeader.DataDecompressedLength, ItemHeader.DataCompressedLength);
		_IStream->seekg((ItemHeader.DataCompressionType == 0) ? (ItemHeader.DataDecompressedLength) : (ItemHeader.DataCompressedLength), std::ios_base::cur);
	}
}

bool Arxx::Archive::Fetch(Arxx::u4byte Offset, Arxx::u4byte Length, Arxx::Buffer * Buffer)
{
	_IStream->seekg(Offset, std::ios_base::beg);
	Buffer->vSetLength(0);
	
	Arxx::BufferWriter BufferWriter(*Buffer);
	
	BufferWriter << std::make_pair(static_cast< Arxx::Buffer::size_type >(Length), reinterpret_cast< std::istream * >(_IStream));
	
	return true;
}

void Arxx::Archive::Save(const std::string & FilePath, bool bAutoCompress)
{
	std::stringstream ssTemporaryFilePath;
	
	ssTemporaryFilePath << "/tmp/ARX" << time(0);
	
	std::ofstream OStream(ssTemporaryFilePath.str().c_str());
	
	if(!OStream)
	{
		throw file_error(ssTemporaryFilePath.str());
	}
	
	Arxx::ArchiveHeader ArchiveHeader;
	
	ArchiveHeader.MajorVersionNumber = 2;
	ArchiveHeader.MinorVersionNumber = 1;
	ArchiveHeader.RevisionNumber = 0;
	ArchiveHeader.CandidateNumber = 0;
	ArchiveHeader.RootItemIdentifier = ((_RootItem != 0) ? (_RootItem->GetIdentifier()) : (g_u4InvalidID));
	ArchiveHeader.NumberOfItems = _Items.size();
	OStream << ArchiveHeader;
	
	std::map< Arxx::u4byte, Arxx::Item * >::iterator iIterator(_Items.begin());
	
	while(iIterator != _Items.end())
	{
		Item & Item(*((*iIterator).second));
		
		if(Item.IsFetched() == false)
		{
			Item.Fetch();
		}
		if((Item.IsFetched() == true) && (Item.IsCompressed() == false) && (Item.GetDecompressedLength() > 32) && (bAutoCompress == true))
		{
			Item.Compress();
		}
		
		Arxx::ItemHeader ItemHeader;
		
		ItemHeader.Identifier = Item.GetIdentifier();
		ItemHeader.Type = Item.GetType();
		ItemHeader.SubType = Item.GetSubType();
		ItemHeader.MajorVersionNumber = Item.GetMajorVersionNumber();
		ItemHeader.MinorVersionNumber = Item.GetMinorVersionNumber();
		ItemHeader.RevisionNumber = Item.GetRevisionNumber();
		ItemHeader.CandidateNumber = Item.GetCandidateNumber();
		ItemHeader.DataCompressionType = Item.GetCompression();
		ItemHeader.NameLength = Item.GetName().length();
		ItemHeader.DataDecompressedLength = Item.GetDecompressedLength();
		ItemHeader.DataCompressedLength = Item.GetCompressedLength();
		
		Arxx::Data StructureBuffer;
		
		StructureBuffer << Item.GetStructure();
		ItemHeader.StructureLength = StructureBuffer.stGetLength();
		OStream << ItemHeader << Item.GetName() << '\0' << StructureBuffer << Item;
		++iIterator;
	}
	OStream.close();
	
	int iChildPID = 0;
	int iReturn = 0;
	
	if((iChildPID = fork()) == 0)
	{
		execlp("mv", "mv", "--force", ssTemporaryFilePath.str().c_str(), FilePath.c_str(), reinterpret_cast< char * >(0));
	}
	waitpid(iChildPID, &iReturn, 0);
}

Arxx::Archive::iterator Arxx::Archive::begin(void)
{
	// will be fed into an implicite constructor for Arxx::Archive::iterator
	return _Items.begin();
}

Arxx::Archive::iterator Arxx::Archive::end(void)
{
	// will be fed into an implicite constructor for Arxx::Archive::iterator
	return _Items.end();
}

Arxx::Archive::const_iterator Arxx::Archive::begin(void) const
{
	// will be fed into an implicite constructor for Arxx::Archive::const_iterator
	return _Items.begin();
}

Arxx::Archive::const_iterator Arxx::Archive::end(void) const
{
	// will be fed into an implicite constructor for Arxx::Archive::const_iterator
	return _Items.end();
}

Arxx::Archive::size_type Arxx::Archive::size(void) const
{
	return _Items.size();
}

Arxx::u4byte Arxx::Archive::GetNumberOfReferences(void) const
{
	return _References.size();
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Arxx::Archive::iterator                                                                        //
///////////////////////////////////////////////////////////////////////////////////////////////////

Arxx::Archive::iterator::iterator(std::map< Arxx::u4byte, Arxx::Item * >::iterator iItem) :
	m_iItem(iItem)
{
}

Arxx::Archive::iterator::~iterator(void)
{
}

Arxx::Archive::iterator & Arxx::Archive::iterator::operator++(void)
{
	++m_iItem;

	return *this;
}

Arxx::Item * Arxx::Archive::iterator::operator*(void)
{
	return m_iItem->second;
}

const Arxx::Item * Arxx::Archive::iterator::operator*(void) const
{
	return m_iItem->second;
}

Arxx::Item * Arxx::Archive::iterator::operator->(void)
{
	return m_iItem->second;
}

bool Arxx::Archive::iterator::operator==(const Arxx::Archive::iterator & iItem) const
{
	return m_iItem == iItem.m_iItem;
}

bool Arxx::Archive::iterator::operator!=(const Arxx::Archive::iterator & iItem) const
{
	return m_iItem != iItem.m_iItem;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Arxx::Archive::const_iterator                                                                  //
///////////////////////////////////////////////////////////////////////////////////////////////////

Arxx::Archive::const_iterator::const_iterator(std::map< Arxx::u4byte, Arxx::Item * >::const_iterator iItem) :
	m_iItem(iItem)
{
}

Arxx::Archive::const_iterator::~const_iterator(void)
{
}

Arxx::Archive::const_iterator & Arxx::Archive::const_iterator::operator++(void)
{
	++m_iItem;

	return *this;
}

const Arxx::Item * Arxx::Archive::const_iterator::operator*(void) const
{
	return m_iItem->second;
}

const Arxx::Item * Arxx::Archive::const_iterator::operator->(void) const
{
	return m_iItem->second;
}

bool Arxx::Archive::const_iterator::operator==(const Arxx::Archive::const_iterator & iItem) const
{
	return m_iItem == iItem.m_iItem;
}

bool Arxx::Archive::const_iterator::operator!=(const Arxx::Archive::const_iterator & iItem) const
{
	return m_iItem != iItem.m_iItem;
}
