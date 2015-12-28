/**
 * libarxx - Advanced Resource files in C++
 * Copyright (C) 2005, 2007  Hagen Möbius
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

#include  <Buffer.h>

const Arxx::Buffer::size_type Arxx::Buffer::m_stDataUpdated = 0x01;
const Arxx::Buffer::size_type Arxx::Buffer::m_stDataDeleted = 0x02;
const Arxx::Buffer::size_type Arxx::Buffer::m_stDataInserted = 0x03;

#ifdef DEBUG
namespace Arxx
{
	std::string sIndentation(Arxx::Buffer * pBuffer)
	{
		std::string sPad = "";

		while(pBuffer->m_pSupBuffer)
		{
			sPad += "  ";
			pBuffer = pBuffer->m_pSupBuffer;
		}

		return sPad;
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
	SubBuffer(Arxx::Buffer & Buffer, size_type stOrder) :
		m_Buffer(Buffer),
		m_stOrder(stOrder)
	{
	}

	/**
	 * @brief A function to retrieve the associated buffer.
	 *
	 * With this function you can reget the associated sub buffer.
	 **/
	Buffer & GetBuffer(void)
	{
		return m_Buffer;
	}

	/**
	 * @brief A function to retrieve the associated order of the sub buffer.
	 *
	 * With this function you can get the associated order.
	 **/
	size_type stGetOrder(void)
	{
		return m_stOrder;
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
	size_type m_stOrder;
};

Arxx::Buffer::Buffer(void) :
	m_pSupBuffer(0),
	m_Begin(0),
	m_stLength(0),
	m_stCapacity(0),
	m_stPosition(0),
	m_bChanging(false)
{
}

Arxx::Buffer::Buffer(Arxx::Buffer & Buffer, size_type stPosition, size_type stLength) :
	m_pSupBuffer(&Buffer),
	m_Begin(0),
	m_stLength(0),
	m_stCapacity(0),
	m_stPosition((Buffer.stGetLength() < stPosition) ? (Buffer.stGetLength()) : (stPosition)),
	m_bChanging(false)
{
	if(Buffer.m_Begin != 0)
	{
		m_Begin = Buffer.m_Begin + m_stPosition;
	}
	if(Buffer.stGetLength() >= m_stPosition + stLength)
	{
		m_stLength = m_stCapacity = stLength;
	}
	else
	{
		m_stLength = m_stCapacity = Buffer.m_stLength - m_stPosition;
	}
	Buffer.vRegister(*this);
}

Arxx::Buffer::~Buffer(void)
{
	if(m_pSupBuffer == 0)
	{
		delete[] m_Begin;
	}
	else
	{
		m_pSupBuffer->vUnregister(*this);
	}

	std::vector< Arxx::Buffer::SubBuffer * >::iterator iBuffer(m_SubBuffers.begin());

	while(iBuffer != m_SubBuffers.end())
	{
		delete *iBuffer;
		m_SubBuffers.erase(iBuffer);
		iBuffer = m_SubBuffers.begin();
	}
}

void Arxx::Buffer::vRegister(Arxx::Buffer & Buffer)
{
	size_type stOrder(1);

	if(m_SubBuffers.size() != 0)
	{
		stOrder = (*(m_SubBuffers.end() - 1))->stGetOrder() + 1;
	}
	m_SubBuffers.push_back(new SubBuffer(Buffer, stOrder));
}

void Arxx::Buffer::vUnregister(Arxx::Buffer & Buffer)
{
	std::vector< SubBuffer * >::iterator iChildBuffer(m_SubBuffers.begin());
	std::vector< SubBuffer * >::iterator iEnd(m_SubBuffers.end());

	while((iChildBuffer != iEnd) && (&((*iChildBuffer)->GetBuffer()) != &(Buffer)))
	{
		++iChildBuffer;
	}
	if(iChildBuffer != iEnd)
	{
		delete *iChildBuffer;
		m_SubBuffers.erase(iChildBuffer);
	}
}

void Arxx::Buffer::vRegister(Arxx::Buffer::Marker & Marker) const
{
	m_Markers.insert(&Marker);
}

