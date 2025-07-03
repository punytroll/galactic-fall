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

#include <arxx/data.h>

/// optional headers
#ifdef HAVE_ZLIB_H
#include <zlib.h>
#endif

#ifdef HAVE_BZLIB_H
#include <bzlib.h>
#endif

/// default compression
#ifdef HAVE_BZLIB_H
Arxx::Data::Compression Arxx::Data::m_DefaultCompression = BZLIB;
#elif HAVE_ZLIB_H
Arxx::Data::Compression Arxx::Data::m_DefaultCompression = ZLIB_9;
#else
Arxx::Data::Compression Arxx::Data::m_DefaultCompression = NONE;
#endif

Arxx::Data::Data() :
	m_Offset(0),
	m_CompressionType(Arxx::Data::NONE),
	m_DecompressedLength(0),
	m_CompressedLength(0),
	m_Fetched(true)
{
}

Arxx::Data::~Data()
{
}

auto Arxx::Data::Decompress() -> void
{
	switch(m_CompressionType)
	{
	case Arxx::Data::NONE:
		{
			break;
		}
	case Arxx::Data::ZLIB_0:
	case Arxx::Data::ZLIB_1:
	case Arxx::Data::ZLIB_2:
	case Arxx::Data::ZLIB_3:
	case Arxx::Data::ZLIB_4:
	case Arxx::Data::ZLIB_5:
	case Arxx::Data::ZLIB_6:
	case Arxx::Data::ZLIB_7:
	case Arxx::Data::ZLIB_8:
	case Arxx::Data::ZLIB_9:
		{
#ifdef HAVE_ZLIB_H
			Arxx::Data::pointer DecompressedData(new Arxx::Data::value_type[m_DecompressedLength]);
			z_stream zStream;
			int iReturn = 0;
			
			zStream.zalloc = 0;
			zStream.zfree = 0;
			zStream.next_in = const_cast< Arxx::u1byte * >(GetBegin());
			/** @todo Why not use u4GetCompressedLength() here? **/
			zStream.avail_in = stGetLength();
			zStream.next_out = DecompressedData;
			/** @todo Why not use u4GetDecompressedLength() here? **/
			zStream.avail_out = m_DecompressedLength;
			if((iReturn = inflateInit(&zStream)) != Z_OK)
			{
				delete[] DecompressedData;
				/** @todo Correctly specify the actual error that occured in zlib. **/
				throw zlib_error("TODO: specify!");
			}
			if((iReturn = inflate(&zStream, Z_FINISH)) != Z_STREAM_END)
			{
				delete[] DecompressedData;
				/** @todo Correctly specify the actual error that occured in zlib. **/
				throw zlib_error("TODO: specify!");
			}
			if((iReturn = inflateEnd(&zStream)) != Z_OK)
			{
				delete[] DecompressedData;
				/** @todo Correctly specify the actual error that occured in zlib. **/
				throw zlib_error("TODO: specify!");
			}
			vSetLength(0);
			vInsert(0, m_DecompressedLength, DecompressedData);
			delete[] DecompressedData;
			m_CompressionType = Arxx::Data::NONE;
			m_CompressedLength = 0;
#else
			throw std::runtime_error("Unsupported decompression method.");
#endif
			
			break;
		}
	case Arxx::Data::BZLIB:
		{
#ifdef HAVE_BZLIB_H
			Arxx::Data::pointer DecompressedData(new Arxx::Data::value_type[m_DecompressedLength]);
			bz_stream BZStream;
			
			BZStream.bzalloc = 0;
			BZStream.bzfree = 0;
			BZStream.opaque = 0;
			BZStream.next_in = reinterpret_cast< char * >(const_cast< Arxx::u1byte * >(GetBegin()));
			/** @todo Why not use u4GetCompressedLength() here? **/
			BZStream.avail_in = stGetLength();
			BZStream.next_out = reinterpret_cast< char * >(DecompressedData);
			/** @todo Why not use u4GetDecompressedLength() here? **/
			BZStream.avail_out = m_DecompressedLength;
			
			int iBZResult;
			
			if((iBZResult = BZ2_bzDecompressInit(&BZStream, 0, 0)) != BZ_OK)
			{
				delete[] DecompressedData;
				/** @todo Correctly specify the actual error that occured in bzlib. **/
				throw bzlib_error("TODO: specify!");
			}
			if((iBZResult = BZ2_bzDecompress(&BZStream)) != BZ_STREAM_END)
			{
				delete[] DecompressedData;
				/** @todo Correctly specify the actual error that occured in bzlib. **/
				throw bzlib_error("TODO: specify!");
			}
			if((iBZResult = BZ2_bzDecompressEnd(&BZStream)) != BZ_OK)
			{
				delete[] DecompressedData;
				/** @todo Correctly specify the actual error that occured in bzlib. **/
				throw bzlib_error("TODO: specify!");
			}
			vSetLength(0);
			vInsert(0, m_DecompressedLength, DecompressedData);
			delete[] DecompressedData;
			m_CompressionType = Arxx::Data::NONE;
			m_CompressedLength = 0;
#else
			throw std::runtime_error("Unsupported decompression method.");
#endif
			break;
		}
	}
}

