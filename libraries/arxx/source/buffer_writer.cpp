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

#include <arxx/buffer.h>
#include <arxx/buffer_writer.h>

#ifdef DEBUG
#include <iostream>
#endif

Arxx::BufferWriter::BufferWriter(Arxx::Buffer & Buffer, Arxx::Buffer::size_type Position) :
	m_Buffer(Buffer),
	m_Marker(Buffer, Position, Arxx::Buffer::Marker::LEFT)
{
}

Arxx::BufferWriter::BufferWriter(Arxx::BufferWriter const & Other) :
	m_Buffer(Other.m_Buffer),
	m_Marker(Other.m_Buffer, Other.GetPosition(), Arxx::Buffer::Marker::LEFT)
{
}

auto Arxx::BufferWriter::Write(Arxx::Buffer::size_type DataLength, Arxx::Buffer::const_pointer Data) -> void
{
	if(m_Marker.IsValid() == false)
	{
		throw std::runtime_error("Arxx::BufferWriter::vWrite: Buffer was destroyed.");
	}
	m_Marker.SetAlignment(Arxx::Buffer::Marker::RIGHT);
	m_Buffer.Insert(m_Marker.GetPosition(), DataLength, Data);
	m_Marker.SetAlignment(Arxx::Buffer::Marker::LEFT);
}

auto Arxx::BufferWriter::GetBuffer() -> Arxx::Buffer &
{
	return m_Buffer;
}

auto Arxx::BufferWriter::GetBuffer() const -> Arxx::Buffer const &
{
	return m_Buffer;
}

auto Arxx::BufferWriter::GetPosition() const -> Arxx::Buffer::size_type
{
	return m_Marker.GetPosition();
}

auto Arxx::BufferWriter::SetPosition(Arxx::Buffer::size_type Position) -> void
{
	return m_Marker.SetPosition(Position);
}

auto Arxx::operator<<(Arxx::BufferWriter & BufferWriter, std::string const & String) -> Arxx::BufferWriter &
{
	BufferWriter.Write(String.length() + 1, reinterpret_cast<Arxx::Buffer::const_pointer>(String.c_str()));

	return BufferWriter;
}

auto Arxx::operator<<(Arxx::BufferWriter & BufferWriter, char const * String) -> Arxx::BufferWriter &
{
	BufferWriter.Write(strlen(String) + 1, reinterpret_cast<Arxx::Buffer::const_pointer>(String));

	return BufferWriter;
}

auto Arxx::operator<<(Arxx::BufferWriter & BufferWriter, float Value) -> Arxx::BufferWriter &
{
	BufferWriter.Write(sizeof(float), reinterpret_cast<Arxx::Buffer::const_pointer>(&Value));
	
	return BufferWriter;
}

auto Arxx::operator<<(Arxx::BufferWriter & BufferWriter, Arxx::u1byte Value) -> Arxx::BufferWriter &
{
	BufferWriter.Write(sizeof(Arxx::u1byte), reinterpret_cast<Arxx::Buffer::const_pointer>(&Value));
	
	return BufferWriter;
}

auto Arxx::operator<<(Arxx::BufferWriter & BufferWriter, Arxx::u4byte Value) -> Arxx::BufferWriter &
{
	BufferWriter.Write(sizeof(Arxx::u4byte), reinterpret_cast<Arxx::Buffer::const_pointer>(&Value));
	
	return BufferWriter;
}

auto Arxx::operator<<(Arxx::BufferWriter & BufferWriter, Arxx::u8byte Value) -> Arxx::BufferWriter &
{
	BufferWriter.Write(sizeof(Arxx::u8byte), reinterpret_cast<Arxx::Buffer::const_pointer>(&Value));
	
	return BufferWriter;
}

auto Arxx::operator<<(Arxx::BufferWriter & BufferWriter, bool Value) -> Arxx::BufferWriter &
{
	BufferWriter.Write(sizeof(bool), reinterpret_cast<Arxx::Buffer::const_pointer>(&Value));
	
	return BufferWriter;
}

auto Arxx::operator<<(Arxx::BufferWriter & BufferWriter, char Value) -> Arxx::BufferWriter &
{
	BufferWriter.Write(sizeof(char), reinterpret_cast<Arxx::Buffer::const_pointer>(&Value));
	
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
			BufferWriter.Write(1, reinterpret_cast<Arxx::Buffer::const_pointer>(&Character));
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
