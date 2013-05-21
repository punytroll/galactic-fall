/**
 * algebra
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

/**
 * This is part of version 1.5.1 of algebra.
 **/

#ifndef ALGEBRA_AXIS_ANGLE_H
#define ALGEBRA_AXIS_ANGLE_H

#include "details.h"

class AxisAngle
{
public:
	Array4f m_V;
	
	AxisAngle(void)
	{
	}
	
	AxisAngle(bool)
	{
		m_V.m_A[0] = m_V.m_A[1] = m_V.m_A[2] = m_V.m_A[3] = 0.0f;
	}
	
	AxisAngle(float X, float Y, float Z, float Angle)
	{
		m_V.m_A[0] = X;
		m_V.m_A[1] = Y;
		m_V.m_A[2] = Z;
		m_V.m_A[3] = Angle;
	}
	
	void Set(float X, float Y, float Z, float Angle)
	{
		m_V.m_A[0] = X;
		m_V.m_A[1] = Y;
		m_V.m_A[2] = Z;
		m_V.m_A[3] = Angle;
	}
	
	float operator[](int Index) const
	{
		return m_V.m_A[Index];
	}
	
	float & operator[](int Index)
	{
		return m_V.m_A[Index];
	}
};

#endif