auto Arxx::Data::Compress(Arxx::Data::Compression CompressionType) -> void
{
	if((IsCompressed() == true) || (IsFetched() == false))
	{
		return;
	}
	switch(CompressionType)
	{
	case Arxx::Data::NONE:
		{
			break;
		}
	case Arxx::Data::ZLIB_0:
	case Arxx::Data::ZLIB_1:
	case Arxx::Data::ZLIB_2:
	case Arxx::Data::ZLIB_3:
	case Arxx::Data::ZLIB_4:
	case Arxx::Data::ZLIB_5:
	case Arxx::Data::ZLIB_6:
	case Arxx::Data::ZLIB_7:
	case Arxx::Data::ZLIB_8:
	case Arxx::Data::ZLIB_9:
		{
#ifdef HAVE_ZLIB_H
			Arxx::Data::pointer CompressedData = new Arxx::Data::value_type[static_cast< Arxx::u4byte >(GetDecompressedLength() * 1.001 + 12)];
			z_stream zStream;
			signed long slReturn = 0;
			
			zStream.zalloc = 0;
			zStream.zfree = 0;
			zStream.next_in = const_cast< Arxx::u1byte * >(GetBegin());
			zStream.avail_in = GetDecompressedLength();
			zStream.next_out = CompressedData;
			zStream.avail_out = static_cast< Arxx::u4byte >(GetDecompressedLength() * 1.001 + 12);
			if((slReturn = deflateInit(&zStream, CompressionType - Arxx::Data::ZLIB_0)) != Z_OK)
			{
				delete[] CompressedData;
				/** @todo Correctly specify the actual error that occured in zlib. **/
				throw zlib_error("TODO: specify!");
			}
			if((slReturn = deflate(&zStream, Z_FINISH)) != Z_STREAM_END)
			{
				delete[] CompressedData;
				/** @todo Correctly specify the actual error that occured in zlib. **/
				throw zlib_error("TODO: specify!");
			}
			if((slReturn = deflateEnd(&zStream)) != Z_OK)
			{
				delete[] CompressedData;
				/** @todo Correctly specify the actual error that occured in zlib. **/
				throw zlib_error("TODO: specify!");
			}
			vSetLength(0);
			vInsert(0, zStream.total_out, CompressedData);
			delete[] CompressedData;
			m_CompressionType = CompressionType;
			m_CompressedLength = zStream.total_out;
#else
			throw std::runtime_error("Unsupported compression method.");
#endif
			
			break;
		}
	case Arxx::Data::BZLIB:
		{
#ifdef HAVE_BZLIB_H
			Arxx::u4byte u4CompressedDataLength(static_cast< Arxx::u4byte >(GetDecompressedLength() * 1.01 + 600));
			Arxx::Data::pointer CompressedData(new Arxx::Data::value_type[u4CompressedDataLength]);
			bz_stream BZStream;
			
			BZStream.bzalloc = 0;
			BZStream.bzfree = 0;
			BZStream.opaque = 0;
			BZStream.next_in = reinterpret_cast< char * >(const_cast< Arxx::u1byte * >(GetBegin()));
			BZStream.avail_in = GetDecompressedLength();
			BZStream.next_out = reinterpret_cast< char * >(CompressedData);
			BZStream.avail_out = u4CompressedDataLength;
			
			int iBZResult;
			
			if((iBZResult = BZ2_bzCompressInit(&BZStream, 9, 0, 0)) != BZ_OK)
			{
				delete[] CompressedData;
				/** @todo Correctly specify the actual error that occured in bzlib. **/
				throw bzlib_error("TODO: specify!");
			}
			if((iBZResult = BZ2_bzCompress(&BZStream, BZ_FINISH)) != BZ_STREAM_END)
			{
				delete[] CompressedData;
				/** @todo Correctly specify the actual error that occured in bzlib. **/
				throw bzlib_error("TODO: specify!");
			}
			if((iBZResult = BZ2_bzCompressEnd(&BZStream)) != BZ_OK)
			{
				delete[] CompressedData;
				/** @todo Correctly specify the actual error that occured in bzlib. **/
				throw bzlib_error("TODO: specify!");
			}
			vSetLength(0);
			vInsert(0, BZStream.total_out_lo32, CompressedData);
			delete[] CompressedData;
			m_CompressionType = CompressionType;
			m_CompressedLength = BZStream.total_out_lo32;
#else
			throw std::runtime_error("Unsupported compression method.");
#endif
			break;
		}
	}
}

