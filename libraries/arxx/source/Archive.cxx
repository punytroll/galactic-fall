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

#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <sstream>

#include <arxx/Archive.h>
#include <arxx/BufferWriter.h>
#include <arxx/Item.h>
#include <arxx/Reference.h>

#include "ArchiveFile.h"
#include "ReferenceImplementation.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
// Arxx::Archive                                                                                  //
///////////////////////////////////////////////////////////////////////////////////////////////////

Arxx::Archive::Archive() :
	m_RootItem(0),
	m_IStream(0)
{
	srand(time(0));
}

Arxx::Archive::~Archive()
{
	Close();
}

auto Arxx::Archive::Load(std::string const & FilePath) -> bool
{
	m_IStream = new std::ifstream(FilePath.c_str());
	if(m_IStream->fail())
	{
		return false;
	}
	// this position I chose after only a few looks at the file => prove me wrong ;)
	Close();
	
	Arxx::ArchiveHeader ArchiveHeader;
	
	(*m_IStream) >> ArchiveHeader;
	if((ArchiveHeader.MajorVersionNumber == 2) && (ArchiveHeader.MinorVersionNumber == 1) && (ArchiveHeader.RevisionNumber == 0) && (ArchiveHeader.CandidateNumber == 0))
	{
		m_Read_2_1_0_0(ArchiveHeader.NumberOfItems);
		m_RootItem = GetItem(ArchiveHeader.RootItemIdentifier);
	}
	else
	{
		std::cerr << "Unknown ARX archive format: version = " << static_cast< Arxx::u4byte >(ArchiveHeader.MajorVersionNumber) << '.' << static_cast< Arxx::u4byte >(ArchiveHeader.MinorVersionNumber) << '.' << static_cast< Arxx::u4byte >(ArchiveHeader.RevisionNumber) << '.' << static_cast< Arxx::u4byte >(ArchiveHeader.CandidateNumber) << '.' << std::endl;
		throw bad_file_format(FilePath);
	}
	
	return true;
}

auto Arxx::Archive::Close() -> void
{
	while(m_Items.begin() != m_Items.end())
	{
		Arxx::Item * pItem(m_Items.begin()->second);
		
		Unregister(pItem);
		Arxx::Item::Delete(pItem);
	}
	m_RootItem = 0;
	
	std::map< Arxx::u4byte, Arxx::Reference >::iterator iReference(m_References.begin());
	
	while(iReference != m_References.end())
	{
		iReference->second.vUnresolve();
		m_References.erase(iReference);
		iReference = m_References.begin();
	}
}

