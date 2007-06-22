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
 * This is part of version 0.9.0 of algebra.
 **/

#ifndef ALGEBRA_VECTOR3F_H
#define ALGEBRA_VECTOR3F_H

#include "details.h"
#include "matrix3f.h"

class Vector3f
{
public:
	Array4f m_V;
	
	Vector3f(void)
	{
	}
	
	Vector3f(bool)
	{
		m_V.m_A[0] = m_V.m_A[1] = m_V.m_A[2] = 0.0f;
	}
	
	Vector3f(const float & Float1, const float & Float2, const float & Float3)
	{
		m_V.m_A[0] = Float1;
		m_V.m_A[1] = Float2;
		m_V.m_A[2] = Float3;
	}
	
	Vector3f(const vector4float & V)
	{
		m_V.m_V = V;
	}
	
	void Set(const float & Float1, const float & Float2, const float & Float3)
	{
		m_V.m_A[0] = Float1;
		m_V.m_A[1] = Float2;
		m_V.m_A[2] = Float3;
	}
	
	float Length(void)
	{
		return sqrtf(m_V.m_A[0] * m_V.m_A[0] + m_V.m_A[1] * m_V.m_A[1] + m_V.m_A[2] * m_V.m_A[2]);
	}
	
	void Normalize(void)
	{
		operator/=(Length());
	}
	
	Vector3f operator+(const Vector3f & Other)
	{
		return Vector3f(m_V.m_A[0] + Other.m_V.m_A[0], m_V.m_A[1] + Other.m_V.m_A[1], m_V.m_A[2] + Other.m_V.m_A[2]);
	}
	
	Vector3f operator-(const Vector3f & Other)
	{
		return Vector3f(m_V.m_A[0] - Other.m_V.m_A[0], m_V.m_A[1] - Other.m_V.m_A[1], m_V.m_A[2] - Other.m_V.m_A[2]);
	}
	
	Vector3f operator*(const float & Scalar)
	{
		return Vector3f(Scalar * m_V.m_A[0], Scalar * m_V.m_A[1], Scalar * m_V.m_A[2]);
	}
	
	Vector3f operator%(const Vector3f & Other)
	{
		return Cross(Other);
	}
	
	Vector3f & operator=(const Vector3f & Other)
	{
		m_V.m_A[0] = Other.m_V.m_A[0];
		m_V.m_A[1] = Other.m_V.m_A[1];
		m_V.m_A[2] = Other.m_V.m_A[2];
		
		return *this;
	}
	
	Vector3f & operator+=(const Vector3f & Other)
	{
		m_V.m_A[0] += Other.m_V.m_A[0];
		m_V.m_A[1] += Other.m_V.m_A[1];
		m_V.m_A[2] += Other.m_V.m_A[2];
		
		return *this;
	}
	
	Vector3f & operator-=(const Vector3f & Other)
	{
		m_V.m_A[0] -= Other.m_V.m_A[0];
		m_V.m_A[1] -= Other.m_V.m_A[1];
		m_V.m_A[2] -= Other.m_V.m_A[2];
		
		return *this;
	}
	
	Vector3f & operator*=(const float & Scalar)
	{
		m_V.m_A[0] *= Scalar;
		m_V.m_A[1] *= Scalar;
		m_V.m_A[2] *= Scalar;
		
		return *this;
	}
	
	Vector3f & operator/=(const float & Scalar)
	{
		m_V.m_A[0] /= Scalar;
		m_V.m_A[1] /= Scalar;
		m_V.m_A[2] /= Scalar;
		
		return *this;
	}

	Vector3f operator+(void) const
	{
		return *this;
	}
	
	Vector3f operator-(void) const
	{
		return Vector3f(-m_V.m_A[0], -m_V.m_A[1], -m_V.m_A[2]);
	}
	
	float Dot(const Vector3f & Other) const
	{
		return m_V.m_A[0] * Other.m_V.m_A[0] + m_V.m_A[1] * Other.m_V.m_A[1] + m_V.m_A[2] * Other.m_V.m_A[2];
	}
	
	Vector3f Cross(const Vector3f & Other) const
	{
		return Vector3f(m_V.m_A[1] * Other.m_V.m_A[2] - m_V.m_A[2] * Other.m_V.m_A[1], m_V.m_A[2] * Other.m_V.m_A[0] - m_V.m_A[0] * Other.m_V.m_A[2], m_V.m_A[0] * Other.m_V.m_A[1] - m_V.m_A[1] * Other.m_V.m_A[0]);
	}
	
	Vector3f & operator*=(const Matrix3f & Matrix)
	{
		float Float1 = (Matrix.m_M[0].m_A[0] * m_V.m_A[0]) + (Matrix.m_M[0].m_A[1] * m_V.m_A[1]) + (Matrix.m_M[0].m_A[2] * m_V.m_A[2]);
		float Float2 = (Matrix.m_M[1].m_A[0] * m_V.m_A[0]) + (Matrix.m_M[1].m_A[1] * m_V.m_A[1]) + (Matrix.m_M[1].m_A[2] * m_V.m_A[2]);
		float Float3 = (Matrix.m_M[2].m_A[0] * m_V.m_A[0]) + (Matrix.m_M[2].m_A[1] * m_V.m_A[1]) + (Matrix.m_M[2].m_A[2] * m_V.m_A[2]);
		
		m_V.m_A[0] = Float1;
		m_V.m_A[1] = Float2;
		m_V.m_A[2] = Float3;
		
		return *this;
	}
};

#endif
