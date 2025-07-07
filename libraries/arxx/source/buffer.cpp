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

#include <iostream>

#include <arxx/buffer.h>

const Arxx::Buffer::size_type Arxx::Buffer::m_DataUpdated = 0x01;
const Arxx::Buffer::size_type Arxx::Buffer::m_DataDeleted = 0x02;
const Arxx::Buffer::size_type Arxx::Buffer::m_DataInserted = 0x03;

#ifdef DEBUG
namespace Arxx
{
	std::string Indentation(Arxx::Buffer * Buffer)
	{
		std::string Pad = "";

		while(Buffer->m_SupBuffer)
		{
			Pad += "  ";
			Buffer = Buffer->m_SupBuffer;
		}

		return Pad;
	}
}
#endif

/**
 * @brief This class represents a sub buffer and its maintainance information inside a superior buffer.
 *
 * Whenever a sub buffer is created the superior buffer will add this informative structure to its m_SubBuffers vector.
 *
 * Currently there is only one piece of information given here: the order.
 *
 * The order of a sub buffer is a numeric value related to the time when it was created as a sub buffer. It actually is not a time value. It only supports the one relation:
 * - Buffer1.stGetOrder() > Buffer2.stGetOrder()  <=>  Buffer1 was created before Buffer2.
 * 
 * Why would anybody need such a thing?
 *
 * Imagine an arbitrary buffer, filled or empty, that doesn't matter. Now mentally create three sub buffers of length 0 at the same position, one after another so that they get different orders. Now consider the three (different) scenarios of inserting an amount of data into one of those buffers.
 * - When inserting into the first buffer (the one, you created first which got the lowest order) it would be nice to have the two remaining buffers at the end of the first buffer, not overlapping.
 * - When inserting into the second buffer it would be nice if the first buffer would still be in front of the second one and the third would be at its end.
 * - When inserting into the third buffer it would be nice if the two remaining buffer would be in front of the third.
 * 
 * Now, since all three buffer share the same values concerning their physical structure (position and length) there is no way of differentiating these three scenarios. What we need is the only difference they have: their times of creation.
 **/
class Arxx::Buffer::SubBuffer
{
public:
	/**
	 * @brief The constructor of a SubBuffer structure.
	 * @param Buffer The associated buffer which actually is the sub buffer.
	 * @param stOrder The order this sub buffer was inserted in the superior buffer.
	 * 
	 * This constructor creates a SubBuffer. The parameters are the information the superior buffer whishes to associate with the sub buffer.
	 **/
	SubBuffer(Arxx::Buffer & Buffer, size_type Order) :
		m_Buffer{Buffer},
		m_Order{Order}
	{
	}

	/**
	 * @brief A function to retrieve the associated buffer.
	 *
	 * With this function you can reget the associated sub buffer.
	 **/
	Buffer & GetBuffer()
	{
		return m_Buffer;
	}

	/**
	 * @brief A function to retrieve the associated order of the sub buffer.
	 *
	 * With this function you can get the associated order.
	 **/
	size_type GetOrder()
	{
		return m_Order;
	}
private:
	/**
	 * @brief The associated Buffer.
	 *
	 * A reference to the associated Buffer. As it is a reference it cannot be re-set or left unspecified. This class is only valid if there actually is such a sub buffer. It must be set with the constructor and may be retrieved with GetBuffer().
	 **/
	Buffer & m_Buffer;

	/**
	 * @brief The associated order of the sub buffer.
	 *
	 * A numeric value describing the order of the associated buffer (m_Buffer) inside the superior buffer. It may be retrieved via stGetOrder().
	 **/
	size_type m_Order;
};

Arxx::Buffer::Buffer(Arxx::Buffer & Buffer, size_type Position, size_type Length) :
	m_SupBuffer{&Buffer},
	m_Position{(Buffer.GetLength() < Position) ? (Buffer.GetLength()) : (Position)}
{
	if(Buffer.m_Begin != nullptr)
	{
		m_Begin = Buffer.m_Begin + m_Position;
	}
	if(Buffer.GetLength() >= m_Position + Length)
	{
		m_Length = m_Capacity = Length;
	}
	else
	{
		m_Length = m_Capacity = Buffer.m_Length - m_Position;
	}
	Buffer.Register(*this);
}

