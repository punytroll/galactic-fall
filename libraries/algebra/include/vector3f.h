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
 * This is part of version 1.8.16 of algebra.
 **/

#ifndef ALGEBRA_VECTOR3F_H
#define ALGEBRA_VECTOR3F_H

#include <math.h>

class Matrix3f;
class Matrix4f;
class Quaternion;
class Vector4f;

class Vector3f
{
	friend Matrix4f;
	friend Quaternion;
	friend Vector3f operator*(float Scalar, const Vector3f & Vector);
private:
	float _[3];
public:
	Vector3f(void)
	{
	}
	
	static Vector3f CreateZero(void);
	
	Vector3f(float Value1, float Value2, float Value3)
	{
		_[0] = Value1;
		_[1] = Value2;
		_[2] = Value3;
	}
	
	static Vector3f CreateFromVector4f(const Vector4f & Vector);
	
	Vector3f & Set(float Value1, float Value2, float Value3)
	{
		_[0] = Value1;
		_[1] = Value2;
		_[2] = Value3;
		
		return *this;
	}
	
	float Length(void) const
	{
		return sqrtf(_[0] * _[0] + _[1] * _[1] + _[2] * _[2]);
	}
	
	float SquaredLength(void) const
	{
		return _[0] * _[0] + _[1] * _[1] + _[2] * _[2];
	}
	
	Vector3f & Negate(void);
	
	Vector3f Negated(void) const;
	
	Vector3f & Normalize(void)
	{
		return operator/=(Length());
	}
	
	Vector3f Normalized(void) const
	{
		return *this / Length();
	}
	
	Vector3f operator+(const Vector3f & Other) const
	{
		return Vector3f(_[0] + Other._[0], _[1] + Other._[1], _[2] + Other._[2]);
	}
	
	Vector3f operator-(const Vector3f & Other) const
	{
		return Vector3f(_[0] - Other._[0], _[1] - Other._[1], _[2] - Other._[2]);
	}

	Vector3f operator+(void) const
	{
		return *this;
	}
	
	Vector3f operator-(void) const
	{
		return Vector3f(-_[0], -_[1], -_[2]);
	}
	
	Vector3f operator*(float Scalar) const
	{
		return Vector3f(_[0] * Scalar, _[1] * Scalar, _[2] * Scalar);
	}
	
	Vector3f operator/(float Scalar) const
	{
		return Vector3f(_[0] / Scalar, _[1] / Scalar, _[2] / Scalar);
	}
	
	Vector3f operator%(const Vector3f & Other)
	{
		return Cross(Other);
	}
	
	Vector3f & operator=(const Vector3f & Other)
	{
		_[0] = Other._[0];
		_[1] = Other._[1];
		_[2] = Other._[2];
		
		return *this;
	}
	
	Vector3f & operator+=(const Vector3f & Other)
	{
		_[0] += Other._[0];
		_[1] += Other._[1];
		_[2] += Other._[2];
		
		return *this;
	}
	
	Vector3f & operator-=(const Vector3f & Other)
	{
		_[0] -= Other._[0];
		_[1] -= Other._[1];
		_[2] -= Other._[2];
		
		return *this;
	}
	
	Vector3f & operator*=(float Scalar)
	{
		_[0] *= Scalar;
		_[1] *= Scalar;
		_[2] *= Scalar;
		
		return *this;
	}
	
	Vector3f & operator/=(float Scalar)
	{
		_[0] /= Scalar;
		_[1] /= Scalar;
		_[2] /= Scalar;
		
		return *this;
	}
	
	float Dot(const Vector3f & Other) const
	{
		return _[0] * Other._[0] + _[1] * Other._[1] + _[2] * Other._[2];
	}
	
	Vector3f Cross(const Vector3f & Other) const
	{
		return Vector3f(_[1] * Other._[2] - _[2] * Other._[1], _[2] * Other._[0] - _[0] * Other._[2], _[0] * Other._[1] - _[1] * Other._[0]);
	}
	
	Vector3f & operator*=(const Matrix3f & Matrix);
	
	Vector3f & Transform(const Matrix4f & Matrix);
	
