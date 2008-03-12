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

#ifndef OBJECT_ASPECT_PHYSICAL_H
#define OBJECT_ASPECT_PHYSICAL_H

#include "type_definitions.h"

class ObjectAspectPhysical
{
public:
	ObjectAspectPhysical(void);
	// getters
	float GetRadialSize(void) const;
	unsigned_numeric GetSpaceRequirement(void) const;
	// setters
	void SetRadialSize(float RadialSize);
	void SetSpaceRequirement(unsigned_numeric SpaceRequirement);
private:
	float m_RadialSize;
	unsigned_numeric m_SpaceRequirement;
};

inline float ObjectAspectPhysical::GetRadialSize(void) const
{
	return m_RadialSize;
}

inline unsigned_numeric ObjectAspectPhysical::GetSpaceRequirement(void) const
{
	return m_SpaceRequirement;
}

inline void ObjectAspectPhysical::SetRadialSize(float RadialSize)
{
	m_RadialSize = RadialSize;
}

inline void ObjectAspectPhysical::SetSpaceRequirement(unsigned_numeric SpaceRequirement)
{
	m_SpaceRequirement = SpaceRequirement;
}

#endif
