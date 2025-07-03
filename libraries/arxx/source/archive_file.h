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

#ifndef GALACTIC_FALL__LIBRARIES__ARXX__SOURCE__ARCHIVEFILE_H
#define GALACTIC_FALL__LIBRARIES__ARXX__SOURCE__ARCHIVEFILE_H

#include <netinet/in.h>

#include <arxx/common.h>

/**
 * @file ArchiveFile.h
 * 
 * A hidden header file with Arxx internal structs and function definitions for reading an ARX archive file.
 **/

namespace Arxx
{
#pragma pack(1)
	/**
	 * @brief The representation of an archive's header in a file.
	 *
	 * This struct is the internal representation of an archive's header as it occures in a general ARX archive file. Reading the first sizeof(ArchiveHeader) bytes from an ARX archive file give you information about the version of the archive, the number of items contained in the archive and the ID of the root item. Thus, these information are considered absolutely neccessary for any loading process and on the other hand easy to get at the saving process.
	 **/
	class ArchiveHeader
	{
	public:
		/**
		 * @brief The major version number of the archive.
		 * @note Represents the XX.xx.xx.xx.
		 *
		 * This is the leading version number.
		 **/
		u1byte MajorVersionNumber;
		
		/**
		 * @brief The minor version number of the archive.
		 * @note Represents the xx.XX.xx.xx.
		 *
		 * This is the second leading version number.
		 **/
		u1byte MinorVersionNumber;
		
		/**
		 * @brief The revision number of the archive.
		 * @note Represents the xx.xx.XX.xx.
		 *
		 * This is the revision number.
		 **/
		u1byte RevisionNumber;
		
		/**
		 * @brief The candidate number of the archive.
		 * @note Represents the xx.xx.xx.XX.
		 *
		 * This is the candidate number.
		 **/
		u1byte CandidateNumber;
		
		/**
		 * @brief The unique identifier of the root item of the archive.
		 *
		 * This member might equal Arxx::g_u4InvalidID when there is no root item specified or this archive.
		 **/
		u4byte RootItemIdentifier;
		
		/**
		 * @brief The number of items stored within the archive.
		 **/
		u4byte NumberOfItems;
	};
	
	/**
	 * @brief The memory representation of a single item.
	 * @note This class is located within a "pragma pack(1)" block.
	 *
	 * This class contains all the meta information associated with a specific item in an ARX archive.
	 **/
	class ItemHeader
	{
	public:
		/**
		 * @brief Default constructor creating an empty ItemHeader.
		 **/
		ItemHeader() :
			Identifier(0),
			Type(0),
			SubType(0),
			MajorVersionNumber(0),
			MinorVersionNumber(0),
			RevisionNumber(0),
			CandidateNumber(0),
			DataCompressionType(0),
			NameLength(0),
			DataDecompressedLength(0),
			DataCompressedLength(0),
			StructureLength(0)
		{
		}
		
		/**
		 * @brief The identifier of the item.
		 *
		 * The identifier of an item is unique in respect to the parenting archive.
		 **/
		u4byte Identifier;
		
		/**
		 * @brief The type of the item.
		 *
		 * The type of the item.
		 **/
		u4byte Type;
		
		/**
		 * @brief The subtype of the item.
		 *
		 * The subtype of the item.
		 **/
		u4byte SubType;
		
		/**
		 * @brief The major version number of the item.
		 * @note Represents the XX.xx.xx.xx.
		 **/
		u1byte MajorVersionNumber;
		
		/**
		 * @brief The minor version number of the item.
		 * @note Represents the xx.XX.xx.xx.
		 **/
		u1byte MinorVersionNumber;
		
		/**
		 * @brief The revision number of the item.
		 * @note Represents the xx.xx.XX.xx.
		 **/
		u1byte RevisionNumber;
		
		/**
		 * @brief The candidate number of the item.
		 * @note Represents the xx.xx.xx.XX.
		 **/
		u1byte CandidateNumber;
		
		/**
		 * @brief Information about the compression of the item.
		 *
		 * The state of compression for the item. Defines the level of compression that was used with the zlib functions or 0 to indicate no compression.
		 **/
		u4byte DataCompressionType;
		
		/**
		 * @brief The length of the item's name.
		 *
		 * The length of the item's name. The name is stored right after the ItemHeader and without a final zero character.
		 **/
		u4byte NameLength;
		
