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

#include <string.h>

#include <istream>

#include <arxx/Buffer.h>
#include <arxx/BufferWriter.h>

#ifdef DEBUG
#include <iostream>
#endif

Arxx::BufferWriter::BufferWriter(Arxx::Buffer & Buffer, Arxx::Buffer::size_type stPosition) :
	m_Buffer(Buffer),
	m_Marker(Buffer, stPosition, Arxx::Buffer::Marker::LEFT)
{
}

Arxx::BufferWriter::BufferWriter(const Arxx::BufferWriter & BufferWriter) :
	m_Buffer(BufferWriter.m_Buffer),
	m_Marker(BufferWriter.m_Buffer, BufferWriter.stGetPosition(), Arxx::Buffer::Marker::LEFT)
{
}

void Arxx::BufferWriter::vWrite(Arxx::Buffer::size_type stDataLength, Arxx::Buffer::const_pointer Data)
{
	if(m_Marker.bIsValid() == false)
	{
		throw std::runtime_error("Arxx::BufferWriter::vWrite: Buffer was destroyed.");
	}
	m_Marker.vSetAlignment(Arxx::Buffer::Marker::RIGHT);
	m_Buffer.vInsert(m_Marker.stGetPosition(), stDataLength, Data);
	m_Marker.vSetAlignment(Arxx::Buffer::Marker::LEFT);
}

Arxx::Buffer & Arxx::BufferWriter::GetBuffer()
{
	return m_Buffer;
}

const Arxx::Buffer & Arxx::BufferWriter::GetBuffer() const
{
	return m_Buffer;
}

Arxx::Buffer::size_type Arxx::BufferWriter::stGetPosition() const
{
	return m_Marker.stGetPosition();
}

void Arxx::BufferWriter::vSetPosition(Arxx::Buffer::size_type stPosition)
{
	return m_Marker.vSetPosition(stPosition);
}

Arxx::BufferWriter & Arxx::operator<<(Arxx::BufferWriter & BufferWriter, const std::string & sString)
{
	BufferWriter.vWrite(sString.length() + 1, reinterpret_cast< Arxx::Buffer::const_pointer >(sString.c_str()));

	return BufferWriter;
}

Arxx::BufferWriter & Arxx::operator<<(Arxx::BufferWriter & BufferWriter, const char * pcString)
{
	BufferWriter.vWrite(strlen(pcString) + 1, reinterpret_cast< Arxx::Buffer::const_pointer >(pcString));

	return BufferWriter;
}
Arxx::BufferWriter & Arxx::operator<<(Arxx::BufferWriter & BufferWriter, const float & fValue)
{
	BufferWriter.vWrite(sizeof(float), reinterpret_cast< Arxx::Buffer::const_pointer >(&fValue));
	
	return BufferWriter;
}

Arxx::BufferWriter & Arxx::operator<<(Arxx::BufferWriter & BufferWriter, const Arxx::u1byte & Value)
{
	BufferWriter.vWrite(sizeof(Arxx::u1byte), reinterpret_cast< Arxx::Buffer::const_pointer >(&Value));
	
	return BufferWriter;
}

Arxx::BufferWriter & Arxx::operator<<(Arxx::BufferWriter & BufferWriter, const Arxx::u4byte & u4Value)
{
	BufferWriter.vWrite(sizeof(Arxx::u4byte), reinterpret_cast< Arxx::Buffer::const_pointer >(&u4Value));
	
	return BufferWriter;
}

Arxx::BufferWriter & Arxx::operator<<(Arxx::BufferWriter & BufferWriter, const Arxx::u8byte & u8Value)
{
	BufferWriter.vWrite(sizeof(Arxx::u8byte), reinterpret_cast< Arxx::Buffer::const_pointer >(&u8Value));
	
	return BufferWriter;
}

Arxx::BufferWriter & Arxx::operator<<(Arxx::BufferWriter & BufferWriter, const bool & bValue)
{
	BufferWriter.vWrite(sizeof(bool), reinterpret_cast< Arxx::Buffer::const_pointer >(&bValue));
	
	return BufferWriter;
}

Arxx::BufferWriter & Arxx::operator<<(Arxx::BufferWriter & BufferWriter, const char & cValue)
{
	BufferWriter.vWrite(sizeof(char), reinterpret_cast< Arxx::Buffer::const_pointer >(&cValue));
	
	return BufferWriter;
}

Arxx::BufferWriter & Arxx::operator<<(Arxx::BufferWriter & BufferWriter, const std::pair< Arxx::Buffer::size_type, std::istream * > & Stream)
{
#ifdef DEBUG
	std::cerr << "Arxx::operator<<(std::pair< size, stream >): size = " << Stream.first << std::endl;
#endif
	Arxx::Buffer::size_type Index(0);

	while(Index < Stream.first)
	{
		char Character;
		
		Stream.second->get(Character);
		if(Stream.second->good() == true)
		{
			++Index;
			BufferWriter.vWrite(1, reinterpret_cast< Arxx::Buffer::pointer >(&Character));
		}
		else
		{
			break;
		}
	}
#ifdef DEBUG
	std::cerr << "Arxx::operator<<(std::pair< size, stream >): read = " << Index << std::endl;
#endif

	return BufferWriter;
}