void Arxx::Buffer::vUnregister(Arxx::Buffer::Marker & Marker) const
{
	m_Markers.erase(m_Markers.find(&Marker));
}

Arxx::Buffer::size_type Arxx::Buffer::stGetLength(void) const
{
	return m_stLength;
}

void Arxx::Buffer::vSetLength(size_type stLength)
{
	if(stLength != m_stLength)
	{
		if(stLength > m_stLength)
		{
			vInsert(m_stLength, stLength - m_stLength);
		}
		else
		{
			vDelete(stLength, m_stLength - stLength);
		}
	}
}

void Arxx::Buffer::vInsert(size_type stPosition, size_type stDataLength, const_pointer Data)
{
	vWrite(*this, stPosition, stDataLength, Data);
}

void Arxx::Buffer::vWrite(Arxx::Buffer & Buffer, Arxx::Buffer::size_type stPosition, size_type stDataLength, const_pointer Data)
{
#ifdef DEBUG
	std::cerr << this << sIndentation(this) << " vInsert(Buffer = " << &Buffer << ", Position = " << stPosition << ", DataLength = " << stDataLength << ")   ---   Status: Position = " << m_stPosition << ", Length = " << m_stLength << ", Capacity = " << m_stCapacity << ", ParentBuffer = " << m_pSupBuffer << std::endl;
#endif
	if(stPosition > m_stLength)
	{
		throw std::out_of_range("Trying to write after the end.");
	}

	pointer OldBegin(m_Begin);

	// if the following condition is met we are in the most superior buffer
	if(m_pSupBuffer == 0)
	{
		// now the real work begins, we have to make the change.
		bool bInsertedData(false);
		
		if(m_stLength + stDataLength > m_stCapacity)
		{
			bInsertedData = true;
			// more data than free space
			size_type stLength(std::max(m_stLength * 2, m_stLength + stDataLength));
			pointer Begin(new value_type[stLength]);
			pointer End(std::copy(m_Begin, m_Begin + stPosition, Begin));
			
			if(Data != 0)
			{
				End = std::copy(Data, Data + stDataLength, End);
			}
			else
			{
				End += stDataLength;
			}
			End = std::copy(m_Begin + stPosition, m_Begin + m_stLength, End);
			delete[] m_Begin;
			m_Begin = Begin;
			m_stCapacity = stLength;
		}
		else
		{
			bInsertedData = true;
			std::copy_backward(m_Begin + stPosition, m_Begin + m_stLength, m_Begin + m_stLength + stDataLength);
			if(Data != 0)
			{
				std::copy(Data, Data + stDataLength, m_Begin + stPosition);
			}
		}
		m_stLength += stDataLength;
		for(std::set< Arxx::Buffer::Marker * >::iterator iMarker = m_Markers.begin(); iMarker != m_Markers.end(); ++iMarker)
		{
			if((((*iMarker)->stGetPosition() == stPosition) && ((*iMarker)->GetAlignment() == Arxx::Buffer::Marker::RIGHT)) || ((*iMarker)->stGetPosition() > stPosition))
			{
				(*iMarker)->vSetPosition((*iMarker)->stGetPosition() + stDataLength);
			}
		}
		
		size_type stOrder = 0;
		std::vector< SubBuffer * >::iterator iBuffer;
		std::vector< SubBuffer * >::iterator iEnd;

		if(&Buffer != this)
		{
			iBuffer = m_SubBuffers.begin();
			iEnd = m_SubBuffers.end();
			while((iBuffer != iEnd) && (&((*iBuffer)->GetBuffer()) != &Buffer))
			{
				++iBuffer;
			}
			if(iBuffer == iEnd)
			{
				throw std::invalid_argument("The Buffer seems to be invalid (it is not registered as a child)");
			}
			stOrder = (*iBuffer)->stGetOrder();
		}
#ifdef DEBUG
		std::cerr << this << sIndentation(this) << " is inserting with order " << stOrder << " and InsertedData = " << bInsertedData << std::endl;
#endif
		iBuffer = m_SubBuffers.begin();
		iEnd = m_SubBuffers.end();
		while(iBuffer != iEnd)
		{
			if(((*iBuffer)->stGetOrder() >= stOrder) && (bInsertedData == true))
			{
				(*iBuffer)->GetBuffer().vParentDataChanged(m_stDataInserted, stPosition, stDataLength);
			}
			else
			{
				if(m_Begin != OldBegin)
				{
					(*iBuffer)->GetBuffer().vParentDataChanged(m_stDataUpdated, 0, 0);
				}
			}
			++iBuffer;
		}
	}
	else
	{
		// this is fairly easy as we are in a child buffer
		//   we just have to pass the change further up.
		m_bChanging = true;
		m_pSupBuffer->vWrite(*this, m_stPosition + stPosition, stDataLength, Data);
		m_bChanging = false;

		size_type stOrder = 0;
		std::vector< SubBuffer * >::iterator iBuffer;
		std::vector< SubBuffer * >::iterator iEnd;

		if(&Buffer != this)
		{
			iBuffer = m_SubBuffers.begin();
			iEnd = m_SubBuffers.end();
			while((iBuffer != iEnd) && (&((*iBuffer)->GetBuffer()) != &Buffer))
			{
				++iBuffer;
			}
			if(iBuffer == iEnd)
			{
				throw std::invalid_argument("The Buffer seems to be invalid (it is not registered as a child)");
			}
			stOrder = (*iBuffer)->stGetOrder();
		}
#ifdef DEBUG
		std::cerr << this << sIndentation(this) << " is inserting with order " << stOrder << std::endl;
#endif
		iBuffer = m_SubBuffers.begin();
		iEnd = m_SubBuffers.end();
		while(iBuffer != iEnd)
		{
			if((*iBuffer)->stGetOrder() >= stOrder)
			{
				(*iBuffer)->GetBuffer().vParentDataChanged(m_stDataInserted, stPosition, stDataLength);
			}
			else
			{
				if(m_Begin != OldBegin)
				{
					(*iBuffer)->GetBuffer().vParentDataChanged(m_stDataUpdated, 0, 0);
				}
			}
			++iBuffer;
		}
	}
}