Arxx::Buffer::~Buffer()
{
	if(m_SupBuffer == nullptr)
	{
		delete[] m_Begin;
	}
	else
	{
		m_SupBuffer->Unregister(*this);
	}

	std::vector<Arxx::Buffer::SubBuffer *>::iterator SubBufferIterator(m_SubBuffers.begin());

	while(SubBufferIterator != m_SubBuffers.end())
	{
		delete *SubBufferIterator;
		SubBufferIterator = m_SubBuffers.erase(SubBufferIterator);
	}
}

void Arxx::Buffer::Register(Arxx::Buffer & Buffer)
{
	size_type Order(1);

	if(m_SubBuffers.size() != 0)
	{
		Order = (*(m_SubBuffers.end() - 1))->GetOrder() + 1;
	}
	m_SubBuffers.push_back(new SubBuffer(Buffer, Order));
}

void Arxx::Buffer::Unregister(Arxx::Buffer & Buffer)
{
	std::vector<Arxx::Buffer::SubBuffer *>::iterator SubBufferIterator(m_SubBuffers.begin());
	std::vector<Arxx::Buffer::SubBuffer *>::iterator SubBufferEnd(m_SubBuffers.end());

	while((SubBufferIterator != SubBufferEnd) && (std::addressof((*SubBufferIterator)->GetBuffer()) != std::addressof(Buffer)))
	{
		++SubBufferIterator;
	}
	if(SubBufferIterator != SubBufferEnd)
	{
		delete *SubBufferIterator;
		m_SubBuffers.erase(SubBufferIterator);
	}
}

void Arxx::Buffer::Register(Arxx::Buffer::Marker & Marker) const
{
	m_Markers.insert(&Marker);
}

void Arxx::Buffer::Unregister(Arxx::Buffer::Marker & Marker) const
{
	m_Markers.erase(m_Markers.find(&Marker));
}

Arxx::Buffer::size_type Arxx::Buffer::GetLength(void) const
{
	return m_Length;
}

void Arxx::Buffer::SetLength(size_type Length)
{
	if(Length != m_Length)
	{
		if(Length > m_Length)
		{
			Insert(m_Length, Length - m_Length);
		}
		else
		{
			Delete(Length, m_Length - Length);
		}
	}
}

void Arxx::Buffer::Insert(size_type Position, size_type DataLength, const_pointer Data)
{
	Write(*this, Position, DataLength, Data);
}

