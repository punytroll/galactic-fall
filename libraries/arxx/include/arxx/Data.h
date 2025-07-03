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

#ifndef GALACTIC_FALL__LIBRARIES__ARXX__INCLUDE__DATA_H
#define GALACTIC_FALL__LIBRARIES__ARXX__INCLUDE__DATA_H

#include <arxx/Buffer.h>
#include <arxx/Common.h>

/**
 * @file Data.h
 * 
 * Declares the interface of Arxx::Data.
 **/

namespace Arxx
{
	/**
	 * @brief A buffer with compression, decompression and external data referencing features.
	 *
	 * This is the component underlying the Arxx::Item class. It delivers the ability to compress and decompress the data. Because of the public inhertitance from Arxx::Buffer it is still possible to call Buffer::stGetLength() but be aware that it will give you the state of the buffer which might not be what you want for two reasons:
	 * - the buffer content might be compressed or decompressed
	 * - the data might not be fetched yet, which makes Arxx::Buffer::stGetLength() return `0` since there is nothing in the buffer
	 * - the data might not be fetched yet, but you have written something in the buffer. Arxx::Buffer::stGetLength() will of course return the length of that data instead of the length of the Item's data, be it compressed or decompressed.
	 **/
	class Data : public Arxx::Buffer
	{
	public:
		typedef enum
		{
			NONE = 0,
			ZLIB_0 = 1,
			ZLIB_1 = 2,
			ZLIB_2 = 3,
			ZLIB_3 = 4,
			ZLIB_4 = 5,
			ZLIB_5 = 6,
			ZLIB_6 = 7,
			ZLIB_7 = 8,
			ZLIB_8 = 9,
			ZLIB_9 = 10,
			BZLIB = 11
		} Compression;
		
		static Arxx::Data::Compression m_DefaultCompression;
		
		/**
		 * @brief The default constructor for a Arxx::Data.
		 **/
		Data();
		
		/**
		 * @brief The destructor of the Arxx::Data.
		 **/
		~Data();
		
		/**
		 * @brief Decompresses the data of the buffer.
		 **/
		auto Decompress() -> void;
		
		/**
		 * @brief Compresses the data of the buffer.
		 **/
		auto Compress(Arxx::Data::Compression Compression = Arxx::Data::m_DefaultCompression) -> void;
		
		/**
		 * @brief Returns true if the data inside the buffer is compressed.
		 **/
		auto IsCompressed() const -> bool;
		
		/**
		 * @brief Returns true if the data inside the buffer is decompressed.
		 **/
		auto IsDecompressed() const -> bool;
		
		/**
		 * @brief Returns the compression information, which will equal the compression rate of the zlib library.
		 **/
		auto GetCompression() const -> Arxx::Data::Compression;
		
		/**
		 * @brief Gets the length of the decompressed data.
		 **/
		auto GetDecompressedLength() const -> Arxx::u4byte;
		
		/**
		 * @brief Gets the length of the compressed data.
		 **/
		auto GetCompressedLength() const -> Arxx::u4byte;
		
		/**
		 * @brief This function requests the item's data at the appropriate data channel.
		 * @return Wether the request to fetch the data could be handed to a data channel.
		 * 
		 * Note that fetching the data does not automatically internalize it.
		 * 
		 * In order to only try to fetch the data m_URI must be valid and bIsFetched() and bIsFetching() must be false. Otherwise the function will return false.
		 * 
		 * For asynchronous fetches the return value may be true because an Arxx::DataChannel could be found but the actuall fetching may fail. This information may be retrieved via Arxx::Data::GetFetchStatus().
		 **/
		auto Fetch() -> bool;
		
		/**
		 * @brief Unfetching the data generally means deleting the data thus releasing the memory in the buffer.
		 * 
		 * This function will empty the data buffer and release the memory associated with it. So why an extra function if this could be done with Buffers::Buffer::vDelete()?
		 * 
		 * Because if you vDelete the content of the buffer you change the data. Hence you don't change the flag whether the data is fetched and will not be able the refetch the data using vFetch() again.
		 * 
		 * This function requires the bIsFetched() to be true.
		 * 
		 * This function sets @a m_FetchStatus to Arxx::UNFETCHED.
		 **/
		auto Unfetch() -> void;
		
		/**
		 * @brief Sets fetch information for the data object.
		 * @param Offset The offset of the fetchable data.
		 * @param Compression The state of the data. If this value if greater than 0 the fake data is considered compressed.
		 * @param DecompressedLength The length of the data if in decompressed state. Elsewise the value which Arxx::Buffer::u4GetDecompressedLength() will return and this data will serve no internal functionality, so you may use it as you like.
		 * @param CompressedLength The length of the data if in compressed state. Elsewise the value which Arxx::Buffer::u4GetCompressedDataLength() will return and this data will serve no internal functionality, so you may use it as you like.
		 * 
		 * This function will set @a _Fetched to false.
		 **/
		auto SetFetchInformation(Arxx::u4byte Offset, Arxx::Data::Compression Compression, Arxx::u4byte DecompressedLength, Arxx::u4byte CompressedLength) -> void;
		
		/**
		 * @brief Returns wether the data is fetched.
		 **/
		auto IsFetched() const -> bool;
        
	protected:
		virtual auto m_Fetch(Arxx::u4byte Offset, Arxx::u4byte Length) -> bool;
        
	private:
		/**
		 * @brief A locator used to further describe the location to the fetcher.
		 * 
		 * After creating an Item this value is intentionally invalid.
		 **/
		Arxx::u4byte m_Offset;
		
		/**
		 * @brief The compressed state the data is currently in.
		 * 
		 * Is set to either 0 for to indicate no compression or some value greater than 0 to indicate the compression level of the data as in the zlib library.
		 **/
		Arxx::Data::Compression m_CompressionType;
		
		/**
		 * @brief The length of the data if decompressed.
		 **/
		Arxx::u4byte m_DecompressedLength;
		
		/**
		 * @brief The length of the data if compressed or 0.
		 * 
		 * Since the compressed size is not known as long as the data is decompressed this value is 0 whenever the data is decompressed.
		 **/
		Arxx::u4byte m_CompressedLength;
		
		/**
		 * @brief A status indicator showing the fetch status of the data.
		 **/
		bool m_Fetched;
        
	};
}

#endif
