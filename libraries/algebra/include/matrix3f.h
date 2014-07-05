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
 * This is part of version 1.8.5 of algebra.
 **/

#ifndef ALGEBRA_MATRIX3F_H
#define ALGEBRA_MATRIX3F_H

#include <math.h>

#include <utility>

class Vector3f;

class Matrix3f
{
	friend Vector3f;
private:
	float _[9];
public:
	Matrix3f(void)
	{
	}
	
	Matrix3f(bool)
	{
		_[0] = 1.0f;
		_[1] = 0.0f;
		_[2] = 0.0f;
		_[3] = 0.0f;
		_[4] = 1.0f;
		_[5] = 0.0f;
		_[6] = 0.0f;
		_[7] = 0.0f;
		_[8] = 1.0f;
	}
	
	Matrix3f(float Value0, float Value1, float Value2, float Value3, float Value4, float Value5, float Value6, float Value7, float Value8)
	{
		_[0] = Value0;
		_[1] = Value1;
		_[2] = Value2;
		_[3] = Value3;
		_[4] = Value4;
		_[5] = Value5;
		_[6] = Value6;
		_[7] = Value7;
		_[8] = Value8;
	}
	
	static Matrix3f CreateZero(void)
	{
		Matrix3f Result;
		
		Result._[0] = 0.0f;
		Result._[1] = 0.0f;
		Result._[2] = 0.0f;
		Result._[3] = 0.0f;
		Result._[4] = 0.0f;
		Result._[5] = 0.0f;
		Result._[6] = 0.0f;
		Result._[7] = 0.0f;
		Result._[8] = 0.0f;
		
		return Result;
	}
	
	static Matrix3f CreateRotationX(float Angle)
	{
		Matrix3f Result;
		float Cos(cos(Angle));
		float Sin(sin(Angle));
		
		Result._[0] = 1.0f;
		Result._[1] = 0.0f;
		Result._[2] = 0.0f;
		Result._[3] = 0.0f;
		Result._[4] = Cos;
		Result._[5] = Sin;
		Result._[6] = 0.0f;
		Result._[7] = -Sin;
		Result._[8] = Cos;
		
		return Result;
	}
	
	static Matrix3f CreateRotationY(float Angle)
	{
		Matrix3f Result;
		float Cos(cos(Angle));
		float Sin(sin(Angle));
		
		Result._[0] = Cos;
		Result._[1] = 0.0f;
		Result._[2] = -Sin;
		Result._[3] = 0.0f;
		Result._[4] = 1.0f;
		Result._[5] = 0.0f;
		Result._[6] = Sin;
		Result._[7] = 0.0f;
		Result._[8] = Cos;
		
		return Result;
	}
	
	static Matrix3f CreateRotationZ(float Angle)
	{
		Matrix3f Result;
		float Cos(cos(Angle));
		float Sin(sin(Angle));
		
		Result._[0] = Cos;
		Result._[1] = Sin;
		Result._[2] = 0.0f;
		Result._[3] = -Sin;
		Result._[4] = Cos;
		Result._[5] = 0.0f;
		Result._[6] = 0.0f;
		Result._[7] = 0.0f;
		Result._[8] = 1.0f;
		
		return Result;
	}
	
	Matrix3f & operator+=(const Matrix3f & Other)
	{
		_[0] += Other._[0];
		_[1] += Other._[1];
		_[2] += Other._[2];
		_[3] += Other._[3];
		_[4] += Other._[4];
		_[5] += Other._[5];
		_[6] += Other._[6];
		_[7] += Other._[7];
		_[8] += Other._[8];
		
		return *this;
	}
	
	Matrix3f & operator-=(const Matrix3f & Other)
	{
		_[0] -= Other._[0];
		_[1] -= Other._[1];
		_[2] -= Other._[2];
		_[3] -= Other._[3];
		_[4] -= Other._[4];
		_[5] -= Other._[5];
		_[6] -= Other._[6];
		_[7] -= Other._[7];
		_[8] -= Other._[8];
		
		return *this;
	}
	