void Arxx::Buffer::vDelete(size_type stPosition, size_type stLength)
{
#ifdef DEBUG
	std::cerr << this << sIndentation(this) << " vDelete(Position = " << stPosition << ", Length = " << stLength << ")   ---   Status: Position = " << m_stPosition << ", Length = " << m_stLength << std::endl;
#endif
	if((stPosition > m_stLength) || (stPosition + stLength > m_stLength))
	{
		throw std::out_of_range("Trying to delete after the end or until after the end.");
	}
	if(m_pSupBuffer == 0)
	{
		std::copy(m_Begin + stPosition + stLength, m_Begin + m_stLength, m_Begin + stPosition);
		m_stLength -= stLength;
		for(std::set< Arxx::Buffer::Marker * >::iterator iMarker = m_Markers.begin(); iMarker != m_Markers.end(); ++iMarker)
		{
			if((*iMarker)->stGetPosition() > m_stLength)
			{
				(*iMarker)->vSetPosition(m_stLength);
			}
		}
		
		std::vector< SubBuffer * >::iterator iBuffer = m_SubBuffers.begin();
		std::vector< SubBuffer * >::iterator iEnd = m_SubBuffers.end();
		
		while(iBuffer != iEnd)
		{
			(*iBuffer)->GetBuffer().vParentDataChanged(m_stDataDeleted, stPosition, stLength);
			++iBuffer;
		}
	}
	else
	{
		m_bChanging = true;
		m_pSupBuffer->vDelete(m_stPosition + stPosition, stLength);
		m_bChanging = false;
	}
}

Arxx::Buffer::const_pointer Arxx::Buffer::GetBegin(void) const
{
	return m_Begin;
}

