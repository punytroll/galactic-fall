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
 * This is part of version 1.2.4 of algebra.
 **/

#ifndef ALGEBRA_VECTOR4D_H
#define ALGEBRA_VECTOR4D_H

#include <math.h>

class Vector4d
{
public:
	double m_V[4];
	
	Vector4d(void)
	{
	}
	
	Vector4d(bool)
	{
		m_V[0] = m_V[1] = m_V[2] = m_V[3] = 0.0;
	}
	
	Vector4d(const double & Value1, const double & Value2, const double & Value3, const double & Value4)
	{
		m_V[0] = Value1;
		m_V[1] = Value2;
		m_V[2] = Value3;
		m_V[3] = Value4;
	}
	
	void Set(const double & Value1, const double & Value2, const double & Value3, const double & Value4)
	{
		m_V[0] = Value1;
		m_V[1] = Value2;
		m_V[2] = Value3;
		m_V[3] = Value4;
	}
	
	double Length(void) const
	{
		return sqrt(m_V[0] * m_V[0] + m_V[1] * m_V[1] + m_V[2] * m_V[2] + m_V[3] * m_V[3]);
	}
	
	double SquaredLength(void) const
	{
		return m_V[0] * m_V[0] + m_V[1] * m_V[1] + m_V[2] * m_V[2] + m_V[3] * m_V[3];
	}
	
	double operator[](int Index) const
	{
		return m_V[Index];
	}
	
	double & operator[](int Index)
	{
		return m_V[Index];
	}
};

inline Vector4d operator*(double Scalar, const Vector4d & Vector)
{
	return Vector4d(Scalar * Vector[0], Scalar * Vector[1], Scalar * Vector[2], Scalar * Vector[3]);
}

#endif