void Arxx::Buffer::Write(Arxx::Buffer & Buffer, Arxx::Buffer::size_type Position, size_type DataLength, const_pointer Data)
{
#ifdef DEBUG
	std::cerr << this << Indentation(this) << " Insert(Buffer = " << &Buffer << ", Position = " << Position << ", DataLength = " << DataLength << ")   ---   Status: Position = " << m_Position << ", Length = " << m_Length << ", Capacity = " << m_Capacity << ", ParentBuffer = " << m_SupBuffer << std::endl;
#endif
	if(Position > m_Length)
	{
		throw std::out_of_range("Trying to write after the end.");
	}

	pointer OldBegin(m_Begin);

	// if the following condition is met we are in the most superior buffer
	if(m_SupBuffer == nullptr)
	{
		// now the real work begins, we have to make the change.
		bool InsertedData(false);
		
		if(m_Length + DataLength > m_Capacity)
		{
			InsertedData = true;
			// more data than free space
			size_type Length(std::max(m_Length * 2, m_Length + DataLength));
			pointer Begin(new value_type[Length]);
			pointer End(std::copy(m_Begin, m_Begin + Position, Begin));
			
			if(Data != nullptr)
			{
				End = std::copy(Data, Data + DataLength, End);
			}
			else
			{
				End += DataLength;
			}
			End = std::copy(m_Begin + Position, m_Begin + m_Length, End);
			delete[] m_Begin;
			m_Begin = Begin;
			m_Capacity = Length;
		}
		else
		{
			InsertedData = true;
			std::copy_backward(m_Begin + Position, m_Begin + m_Length, m_Begin + m_Length + DataLength);
			if(Data != nullptr)
			{
				std::copy(Data, Data + DataLength, m_Begin + Position);
			}
		}
		m_Length += DataLength;
		for(std::set<Arxx::Buffer::Marker *>::iterator MarkerIterator = m_Markers.begin(); MarkerIterator != m_Markers.end(); ++MarkerIterator)
		{
			if((((*MarkerIterator)->GetPosition() == Position) && ((*MarkerIterator)->GetAlignment() == Arxx::Buffer::Marker::RIGHT)) || ((*MarkerIterator)->GetPosition() > Position))
			{
				(*MarkerIterator)->SetPosition((*MarkerIterator)->GetPosition() + DataLength);
			}
		}
		
		size_type Order = 0;
		std::vector<Arxx::Buffer::SubBuffer *>::iterator SubBufferIterator;
		std::vector<Arxx::Buffer::SubBuffer *>::iterator SubBufferEnd;

		if(&Buffer != this)
		{
			SubBufferIterator = m_SubBuffers.begin();
			SubBufferEnd = m_SubBuffers.end();
			while((SubBufferIterator != SubBufferEnd) && (std::addressof((*SubBufferIterator)->GetBuffer()) != std::addressof(Buffer)))
			{
				++SubBufferIterator;
			}
			if(SubBufferIterator == SubBufferEnd)
			{
				throw std::invalid_argument("The Buffer seems to be invalid (it is not registered as a child)");
			}
			Order = (*SubBufferIterator)->GetOrder();
		}
#ifdef DEBUG
		std::cerr << this << Indentation(this) << " is inserting with order " << Order << " and InsertedData = " << InsertedData << std::endl;
#endif
		SubBufferIterator = m_SubBuffers.begin();
		SubBufferEnd = m_SubBuffers.end();
		while(SubBufferIterator != SubBufferEnd)
		{
			if(((*SubBufferIterator)->GetOrder() >= Order) && (InsertedData == true))
			{
				(*SubBufferIterator)->GetBuffer().ParentDataChanged(m_DataInserted, Position, DataLength);
			}
			else
			{
				if(m_Begin != OldBegin)
				{
					(*SubBufferIterator)->GetBuffer().ParentDataChanged(m_DataUpdated, 0, 0);
				}
			}
			++SubBufferIterator;
		}
	}
	else
	{
		// this is fairly easy as we are in a child buffer
		//   we just have to pass the change further up.
		m_Changing = true;
		m_SupBuffer->Write(*this, m_Position + Position, DataLength, Data);
		m_Changing = false;

		size_type Order = 0;
		std::vector<Arxx::Buffer::SubBuffer *>::iterator SubBufferIterator;
		std::vector<Arxx::Buffer::SubBuffer *>::iterator SubBufferEnd;

		if(&Buffer != this)
		{
			SubBufferIterator = m_SubBuffers.begin();
			SubBufferEnd = m_SubBuffers.end();
			while((SubBufferIterator != SubBufferEnd) && (std::addressof((*SubBufferIterator)->GetBuffer()) != std::addressof(Buffer)))
			{
				++SubBufferIterator;
			}
			if(SubBufferIterator == SubBufferEnd)
			{
				throw std::invalid_argument("The Buffer seems to be invalid (it is not registered as a child)");
			}
			Order = (*SubBufferIterator)->GetOrder();
		}
#ifdef DEBUG
		std::cerr << this << Indentation(this) << " is inserting with order " << Order << std::endl;
#endif
		SubBufferIterator = m_SubBuffers.begin();
		SubBufferEnd = m_SubBuffers.end();
		while(SubBufferIterator != SubBufferEnd)
		{
			if((*SubBufferIterator)->GetOrder() >= Order)
			{
				(*SubBufferIterator)->GetBuffer().ParentDataChanged(m_DataInserted, Position, DataLength);
			}
			else
			{
				if(m_Begin != OldBegin)
				{
					(*SubBufferIterator)->GetBuffer().ParentDataChanged(m_DataUpdated, 0, 0);
				}
			}
			++SubBufferIterator;
		}
	}
}

void Arxx::Buffer::Delete(size_type Position, size_type Length)
{
#ifdef DEBUG
	std::cerr << this << Indentation(this) << " Delete(Position = " << Position << ", Length = " << Length << ")   ---   Status: Position = " << m_Position << ", Length = " << m_Length << std::endl;
#endif
	if((Position > m_Length) || (Position + Length > m_Length))
	{
		throw std::out_of_range("Trying to delete after the end or until after the end.");
	}
	if(m_SupBuffer == nullptr)
	{
		std::copy(m_Begin + Position + Length, m_Begin + m_Length, m_Begin + Position);
		m_Length -= Length;
		for(std::set<Arxx::Buffer::Marker *>::iterator MarkerIterator = m_Markers.begin(); MarkerIterator != m_Markers.end(); ++MarkerIterator)
		{
			if((*MarkerIterator)->GetPosition() > m_Length)
			{
				(*MarkerIterator)->SetPosition(m_Length);
			}
		}
		
		std::vector<Arxx::Buffer::SubBuffer *>::iterator SubBufferIterator = m_SubBuffers.begin();
		std::vector<Arxx::Buffer::SubBuffer *>::iterator SubBufferEnd = m_SubBuffers.end();
		
		while(SubBufferIterator != SubBufferEnd)
		{
			(*SubBufferIterator)->GetBuffer().ParentDataChanged(m_DataDeleted, Position, Length);
			++SubBufferIterator;
		}
	}
	else
	{
		m_Changing = true;
		m_SupBuffer->Delete(m_Position + Position, Length);
		m_Changing = false;
	}
}