void Arxx::Buffer::vParentDataChanged(size_type stChangeMode, size_type stPosition, size_type stLength)
{
#ifdef DEBUG
	std::cerr << this << sIndentation(this) << " vParentDataChanged(ChangeMode = " << stChangeMode << ", Position = " << stPosition << ", u4Length = " << stLength << ")   ---   Status: Position = " << m_stPosition << ", Length = " << m_stLength << ", Changing = " << m_bChanging << std::endl;
#endif
	switch(stChangeMode)
	{
	case m_stDataUpdated:
		{
			m_Begin = m_pSupBuffer->m_Begin + m_stPosition;

			std::vector< SubBuffer * >::iterator iBuffer = m_SubBuffers.begin();
			std::vector< SubBuffer * >::iterator iEnd = m_SubBuffers.end();

			while(iBuffer != iEnd)
			{
				(*iBuffer)->GetBuffer().vParentDataChanged(m_stDataUpdated, 0, 0);
				++iBuffer;
			}

			break;
		}
	case m_stDataDeleted:
		{
			if(stPosition < m_stPosition)
			{
				if(stPosition + stLength < m_stPosition)
				{
					// deleting completely before Buffer
					m_stPosition -= stLength;
				}
				else
				{
					// deletion and Buffer intersect
					if(stPosition + stLength < m_stPosition + m_stLength)
					{
						// partly intersection
						m_stLength = m_stPosition + m_stLength - (stPosition + stLength);
						for(std::set< Arxx::Buffer::Marker * >::iterator iMarker = m_Markers.begin(); iMarker != m_Markers.end(); ++iMarker)
						{
							if(m_stPosition + (*iMarker)->stGetPosition() <= stPosition + stLength)
							{
								(*iMarker)->vSetPosition(0);
							}
							else
							{
								(*iMarker)->vSetPosition(m_stPosition + (*iMarker)->stGetPosition() - (stPosition + stLength));
							}
						}
					}
					else
					{
						// deletion covers Buffer
						m_stLength = 0;
						for(std::set< Arxx::Buffer::Marker * >::iterator iMarker = m_Markers.begin(); iMarker != m_Markers.end(); ++iMarker)
						{
							(*iMarker)->vSetPosition(0);
						}
					}
					m_stPosition = stPosition;
				}
			}
			else
			{
				// HERE: The deletion is after or at the begin of _this_ buffer
				//    => The position of _this_ buffer must not change
				if(stPosition < m_stPosition + m_stLength)
				{
					// HERE: The deletion is NOT after the end of _this_ buffer
					if(stPosition + stLength > m_stPosition + m_stLength)
					{
						// HERE: The deletion overlaps the end of _this_ buffer
						//    => Reduce the size of _this_ buffer by the length of the overlapping
						//
						// Condition:
						//     stPosition >= m_stPosition
						// =>  X = stPosition - m_stPosition
						// =>  X >= 0
						// =>  this difference is greater than/or equal zero and thus safe
						m_stLength = stPosition - m_stPosition;
					}
					else
					{
						// HERE: The deletion is completely inside _this_ buffer
						//    => Reduce the size of _this_ buffer by the size of the deletion
						//
						// Condition 1:
						//     stPosition >= m_stPosition
						// =>  X = stPosition - m_stPosition
						// =>  X >= 0
						//
						// Condition 2:
						//                      stPosition + stLength <= m_stPosition + m_stLength
						// =>  (stPosition - m_stPosition) + stLength <= m_stLength
						// =>                            X + stLength <= m_stLength
						// =>                              m_stLength >= stLength + X
						// =>  this difference is greater than/or equal zero and thus safe
						m_stLength -= stLength;
					}
				}
				else
				{
					// HERE: The deletion is after the end of _this_ buffer
					//    => Nothing to be done
				}
			}
			m_Begin = m_pSupBuffer->m_Begin + m_stPosition;

			std::vector< SubBuffer * >::iterator iBuffer = m_SubBuffers.begin();
			std::vector< SubBuffer * >::iterator iEnd = m_SubBuffers.end();

			while(iBuffer != iEnd)
			{
				(*iBuffer)->GetBuffer().vParentDataChanged(m_stDataDeleted, stPosition - m_stPosition, stLength);
				++iBuffer;
			}

			break;
		}
	case m_stDataInserted:
		{
			if((stPosition == m_stPosition) && (m_bChanging == true))
			{
				// prepending
				m_stLength += stLength;
			}
			else
			{
				if(stPosition <= m_stPosition)
				{
					// data inserted; we are not prepending; inserting position is before this buffer
					m_stPosition += stLength;
				}
				else
				{
					if((stPosition <= m_stPosition + m_stLength) && (m_bChanging == true)) // no "&& (m_bOverwrite == false)" because then the length wouldn't change when overwriting over the end
					{
						m_stLength += stLength;
					}
				}
			}
			// data inserted; we are not prepending; inserting position is after the begin of this buffer
			// update I/O position
			for(std::set< Arxx::Buffer::Marker * >::iterator iMarker = m_Markers.begin(); iMarker != m_Markers.end(); ++iMarker)
			{
				if(((m_stPosition + (*iMarker)->stGetPosition() == stPosition) && ((*iMarker)->GetAlignment() == Arxx::Buffer::Marker::RIGHT)) || ((*iMarker)->stGetPosition() > stPosition))
				{
					// only if the data was inserted BEFORE or AT the I/O position
					(*iMarker)->vSetPosition((*iMarker)->stGetPosition() + stLength);
				}
			}
			m_Begin = m_pSupBuffer->m_Begin + m_stPosition;

			// NEXT: because, if we are changing there is an order, which must be followed. handle from vInsert
			if(m_bChanging == false)
			{
				std::vector< SubBuffer * >::iterator iBuffer = m_SubBuffers.begin();
				std::vector< SubBuffer * >::iterator iEnd = m_SubBuffers.end();

				while(iBuffer != iEnd)
				{
					if(stPosition >= m_stPosition)
					{
						(*iBuffer)->GetBuffer().vParentDataChanged(m_stDataInserted, stPosition - m_stPosition, stLength);
					}
					++iBuffer;
				}
			}

			break;
		}
	}
}