auto Arxx::Data::IsCompressed() const -> bool
{
	return m_CompressionType != Arxx::Data::NONE;
}

auto Arxx::Data::IsDecompressed() const -> bool
{
	return m_CompressionType == Arxx::Data::NONE;
}

auto Arxx::Data::GetCompression() const -> Arxx::Data::Compression
{
	return m_CompressionType;
}

auto Arxx::Data::GetDecompressedLength() const -> Arxx::u4byte
{
	if((IsFetched() == true) && (IsDecompressed() == true))
	{
		return stGetLength();
	}
	else
	{
		return m_DecompressedLength;
	}
}

auto Arxx::Data::GetCompressedLength() const -> Arxx::u4byte
{
	if((IsFetched() == true) && (IsCompressed() == true))
	{
		return stGetLength();
	}
	else
	{
		return m_CompressedLength;
	}
}

auto Arxx::Data::Fetch() -> bool
{
	if(m_Fetched == false)
	{
		m_Fetched = m_Fetch(m_Offset, ((m_CompressionType == Arxx::Data::NONE) ? (m_DecompressedLength) : (m_CompressedLength)));
		
		return m_Fetched;
	}
	else
	{
		return false;
	}
}

auto Arxx::Data::Unfetch() -> void
{
	if(IsFetched() == true)
	{
		vDelete(0, stGetLength());
		m_Fetched = false;
	}
}

auto Arxx::Data::SetFetchInformation(Arxx::u4byte Offset, Arxx::Data::Compression CompressionType, Arxx::u4byte DecompressedLength, Arxx::u4byte CompressedLength) -> void
{
	m_Offset = Offset;
	m_CompressionType = CompressionType;
	m_DecompressedLength = DecompressedLength;
	m_CompressedLength = CompressedLength;
	m_Fetched = false;
}

auto Arxx::Data::IsFetched() const -> bool
{
	return m_Fetched;
}

auto Arxx::Data::m_Fetch([[maybe_unused]] Arxx::u4byte Offset, Arxx::u4byte Length) -> bool
{
	return Length == 0;
}
