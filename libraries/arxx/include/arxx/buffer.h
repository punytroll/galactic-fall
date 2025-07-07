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

#ifndef GALACTIC_FALL__LIBRARIES__ARXX__INCLUDE__BUFFER_H
#define GALACTIC_FALL__LIBRARIES__ARXX__INCLUDE__BUFFER_H

#include <set>
#include <stdexcept>
#include <string>
#include <vector>

#include <arxx/common.h>

/**
 * @file Buffer.h
 * 
 * Declares the interface of Arxx::Buffer.
 **/

namespace Arxx
{
	/**
	 * @brief The core class of the Buffer.
	 *
	 * This class provides the concept of nested buffers. That means: you create one main buffer and it will behave like any other ordinary buffer you know. It is autoresizing as you add data, it provides prepend and append function, it has a position pointer which can be placed anywhere in the buffer to allow appending to that particular position.
	 *
	 * Additionally to that the nested buffers allow you to create compositions of buffers all acting on the same data block.
	 *
	 * Imagine a buffer of the size 16 bytes (name: Superior). Although this data may be handled as one entity, you, as the user of the Buffers library, know the semantics of this buffer: It may actually be three entities of data:
	 * - a 7-byte long block
	 * - a 5-byte long block
	 * - a 4-byte long block
	 * 
	 * Suppose now, that there is a library which (in any way) allows handling of a certain kind of data blocks. In our example it may be the first block of length 7. What you have to do is pass the buffer to the library, but this will violate the information hiding principle since this library now has access to the other datablock as well, maybe not even realizing that it isn't meant to meddle with them.
	 *
	 * Here comes the Buffers library which allows you to create sub buffers of our Superior buffer. By creating a buffer via Buffer(Superior, 0, 7)  you get a sub buffer with length 7 and I/O position at 0 which you may pass to the library. The content of this sub buffer is a reference to the superior buffer and thus contains exactly what is in the superior buffer from 0 to 7 at any given time. Now the library is only able to access the content it is meant to see.
	 *
	 * But this concept strives for more functionality. So it is also possible to perform any operations like appending, inserting and deleting. Using these operations will affect the content of the superior buffer but the effect will be limited to the region of the sub buffer. Any increases of the size of the sub buffer will also expand the superior buffer by the same amount.
	 * 
	 * The concept of buffers doesn't end at the first level of depth. They allow you to nest sub buffers in sub buffers, thus creating a buffer structure of arbitrary depth and complexity.
	 * 
	 * For that reason all operations that work on the buffer have to be implemented recursively. If you change a portion of data in a sub buffer, that change has to be propagated to the most superior buffer, which actually owns the associated memory. Only this sup buffer is allowed to change the content of the memory and in turn it has to ensure that the effect of any such change will be passed down to the leaf childs.
	 **/
	class Buffer
	{
	public:
		friend std::string Indentation(Arxx::Buffer * Buffer);
        
		/**
		 * @brief The type of an element stored in this buffer.
		 *
		 * Use Buffers::Buffer::value_type to work with the elements to be safe from API changes.
		 **/
		typedef unsigned char value_type;
		
		/**
		 * @brief The type of a reference to an element stored in this buffer.
		 *
		 * Use Buffers::Buffer::reference to work with the elements to be safe from API changes.
		 **/
		typedef value_type & reference;
		
		/**
		 * @brief The type of a pointer to elements stored in this buffer.
		 *
		 * Use Buffers::Buffer::pointer to work with arrays of elements to be safe from API changes.
		 **/
		typedef value_type * pointer;
		
		/**
		 * @brief The type of a const element stored in this buffer.
		 *
		 * Use Buffers::Buffer::const_value_type to work with const elements in the buffer to be safe from API changes.
		 **/
		typedef const value_type const_value_type;
		
		/**
		 * @brief The type of a const pointer to elements stored in this buffer.
		 *
		 * Use Buffers::Buffer::const_pointer to work with const arrays of elements in the buffer to be safe from API changes.
		 **/
		typedef const_value_type * const_pointer;
		
		/**
		 * @brief The type of positions, lengths and sizes in this buffer.
		 *
		 * Use Buffers::Buffer::size_type when working with positions, sizes and length in a buffer to be safe from API changes.
		 **/
		typedef Arxx::u4byte size_type;
		