	Matrix3f & operator*=(const Matrix3f & Other)
	{
		float Value0(_[0] * Other._[0] + _[3] * Other._[1] + _[6] * Other._[2]);
		float Value1(_[1] * Other._[0] + _[4] * Other._[1] + _[7] * Other._[2]);
		float Value2(_[2] * Other._[0] + _[5] * Other._[1] + _[8] * Other._[2]);
		float Value3(_[0] * Other._[3] + _[3] * Other._[4] + _[6] * Other._[5]);
		float Value4(_[1] * Other._[3] + _[4] * Other._[4] + _[7] * Other._[5]);
		float Value5(_[2] * Other._[3] + _[5] * Other._[4] + _[8] * Other._[5]);
		float Value6(_[0] * Other._[6] + _[3] * Other._[7] + _[6] * Other._[8]);
		float Value7(_[1] * Other._[6] + _[4] * Other._[7] + _[7] * Other._[8]);
		float Value8(_[2] * Other._[6] + _[5] * Other._[7] + _[8] * Other._[8]);
		
		_[0] = Value0;
		_[1] = Value1;
		_[2] = Value2;
		_[3] = Value3;
		_[4] = Value4;
		_[5] = Value5;
		_[6] = Value6;
		_[7] = Value7;
		_[8] = Value8;
		
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
	
	const float * const GetPointer(void) const
	{
		return _;
	}
	
	Matrix3f & Identity(void)
	{
		_[0] = 1.0f;
		_[1] = 0.0f;
		_[2] = 0.0f;
		_[3] = 0.0f;
		_[4] = 1.0f;
		_[5] = 0.0f;
		_[6] = 0.0f;
		_[7] = 0.0f;
		_[8] = 1.0f;
		
		return *this;
	}
	
	Matrix3f & Invert(void)
	{
		float Determinant = -_[2] * _[4] * _[6] + _[1] * _[5] * _[6] + _[2] * _[3] * _[7] - _[0] * _[5] * _[7] - _[1] * _[3] * _[8] + _[0] * _[4] * _[8];
		float Value0 = -_[5] * _[7] + _[4] * _[8];
		float Value1 = _[2] * _[7] - _[1] * _[8];
		float Value2 = -_[2] * _[4] + _[1] * _[5];
		float Value3 = _[5] * _[6] - _[3] * _[8];
		float Value4 = -_[2] * _[6] + _[0] * _[8];
		float Value5 = _[2] * _[3] - _[0] * _[5];
		float Value6 = -_[4] * _[6] + _[3] * _[7];
		float Value7 = _[1] * _[6] - _[0] * _[7];
		float Value8 = -_[1] * _[3] + _[0] * _[4];
		
		_[0] = Value0 / Determinant;
		_[1] = Value1 / Determinant;
		_[2] = Value2 / Determinant;
		_[3] = Value3 / Determinant;
		_[4] = Value4 / Determinant;
		_[5] = Value5 / Determinant;
		_[6] = Value6 / Determinant;
		_[7] = Value7 / Determinant;
		_[8] = Value8 / Determinant;
		
		return *this;
	}
	
	Matrix3f & RotationX(float Angle)
	{
		float Cos(cos(Angle));
		float Sin(sin(Angle));
		
		_[0] = 1.0f;
		_[1] = 0.0f;
		_[2] = 0.0f;
		_[3] = 0.0f;
		_[4] = Cos;
		_[5] = Sin;
		_[6] = 0.0f;
		_[7] = -Sin;
		_[8] = Cos;
		
		return *this;
	}
	
	Matrix3f & RotationY(float Angle)
	{
		float Cos(cos(Angle));
		float Sin(sin(Angle));
		
		_[0] = Cos;
		_[1] = 0.0f;
		_[2] = -Sin;
		_[3] = 0.0f;
		_[4] = 1.0f;
		_[5] = 0.0f;
		_[6] = Sin;
		_[7] = 0.0f;
		_[8] = Cos;
		
		return *this;
	}
	
	Matrix3f & RotationZ(float Angle)
	{
		float Cos(cos(Angle));
		float Sin(sin(Angle));
		
		_[0] = Cos;
		_[1] = Sin;
		_[2] = 0.0f;
		_[3] = -Sin;
		_[4] = Cos;
		_[5] = 0.0f;
		_[6] = 0.0f;
		_[7] = 0.0f;
		_[8] = 1.0f;
		
		return *this;
	}
	
	Matrix3f & Transpose(void)
	{
		std::swap(_[1], _[3]);
		std::swap(_[2], _[6]);
		std::swap(_[5], _[7]);
		
		return *this;
	}
	
	Matrix3f & Zero(void)
	{
		_[0] = 0.0f;
		_[1] = 0.0f;
		_[2] = 0.0f;
		_[3] = 0.0f;
		_[4] = 0.0f;
		_[5] = 0.0f;
		_[6] = 0.0f;
		_[7] = 0.0f;
		_[8] = 0.0f;
		
		return *this;
	}
};

#endif