	Vector3f & Rotate(const Quaternion & Quaternion);
	
	const float & operator[](int Index) const
	{
		return _[Index];
	}
	
	float & operator[](int Index)
	{
		return _[Index];
	}
	
	const float * const GetPointer(void) const
	{
		return _;
	}
};

/**
 * IMPLEMENTATION
 **/

#include "matrix3f.h"
#include "matrix4f.h"
#include "quaternion.h"
#include "vector4f.h"

inline Vector3f Vector3f::CreateFromVector4f(const Vector4f & Vector)
{
	Vector3f Result;
	
	Result._[0] = Vector._[0];
	Result._[1] = Vector._[1];
	Result._[2] = Vector._[2];
	
	return Result;
}

inline Vector3f Vector3f::CreateZero(void)
{
	Vector3f Result;
	
	Result._[0] = 0.0f;
	Result._[1] = 0.0f;
	Result._[2] = 0.0f;
	
	return Result;
}

inline Vector3f & Vector3f::Negate(void)
{
	_[0] = -_[0];
	_[1] = -_[1];
	_[2] = -_[2];
	
	return *this;
}

inline Vector3f Vector3f::Negated(void) const
{
	Vector3f Result;
	
	Result._[0] = -_[0];
	Result._[1] = -_[1];
	Result._[2] = -_[2];
	
	return Result;
}

inline Vector3f & Vector3f::operator*=(const Matrix3f & Matrix)
{
	float Value0((Matrix._[0] * _[0]) + (Matrix._[3] * _[1]) + (Matrix._[6] * _[2]));
	float Value1((Matrix._[1] * _[0]) + (Matrix._[4] * _[1]) + (Matrix._[7] * _[2]));
	float Value2((Matrix._[2] * _[0]) + (Matrix._[5] * _[1]) + (Matrix._[8] * _[2]));
	
	_[0] = Value0;
	_[1] = Value1;
	_[2] = Value2;
	
	return *this;
}

inline Vector3f & Vector3f::Transform(const Matrix4f & Matrix)
{
	float Value0((Matrix._[0] * _[0]) + (Matrix._[4] * _[1]) + (Matrix._[8] * _[2]) + Matrix._[12]);
	float Value1((Matrix._[1] * _[0]) + (Matrix._[5] * _[1]) + (Matrix._[9] * _[2]) + Matrix._[13]);
	float Value2((Matrix._[2] * _[0]) + (Matrix._[6] * _[1]) + (Matrix._[10] * _[2]) + Matrix._[14]);
	
	_[0] = Value0;
	_[1] = Value1;
	_[2] = Value2;
	
	return *this;
}

inline Vector3f & Vector3f::Rotate(const Quaternion & Quaternion)
{
	auto Term1(Quaternion._[3] - _[0] * Quaternion._[2] + _[1] * Quaternion._[1] + _[2] * Quaternion._[0]);
	auto Term2(Quaternion._[2] + _[0] * Quaternion._[3] + _[1] * Quaternion._[0] - _[2] * Quaternion._[1]);
	auto Term3(Quaternion._[1] + _[0] * Quaternion._[0] - _[1] * Quaternion._[3] + _[2] * Quaternion._[2]);
	auto Term4(Quaternion._[0] - _[0] * Quaternion._[1] - _[1] * Quaternion._[2] - _[2] * Quaternion._[3]);
	
	_[0] = Quaternion._[2] * Term1 - Quaternion._[3] * Term2 + Quaternion._[0] * Term3 - Quaternion._[1] * Term4;
	_[1] = -Quaternion._[1] * Term1 + Quaternion._[0] * Term2 + Quaternion._[3] * Term3 - Quaternion._[2] * Term4;
	_[2] = Quaternion._[0] * Term1 + Quaternion._[1] * Term2 - Quaternion._[2] * Term3 - Quaternion._[3] * Term4;
	
	return *this;
}

inline Vector3f operator*(float Scalar, const Vector3f & Vector)
{
	return Vector3f(Scalar * Vector._[0], Scalar * Vector._[1], Scalar * Vector._[2]);
}

#endif
