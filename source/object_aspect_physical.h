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

#include <cstdint>

class ObjectAspectPhysical
{
public:
	ObjectAspectPhysical(void);
	// getters
	float GetRadialSize(void) const;
	std::uint32_t GetSpaceRequirement(void) const;
	// setters
	void SetRadialSize(float RadialSize);
	void SetSpaceRequirement(std::uint32_t SpaceRequirement);
private:
	float m_RadialSize;
	std::uint32_t m_SpaceRequirement;
};

inline float ObjectAspectPhysical::GetRadialSize(void) const
{
	return m_RadialSize;
}

inline std::uint32_t ObjectAspectPhysical::GetSpaceRequirement(void) const
{
	return m_SpaceRequirement;
}

inline void ObjectAspectPhysical::SetRadialSize(float RadialSize)
{
	m_RadialSize = RadialSize;
}

inline void ObjectAspectPhysical::SetSpaceRequirement(std::uint32_t SpaceRequirement)
{
	m_SpaceRequirement = SpaceRequirement;
}

#endif
