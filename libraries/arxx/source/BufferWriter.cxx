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

Arxx::BufferWriter::BufferWriter(Arxx::BufferWriter const & Other) :
	m_Buffer(Other.m_Buffer),
	m_Marker(Other.m_Buffer, Other.stGetPosition(), Arxx::Buffer::Marker::LEFT)
{
}

auto Arxx::BufferWriter::vWrite(Arxx::Buffer::size_type stDataLength, Arxx::Buffer::const_pointer Data) -> void
{
	if(m_Marker.bIsValid() == false)
	{
		throw std::runtime_error("Arxx::BufferWriter::vWrite: Buffer was destroyed.");
	}
	m_Marker.vSetAlignment(Arxx::Buffer::Marker::RIGHT);
	m_Buffer.vInsert(m_Marker.stGetPosition(), stDataLength, Data);
	m_Marker.vSetAlignment(Arxx::Buffer::Marker::LEFT);
}

auto Arxx::BufferWriter::GetBuffer() -> Arxx::Buffer &
{
	return m_Buffer;
}

auto Arxx::BufferWriter::GetBuffer() const -> Arxx::Buffer const &
{
	return m_Buffer;
}

auto Arxx::BufferWriter::stGetPosition() const -> Arxx::Buffer::size_type
{
	return m_Marker.stGetPosition();
}

auto Arxx::BufferWriter::vSetPosition(Arxx::Buffer::size_type stPosition) -> void
{
	return m_Marker.vSetPosition(stPosition);
}

auto Arxx::operator<<(Arxx::BufferWriter & BufferWriter, std::string const & sString) -> Arxx::BufferWriter &
{
	BufferWriter.vWrite(sString.length() + 1, reinterpret_cast< Arxx::Buffer::const_pointer >(sString.c_str()));

	return BufferWriter;
}

auto Arxx::operator<<(Arxx::BufferWriter & BufferWriter, char const * pcString) -> Arxx::BufferWriter &
{
	BufferWriter.vWrite(strlen(pcString) + 1, reinterpret_cast< Arxx::Buffer::const_pointer >(pcString));

	return BufferWriter;
}

auto Arxx::operator<<(Arxx::BufferWriter & BufferWriter, float fValue) -> Arxx::BufferWriter &
{
	BufferWriter.vWrite(sizeof(float), reinterpret_cast< Arxx::Buffer::const_pointer >(&fValue));
	
	return BufferWriter;
}

auto Arxx::operator<<(Arxx::BufferWriter & BufferWriter, Arxx::u1byte Value) -> Arxx::BufferWriter &
{
	BufferWriter.vWrite(sizeof(Arxx::u1byte), reinterpret_cast< Arxx::Buffer::const_pointer >(&Value));
	
	return BufferWriter;
}

auto Arxx::operator<<(Arxx::BufferWriter & BufferWriter, Arxx::u4byte u4Value) -> Arxx::BufferWriter &
{
	BufferWriter.vWrite(sizeof(Arxx::u4byte), reinterpret_cast< Arxx::Buffer::const_pointer >(&u4Value));
	
	return BufferWriter;
}

auto Arxx::operator<<(Arxx::BufferWriter & BufferWriter, Arxx::u8byte u8Value) -> Arxx::BufferWriter &
{
	BufferWriter.vWrite(sizeof(Arxx::u8byte), reinterpret_cast< Arxx::Buffer::const_pointer >(&u8Value));
	
	return BufferWriter;
}

auto Arxx::operator<<(Arxx::BufferWriter & BufferWriter, bool bValue) -> Arxx::BufferWriter &
{
	BufferWriter.vWrite(sizeof(bool), reinterpret_cast< Arxx::Buffer::const_pointer >(&bValue));
	
	return BufferWriter;
}

auto Arxx::operator<<(Arxx::BufferWriter & BufferWriter, char cValue) -> Arxx::BufferWriter &
{
	BufferWriter.vWrite(sizeof(char), reinterpret_cast< Arxx::Buffer::const_pointer >(&cValue));
	
	return BufferWriter;
}

auto Arxx::operator<<(Arxx::BufferWriter & BufferWriter, std::pair< Arxx::Buffer::size_type, std::istream * > const & Stream) -> Arxx::BufferWriter &
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
