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

#include "math/vector3f.h"
#include "referencing.h"

class PhysicalObject;

class Slot
{
public:
	Slot(const std::string & Identifier);
	// getters
	const std::string & GetIdentifier(void) const;
	const Reference< PhysicalObject > & GetMountedObject(void) const;
	Reference< PhysicalObject > & GetMountedObject(void);
	const Vector3f & GetPosition(void) const;
	const std::string & GetType(void) const;
	// setters
	void SetMountedObject(Reference< PhysicalObject > MountedObject);
	void SetPosition(const Vector3f & Position);
	void SetType(const std::string & Type);
private:
	std::string m_Identifier;
	Reference< PhysicalObject > m_MountedObject;
	Vector3f m_Position;
	std::string m_Type;
};

inline const std::string & Slot::GetIdentifier(void) const
{
	return m_Identifier;
}

inline const Reference< PhysicalObject > & Slot::GetMountedObject(void) const
{
	return m_MountedObject;
}

inline Reference< PhysicalObject > & Slot::GetMountedObject(void)
{
	return m_MountedObject;
}

inline const Vector3f & Slot::GetPosition(void) const
{
	return m_Position;
}

inline const std::string & Slot::GetType(void) const
{
	return m_Type;
}

inline void Slot::SetMountedObject(Reference< PhysicalObject > MountedObject)
{
	m_MountedObject = MountedObject;
}

inline void Slot::SetPosition(const Vector3f & Position)
{
	m_Position = Position;
}

inline void Slot::SetType(const std::string & Type)
{
	m_Type = Type;
}

#endif
