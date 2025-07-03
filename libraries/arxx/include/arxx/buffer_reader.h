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

#ifndef GALACTIC_FALL__LIBRARIES__ARXX__INCLUDE__BUFFERREADER_H
#define GALACTIC_FALL__LIBRARIES__ARXX__INCLUDE__BUFFERREADER_H

#include <arxx/buffer.h>

/**
 * @file BufferReader.h
 * 
 * This file declares the interface for the Arxx::BufferReader class.
 **/

namespace Arxx
{
	/**
	 * @brief Implements a convenient output interface for Arxx::Buffer.
	 **/
	class BufferReader
	{
	public:
		BufferReader(Arxx::Buffer const & Buffer, Arxx::Buffer::size_type Position = Arxx::Buffer::Marker::BEGIN);
		
		/**
		 * @brief A copy constructor for the BufferReader.
		 * 
		 * This constructor creates a second BufferReader which refers to the same Buffer but using its own marker at the same location. The new BufferReader behaves as if created on its own.
		 **/
		BufferReader(Arxx::BufferReader const & Other);
		
		/**
		 * @brief This functions allows to get more than one byte at a time from the buffer.
		 * @param stLength The length of the chunk of data you wish to read from the buffer.
		 * @param Buffer Not a Buffer but a buffer, an address of a block of memory.
		 *
		 * This function reads stLength bytes from @em this Buffer starting at the I/O position and writes them to the memory specified by Buffer.
		 *
		 * This function does not give you access to the content of the buffer but creates of copy of it.
		 *
		 * @note This function refuses to read more elements than there are from the I/O position to the end. It will throw a std::out_of_range exception in this case.
		 **/
		auto Read(Arxx::Buffer::size_type Length, Arxx::Buffer::pointer Buffer) -> Arxx::Buffer::size_type;
		
		/**
		 * @brief Returns the associated Arxx::Buffer object.
		 **/
		auto GetBuffer() const -> Arxx::Buffer const &;
		
		/**
		 * @brief Returns the marker's position.
		 **/
		auto GetPosition() const -> Arxx::Buffer::size_type;
		
		/**
		 * @brief Sets the marker's position.
		 **/
		auto SetPosition(Arxx::Buffer::size_type Position) -> void;
        
	private:
		Arxx::Buffer const & m_Buffer;
		Arxx::Buffer::Marker m_Marker;
        
	};
	
	/**
	 * @brief A helper function for reading a string from a buffer.
	 * @param BufferReader The BufferReader from which the string will be read.
	 * @param String A std::string which will be filled with the string from the buffer.
	 *
	 * This function reads a string from the @a BufferReader. The string may be either null-terminated or end at the end of the buffer.
	 *
	 * @note This function will read until it finds either the first null character in the buffer or the end of the buffer so that the string itself cannot contain a null character.
	 **/
	auto operator>>(Arxx::BufferReader & BufferReader, std::string & String) -> Arxx::BufferReader &;
	
	/**
	 * @brief A helper function for reading float values from a buffer.
	 * @param BufferReader The BufferReader from which the float value will be read.
	 * @param Value The target for the float value that is to be read.
	 * @return The Buffer after the output execution.
	 * 
	 * This function reads a float value from the buffer storing it in @a Value .
	 **/
	auto operator>>(Arxx::BufferReader & BufferReader, float & Value) -> Arxx::BufferReader &;
	
	/**
	 * @brief A helper function for reading u1byte values from a buffer.
	 * @param BufferReader The BufferReader from which the u1byte value will be read.
	 * @param Value The target for the u1byte value that is to be read.
	 * @return The Buffer after the output execution.
	 * 
	 * This function reads a u1byte value from the buffer storing it in @a Value .
	 **/
	auto operator>>(Arxx::BufferReader & BufferReader, Arxx::u1byte & Value) -> Arxx::BufferReader &;
	
	/**
	 * @brief A helper function for reading u4byte values from a buffer.
	 * @param BufferReader The BufferReader from which the u4byte value will be read.
	 * @param Value The target for the u4byte value that is to be read.
	 * @return The Buffer after the output execution.
	 * 
	 * This function reads a u4byte value from the buffer storing it in @a Value .
	 **/
	auto operator>>(Arxx::BufferReader & BufferReader, Arxx::u4byte & Value) -> Arxx::BufferReader &;
	
	/**
	 * @brief A helper function for reading Arxx::u8byte values from a buffer.
	 * @param BufferReader The BufferReader from which the Arxx::u8byte value will be read.
	 * @param Value The target for the Arxx::u8byte value that is to be read.
	 * @return The Buffer after the output execution.
	 * 
	 * This function reads a Arxx::u8byte value from the buffer storing it in @a Value .
	 **/
	auto operator>>(Arxx::BufferReader & BufferReader, Arxx::u8byte & Value) -> Arxx::BufferReader &;
	
	/**
	 * @brief A helper function for reading bool objects from a buffer.
	 * @param BufferReader The BufferReader from which the bool will be read.
	 * @param Value The target for the bool value that is to be read.
	 * @return The Buffer after the output execution.
	 * 
	 * This function reads the bool value @a Value from the buffer using an output function.
	 **/
	auto operator>>(Arxx::BufferReader & BufferReader, bool & Value) -> Arxx::BufferReader &;
}

#endif
