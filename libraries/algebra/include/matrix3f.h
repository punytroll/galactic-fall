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
 * This is part of version 1.7.0 of algebra.
 **/

#ifndef ALGEBRA_MATRIX3F_H
#define ALGEBRA_MATRIX3F_H

#include <math.h>

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
	
	void Zero(void)
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
	}
	
	void Identity(void)
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
	
	void RotationX(float Angle)
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
	}
	
	void RotationY(float Angle)
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
	}
	
	void RotationZ(float Angle)
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
};

#endif