		/**
		 * @brief A marked position in a buffer, changing with content changes.
		 **/
		class Marker
		{
		public:
			typedef enum
			{
				LEFT,
				RIGHT
			} Alignment;
			
			typedef enum
			{
				BEGIN = 0,
				END = 0xFFFFFFFF
			} Position;
			
			/**
			 * @brief The Marker's constructor.
			 * 
			 * The constructor will call Arxx::Buffer::vRegister(this) to register the marker at the associated Arxx::Buffer.
			 * 
			 * The position is truncated at Buffer.stGetLength() if greater than that.
			 **/
			Marker(const Arxx::Buffer & Buffer, Arxx::Buffer::size_type Position = 0, Arxx::Buffer::Marker::Alignment Alignment = LEFT);
			
			/**
			 * @brief The Marker's destructor.
			 * 
			 * The destructor will call Arxx::Buffer::vUnregister(this) to unregister the marker at the associated Arxx::Buffer.
			 **/
			~Marker();
			
			/**
			 * @brief Returns the marker's current position inside the buffer.
			 * 
			 * The position is guaranteed to be valid inside the buffer.
			 **/
			auto GetPosition() const -> Arxx::Buffer::size_type;
			
			/**
			 * @brief Sets the marker's position in the buffer.
			 * 
			 * Will not complain about the position being behind the buffer's end but will adjust to the length.
			 **/
			auto SetPosition(Arxx::Buffer::size_type Position) -> void;
			
			/**
			 * @brief Returns the marker's alignment.
			 * 
			 * @sa Arxx::Buffer::Marker::m_Alignment
			 **/
			auto GetAlignment() const -> Arxx::Buffer::Marker::Alignment;
			
			/**
			 * @brief Sets the marker's alignment.
			 * 
			 * @sa Arxx::Buffer::Marker::m_Alignment
			 **/
			auto SetAlignment(Arxx::Buffer::Marker::Alignment Alignment) -> void;
			
			/**
			 * @brief Returns whether the marker is valid.
			 * 
			 * Currently the only occasion when a marker might be invalid is that the associated Arxx::Buffer got destroyed.
			 **/
			auto IsValid() const -> bool;
            
		private:
			/**
			 * @brief Invalidates the marker's associated buffer.
			 * 
			 * This function sets m_Buffer to `nullptr`.
			 * 
			 * Calling this function invalidates the marker.
			 **/
			auto m_InvalidateBuffer() -> void;
            
			/**
			 * @brief A pointer to the associated Arxx::Buffer.
			 * 
			 * Is initialized by the constructor and might be set to `nullptr` by Arxx::Buffer::Marker::InvalidateBuffer().
			 **/
			Arxx::Buffer const * m_Buffer;
			
			/**
			 * @brief The marker's current position inside the associated Arxx::Buffer.
			 **/
			Arxx::Buffer::size_type m_Position;
			
			/**
			 * @brief The marker's alignment.
			 * 
			 * The alignment of a marker can be LEFT or RIGHT and indicates where the marker moves if data is inserted at the marker's position.
			 * - LEFT alignment means, that the marker will be at the beginning of the inserted data.
			 * - RIGHT alignments means, that the marker will stay where it is and the data is inserted after it.
			 * 
			 * The Alignment property is only of relevance if data is inserted directly at the marker's position.
			 **/
			Arxx::Buffer::Marker::Alignment m_Alignment;
                
		};
		
		friend class Arxx::Buffer::Marker;
		
		/**
		 * @brief The standard constructor.
		 * 
		 * Constructs a buffer of size 0, I/O position is 0, overwrite state is false.
		 **/
		Buffer() = default;
		
		/**
		 * @brief The sub buffer constructor.
		 * @param Buffer The superior buffer.
		 * @param stPosition The sub buffer's begining position in the superior buffer.
		 * @param stLength The length that should be covered inside the superior buffer starting from @a u4Position.
		 *
		 * Use this constructor if you want to have a sub buffer of the superior buffer @a Buffer. It may be of length 0 to get an insertion spot at a later time.
		 **/
		Buffer(Buffer & Buffer, Arxx::Buffer::size_type Position, Arxx::Buffer::size_type Length);
        
