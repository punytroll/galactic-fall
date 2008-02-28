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

#ifndef OBJECT_ASPECT_ACCESSORY_H
#define OBJECT_ASPECT_ACCESSORY_H

#include <string>

class Slot;

class ObjectAspectAccessory
{
public:
	// constructor & destructor
	ObjectAspectAccessory(void);
	~ObjectAspectAccessory(void);
	// getters
	Slot * GetSlot(void);
	const Slot * GetSlot(void) const;
	const std::string & GetSlotClassIdentifier(void) const;
	// setters
	void SetSlot(Slot * Slot);
	void SetSlotClassIdentifier(const std::string & SlotClassIdentifier);
private:
	Slot * m_Slot;
	std::string m_SlotClassIdentifier;
};

inline Slot * ObjectAspectAccessory::GetSlot(void)
{
	return m_Slot;
}

inline const Slot * ObjectAspectAccessory::GetSlot(void) const
{
	return m_Slot;
}

inline const std::string & ObjectAspectAccessory::GetSlotClassIdentifier(void) const
{
	return m_SlotClassIdentifier;
}

inline void ObjectAspectAccessory::SetSlot(Slot * Slot)
{
	m_Slot = Slot;
}

inline void ObjectAspectAccessory::SetSlotClassIdentifier(const std::string & SlotClassIdentifier)
{
	m_SlotClassIdentifier = SlotClassIdentifier;
}

#endif
