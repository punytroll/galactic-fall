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
 * This is part of version 1.1.7 of algebra.
 **/

#ifndef ALGEBRA_VECTOR4F_H
#define ALGEBRA_VECTOR4F_H

#include <math.h>

#include "details.h"
#include "quaternion.h"

class Vector4f
{
public:
	Array4f m_V;
	
	Vector4f(void)
	{
	}
	
	Vector4f(bool)
	{
		m_V.m_A[0] = m_V.m_A[1] = m_V.m_A[2] = m_V.m_A[3] = 0.0f;
	}
	
	Vector4f(const float & Float1, const float & Float2, const float & Float3, const float & Float4)
	{
		m_V.m_A[0] = Float1;
		m_V.m_A[1] = Float2;
		m_V.m_A[2] = Float3;
		m_V.m_A[3] = Float4;
	}
	
	Vector4f(const vector4float & V)
	{
		m_V.m_V = V;
	}
	
	void Set(const float & Float1, const float & Float2, const float & Float3, const float & Float4)
	{
		m_V.m_A[0] = Float1;
		m_V.m_A[1] = Float2;
		m_V.m_A[2] = Float3;
		m_V.m_A[3] = Float4;
	}
	
	float Length(void) const
	{
		return sqrtf(m_V.m_A[0] * m_V.m_A[0] + m_V.m_A[1] * m_V.m_A[1] + m_V.m_A[2] * m_V.m_A[2] + m_V.m_A[3] * m_V.m_A[3]);
	}
	
	float SquaredLength(void) const
	{
		return m_V.m_A[0] * m_V.m_A[0] + m_V.m_A[1] * m_V.m_A[1] + m_V.m_A[2] * m_V.m_A[2] + m_V.m_A[3] * m_V.m_A[3];
	}
	
	Vector4f & Normalize(void)
	{
		return operator/=(Length());
	}
	
	Vector4f Normalized(void) const
	{
		return *this / Length();
	}
	
	Vector4f operator+(const Vector4f & Other) const
	{
		return Vector4f(m_V.m_A[0] + Other.m_V.m_A[0], m_V.m_A[1] + Other.m_V.m_A[1], m_V.m_A[2] + Other.m_V.m_A[2], m_V.m_A[3] + Other.m_V.m_A[3]);
	}
	
	Vector4f operator-(const Vector4f & Other) const
	{
		return Vector4f(m_V.m_A[0] - Other.m_V.m_A[0], m_V.m_A[1] - Other.m_V.m_A[1], m_V.m_A[2] - Other.m_V.m_A[2], m_V.m_A[3] - Other.m_V.m_A[3]);
	}
	
	Vector4f operator*(const float & Scalar) const
	{
		return Vector4f(Scalar * m_V.m_A[0], Scalar * m_V.m_A[1], Scalar * m_V.m_A[2], Scalar * m_V.m_A[3]);
	}
	
	Vector4f operator/(const float & Scalar) const
	{
		return Vector4f(m_V.m_A[0] / Scalar, m_V.m_A[1] / Scalar, m_V.m_A[2] / Scalar, m_V.m_A[3] / Scalar);
	}
	
	Vector4f & operator=(const Vector4f & Other)
	{
		m_V.m_A[0] = Other.m_V.m_A[0];
		m_V.m_A[1] = Other.m_V.m_A[1];
		m_V.m_A[2] = Other.m_V.m_A[2];
		m_V.m_A[3] = Other.m_V.m_A[3];
		
		return *this;
	}
	
	Vector4f & operator+=(const Vector4f & Other)
	{
		m_V.m_A[0] += Other.m_V.m_A[0];
		m_V.m_A[1] += Other.m_V.m_A[1];
		m_V.m_A[2] += Other.m_V.m_A[2];
		m_V.m_A[3] += Other.m_V.m_A[3];
		
		return *this;
	}
	
	Vector4f & operator-=(const Vector4f & Other)
	{
		m_V.m_A[0] -= Other.m_V.m_A[0];
		m_V.m_A[1] -= Other.m_V.m_A[1];
		m_V.m_A[2] -= Other.m_V.m_A[2];
		m_V.m_A[3] -= Other.m_V.m_A[3];
		
		return *this;
	}
	
	Vector4f & operator*=(const float & Scalar)
	{
		m_V.m_A[0] *= Scalar;
		m_V.m_A[1] *= Scalar;
		m_V.m_A[2] *= Scalar;
		m_V.m_A[3] *= Scalar;
		
		return *this;
	}
	
	Vector4f & operator/=(const float & Scalar)
	{
		m_V.m_A[0] /= Scalar;
		m_V.m_A[1] /= Scalar;
		m_V.m_A[2] /= Scalar;
		m_V.m_A[3] /= Scalar;
		
		return *this;
	}

	Vector4f operator+(void) const
	{
		return *this;
	}
	
	Vector4f operator-(void) const
	{
		return Vector4f(-m_V.m_A[0], -m_V.m_A[1], -m_V.m_A[2], -m_V.m_A[3]);
	}
	
	float Dot(const Vector4f & Other) const
	{
		return m_V.m_A[0] * Other.m_V.m_A[0] + m_V.m_A[1] * Other.m_V.m_A[1] + m_V.m_A[2] * Other.m_V.m_A[2] + m_V.m_A[3] * Other.m_V.m_A[3];
	}
	
	Vector4f Cross3D(const Vector4f & Other) const
	{
		return Vector4f(m_V.m_A[1] * Other.m_V.m_A[2] - m_V.m_A[2] * Other.m_V.m_A[1], m_V.m_A[2] * Other.m_V.m_A[0] - m_V.m_A[0] * Other.m_V.m_A[2], m_V.m_A[0] * Other.m_V.m_A[1] - m_V.m_A[1] * Other.m_V.m_A[0], 0.0f);
	}
	
	Vector4f & operator*=(const Quaternion & AQuaternion)
	{
		Quaternion Result(AQuaternion * Quaternion(m_V.m_A[3], m_V.m_A[0], m_V.m_A[1], m_V.m_A[2]) * AQuaternion.Conjugated());
		
		m_V.m_A[0] = Result.m_V.m_A[1];
		m_V.m_A[1] = Result.m_V.m_A[2];
		m_V.m_A[2] = Result.m_V.m_A[3];
		m_V.m_A[3] = Result.m_V.m_A[0];
		
		return *this;
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