		/**
		 * @brief Deleted copy constructor, to prevent copying a buffer.
		 **/
		Buffer(Arxx::Buffer const & Other) = delete;
		
		/**
		 * @brief The destructor of a buffer. It is virtual since the Buffer may be overloaded to support external data.
		 * 
		 * The destructor of a buffer deletes it's content if it is the most superior buffer. Otherwise it will unregister itself from its superior buffer.
		 *
		 * @note The destructor of a buffer will not touch its sub buffers (e.g. to delete them). Currently it will not even notify them that their reference buffers are dead. That will be changed later.
		 **/
		virtual ~Buffer();
		
		/**
		 * @brief Deleted copy assignment operator, to prevent assignment of a buffer.
		 **/
		auto operator=(Arxx::Buffer const & Buffer) -> Arxx::Buffer & = delete;
		
		/**
		 * @brief Returns the length of the buffer.
		 * 
		 * This length is the length of the data inside the buffer, so the number of bytes actually used by the data.
		 **/
		auto GetLength() const -> Arxx::Buffer::size_type;
		
		/**
		 * @brief Sets the length of the data actually in the buffer.
		 * @param stLength The new length of the data.
		 *
		 * This is a convenience function as explained below:
		 * 
		 * - If the specified length is greater than the current length the buffer will be expanded to this length. The call is then equivalent to:
		 * @code vInsert(stGetLength(), stLength - stGetLength()); @endcode
		 *
		 * - If the specified length is smaller than the current length the buffer will be shrunken to this length. This will happen back to front, so all child buffers which reside anywhere between stLength and stGetLength() will be modified as well. This call is then equivalent to:
		 * @code vDelete(stLength, stGetLength() - stLength); @endcode
		 **/
		auto SetLength(Arxx::Buffer::size_type Length = 0) -> void;
		
		/**
		 * @brief Insert a given amount of data at a specified postion.
		 * @param stPosition The position inside the buffer.
		 * @param stDataLength The length of the data block identified by @a Data.
		 * @param Data The block of data to be inserted in the buffer. This parameter may be omitted which will result in a null-pointer, indicating that the inserted block of data will be filled with Zeros.
		 *
		 * The data which is defined by @a Data will be inserted at position @a stPosition. The data is taken to be of length @a stDataLength at minimum. The buffer will be expanded to contain stGetLength() + @a stDataLength bytes.
		 *
		 * @note This function directly calls the private function vWrite(), so this function throws the same exceptions as vWrite().
		 **/
		auto Insert(Arxx::Buffer::size_type Position, Arxx::Buffer::size_type DataLength, Arxx::Buffer::const_pointer Data = nullptr) -> void;
		
		/**
		 * @brief Deletes a given amount of data at a specified position.
		 * @param stPosition The position inside the buffer.
		 * @param stLength The length of the block that should be deleted.
		 * 
		 * This function deletes stLength elements at position stPosition.
		 **/
		auto Delete(Arxx::Buffer::size_type Position, Arxx::Buffer::size_type Length) -> void;
		
		/**
		 * @brief Allows read-only access to indexed data members for const buffers.
		 * @param stIndex The index of the data member to access. Begining with zero.
		 *
		 * Returns a copy of the data member with index @a stIndex. This function will throw a std::out_of_range error if stIndex is beyond stGetLength().
		 *
		 * @note Since this function addresses data in the buffer it will call vAssureDataPresence().
		 **/
		auto operator[](Arxx::Buffer::size_type Index) const -> Arxx::Buffer::value_type;
		
		/**
		 * @brief Allows read/write access to indexed data members.
		 * @param stIndex The index of the data member to access. Begining with zero.
		 *
		 * Returns a refernece to the data member with index @a stIndex. This function will throw a std::out_of_range error if stIndex is beyond stGetLength().
		 *
		 * @note Since this function addresses data in the buffer it will call vAssureDataPresence().
		 **/
		auto operator[](Arxx::Buffer::size_type Index) -> Arxx::Buffer::reference;
		
		/**
		 * @brief Retreives a const pointer to the data of this buffer.
		 * 
		 * There is no public way to access the content of the buffer directly. m_Begin is private so only the Buffers library may access the content directly for read/write access. To fill the gap between no public access and full private access lies the read-only access for derivatives.
		 **/
		auto GetBegin() const -> Arxx::Buffer::const_pointer;
        
