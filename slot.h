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

class Object;

class Slot
{
public:
	Slot(void);
	// getters
	const Object * GetMountedObject(void) const;
	const Vector3f & GetPosition(void) const;
	const std::string & GetType(void) const;
	// setters
	void SetMountedObject(Object * MountedObject);
	void SetPosition(const Vector3f & Position);
	void SetType(const std::string & Type);
private:
	Object * m_MountedObject;
	Vector3f m_Position;
	std::string m_Type;
};

inline const Object * Slot::GetMountedObject(void) const
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

inline void Slot::SetMountedObject(Object * MountedObject)
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