Arxx::Buffer::const_pointer Arxx::Buffer::GetBegin() const
{
	return m_Begin;
}

void Arxx::Buffer::ParentDataChanged(size_type ChangeMode, size_type Position, size_type Length)
{
#ifdef DEBUG
	std::cerr << this << Indentation(this) << " ParentDataChanged(ChangeMode = " << ChangeMode << ", Position = " << Position << ", Length = " << Length << ")   ---   Status: Position = " << m_Position << ", Length = " << m_Length << ", Changing = " << m_Changing << std::endl;
#endif
	switch(ChangeMode)
	{
	case m_DataUpdated:
		{
			m_Begin = m_SupBuffer->m_Begin + m_Position;

			std::vector<Arxx::Buffer::SubBuffer *>::iterator SubBufferIterator = m_SubBuffers.begin();
			std::vector<Arxx::Buffer::SubBuffer *>::iterator SubBufferEnd = m_SubBuffers.end();

			while(SubBufferIterator != SubBufferEnd)
			{
				(*SubBufferIterator)->GetBuffer().ParentDataChanged(m_DataUpdated, 0, 0);
				++SubBufferIterator;
			}

			break;
		}
	case m_DataDeleted:
		{
			if(Position < m_Position)
			{
				if(Position + Length < m_Position)
				{
					// deleting completely before Buffer
					m_Position -= Length;
				}
				else
				{
					// deletion and Buffer intersect
					if(Position + Length < m_Position + m_Length)
					{
						// partly intersection
						m_Length = m_Position + m_Length - (Position + Length);
						for(std::set<Arxx::Buffer::Marker *>::iterator MarkerIterator = m_Markers.begin(); MarkerIterator != m_Markers.end(); ++MarkerIterator)
						{
							if(m_Position + (*MarkerIterator)->GetPosition() <= Position + Length)
							{
								(*MarkerIterator)->SetPosition(0);
							}
							else
							{
								(*MarkerIterator)->SetPosition(m_Position + (*MarkerIterator)->GetPosition() - (Position + Length));
							}
						}
					}
					else
					{
						// deletion covers Buffer
						m_Length = 0;
						for(std::set<Arxx::Buffer::Marker *>::iterator MarkerIterator = m_Markers.begin(); MarkerIterator != m_Markers.end(); ++MarkerIterator)
						{
							(*MarkerIterator)->SetPosition(0);
						}
					}
					m_Position = Position;
				}
			}
			else
			{
				// HERE: The deletion is after or at the begin of _this_ buffer
				//    => The position of _this_ buffer must not change
				if(Position < m_Position + m_Length)
				{
					// HERE: The deletion is NOT after the end of _this_ buffer
					if(Position + Length > m_Position + m_Length)
					{
						// HERE: The deletion overlaps the end of _this_ buffer
						//    => Reduce the size of _this_ buffer by the length of the overlapping
						//
						// Condition:
						//                  Position >= m_Position
						// =>  Position - m_Position >= 0
						// =>  this difference is greater than/or equal zero and thus safe
						m_Length = Position - m_Position;
					}
					else
					{
						// HERE: The deletion is completely inside _this_ buffer
						//    => Reduce the size of _this_ buffer by the size of the deletion
						//
						// Condition 1:
						//                  Position >= m_Position
						// =>  Position - m_Position >= 0
						//
						// Condition 2:
						//                    Position + Length <= m_Position + m_Length
						// =>  (Position - m_Position) + Length <= m_Length
						// =>                        X + Length <= m_Length
						// =>                          m_Length >= Length + X
						// =>  this difference is greater than/or equal zero and thus safe
						m_Length -= Length;
					}
				}
				else
				{
					// HERE: The deletion is after the end of _this_ buffer
					//    => Nothing to be done
				}
			}
			m_Begin = m_SupBuffer->m_Begin + m_Position;

			std::vector<Arxx::Buffer::SubBuffer *>::iterator SubBufferIterator = m_SubBuffers.begin();
			std::vector<Arxx::Buffer::SubBuffer *>::iterator SubBufferEnd = m_SubBuffers.end();

			while(SubBufferIterator != SubBufferEnd)
			{
				(*SubBufferIterator)->GetBuffer().ParentDataChanged(m_DataDeleted, Position - m_Position, Length);
				++SubBufferIterator;
			}

			break;
		}
	case m_DataInserted:
		{
			if((Position == m_Position) && (m_Changing == true))
			{
				// prepending
				m_Length += Length;
			}
			else
			{
				if(Position <= m_Position)
				{
					// data inserted; we are not prepending; inserting position is before this buffer
					m_Position += Length;
				}
				else
				{
					if((Position <= m_Position + m_Length) && (m_Changing == true))
					{
						m_Length += Length;
					}
				}
			}
			// data inserted; we are not prepending; inserting position is after the begin of this buffer
			// update I/O position
			for(std::set<Arxx::Buffer::Marker *>::iterator MarkerIterator = m_Markers.begin(); MarkerIterator != m_Markers.end(); ++MarkerIterator)
			{
				if(((m_Position + (*MarkerIterator)->GetPosition() == Position) && ((*MarkerIterator)->GetAlignment() == Arxx::Buffer::Marker::RIGHT)) || ((*MarkerIterator)->GetPosition() > Position))
				{
					// only if the data was inserted BEFORE or AT the I/O position
					(*MarkerIterator)->SetPosition((*MarkerIterator)->GetPosition() + Length);
				}
			}
			m_Begin = m_SupBuffer->m_Begin + m_Position;

			// NEXT: because, if we are changing there is an order, which must be followed. handle from vInsert
			if(m_Changing == false)
			{
				std::vector<Arxx::Buffer::SubBuffer *>::iterator SubBufferIterator = m_SubBuffers.begin();
				std::vector<Arxx::Buffer::SubBuffer *>::iterator SubBufferEnd = m_SubBuffers.end();

				while(SubBufferIterator != SubBufferEnd)
				{
					if(Position >= m_Position)
					{
						(*SubBufferIterator)->GetBuffer().ParentDataChanged(m_DataInserted, Position - m_Position, Length);
					}
					++SubBufferIterator;
				}
			}

			break;
		}
	}
}