	protected:
		/**
		 * @brief A constant. Its value will be passed to ParentDataChanged() whenever data is to be updated.
		 * @sa ParentDataChanged() for further details.
		 **/
		static const Arxx::Buffer::size_type m_DataUpdated;
		
		/**
		 * @brief A constant. Its value will be passed to ParentDataChanged() whenever data was deleted.
		 * @sa ParentDataChanged() for further details.
		 **/
		static const Arxx::Buffer::size_type m_DataDeleted;
		
		/**
		 * @brief A constant. Its value will be passed to ParentDataChanged() whenever data was inserted.
		 * @sa ParentDataChanged() for further details.
		 **/
		static const Arxx::Buffer::size_type m_DataInserted;
        
	private:
		class SubBuffer;
		
		/**
		 * @brief This function registers a buffer to be a sub buffer of @em this buffer.
		 *
		 * This function handles the necessary steps to organise the control structure which contains all the sub buffers. This structure is needed to propagate changes.
		 **/
		auto Register(Arxx::Buffer & SubBuffer) -> void;
		
		/**
		 * @brief This function removes a buffer from the list of all sub buffer of @em this buffer.
		 *
		 * This function handles the necessary steps to organise the control structure which contains all the sub buffers. This structure is needed to propagate changes.
		 **/
		auto Unregister(Arxx::Buffer & SubBuffer) -> void;
		
		auto Register(Arxx::Buffer::Marker & Marker) const -> void;
		
		auto Unregister(Arxx::Buffer::Marker & Marker) const -> void;
		
		/**
		 * @brief This function is responsible for any functionality that writes data to the buffer.
		 * @param Buffer The buffer that makes the write request.
		 * @param stPosition The position (inside this buffer) where the data is to be stored.
		 * @param stDataLength The length of the data block @a Data.
		 * @param Data The data block to be written into the buffer.
		 * 
		 * This function is one of the two core functions of the buffer. It allows to insert any data in any buffer and will pipe the call to the most superior buffer which actually copies the data. It will then decide which sub buffers should be notified that content or position has changed.
		 **/
		auto Write(Arxx::Buffer & Buffer, Arxx::Buffer::size_type Position, Arxx::Buffer::size_type DataLength, Arxx::Buffer::const_pointer Data = nullptr) -> void;
		
		/**
		 * @brief This function is called whenever the data of the sup buffer is changed in a way that also affects this buffer.
		 * @param stChangeMode Categorizes the type of change that affects @em this buffer.
		 * @param stPosition Indicates the location of the change inside @em this buffer.
		 * @param stLength Specifies the length of the change inside @em this buffer.
		 *
		 * There are three kinds of changes that may occure:
		 * - Updates
		 * - Deletes
		 * - Inserts
		 *
		 * Additionally there are two kinds of actions that need to be taken:
		 * - changes to @em this buffer (local changes)
		 * - changes to @em this buffer's sub buffers (structural changes).
		 *
		 * Updates are those changes that neither affect the length of @em this buffer nor its location inside the superior buffer. For that reason the parameters @a stPosition and @a stLength are not considered. The most probable reason why this change is issued is that the most superior buffer had to reallocate the memory for the buffer, because somewhere else so much data was inserted in the buffer that the capacity had to be enlarged.
		 * - The only local action taken here is to reset the m_Begin pointer to the correct location inside the sup buffer (m_pSupBuffer->m_Begin + m_stPosition).
		 * - The structural changes are equally simple: notify all sub buffers via vParentDataChanged(m_stDataUpdated, ?, ?). Position and length are not important here.
		 *
		 * Inserts are issued whenever the superior buffer inserts data. The @a stPosition indicates the location of the insert inside the sup buffer, so it is comparable with m_stPosition without further calculation. The @a stLength is the length of the portion of inserted data.
		 * @note Notice that this function is called even if the insert was not inside @em this buffer. If the Insertion was @em before this buffer (stPosition < m_stPosition) we need to reset m_stPosition. (This SHOULD be changed.)
		 * 
		 * - The local changes depend on the position of the insertion. If the data was inserted before this buffer we need to reset m_stPosition and m_Begin. If the data was inserted inside the buffer we need to reset m_stLength and possibly m_stIOPosition if (stPosition - m_stPosition) <= m_stIOPosition.
		 * - The structural changes are easy enough: just pass the change to any sub buffer by calling vParentDataChanged(m_stDataInserted, stPosition - m_stPosition, stLength). There is one exception to this: If @em this buffer made the initial change and the change request was passed up to the most superior buffer and now the changes are passed down again we don't apply changes here. Once all the changes are done and we return to the initial insertion function it is done then.
		 *
		 * Deletes are emitted whenever the superior buffer has deleted a portion of its data. The @a stPosition inticates where the beginning of the deletion block inside the parent is. Because of its scope, stPosition is directly comparable with m_stPosition. The @a stLength defines the length of the deletion.
		 * - The local changes is defined by the position and length of the deletion. There are six possibilities:
		 *   - The deletion is completely before @em this buffer: only m_stPosition needs to be decremented by stLength.
		 *   - The deletion begins before @em this buffer and overlaps the beginning: m_stPosition needs to be set to stPosition, m_stLength needs to be reduced by the overlapping.
		 *   - The deletion is completely inside @em this buffer: m_stLength needs to be reduced by stLength.
		 *   - The deletion begins inside @em this buffer and overlaps the end: m_stLength needs to be reduced by the size of the overlapping.
		 *   - The deletion is completely behind @em this buffer: nothing to be done.
		 *   - The deletion overlaps @em this buffer completely: m_Length is set to 0 and m_Position is set to Position.
		 * - The structural changes are very much the same as for Inserts. We just need to pass this change down to all sub buffers by calling vParentDataChanged(m_stDataDeleted, stPosition - m_stPosition, stLength).
		 **/
		auto ParentDataChanged(size_type ChangeMode, Arxx::Buffer::size_type Position, Arxx::Buffer::size_type Length) -> void;
    
