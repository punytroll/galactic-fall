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

#include <arxx/buffer.h>
#include <arxx/buffer_reader.h>

Arxx::BufferReader::BufferReader(Arxx::Buffer const & Buffer, Arxx::Buffer::size_type Position) :
	m_Buffer(Buffer),
	m_Marker(Buffer, Position, Arxx::Buffer::Marker::LEFT)
{
}

Arxx::BufferReader::BufferReader(Arxx::BufferReader const & Other) :
	m_Buffer(Other.m_Buffer),
	m_Marker(Other.m_Buffer, Other.GetPosition(), Arxx::Buffer::Marker::LEFT)
{
}

auto Arxx::BufferReader::Read(Arxx::Buffer::size_type Length, Arxx::Buffer::pointer Buffer) -> Arxx::Buffer::size_type
{
	if(m_Marker.IsValid() == false)
	{
		throw std::runtime_error("Arxx::BufferWriter::Write: Buffer was destroyed.");
	}
	if(m_Marker.GetPosition() + Length > m_Buffer.GetLength())
	{
		throw std::out_of_range("Trying to read after the end.");
	}
	std::copy(m_Buffer.GetBegin() + m_Marker.GetPosition(), m_Buffer.GetBegin() + m_Marker.GetPosition() + Length, Buffer);
	m_Marker.SetPosition(m_Marker.GetPosition() + Length);
	
	return Length;
}

auto Arxx::BufferReader::GetBuffer() const -> Arxx::Buffer const &
{
	return m_Buffer;
}

auto Arxx::BufferReader::GetPosition() const -> Arxx::Buffer::size_type
{
	return m_Marker.GetPosition();
}

auto Arxx::BufferReader::SetPosition(Arxx::Buffer::size_type Position) -> void
{
	return m_Marker.SetPosition(Position);
}

auto Arxx::operator>>(Arxx::BufferReader & BufferReader, std::string & String) -> Arxx::BufferReader &
{
	Arxx::Buffer const & Buffer(BufferReader.GetBuffer());
	Arxx::Buffer::size_type Index(BufferReader.GetPosition());
	Arxx::Buffer::size_type Length(Buffer.GetLength());
	char Char(0);

	while(Length > Index)
	{
		if((Char = Buffer[Index++]) == '\0')
		{
			break;
		}
		String += Char;
	}
	BufferReader.SetPosition(BufferReader.GetPosition() + String.length() + 1);

	return BufferReader;
}

auto Arxx::operator>>(Arxx::BufferReader & BufferReader, float & Value) -> Arxx::BufferReader &
{
	BufferReader.Read(sizeof(float), reinterpret_cast<Arxx::Buffer::pointer>(&Value));
	
	return BufferReader;
}

auto Arxx::operator>>(Arxx::BufferReader & BufferReader, Arxx::u1byte & Value) -> Arxx::BufferReader &
{
	BufferReader.Read(sizeof(Arxx::u1byte), reinterpret_cast<Arxx::Buffer::pointer>(&Value));
	
	return BufferReader;
}

auto Arxx::operator>>(Arxx::BufferReader & BufferReader, Arxx::u4byte & Value) -> Arxx::BufferReader &
{
	BufferReader.Read(sizeof(Arxx::u4byte), reinterpret_cast<Arxx::Buffer::pointer>(&Value));
	
	return BufferReader;
}

auto Arxx::operator>>(Arxx::BufferReader & BufferReader, Arxx::u8byte & Value) -> Arxx::BufferReader &
{
	BufferReader.Read(sizeof(Arxx::u8byte), reinterpret_cast<Arxx::Buffer::pointer>(&Value));
	
	return BufferReader;
}

auto Arxx::operator>>(Arxx::BufferReader & BufferReader, bool & Value) -> Arxx::BufferReader &
{
	BufferReader.Read(sizeof(bool), reinterpret_cast<Arxx::Buffer::pointer>(&Value));
	
	return BufferReader;
}
