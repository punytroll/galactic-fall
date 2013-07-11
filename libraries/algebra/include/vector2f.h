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
 * This is part of version 1.5.6 of algebra.
 **/

#ifndef ALGEBRA_VECTOR2F_H
#define ALGEBRA_VECTOR2F_H

#include <math.h>

class Vector2f
{
private:
	float _[2];
public:
	Vector2f(void)
	{
	}
	
	Vector2f(bool)
	{
		_[0] = 0.0f;
		_[1] = 0.0f;
	}
	
	Vector2f(float Value1, float Value2)
	{
		_[0] = Value1;
		_[1] = Value2;
	}
	
	static Vector2f CreateFromMagnitudeAndAngle(float Magnitude, float Angle)
	{
		return Vector2f(Magnitude * cos(Angle), Magnitude * sin(Angle));
	}
	
	Vector2f & Set(float Value1, float Value2)
	{
		_[0] = Value1;
		_[1] = Value2;
		
		return *this;
	}
	
	float Length(void) const
	{
		return sqrtf(_[0] * _[0] + _[1] * _[1]);
	}
	
	float SquaredLength(void) const
	{
		return _[0] * _[0] + _[1] * _[1];
	}
	
	Vector2f & Normalize(void)
	{
		float L(sqrtf(_[0] * _[0] + _[1] * _[1]));
		
		_[0] /= L;
		_[1] /= L;
		
		return *this;
	}
	
	Vector2f Normalized(void) const
	{
		float L(sqrtf(_[0] * _[0] + _[1] * _[1]));
		
		return Vector2f(_[0] / L, _[1] / L);
	}
	
	Vector2f & Scale(float Scalar)
	{
		_[0] *= Scalar;
		_[1] *= Scalar;
		
		return *this;
	}
	
	Vector2f Scaled(float Scalar)
	{
		return Vector2f(_[0] * Scalar, _[1] * Scalar);
	}
	
	Vector2f operator+(const Vector2f & Other) const
	{
		return Vector2f(_[0] + Other._[0], _[1] + Other._[1]);
	}
	
	Vector2f operator-(const Vector2f & Other) const
	{
		return Vector2f(_[0] - Other._[0], _[1] - Other._[1]);
	}
	
	Vector2f operator*(float Scalar) const
	{
		return Vector2f(_[0] * Scalar, _[1] * Scalar);
	}
	
	Vector2f operator/(float Scalar) const
	{
		return Vector2f(_[0] / Scalar, _[1] / Scalar);
	}
	
	Vector2f & operator=(const Vector2f & Other)
	{
		_[0] = Other._[0];
		_[1] = Other._[1];
		
		return *this;
	}
	
	Vector2f & operator+=(const Vector2f & Other)
	{
		_[0] += Other._[0];
		_[1] += Other._[1];
		
		return *this;
	}
	
	Vector2f & operator-=(const Vector2f & Other)
	{
		_[0] -= Other._[0];
		_[1] -= Other._[1];
		
		return *this;
	}
	
	Vector2f & operator*=(float Scalar)
	{
		_[0] *= Scalar;
		_[1] *= Scalar;
		
		return *this;
	}
	
	Vector2f & operator/=(float Scalar)
	{
		_[0] /= Scalar;
		_[1] /= Scalar;
		
		return *this;
	}

	Vector2f operator+(void) const
	{
		return *this;
	}
	
	Vector2f operator-(void) const
	{
		return Vector2f(-_[0], -_[1]);
	}
	
	Vector2f & Negate(void)
	{
		_[0] = -_[0];
		_[1] = -_[1];
		
		return *this;
	}
	
	Vector2f Negated(void)
	{
		return Vector2f(-_[0], -_[1]);
	}
	
	float Dot(const Vector2f & Other) const
	{
		return _[0] * Other._[0] + _[1] * Other._[1];
	}
	
	Vector2f & Turn(float Angle)
	{
		float SinAngle(sin(Angle));
		float CosAngle(cos(Angle));
		float fX((CosAngle * _[0]) + (-SinAngle * _[1]));
		float fY((SinAngle * _[0]) + (CosAngle * _[1]));
		
		_[0] = fX;
		_[1] = fY;
		
		return *this;
	}
	
	Vector2f Turned(float Angle) const
	{
		float SinAngle(sin(Angle));
		float CosAngle(cos(Angle));
		
		return Vector2f((CosAngle * _[0]) + (-SinAngle * _[1]), (SinAngle * _[0]) + (CosAngle * _[1]));
	}
	
	float operator[](int Index) const
	{
		return _[Index];
	}
	
	float & operator[](int Index)
	{
		return _[Index];
	}
	
	bool operator==(const Vector2f & Other) const
	{
		return (_[0] == Other._[0]) && (_[1] == Other._[1]);
	}
};

inline Vector2f operator*(float Scalar, const Vector2f & Vector)
{
	return Vector2f(Scalar * Vector[0], Scalar * Vector[1]);
}

#endif
