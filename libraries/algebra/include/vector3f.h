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
 * This is part of version 1.5.5 of algebra.
 **/

#ifndef ALGEBRA_VECTOR3F_H
#define ALGEBRA_VECTOR3F_H

#include "matrix3f.h"
#include "quaternion.h"

class Matrix4f;

class Vector3f
{
	friend Matrix4f;
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
	
	Vector3f(const float & X, const float & Y, const float & Z)
	{
		_[0] = X;
		_[1] = Y;
		_[2] = Z;
	}
	
	void Set(const float & X, const float & Y, const float & Z)
	{
		_[0] = X;
		_[1] = Y;
		_[2] = Z;
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
	
	Vector3f & operator*=(const float & Scalar)
	{
		_[0] *= Scalar;
		_[1] *= Scalar;
		_[2] *= Scalar;
		
		return *this;
	}
	
	Vector3f & operator/=(const float & Scalar)
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
	
	Vector3f & operator*=(const Matrix3f & Matrix)
	{
		float X = (Matrix.m_M[0].m_A[0] * _[0]) + (Matrix.m_M[0].m_A[1] * _[1]) + (Matrix.m_M[0].m_A[2] * _[2]);
		float Y = (Matrix.m_M[1].m_A[0] * _[0]) + (Matrix.m_M[1].m_A[1] * _[1]) + (Matrix.m_M[1].m_A[2] * _[2]);
		float Z = (Matrix.m_M[2].m_A[0] * _[0]) + (Matrix.m_M[2].m_A[1] * _[1]) + (Matrix.m_M[2].m_A[2] * _[2]);
		
		_[0] = X;
		_[1] = Y;
		_[2] = Z;
		
		return *this;
	}
	
	Vector3f & operator*=(const Quaternion & AQuaternion)
	{
		Quaternion Result(AQuaternion * Quaternion(0.0f, _[0],_[1], _[2]) * AQuaternion.Conjugated());
		
		_[0] = Result.m_V.m_A[1];
		_[1] = Result.m_V.m_A[2];
		_[2] = Result.m_V.m_A[3];
		
		return *this;
	}
	
	float operator[](int Index) const
	{
		return _[Index];
	}
	
	float & operator[](int Index)
	{
		return _[Index];
	}
};

inline Vector3f operator*(float Scalar, const Vector3f & Vector)
{
	return Vector3f(Scalar * Vector._[0], Scalar * Vector._[1], Scalar * Vector._[2]);
}

#endif
