/**
 * galactic-fall
 * Copyright (C) 2006  Hagen Möbius
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

#ifndef PHYSICAL_OBJECT_H
#define PHYSICAL_OBJECT_H

#include "position.h"

class PhysicalObject : public Position
{
public:
	virtual ~PhysicalObject(void);
	virtual void Draw(void) const = 0;
	const std::string & GetName(void) const;
	void SetName(const std::string & Name);
	void SetRadialSize(float RadialSize);
	float GetRadialSize(void) const;
private:
	std::string m_Name;
	float m_RadialSize;
};

inline const std::string & PhysicalObject::GetName(void) const
{
	return m_Name;
}

inline void PhysicalObject::SetName(const std::string & Name)
{
	m_Name = Name;
}

inline float PhysicalObject::GetRadialSize(void) const
{
	return m_RadialSize;
}

#endif