Arxx::Buffer::value_type Arxx::Buffer::operator[](size_type Index) const
{
	if(Index >= m_Length)
	{
		throw std::out_of_range("Trying to read after the end.");
	}

	return m_Begin[Index];
}

Arxx::Buffer::reference Arxx::Buffer::operator[](size_type Index)
{
	if(Index >= m_Length)
	{
		throw std::out_of_range("Trying to read after the end.");
	}

	return m_Begin[Index];
}

std::ostream & Arxx::operator<<(std::ostream & OStream, const Arxx::Buffer & Buffer)
{
	OStream.write(const_cast<char *>(reinterpret_cast<char const *>(Buffer.GetBegin())), Buffer.GetLength());
	
	return OStream;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Arxx::Buffer::Marker                                                                          //
///////////////////////////////////////////////////////////////////////////////////////////////////

Arxx::Buffer::Marker::Marker(Arxx::Buffer const & Buffer, Arxx::Buffer::size_type Position, Arxx::Buffer::Marker::Alignment Alignment) :
	m_Buffer{&Buffer},
	m_Position{(m_Buffer->GetLength() > Position) ? (Position) : (m_Buffer->GetLength())},
	m_Alignment{Alignment}
{
	m_Buffer->Register(*this);
}

Arxx::Buffer::Marker::~Marker()
{
	m_Buffer->Unregister(*this);
}

Arxx::Buffer::size_type Arxx::Buffer::Marker::GetPosition() const
{
	return m_Position;
}

void Arxx::Buffer::Marker::SetPosition(Arxx::Buffer::size_type Position)
{
	m_Position = (m_Buffer->GetLength() > Position) ? (Position) : (m_Buffer->GetLength());
}

Arxx::Buffer::Marker::Alignment Arxx::Buffer::Marker::GetAlignment() const
{
	return m_Alignment;
}

void Arxx::Buffer::Marker::SetAlignment(Arxx::Buffer::Marker::Alignment Alignment)
{
	m_Alignment = Alignment;
}

bool Arxx::Buffer::Marker::IsValid() const
{
	return m_Buffer != nullptr;
}

void Arxx::Buffer::Marker::m_InvalidateBuffer()
{
	m_Buffer = nullptr;
}