Arxx::Buffer::value_type Arxx::Buffer::operator[](size_type stIndex) const
{
	if(stIndex >= m_stLength)
	{
		throw std::out_of_range("Trying to read after the end.");
	}

	return m_Begin[stIndex];
}

Arxx::Buffer::reference Arxx::Buffer::operator[](size_type stIndex)
{
	if(stIndex >= m_stLength)
	{
		throw std::out_of_range("Trying to read after the end.");
	}

	return m_Begin[stIndex];
}

std::ostream & Arxx::operator<<(std::ostream & OStream, const Arxx::Buffer & Buffer)
{
	OStream.write(const_cast< char * >(reinterpret_cast< const char * >(Buffer.GetBegin())), Buffer.stGetLength());
	
	return OStream;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Arxx::Buffer::Marker                                                                          //
///////////////////////////////////////////////////////////////////////////////////////////////////

Arxx::Buffer::Marker::Marker(const Arxx::Buffer & Buffer, Arxx::Buffer::size_type stPosition, Arxx::Buffer::Marker::Alignment Alignment) :
	m_pBuffer(&Buffer),
	m_stPosition((m_pBuffer->stGetLength() > stPosition) ? (stPosition) : (m_pBuffer->stGetLength())),
	m_Alignment(Alignment)
{
	m_pBuffer->vRegister(*this);
}

Arxx::Buffer::Marker::~Marker(void)
{
	m_pBuffer->vUnregister(*this);
}

Arxx::Buffer::size_type Arxx::Buffer::Marker::stGetPosition(void) const
{
	return m_stPosition;
}

void Arxx::Buffer::Marker::vSetPosition(Arxx::Buffer::size_type stPosition)
{
	m_stPosition = (m_pBuffer->stGetLength() > stPosition) ? (stPosition) : (m_pBuffer->stGetLength());
}

Arxx::Buffer::Marker::Alignment Arxx::Buffer::Marker::GetAlignment(void) const
{
	return m_Alignment;
}

void Arxx::Buffer::Marker::vSetAlignment(Arxx::Buffer::Marker::Alignment Alignment)
{
	m_Alignment = Alignment;
}

bool Arxx::Buffer::Marker::bIsValid(void) const
{
	return m_pBuffer != 0;
}

void Arxx::Buffer::Marker::vInvalidateBuffer(void)
{
	m_pBuffer = 0;
}