		/**
		 * @brief The length of the item's data when decompressed.
		 *
		 * The length of the data when it is decompressed. This member does not imply that the data actually IS decompressed. It merely gives you a 'would-be' information.
		 **/
		u4byte DataDecompressedLength;
		
		/**
		 * @brief The length of the item's data when compressed.
		 *
		 * The length of the data when it is compressed. This member does not imply that the data actually IS compressed. It merely gives you a 'would-be' information. If the item's data IS compressed, this member states the length of the compressed data block. On the other hand, when the item's data is decompressed, this member is invalid. (In the current implementation this member actually states the length of the last compression, but it CAN only give you a hint about the size of the data, in case you decompressed it externaly.
		 **/
		u4byte DataCompressedLength;
		
		/**
		 * @brief This component describes the length of the structure description for the item.
		 *
		 * The length of the structure buffer is needed when it will be read on loading an archive. The structure of the archive is part of its header information and therefore placed in the ItemHeader. The structure data follows immediately after the closing 0 of the item's name.
		 **/
		u4byte StructureLength;
	};
#pragma pack()

	/**
	 * @brief The output operator for a Arxx::ArchiveHeader struct.
	 * 
	 * This function takes care of the correct output of a ArchiveHeader to any kind of std::ostream.
	 **/
	inline std::ostream & operator<<(std::ostream & OStream, Arxx::ArchiveHeader & ArchiveHeader)
	{
		OStream.write(reinterpret_cast<char const *>(&ArchiveHeader.MajorVersionNumber), sizeof(u1byte));
		OStream.write(reinterpret_cast<char const *>(&ArchiveHeader.MinorVersionNumber), sizeof(u1byte));
		OStream.write(reinterpret_cast<char const *>(&ArchiveHeader.RevisionNumber), sizeof(u1byte));
		OStream.write(reinterpret_cast<char const *>(&ArchiveHeader.CandidateNumber), sizeof(u1byte));
		
		u4byte RootItemIdentifier(htonl(ArchiveHeader.RootItemIdentifier));
		
		OStream.write(reinterpret_cast<char const *>(&RootItemIdentifier), sizeof(u4byte));
		
		u4byte NumberOfItems(htonl(ArchiveHeader.NumberOfItems));
		
		OStream.write(reinterpret_cast<char const *>(&NumberOfItems), sizeof(u4byte));
	
		return OStream;
	}
	
	/**
	 * @brief The output operator for a Arxx::ItemHeader struct.
	 * 
	 * This function takes care of the correct output of a ItemHeader to any kind of std::ostream.
	 **/
	inline std::ostream & operator<<(std::ostream & OStream, Arxx::ItemHeader & ItemHeader)
	{
		u4byte Identifier(htonl(ItemHeader.Identifier));
		
		OStream.write(reinterpret_cast<char const *>(&Identifier), sizeof(u4byte));
		
		u4byte Type(htonl(ItemHeader.Type));
		
		OStream.write(reinterpret_cast<char const *>(&Type), sizeof(u4byte));
		
		u4byte SubType(htonl(ItemHeader.SubType));
		
		OStream.write(reinterpret_cast<char const *>(&SubType), sizeof(u4byte));
		
		OStream.write(reinterpret_cast<char const *>(&ItemHeader.MajorVersionNumber), sizeof(u1byte));
		OStream.write(reinterpret_cast<char const *>(&ItemHeader.MinorVersionNumber), sizeof(u1byte));
		OStream.write(reinterpret_cast<char const *>(&ItemHeader.RevisionNumber), sizeof(u1byte));
		OStream.write(reinterpret_cast<char const *>(&ItemHeader.CandidateNumber), sizeof(u1byte));
		
		u4byte DataCompressionType(htonl(ItemHeader.DataCompressionType));
		
		OStream.write(reinterpret_cast<char const *>(&DataCompressionType), sizeof(u4byte));
		
		u4byte NameLength(htonl(ItemHeader.NameLength));
		
		OStream.write(reinterpret_cast<char const *>(&NameLength), sizeof(u4byte));
		
		u4byte DataDecompressedLength(htonl(ItemHeader.DataDecompressedLength));
		
		OStream.write(reinterpret_cast<char const *>(&DataDecompressedLength), sizeof(u4byte));
		
		u4byte DataCompressedLength(htonl(ItemHeader.DataCompressedLength));
		
		OStream.write(reinterpret_cast<char const *>(&DataCompressedLength), sizeof(u4byte));
		
		u4byte StructureLength(htonl(ItemHeader.StructureLength));
		
		OStream.write(reinterpret_cast<char const *>(&StructureLength), sizeof(u4byte));
	
		return OStream;
	}
	
