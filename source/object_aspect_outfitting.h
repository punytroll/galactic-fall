/**
 * galactic-fall
 * Copyright (C) 2008  Hagen Möbius
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

#ifndef OBJECT_ASPECT_OUTFITTING_H
#define OBJECT_ASPECT_OUTFITTING_H

#include <map>
#include <string>

class Slot;

class ObjectAspectOutfitting
{
public:
	// constructor & destructor
	ObjectAspectOutfitting(void);
	~ObjectAspectOutfitting(void);
	// getters
	Slot * GetSlot(const std::string & SlotIdentifier);
	const Slot * GetSlot(const std::string & SlotIdentifier) const;
	const std::map< std::string, Slot * > & GetSlots(void) const;
	// modifiers
	void AddSlot(Slot * Slot);
private:
	std::map< std::string, Slot * > m_Slots;
};

inline Slot * ObjectAspectOutfitting::GetSlot(const std::string & SlotIdentifier)
{
	std::map< std::string, Slot * >::iterator SlotIterator(m_Slots.find(SlotIdentifier));
	
	return ((SlotIterator != m_Slots.end()) ? (SlotIterator->second) : (0));
}

inline const Slot * ObjectAspectOutfitting::GetSlot(const std::string & SlotIdentifier) const
{
	std::map< std::string, Slot * >::const_iterator SlotIterator(m_Slots.find(SlotIdentifier));
	
	return ((SlotIterator != m_Slots.end()) ? (SlotIterator->second) : (0));
}

inline const std::map< std::string, Slot * > & ObjectAspectOutfitting::GetSlots(void) const
{
	return m_Slots;
}

#endif