		/**
		 * @brief A pointer pointing to the superior buffer.
		 *
		 * This pointer points to the Buffer that this buffer is a part of. In case this buffer is the most superior buffer it is 'nullptr'.
		 **/
		Arxx::Buffer * m_SupBuffer{nullptr};
		
		/**
		 * @brief The pointer to the actual content of @em this buffer.
		 **/
		Arxx::Buffer::pointer m_Begin{nullptr};
		
		/**
		 * @brief The length of the content within @em this buffer.
		 *
		 * @note There is a separate property m_Capacity for the size of the allocated memory.
		 **/
		Arxx::Buffer::size_type m_Length{0};
		
		/**
		 * @brief The size of the memory block reserved for the content of @em this buffer.
		 * 
		 * This is an internal-only data. It describes how much data may be stored in @em this buffer currently without having to reallocate the buffers' memory.
		 *
		 * @note There is a separate property m_Length for the length of the content.
		 **/
		Arxx::Buffer::size_type m_Capacity{0};
		
		/**
		 * @brief The associated markers.
		 **/
		mutable std::set<Arxx::Buffer::Marker *> m_Markers;
		
		/**
		 * @brief The position of the begin of @em this buffer itside its superior buffer.
		 *
		 * This value is zero-based.
		 **/
		Arxx::Buffer::size_type m_Position{0};
        
		/**
		 * @brief A vector of pointers to structures describing the sub buffers.
		 *
		 * The order of these pointers is irrelevant except for one thing: the last sub buffer must always have the greatest m_stOrder value because it will be the reference for the m_stOrder of new registering buffers.
		 **/
		std::vector<Arxx::Buffer::SubBuffer *> m_SubBuffers;
		
		/**
		 * @brief Indicates the changing state of @em this buffer.
		 *
		 * This is a fluid state and will be changed in vWrite() and vDelete() when @em this buffer is actually changing. It will not be true from anywhere outside the library.
		 **/
		bool m_Changing{false};
        
	};
	
	/**
	 * @brief An output operator for std::ostream which uses the Buffers::Buffer::Ouput function.
	 * 
	 * This operator is implemented for your convinience and allows you to write:
	 * @code
	 * std::cout << Buffer;
	 * @endcode
	 * or any other std::ostream object. Remember that the data inside the buffer may be binary and thus is not meant to be pushed to std::cout.
	 **/
	auto operator<<(std::ostream & OStream, Arxx::Buffer const & Buffer) -> std::ostream &;
};

#endif
