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

#include <arxx/Buffer.h>
#include <arxx/BufferReader.h>

Arxx::BufferReader::BufferReader(const Arxx::Buffer & Buffer, Arxx::Buffer::size_type stPosition) :
	m_Buffer(Buffer),
	m_Marker(Buffer, stPosition, Arxx::Buffer::Marker::LEFT)
{
}

Arxx::BufferReader::BufferReader(const Arxx::BufferReader & BufferReader) :
	m_Buffer(BufferReader.m_Buffer),
	m_Marker(BufferReader.m_Buffer, BufferReader.stGetPosition(), Arxx::Buffer::Marker::LEFT)
{
}

Arxx::Buffer::size_type Arxx::BufferReader::stRead(Arxx::Buffer::size_type stLength, Arxx::Buffer::pointer Buffer)
{
	if(m_Marker.bIsValid() == false)
	{
		throw std::runtime_error("Arxx::BufferWriter::vWrite: Buffer was destroyed.");
	}
	if(m_Marker.stGetPosition() + stLength > m_Buffer.stGetLength())
	{
		throw std::out_of_range("Trying to read after the end.");
	}
	std::copy(m_Buffer.GetBegin() + m_Marker.stGetPosition(), m_Buffer.GetBegin() + m_Marker.stGetPosition() + stLength, Buffer);
	m_Marker.vSetPosition(m_Marker.stGetPosition() + stLength);
	
	return stLength;
}

const Arxx::Buffer & Arxx::BufferReader::GetBuffer(void) const
{
	return m_Buffer;
}

Arxx::Buffer::size_type Arxx::BufferReader::stGetPosition(void) const
{
	return m_Marker.stGetPosition();
}

void Arxx::BufferReader::vSetPosition(Arxx::Buffer::size_type stPosition)
{
	return m_Marker.vSetPosition(stPosition);
}

Arxx::BufferReader & Arxx::operator>>(Arxx::BufferReader & BufferReader, std::string & sString)
{
	const Arxx::Buffer & Buffer(BufferReader.GetBuffer());
	Arxx::Buffer::size_type stIndex(BufferReader.stGetPosition());
	Arxx::Buffer::size_type stLength(Buffer.stGetLength());
	char cChar(0);

	while(stLength > stIndex)
	{
		if((cChar = Buffer[stIndex++]) == '\0')
		{
			break;
		}
		sString += cChar;
	}
	BufferReader.vSetPosition(BufferReader.stGetPosition() + sString.length() + 1);

	return BufferReader;
}

Arxx::BufferReader & Arxx::operator>>(Arxx::BufferReader & BufferReader, float & fValue)
{
	BufferReader.stRead(sizeof(float), reinterpret_cast< Arxx::Buffer::pointer >(&fValue));
	
	return BufferReader;
}

Arxx::BufferReader & Arxx::operator>>(Arxx::BufferReader & BufferReader, Arxx::u1byte & u1Value)
{
	BufferReader.stRead(sizeof(Arxx::u1byte), reinterpret_cast< Arxx::Buffer::pointer >(&u1Value));
	
	return BufferReader;
}

Arxx::BufferReader & Arxx::operator>>(Arxx::BufferReader & BufferReader, Arxx::u4byte & u4Value)
{
	BufferReader.stRead(sizeof(Arxx::u4byte), reinterpret_cast< Arxx::Buffer::pointer >(&u4Value));
	
	return BufferReader;
}

Arxx::BufferReader & Arxx::operator>>(Arxx::BufferReader & BufferReader, Arxx::u8byte & Value)
{
	BufferReader.stRead(sizeof(Arxx::u8byte), reinterpret_cast< Arxx::Buffer::pointer >(&Value));
	
	return BufferReader;
}

Arxx::BufferReader & Arxx::operator>>(Arxx::BufferReader & BufferReader, bool & bValue)
{
	BufferReader.stRead(sizeof(bool), reinterpret_cast< Arxx::Buffer::pointer >(&bValue));
	
	return BufferReader;
}
