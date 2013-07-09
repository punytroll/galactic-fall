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
 * This is part of version 1.5.3 of algebra.
 **/

#ifndef ALGEBRA_VECTOR2F_H
#define ALGEBRA_VECTOR2F_H

#include <math.h>

#include "details.h"

class Vector2f
{
public:
	Array2f m_V;
	
	Vector2f(void)
	{
	}
	
	Vector2f(bool)
	{
		m_V.m_A[0] = 0.0f;
		m_V.m_A[1] = 0.0f;
	}
	
	Vector2f(float X, float Y)
	{
		m_V.m_A[0] = X;
		m_V.m_A[1] = Y;
	}
	
	static Vector2f CreateFromMagnitudeAndAngle(float Magnitude, float Angle)
	{
		return Vector2f(Magnitude * cos(Angle), Magnitude * sin(Angle));
	}
	
	Vector2f & Set(float Float1, float Float2)
	{
		m_V.m_A[0] = Float1;
		m_V.m_A[1] = Float2;
		
		return *this;
	}
	
	float Length(void) const
	{
		return sqrtf(m_V.m_A[0] * m_V.m_A[0] + m_V.m_A[1] * m_V.m_A[1]);
	}
	
	float SquaredLength(void) const
	{
		return m_V.m_A[0] * m_V.m_A[0] + m_V.m_A[1] * m_V.m_A[1];
	}
	
	Vector2f & Normalize(void)
	{
		return operator/=(Length());
	}
	
	Vector2f Normalized(void) const
	{
		return *this / Length();
	}
	
	Vector2f & Scale(float Scalar)
	{
		m_V.m_A[0] *= Scalar;
		m_V.m_A[1] *= Scalar;
		
		return *this;
	}
	
	Vector2f operator+(const Vector2f & Other) const
	{
		return Vector2f(m_V.m_A[0] + Other.m_V.m_A[0], m_V.m_A[1] + Other.m_V.m_A[1]);
	}
	
	Vector2f operator-(const Vector2f & Other) const
	{
		return Vector2f(m_V.m_A[0] - Other.m_V.m_A[0], m_V.m_A[1] - Other.m_V.m_A[1]);
	}
	
	Vector2f operator*(float Scalar) const
	{
		return Vector2f(Scalar * m_V.m_A[0], Scalar * m_V.m_A[1]);
	}
	
	Vector2f operator/(float Scalar) const
	{
		return Vector2f(m_V.m_A[0] / Scalar, m_V.m_A[1] / Scalar);
	}
	
	Vector2f & operator=(const Vector2f & Other)
	{
		m_V.m_A[0] = Other.m_V.m_A[0];
		m_V.m_A[1] = Other.m_V.m_A[1];
		
		return *this;
	}
	
	Vector2f & operator+=(const Vector2f & Other)
	{
		m_V.m_A[0] += Other.m_V.m_A[0];
		m_V.m_A[1] += Other.m_V.m_A[1];
		
		return *this;
	}
	
	Vector2f & operator-=(const Vector2f & Other)
	{
		m_V.m_A[0] -= Other.m_V.m_A[0];
		m_V.m_A[1] -= Other.m_V.m_A[1];
		
		return *this;
	}
	
	Vector2f & operator*=(float Scalar)
	{
		m_V.m_A[0] *= Scalar;
		m_V.m_A[1] *= Scalar;
		
		return *this;
	}
	
	Vector2f & operator/=(float Scalar)
	{
		m_V.m_A[0] /= Scalar;
		m_V.m_A[1] /= Scalar;
		
		return *this;
	}

	Vector2f operator+(void) const
	{
		return *this;
	}
	
	Vector2f operator-(void) const
	{
		return Vector2f(-m_V.m_A[0], -m_V.m_A[1]);
	}
	
	Vector2f & Negate(void)
	{
		m_V.m_A[0] = -m_V.m_A[0];
		m_V.m_A[1] = -m_V.m_A[1];
		
		return *this;
	}
	
	Vector2f Negated(void)
	{
		return Vector2f(-m_V.m_A[0], -m_V.m_A[1]);
	}
	
	float Dot(const Vector2f & Other) const
	{
		return m_V.m_A[0] * Other.m_V.m_A[0] + m_V.m_A[1] * Other.m_V.m_A[1];
	}
	
	Vector2f & Turn(float Angle)
	{
		float SinAngle(sin(Angle));
		float CosAngle(cos(Angle));
		float fX((CosAngle * m_V.m_A[0]) + (-SinAngle * m_V.m_A[1]));
		float fY((SinAngle * m_V.m_A[0]) + (CosAngle * m_V.m_A[1]));
		
		m_V.m_A[0] = fX;
		m_V.m_A[1] = fY;
		
		return *this;
	}
	
	Vector2f Turned(float Angle) const
	{
		float SinAngle(sin(Angle));
		float CosAngle(cos(Angle));
		
		return Vector2f((CosAngle * m_V.m_A[0]) + (-SinAngle * m_V.m_A[1]), (SinAngle * m_V.m_A[0]) + (CosAngle * m_V.m_A[1]));
	}
	
	float operator[](int Index) const
	{
		return m_V.m_A[Index];
	}
	
	float & operator[](int Index)
	{
		return m_V.m_A[Index];
	}
	
	bool operator==(const Vector2f & Other) const
	{
		return (m_V.m_A[0] == Other.m_V.m_A[0]) && (m_V.m_A[1] == Other.m_V.m_A[1]);
	}
};

inline Vector2f operator*(float Scalar, const Vector2f & Vector)
{
	return Vector2f(Scalar * Vector[0], Scalar * Vector[1]);
}

#endif