	/**
	 * @brief The input operator for a Arxx::ArchiveHeader struct.
	 * 
	 * This function takes care of the correct reading of a ArchiveHeader from any kind of std::istream.
	 **/
	inline std::istream & operator>>(std::istream & IStream, Arxx::ArchiveHeader & ArchiveHeader)
	{
		IStream.read(reinterpret_cast<char *>(&ArchiveHeader.MajorVersionNumber), sizeof(u1byte));
		IStream.read(reinterpret_cast<char *>(&ArchiveHeader.MinorVersionNumber), sizeof(u1byte));
		IStream.read(reinterpret_cast<char *>(&ArchiveHeader.RevisionNumber), sizeof(u1byte));
		IStream.read(reinterpret_cast<char *>(&ArchiveHeader.CandidateNumber), sizeof(u1byte));
		
		u4byte RootItemIdentifier;
		
		IStream.read(reinterpret_cast<char *>(&RootItemIdentifier), sizeof(u4byte));
		ArchiveHeader.RootItemIdentifier = ntohl(RootItemIdentifier);
		
		u4byte NumberOfItems;
		
		IStream.read(reinterpret_cast<char *>(&NumberOfItems), sizeof(u4byte));
		ArchiveHeader.NumberOfItems = ntohl(NumberOfItems);
		
		return IStream;
	}
	
	/**
	 * @brief The input operator for a Arxx::ItemHeader struct.
	 * 
	 * This function takes care of the correct reading of a ItemHeader from any kind of std::istream.
	 **/
	inline std::istream & operator>>(std::istream & IStream, Arxx::ItemHeader & ItemHeader)
	{
		u4byte Identifier;
		
		IStream.read(reinterpret_cast<char *>(&Identifier), sizeof(u4byte));
		ItemHeader.Identifier = ntohl(Identifier);
		
		u4byte Type;
		
		IStream.read(reinterpret_cast<char *>(&Type), sizeof(u4byte));
		ItemHeader.Type = ntohl(Type);
		
		u4byte SubType;
		
		IStream.read(reinterpret_cast<char *>(&SubType), sizeof(u4byte));
		ItemHeader.SubType = ntohl(SubType);
		IStream.read(reinterpret_cast<char *>(&ItemHeader.MajorVersionNumber), sizeof(u1byte));
		IStream.read(reinterpret_cast<char *>(&ItemHeader.MinorVersionNumber), sizeof(u1byte));
		IStream.read(reinterpret_cast<char *>(&ItemHeader.RevisionNumber), sizeof(u1byte));
		IStream.read(reinterpret_cast<char *>(&ItemHeader.CandidateNumber), sizeof(u1byte));
		
		u4byte DataCompressionType;
		
		IStream.read(reinterpret_cast<char *>(&DataCompressionType), sizeof(u4byte));
		ItemHeader.DataCompressionType = ntohl(DataCompressionType);
		
		u4byte NameLength;
		
		IStream.read(reinterpret_cast<char *>(&NameLength), sizeof(u4byte));
		ItemHeader.NameLength = ntohl(NameLength);
		
		u4byte DataDecompressedLength;
		
		IStream.read(reinterpret_cast<char *>(&DataDecompressedLength), sizeof(u4byte));
		ItemHeader.DataDecompressedLength = ntohl(DataDecompressedLength);
		
		u4byte DataCompressedLength;
		
		IStream.read(reinterpret_cast<char *>(&DataCompressedLength), sizeof(u4byte));
		ItemHeader.DataCompressedLength = ntohl(DataCompressedLength);
		
		u4byte StructureLength;
		
		IStream.read(reinterpret_cast<char *>(&StructureLength), sizeof(u4byte));
		ItemHeader.StructureLength = ntohl(StructureLength);
		
		return IStream;
	}
}

#endif
