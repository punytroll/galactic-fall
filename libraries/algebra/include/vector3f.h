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
 * This is part of version 1.8.9 of algebra.
 **/

#ifndef ALGEBRA_VECTOR3F_H
#define ALGEBRA_VECTOR3F_H

#include <math.h>

class Matrix3f;
class Matrix4f;
class Quaternion;

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
	
	Vector3f(bool)
	{
		_[0] = 0.0f;
		_[1] = 0.0f;
		_[2] = 0.0f;
	}
	
	Vector3f(float Value1, float Value2, float Value3)
	{
		_[0] = Value1;
		_[1] = Value2;
		_[2] = Value3;
	}
	
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
	
	Vector3f & operator*=(const Quaternion & Quaternion);
	
	Vector3f & Transform(const Quaternion & Quaternion);
	
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
#include "quaternion.h"

inline Vector3f & Vector3f::operator*=(const Matrix3f & Matrix)
{
	float X = (Matrix._[0] * _[0]) + (Matrix._[4] * _[1]) + (Matrix._[6] * _[2]);
	float Y = (Matrix._[1] * _[0]) + (Matrix._[5] * _[1]) + (Matrix._[7] * _[2]);
	float Z = (Matrix._[3] * _[0]) + (Matrix._[6] * _[1]) + (Matrix._[8] * _[2]);
	
	_[0] = X;
	_[1] = Y;
	_[2] = Z;
	
	return *this;
}

inline Vector3f & Vector3f::operator*=(const Quaternion & AQuaternion)
{
	Quaternion Result(AQuaternion * Quaternion(0.0f, _[0],_[1], _[2]) * AQuaternion.Conjugated());
	
	_[0] = Result._[1];
	_[1] = Result._[2];
	_[2] = Result._[3];
	
	return *this;
}

inline Vector3f & Vector3f::Transform(const Quaternion & AQuaternion)
{
	auto Result(AQuaternion * Quaternion(0.0f, _[0],_[1], _[2]) * AQuaternion.Conjugated());
	
	_[0] = Result._[1];
	_[1] = Result._[2];
	_[2] = Result._[3];
	
	return *this;
}

inline Vector3f operator*(float Scalar, const Vector3f & Vector)
{
	return Vector3f(Scalar * Vector._[0], Scalar * Vector._[1], Scalar * Vector._[2]);
}

#endif