auto Arxx::Archive::Register(Arxx::Item * Item) -> void
{
	if(Item == 0)
	{
		throw std::invalid_argument("Item");
	}
	if(Item->m_Archive == 0)
	{
		Item->m_Archive = this;
	}
	if(Item->m_Archive != this)
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
			iIterator = m_Items.find(NewIdentifier);
		} while((iIterator != m_Items.end()) || (NewIdentifier == g_u4InvalidID));
		Item->SetIdentifier(NewIdentifier);
	}
	else
	{
		std::map< Arxx::u4byte, Arxx::Item * >::iterator iIterator = m_Items.find(Item->GetIdentifier());
		
		if(iIterator != m_Items.end())
		{
			std::stringstream ssID;
			
			ssID << Item->GetIdentifier();
			
			throw id_not_unique(ssID.str());
		}
		m_Items[Item->GetIdentifier()] = Item;
		
		std::map< Arxx::u4byte, Arxx::Reference >::iterator iReference(m_References.find(Item->GetIdentifier()));
		
		if(iReference == m_References.end())
		{
			m_References.insert(std::make_pair(Item->GetIdentifier(), Arxx::Reference(*Item)));
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

auto Arxx::Archive::Unregister(Arxx::Item * Item) -> void
{
	if(Item->m_Archive != this)
	{
		throw std::invalid_argument("Item");
	}
	
	std::map< Arxx::u4byte, Arxx::Item * >::iterator iIterator(m_Items.find(Item->GetIdentifier()));
	std::map< Arxx::u4byte, Arxx::Reference >::iterator iReference(m_References.find(Item->GetIdentifier()));
	
	if((iIterator == m_Items.end()) || (iReference == m_References.end()))
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
	m_Items.erase(iIterator);
	Item->m_Archive = 0;
	iReference->second.vUnresolve();
	iReference->second.vDecoupleFromArchive();
	m_References.erase(iReference);
}

auto Arxx::Archive::SetRootItem(Arxx::Item * Item) -> void
{
	if((Item != 0) && (Item->m_Archive != this))
	{
		throw std::invalid_argument("pItem");
	}
	m_RootItem = Item;
}

auto Arxx::Archive::GetItem(Arxx::u4byte ItemIdentifier) const -> Arxx::Item const *
{
	if(ItemIdentifier == g_u4InvalidID)
	{
		return 0;
	}
	
	std::map< Arxx::u4byte, Arxx::Item * >::const_iterator iIterator(m_Items.find(ItemIdentifier));
	
	if(iIterator == m_Items.end())
	{
		return 0;
	}
	
	return (*iIterator).second;
}

auto Arxx::Archive::GetItem(Arxx::u4byte ItemIdentifier) -> Arxx::Item *
{
	if(ItemIdentifier == g_u4InvalidID)
	{
		return 0;
	}
	
	std::map< Arxx::u4byte, Arxx::Item * >::iterator iIterator(m_Items.find(ItemIdentifier));
	
	if(iIterator == m_Items.end())
	{
		return 0;
	}
	
	return (*iIterator).second;
}

auto Arxx::Archive::GetItem(std::string Path) -> Arxx::Item *
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

auto Arxx::Archive::GetItem(std::string Path) const -> Arxx::Item const *
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

auto Arxx::Archive::GetRootItem() -> Arxx::Item *
{
	return m_RootItem;
}

auto Arxx::Archive::GetRootItem() const -> Arxx::Item const *
{
	return m_RootItem;
}

auto Arxx::Archive::GetReference(Arxx::u4byte ItemIdentifier) -> Arxx::Reference
{
	std::map< Arxx::u4byte, Arxx::Reference >::iterator iReference(m_References.find(ItemIdentifier));
	
	if(iReference == m_References.end())
	{
		return m_References.insert(std::make_pair(ItemIdentifier, Arxx::Reference(ItemIdentifier, this))).first->second;
	}
	else
	{
		return iReference->second;
	}
}

auto Arxx::Archive::ReleaseReference(Arxx::ReferenceImplementation * pReference) -> void
{
	std::map< Arxx::u4byte, Arxx::Reference >::iterator iReference(m_References.find(pReference->u4GetUniqueID()));
	
	if(iReference == m_References.end())
	{
		throw std::runtime_error("Arxx::Archive::vReleaseReference: Reference not found in the reference map.");
	}
	m_References.erase(iReference);
}

auto Arxx::Archive::m_Read_2_1_0_0(Arxx::u4byte u4ItemCount) -> void
{
	Arxx::ItemHeader ItemHeader;
	Arxx::u4byte u4I(0);
	
	while(++u4I <= u4ItemCount)
	{
		(*m_IStream) >> ItemHeader;
		
		Item * pItem(Arxx::Item::Create(this, ItemHeader.Identifier));
		
		pItem->SetType(ItemHeader.Type);
		pItem->SetSubType(ItemHeader.SubType);
		pItem->SetVersionNumbers(ItemHeader.MajorVersionNumber, ItemHeader.MinorVersionNumber, ItemHeader.RevisionNumber, ItemHeader.CandidateNumber);
		
		char * pcName = new char[ItemHeader.NameLength + 1];
		
		m_IStream->read(pcName, ItemHeader.NameLength + 1);
		pcName[ItemHeader.NameLength] = '\0';
		pItem->SetName(pcName);
		delete[] pcName;
		pItem->GetStructure().vReadFromStream(ItemHeader.StructureLength, *m_IStream);
		pItem->SetFetchInformation(m_IStream->tellg(), static_cast< Arxx::Data::Compression >(ItemHeader.DataCompressionType), ItemHeader.DataDecompressedLength, ItemHeader.DataCompressedLength);
		m_IStream->seekg((ItemHeader.DataCompressionType == 0) ? (ItemHeader.DataDecompressedLength) : (ItemHeader.DataCompressedLength), std::ios_base::cur);
	}
}

auto Arxx::Archive::Fetch(Arxx::u4byte Offset, Arxx::u4byte Length, Arxx::Buffer * Buffer) -> bool
{
	m_IStream->seekg(Offset, std::ios_base::beg);
	Buffer->vSetLength(0);
	
	Arxx::BufferWriter BufferWriter(*Buffer);
	
	BufferWriter << std::make_pair(static_cast< Arxx::Buffer::size_type >(Length), reinterpret_cast< std::istream * >(m_IStream));
	
	return true;
}

auto Arxx::Archive::Save(std::string const & FilePath, bool bAutoCompress) -> void
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
	ArchiveHeader.RootItemIdentifier = ((m_RootItem != 0) ? (m_RootItem->GetIdentifier()) : (g_u4InvalidID));
	ArchiveHeader.NumberOfItems = m_Items.size();
	OStream << ArchiveHeader;
	
	std::map< Arxx::u4byte, Arxx::Item * >::iterator iIterator(m_Items.begin());
	
	while(iIterator != m_Items.end())
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

auto Arxx::Archive::begin() -> Arxx::Archive::iterator
{
	// will be fed into an implicite constructor for Arxx::Archive::iterator
	return m_Items.begin();
}

auto Arxx::Archive::end() -> Arxx::Archive::iterator
{
	// will be fed into an implicite constructor for Arxx::Archive::iterator
	return m_Items.end();
}

auto Arxx::Archive::begin() const -> Arxx::Archive::const_iterator
{
	// will be fed into an implicite constructor for Arxx::Archive::const_iterator
	return m_Items.begin();
}

auto Arxx::Archive::end() const -> Arxx::Archive::const_iterator
{
	// will be fed into an implicite constructor for Arxx::Archive::const_iterator
	return m_Items.end();
}

auto Arxx::Archive::size() const -> Arxx::Archive::size_type
{
	return m_Items.size();
}

auto Arxx::Archive::GetNumberOfReferences() const -> Arxx::u4byte
{
	return m_References.size();
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Arxx::Archive::iterator                                                                        //
///////////////////////////////////////////////////////////////////////////////////////////////////

Arxx::Archive::iterator::iterator(std::map< Arxx::u4byte, Arxx::Item * >::iterator iItem) :
	m_iItem(iItem)
{
}

Arxx::Archive::iterator::~iterator()
{
}

auto Arxx::Archive::iterator::operator++() -> Arxx::Archive::iterator &
{
	++m_iItem;

	return *this;
}

auto Arxx::Archive::iterator::operator*() -> Arxx::Item *
{
	return m_iItem->second;
}

auto Arxx::Archive::iterator::operator*() const -> Arxx::Item const *
{
	return m_iItem->second;
}

auto Arxx::Archive::iterator::operator->() -> Arxx::Item *
{
	return m_iItem->second;
}

auto Arxx::Archive::iterator::operator==(Arxx::Archive::iterator const & Other) const -> bool
{
	return m_iItem == Other.m_iItem;
}

auto Arxx::Archive::iterator::operator!=(Arxx::Archive::iterator const & Other) const -> bool
{
	return m_iItem != Other.m_iItem;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Arxx::Archive::const_iterator                                                                  //
///////////////////////////////////////////////////////////////////////////////////////////////////

Arxx::Archive::const_iterator::const_iterator(std::map< Arxx::u4byte, Arxx::Item * >::const_iterator iItem) :
	m_iItem(iItem)
{
}

Arxx::Archive::const_iterator::~const_iterator()
{
}

auto Arxx::Archive::const_iterator::operator++() -> Arxx::Archive::const_iterator &
{
	++m_iItem;

	return *this;
}

auto Arxx::Archive::const_iterator::operator*() const -> Arxx::Item const *
{
	return m_iItem->second;
}

auto Arxx::Archive::const_iterator::operator->() const -> Arxx::Item const *
{
	return m_iItem->second;
}

auto Arxx::Archive::const_iterator::operator==(Arxx::Archive::const_iterator const & Other) const -> bool
{
	return m_iItem == Other.m_iItem;
}

auto Arxx::Archive::const_iterator::operator!=(Arxx::Archive::const_iterator const & Other) const -> bool
{
	return m_iItem != Other.m_iItem;
}
