/**
 * libarxx - Advanced Resource files in C++
 * Copyright (C) 2005  Hagen Möbius
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

#ifndef ARXX_BUFFERWRITER_H
#define ARXX_BUFFERWRITER_H

#include "Buffer.h"

/**
 * @file BufferWriter.h
 * 
 * This file declares the interface for the Arxx::BufferWriter class.
 **/

namespace Arxx
{
	/**
	 * @brief Implements a convenient input interface for Arxx::Buffer.
	 * 
	 * The BufferWriter is implemented using a Arxx::Buffer::Marker to indicate the position at which the data is to be inserted into the buffer.
	 * 
	 * @note This also means that a BufferWriter is not referring to an index into the buffer but to a position in the data inside the buffer. Whenever changes occure to the data the input position will change in the same way that a Arxx::Buffer::Marker would. The default alignment of the Marker representing the insert position is LEFT, so that data changes at the insert position of the BufferWriter will not affect the marker. However if you insert data using the BufferWriter, the Alignment of the marker is set to RIGHT for the time of the insert thus moving the insert position behind the inserted data.
	 **/
	class BufferWriter
	{
	public:
		/**
		 * @brief The BufferWriter constructor.
		 * @note Sets the Buffers IO position to the buffer's length.
		 **/
		BufferWriter(Arxx::Buffer & Buffer, Arxx::Buffer::size_type stPosition = Arxx::Buffer::Marker::END);
		
		/**
		 * @brief A copy constructor for the BufferWriter.
		 * 
		 * This constructor creates a second BufferWriter which refers to the same Buffer but using an own marker. The new BufferWriter behaves as if created on its own.
		 **/
		BufferWriter(const Arxx::BufferWriter & BufferWriter);
		
		/**
		 * @brief Write a block of data at the writer's position in the buffer.
		 * @param stDataLength The length of the data block @a Data
		 * @param Data The data block to insert at the writer's position. May be omitted to insert @a stDataLength zeroed bytes.
		 **/
		void vWrite(Arxx::Buffer::size_type stDataLength, Arxx::Buffer::const_pointer Data = 0);
		
		/**
		 * @brief Returns the associated Arxx::Buffer object.
		 * @return A non-const Arxx::Buffer reference.
		 **/
		Arxx::Buffer & GetBuffer(void);
		
		/**
		 * @brief Returns the associated Arxx::Buffer object.
		 * @return A const Arxx::Buffer reference.
		 **/
		const Arxx::Buffer & GetBuffer(void) const;
		
		/**
		 * @brief Returns the marker's position.
		 **/
		Arxx::Buffer::size_type stGetPosition(void) const;
		
		/**
		 * @brief Sets the marker's position.
		 **/
		void vSetPosition(Arxx::Buffer::size_type stPosition);
	private:
		Arxx::Buffer & m_Buffer;
		Arxx::Buffer::Marker m_Marker;
	};

	/**
	 * @brief A helper function for storing a std::string instance in a buffer.
	 * @param BufferWriter The buffer in which the string will be stored.
	 * @param sString The string which is about to be stored.
	 * @return The BufferWriter after the input execution.
	 *
	 * This helper function stores the std::string @a sString in the buffer @a Buffer. It uses the Buffers::Buffer::vInput function so that the buffer current I/O position will be used as the insertion position. Therefore also the overwrite state of the buffer will be adhered. Afterwards the I/O position will be set to the end of the insertion by Buffer::Buffer::vInput().
	 *
	 * @note The string will be stored as a zero-terminated string so that a trailing zero will be appended.
	 **/
	Arxx::BufferWriter & operator<<(Arxx::BufferWriter & BufferWriter, const std::string & sString);

	/**
	 * @brief A helper function for storing a C string in a buffer.
	 * @param BufferWriter The buffer in which the C string will be stored.
	 * @param pcString The C string which is about to be stored.
	 * @return The BufferWriter after the input execution.
	 *
	 * This helper function stores the C string @a pcString in the buffer @a Buffer.
	 * 
	 * The function uses strlen(3) to determine the length of the string.
	 *
	 * @note The string will be stored as a zero-terminated string so that a trailing zero will be appended.
	 **/
	Arxx::BufferWriter & operator<<(Arxx::BufferWriter & BufferWriter, const char * pcString);
	
