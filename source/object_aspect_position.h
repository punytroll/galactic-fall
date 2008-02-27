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

#ifndef OBJECT_ASPECT_POSITION_H
#define OBJECT_ASPECT_POSITION_H

#include "math/vector3f.h"

class ObjectAspectPosition
{
public:
	ObjectAspectPosition(void);
	// getters
	const Vector3f & GetPosition(void) const;
	// setters
	void SetPosition(const Vector3f & Position);
private:
	Vector3f m_Position;
};

inline void ObjectAspectPosition::SetPosition(const Vector3f & Position)
{
	m_Position = Position;
}

inline const Vector3f & ObjectAspectPosition::GetPosition(void) const
{
	return m_Position;
}

#endif
