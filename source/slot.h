/**
 * galactic-fall
 * Copyright (C) 2007  Hagen Möbius
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

#ifndef SLOT_H
#define SLOT_H

#include <string>

#include "math/quaternion.h"
#include "math/vector3f.h"
#include "referencing.h"

class Object;
class SlotClass;

class Slot
{
public:
	Slot(const SlotClass * SlotClass, const std::string & Identifier);
	// getters
	const std::string & GetIdentifier(void) const;
	Reference< Object > & GetMountedObject(void);
	const Reference< Object > & GetMountedObject(void) const;
	const std::string & GetName(void) const;
	const Quaternion & GetOrientation(void) const;
	const Vector3f & GetPosition(void) const;
	const SlotClass * GetSlotClass(void) const;
	// setters
	void SetMountedObject(Reference< Object > MountedObject);
	void SetName(const std::string & Name);
	void SetOrientation(const Quaternion & Orientation);
	void SetPosition(const Vector3f & Position);
private:
	const SlotClass * m_SlotClass;
	std::string m_Identifier;
	std::string m_Name;
	Reference< Object > m_MountedObject;
	Quaternion m_Orientation;
	Vector3f m_Position;
};

inline const std::string & Slot::GetIdentifier(void) const
{
	return m_Identifier;
}

inline Reference< Object > & Slot::GetMountedObject(void)
{
	return m_MountedObject;
}

inline const Reference< Object > & Slot::GetMountedObject(void) const
{
	return m_MountedObject;
}

inline const std::string & Slot::GetName(void) const
{
	return m_Name;
}

inline const Quaternion & Slot::GetOrientation(void) const
{
	return m_Orientation;
}

inline const Vector3f & Slot::GetPosition(void) const
{
	return m_Position;
}

inline const SlotClass * Slot::GetSlotClass(void) const
{
	return m_SlotClass;
}

inline void Slot::SetMountedObject(Reference< Object > MountedObject)
{
	m_MountedObject = MountedObject;
}

inline void Slot::SetName(const std::string & Name)
{
	m_Name = Name;
}

inline void Slot::SetOrientation(const Quaternion & Orientation)
{
	m_Orientation = Orientation;
}

inline void Slot::SetPosition(const Vector3f & Position)
{
	m_Position = Position;
}

#endif