	/**
	 * @brief A helper function for storing float numbers in a buffer.
	 * @param BufferWriter The buffer in which the float number will be stored.
	 * @param fValue The float value which is to be stored.
	 * @return The BufferWriter after the input execution.
	 * 
	 * This function stores the float value @a fValue in the buffer using an input function.
	 **/
	Arxx::BufferWriter & operator<<(Arxx::BufferWriter & BufferWriter, const float & fValue);
	
	/**
	 * @brief A helper function for storing Arxx::u1byte objects in a buffer.
	 * @param BufferWriter The buffer in which the Arxx::u1byte will be stored.
	 * @param u4Value The Arxx::u1byte value which is to be stored.
	 * @return The BufferWriter after the input execution.
	 * 
	 * This function stores the Arxx::u1byte value @a Value in the buffer using an input function.
	 **/
	Arxx::BufferWriter & operator<<(Arxx::BufferWriter & BufferWriter, const Arxx::u1byte & Value);
	
	/**
	 * @brief A helper function for storing Arxx::u4byte objects in a buffer.
	 * @param BufferWriter The buffer in which the Arxx::u4byte will be stored.
	 * @param u4Value The Arxx::u4byte value which is to be stored.
	 * @return The BufferWriter after the input execution.
	 * 
	 * This function stores the Arxx::u4byte value @a u4Value in the buffer using an input function.
	 **/
	Arxx::BufferWriter & operator<<(Arxx::BufferWriter & BufferWriter, const Arxx::u4byte & u4Value);
	
	/**
	 * @brief A helper function for storing Arxx::u8byte objects in a buffer.
	 * @param BufferWriter The buffer in which the Arxx::u8byte will be stored.
	 * @param u8Value The Arxx::u8byte value which is to be stored.
	 * @return The BufferWriter after the input execution.
	 * 
	 * This function stores the Arxx::u8byte value @a u8Value in the buffer using an input function.
	 **/
	Arxx::BufferWriter & operator<<(Arxx::BufferWriter & BufferWriter, const Arxx::u8byte & u8Value);
	
	/**
	 * @brief A helper function for storing bool objects in a buffer.
	 * @param BufferWriter The buffer in which the bool will be stored.
	 * @param bValue The bool value which is to be stored.
	 * @return The BufferWriter after the input execution.
	 * 
	 * This function stores the bool value @a bValue in the buffer using an input function.
	 **/
	Arxx::BufferWriter & operator<<(Arxx::BufferWriter & BufferWriter, const bool & bValue);
	
	/**
	 * @brief A helper function for storing char objects in a buffer.
	 * @param BufferWriter The buffer in which the bool will be stored.
	 * @param cValue The char value which is to be stored.
	 * @return The BufferWriter after the input execution.
	 * 
	 * This function stores the char value @a cValue in the buffer using an input function.
	 **/
	Arxx::BufferWriter & operator<<(Arxx::BufferWriter & BufferWriter, const char & cValue);
	
	/**
	 * @brief A helper function that allows filling a buffer from an std::istream.
	 * @param BufferWriter The buffer which will be filled from the content of the istream.
	 * @param Stream A std::pair which's first component describes the amount of bytes to be read from the stream specified in its second component.
	 * @return The BufferWriter after the input execution.
	 * 
	 * This function will append the stream content at the I/O position of the buffer. It will read @a Stream.first bytes from the stream. In case the stream end before that amount of bytes the buffer will contain only as much bytes as could be read from the stream. This function is an I/O operation and thus will modify the I/O position of the buffer to point after the last byte read.
	 **/
	Arxx::BufferWriter & operator<<(Arxx::BufferWriter & BufferWriter, const std::pair< Arxx::Buffer::size_type, std::istream * > & Stream);
}

#endif
