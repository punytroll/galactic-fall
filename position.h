/**
 * galactic-fall
 * Copyright (C) 2006  Hagen MÃ¶bius
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

#ifndef POSITION_H
#define POSITION_H

#include <math3d/vector2f.h>

#include "object.h"

class Position : public Object
{
public:
	void SetPosition(const math3d::vector2f & Position);
	const math3d::vector2f & GetPosition(void) const;
	math3d::vector2f m_Position;
};

inline void Position::SetPosition(const math3d::vector2f & Position)
{
	m_Position = Position;
}

inline const math3d::vector2f & Position::GetPosition(void) const
{
	return m_Position;
}

#endif
