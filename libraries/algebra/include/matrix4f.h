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

#ifndef ALGEBRA_MATRIX4F_H
#define ALGEBRA_MATRIX4F_H

#include <math.h>

#include <utility>

class Quaternion;
class Vector3f;
class Vector4f;

class Matrix4f
{
private:
	float _[16];
public:
	Matrix4f(void)
	{
	}
	
	Matrix4f(bool)
	{
		_[0] = 1.0f;
		_[1] = 0.0f;
		_[2] = 0.0f;
		_[3] = 0.0f;
		_[4] = 0.0f;
		_[5] = 1.0f;
		_[6] = 0.0f;
		_[7] = 0.0f;
		_[8] = 0.0f;
		_[9] = 0.0f;
		_[10] = 1.0f;
		_[11] = 0.0f;
		_[12] = 0.0f;
		_[13] = 0.0f;
		_[14] = 0.0f;
		_[15] = 1.0f;
	}
	
	static Matrix4f CreateFromTranslationComponents(float X, float Y, float Z)
	{
		Matrix4f Result;
		
		Result._[0] = 1.0f;
		Result._[1] = 0.0f;
		Result._[2] = 0.0f;
		Result._[3] = 0.0f;
		Result._[4] = 0.0f;
		Result._[5] = 1.0f;
		Result._[6] = 0.0f;
		Result._[7] = 0.0f;
		Result._[8] = 0.0f;
		Result._[9] = 0.0f;
		Result._[10] = 1.0f;
		Result._[11] = 0.0f;
		Result._[12] = X;
		Result._[13] = Y;
		Result._[14] = Z;
		Result._[15] = 1.0f;
		
		return Result;
	}
	
	static Matrix4f CreateFromTranslationVector(const Vector3f & TranslationVector);
	
	static Matrix4f CreateFromTranslationVector(const Vector4f & TranslationVector);
	
	static Matrix4f CreateFromQuaternion(const Quaternion & Quaternion);
	
	static Matrix4f CreateRotationX(float Angle);
	
	static Matrix4f CreateRotationY(float Angle);
	
	static Matrix4f CreateRotationZ(float Angle);
	
	const float * const GetPointer(void) const
	{
		return _;
	}
	
	Matrix4f & Identity(void)
	{
		_[0] = 1.0f;
		_[1] = 0.0f;
		_[2] = 0.0f;
		_[3] = 0.0f;
		_[4] = 0.0f;
		_[5] = 1.0f;
		_[6] = 0.0f;
		_[7] = 0.0f;
		_[8] = 0.0f;
		_[9] = 0.0f;
		_[10] = 1.0f;
		_[11] = 0.0f;
		_[12] = 0.0f;
		_[13] = 0.0f;
		_[14] = 0.0f;
		_[15] = 1.0f;
		
		return *this;
	}
	
	Matrix4f & Invert(void)
	{
		float Determinant = _[12] * _[9] * _[6] * _[3] - _[8] * _[13] * _[6] * _[3] - _[12] * _[5] * _[10] * _[3] + _[4] * _[13] * _[10] * _[3] + _[8] * _[5] * _[14] * _[3] - _[4] * _[9] * _[14] * _[3] - _[12] * _[9] * _[2] * _[7] + _[8] * _[13] * _[2] * _[7] + _[12] * _[1] * _[10] * _[7] - _[0] * _[13] * _[10] * _[7] - _[8] * _[1] * _[14] * _[7] + _[0] * _[9] * _[14] * _[7] + _[12] * _[5] * _[2] * _[11] - _[4] * _[13] * _[2] * _[11] - _[12] * _[1] * _[6] * _[11] + _[0] * _[13] * _[6] * _[11] + _[4] * _[1] * _[14] * _[11] - _[0] * _[5] * _[14] * _[11] - _[8] * _[5] * _[2] * _[15] + _[4] * _[9] * _[2] * _[15] + _[8] * _[1] * _[6] * _[15] - _[0] * _[9] * _[6] * _[15] - _[4] * _[1] * _[10] * _[15] + _[0] * _[5] * _[10] * _[15];
		float Value0 = -_[13] * _[10] * _[7] + _[9] * _[14] * _[7] + _[13] * _[6] * _[11] - _[5] * _[14] * _[11] - _[9] * _[6] * _[15] + _[5] * _[10] * _[15];
		float Value1 = _[13] * _[10] * _[3] - _[9] * _[14] * _[3] - _[13] * _[2] * _[11] + _[1] * _[14] * _[11] + _[9] * _[2] * _[15] - _[1] * _[10] * _[15];
		float Value2 = -_[13] * _[6] * _[3] + _[5] * _[14] * _[3] + _[13] * _[2] * _[7] - _[1] * _[14] * _[7] - _[5] * _[2] * _[15] + _[1] * _[6] * _[15];
		float Value3 = _[9] * _[6] * _[3] - _[5] * _[10] * _[3] - _[9] * _[2] * _[7] + _[1] * _[10] * _[7] + _[5] * _[2] * _[11] - _[1] * _[6] * _[11];
		float Value4 = _[12] * _[10] * _[7] - _[8] * _[14] * _[7] - _[12] * _[6] * _[11] + _[4] * _[14] * _[11] + _[8] * _[6] * _[15] - _[4] * _[10] * _[15];
		float Value5 = -_[12] * _[10] * _[3] + _[8] * _[14] * _[3] + _[12] * _[2] * _[11] - _[0] * _[14] * _[11] - _[8] * _[2] * _[15] + _[0] * _[10] * _[15];
		float Value6 = _[12] * _[6] * _[3] - _[4] * _[14] * _[3] - _[12] * _[2] * _[7] + _[0] * _[14] * _[7] + _[4] * _[2] * _[15] - _[0] * _[6] * _[15];
		float Value7 = -_[8] * _[6] * _[3] + _[4] * _[10] * _[3] + _[8] * _[2] * _[7] - _[0] * _[10] * _[7] - _[4] * _[2] * _[11] + _[0] * _[6] * _[11];
		float Value8 = -_[12] * _[9] * _[7] + _[8] * _[13] * _[7] + _[12] * _[5] * _[11] - _[4] * _[13] * _[11] - _[8] * _[5] * _[15] + _[4] * _[9] * _[15];
		float Value9 = _[12] * _[9] * _[3] - _[8] * _[13] * _[3] - _[12] * _[1] * _[11] + _[0] * _[13] * _[11] + _[8] * _[1] * _[15] - _[0] * _[9] * _[15];
		float Value10 = -_[12] * _[5] * _[3] + _[4] * _[13] * _[3] + _[12] * _[1] * _[7] - _[0] * _[13] * _[7] - _[4] * _[1] * _[15] + _[0] * _[5] * _[15];
		float Value11 = _[8] * _[5] * _[3] - _[4] * _[9] * _[3] - _[8] * _[1] * _[7] + _[0] * _[9] * _[7] + _[4] * _[1] * _[11] - _[0] * _[5] * _[11];
		float Value12 = _[12] * _[9] * _[6] - _[8] * _[13] * _[6] - _[12] * _[5] * _[10] + _[4] * _[13] * _[10] + _[8] * _[5] * _[14] - _[4] * _[9] * _[14];
		float Value13 = -_[12] * _[9] * _[2] + _[8] * _[13] * _[2] + _[12] * _[1] * _[10] - _[0] * _[13] * _[10] - _[8] * _[1] * _[14] + _[0] * _[9] * _[14];
		float Value14 = _[12] * _[5] * _[2] - _[4] * _[13] * _[2] - _[12] * _[1] * _[6] + _[0] * _[13] * _[6] + _[4] * _[1] * _[14] - _[0] * _[5] * _[14];
		float Value15 = -_[8] * _[5] * _[2] + _[4] * _[9] * _[2] + _[8] * _[1] * _[6] - _[0] * _[9] * _[6] - _[4] * _[1] * _[10] + _[0] * _[5] * _[10];
		
		_[0] = Value0 / Determinant;
		_[1] = Value1 / Determinant;
		_[2] = Value2 / Determinant;
		_[3] = Value3 / Determinant;
		_[4] = Value4 / Determinant;
		_[5] = Value5 / Determinant;
		_[6] = Value6 / Determinant;
		_[7] = Value7 / Determinant;
		_[8] = Value8 / Determinant;
		_[9] = Value9 / Determinant;
		_[10] = Value10 / Determinant;
		_[11] = Value11 / Determinant;
		_[12] = Value12 / Determinant;
		_[13] = Value13 / Determinant;
		_[14] = Value14 / Determinant;
		_[15] = Value15 / Determinant;
		
		return *this;
	}
	
	Matrix4f Inverted(void) const
	{
		float Determinant = _[12] * _[9] * _[6] * _[3] - _[8] * _[13] * _[6] * _[3] - _[12] * _[5] * _[10] * _[3] + _[4] * _[13] * _[10] * _[3] + _[8] * _[5] * _[14] * _[3] - _[4] * _[9] * _[14] * _[3] - _[12] * _[9] * _[2] * _[7] + _[8] * _[13] * _[2] * _[7] + _[12] * _[1] * _[10] * _[7] - _[0] * _[13] * _[10] * _[7] - _[8] * _[1] * _[14] * _[7] + _[0] * _[9] * _[14] * _[7] + _[12] * _[5] * _[2] * _[11] - _[4] * _[13] * _[2] * _[11] - _[12] * _[1] * _[6] * _[11] + _[0] * _[13] * _[6] * _[11] + _[4] * _[1] * _[14] * _[11] - _[0] * _[5] * _[14] * _[11] - _[8] * _[5] * _[2] * _[15] + _[4] * _[9] * _[2] * _[15] + _[8] * _[1] * _[6] * _[15] - _[0] * _[9] * _[6] * _[15] - _[4] * _[1] * _[10] * _[15] + _[0] * _[5] * _[10] * _[15];
		Matrix4f Result;
		
		Result._[0] = (-_[13] * _[10] * _[7] + _[9] * _[14] * _[7] + _[13] * _[6] * _[11] - _[5] * _[14] * _[11] - _[9] * _[6] * _[15] + _[5] * _[10] * _[15]) / Determinant;
		Result._[1] = (_[13] * _[10] * _[3] - _[9] * _[14] * _[3] - _[13] * _[2] * _[11] + _[1] * _[14] * _[11] + _[9] * _[2] * _[15] - _[1] * _[10] * _[15]) / Determinant;
		Result._[2] = (-_[13] * _[6] * _[3] + _[5] * _[14] * _[3] + _[13] * _[2] * _[7] - _[1] * _[14] * _[7] - _[5] * _[2] * _[15] + _[1] * _[6] * _[15]) / Determinant;
		Result._[3] = (_[9] * _[6] * _[3] - _[5] * _[10] * _[3] - _[9] * _[2] * _[7] + _[1] * _[10] * _[7] + _[5] * _[2] * _[11] - _[1] * _[6] * _[11]) / Determinant;
		Result._[4] = (_[12] * _[10] * _[7] - _[8] * _[14] * _[7] - _[12] * _[6] * _[11] + _[4] * _[14] * _[11] + _[8] * _[6] * _[15] - _[4] * _[10] * _[15]) / Determinant;
		Result._[5] = (-_[12] * _[10] * _[3] + _[8] * _[14] * _[3] + _[12] * _[2] * _[11] - _[0] * _[14] * _[11] - _[8] * _[2] * _[15] + _[0] * _[10] * _[15]) / Determinant;
		Result._[6] = (_[12] * _[6] * _[3] - _[4] * _[14] * _[3] - _[12] * _[2] * _[7] + _[0] * _[14] * _[7] + _[4] * _[2] * _[15] - _[0] * _[6] * _[15]) / Determinant;
		Result._[7] = (-_[8] * _[6] * _[3] + _[4] * _[10] * _[3] + _[8] * _[2] * _[7] - _[0] * _[10] * _[7] - _[4] * _[2] * _[11] + _[0] * _[6] * _[11]) / Determinant;
		Result._[8] = (-_[12] * _[9] * _[7] + _[8] * _[13] * _[7] + _[12] * _[5] * _[11] - _[4] * _[13] * _[11] - _[8] * _[5] * _[15] + _[4] * _[9] * _[15]) / Determinant;
		Result._[9] = (_[12] * _[9] * _[3] - _[8] * _[13] * _[3] - _[12] * _[1] * _[11] + _[0] * _[13] * _[11] + _[8] * _[1] * _[15] - _[0] * _[9] * _[15]) / Determinant;
		Result._[10] = (-_[12] * _[5] * _[3] + _[4] * _[13] * _[3] + _[12] * _[1] * _[7] - _[0] * _[13] * _[7] - _[4] * _[1] * _[15] + _[0] * _[5] * _[15]) / Determinant;
		Result._[11] = (_[8] * _[5] * _[3] - _[4] * _[9] * _[3] - _[8] * _[1] * _[7] + _[0] * _[9] * _[7] + _[4] * _[1] * _[11] - _[0] * _[5] * _[11]) / Determinant;
		Result._[12] = (_[12] * _[9] * _[6] - _[8] * _[13] * _[6] - _[12] * _[5] * _[10] + _[4] * _[13] * _[10] + _[8] * _[5] * _[14] - _[4] * _[9] * _[14]) / Determinant;
		Result._[13] = (-_[12] * _[9] * _[2] + _[8] * _[13] * _[2] + _[12] * _[1] * _[10] - _[0] * _[13] * _[10] - _[8] * _[1] * _[14] + _[0] * _[9] * _[14]) / Determinant;
		Result._[14] = (_[12] * _[5] * _[2] - _[4] * _[13] * _[2] - _[12] * _[1] * _[6] + _[0] * _[13] * _[6] + _[4] * _[1] * _[14] - _[0] * _[5] * _[14]) / Determinant;
		Result._[15] = (-_[8] * _[5] * _[2] + _[4] * _[9] * _[2] + _[8] * _[1] * _[6] - _[0] * _[9] * _[6] - _[4] * _[1] * _[10] + _[0] * _[5] * _[10]) / Determinant;
		
		return Result;
	}
	
	Matrix4f & Rotate(const Quaternion & Quaternion);
	
	Matrix4f & RotateX(float Radians)
	{
		float Sin(sin(Radians));
		float Cos(cos(Radians));
		float Value8(Cos * _[8] - Sin * _[4]);
		float Value9(Cos * _[9] - Sin * _[5]);
		float Value10(Cos * _[10] - Sin * _[6]);
		float Value11(Cos * _[11] - Sin * _[7]);
		
		_[4] = Cos * _[4] + Sin * _[8];
		_[5] = Cos * _[5] + Sin * _[9];
		_[6] = Cos * _[6] + Sin * _[10];
		_[7] = Cos * _[7] + Sin * _[11];
		_[8] = Value8;
		_[9] = Value9;
		_[10] = Value10;
		_[11] = Value11;
		
		return *this;
	}
	
	Matrix4f & RotateY(float Radians)
	{
		float Sin(sin(Radians));
		float Cos(cos(Radians));
		float Value8(Cos * _[8] + Sin * _[0]);
		float Value9(Cos * _[9] + Sin * _[1]);
		float Value10(Cos * _[10] + Sin * _[2]);
		float Value11(Cos * _[11] + Sin * _[3]);
		
		_[0] = Cos * _[0] - Sin * _[8];
		_[1] = Cos * _[1] - Sin * _[9];
		_[2] = Cos * _[2] - Sin * _[10];
		_[3] = Cos * _[3] - Sin * _[11];
		_[8] = Value8;
		_[9] = Value9;
		_[10] = Value10;
		_[11] = Value11;
		
		return *this;
	}
	
	Matrix4f & RotateZ(float Radians)
	{
		float Sin(sin(Radians));
		float Cos(cos(Radians));
		float Value4(Cos * _[4] - Sin * _[0]);
		float Value5(Cos * _[5] - Sin * _[1]);
		float Value6(Cos * _[6] - Sin * _[2]);
		float Value7(Cos * _[7] - Sin * _[3]);
		
		_[0] = Cos * _[0] + Sin * _[4];
		_[1] = Cos * _[1] + Sin * _[5];
		_[2] = Cos * _[2] + Sin * _[6];
		_[3] = Cos * _[3] + Sin * _[7];
		_[4] = Value4;
		_[5] = Value5;
		_[6] = Value6;
		_[7] = Value7;
		
		return *this;
	}
	
	Matrix4f & Scale(const Vector3f & Scale);
	
	Matrix4f & Transform(const Matrix4f & Other)
	{
		float Value0(_[0] * Other._[0] + _[4] * Other._[1] + _[8] * Other._[2] + _[12] * Other._[3]);
		float Value1(_[1] * Other._[0] + _[5] * Other._[1] + _[9] * Other._[2] + _[13] * Other._[3]);
		float Value2(_[2] * Other._[0] + _[6] * Other._[1] + _[10] * Other._[2] + _[14] * Other._[3]);
		float Value3(_[3] * Other._[0] + _[7] * Other._[1] + _[11] * Other._[2] + _[15] * Other._[3]);
		float Value4(_[0] * Other._[4] + _[4] * Other._[5] + _[8] * Other._[6] + _[12] * Other._[7]);
		float Value5(_[1] * Other._[4] + _[5] * Other._[5] + _[9] * Other._[6] + _[13] * Other._[7]);
		float Value6(_[2] * Other._[4] + _[6] * Other._[5] + _[10] * Other._[6] + _[14] * Other._[7]);
		float Value7(_[3] * Other._[4] + _[7] * Other._[5] + _[11] * Other._[6] + _[15] * Other._[7]);
		float Value8(_[0] * Other._[8] + _[4] * Other._[9] + _[8] * Other._[10] + _[12] * Other._[11]);
		float Value9(_[1] * Other._[8] + _[5] * Other._[9] + _[9] * Other._[10] + _[13] * Other._[11]);
		float Value11(_[3] * Other._[8] + _[7] * Other._[9] + _[11] * Other._[10] + _[15] * Other._[11]);
		float Value10(_[2] * Other._[8] + _[6] * Other._[9] + _[10] * Other._[10] + _[14] * Other._[11]);
		float Value12(_[0] * Other._[12] + _[4] * Other._[13] + _[8] * Other._[14] + _[12] * Other._[15]);
		float Value13(_[1] * Other._[12] + _[5] * Other._[13] + _[9] * Other._[14] + _[13] * Other._[15]);
		float Value14(_[2] * Other._[12] + _[6] * Other._[13] + _[10] * Other._[14] + _[14] * Other._[15]);
		float Value15(_[3] * Other._[12] + _[7] * Other._[13] + _[11] * Other._[14] + _[15] * Other._[15]);
		
		_[0] = Value0;
		_[1] = Value1;
		_[2] = Value2;
		_[3] = Value3;
		_[4] = Value4;
		_[5] = Value5;
		_[6] = Value6;
		_[7] = Value7;
		_[8] = Value8;
		_[9] = Value9;
		_[10] = Value10;
		_[11] = Value11;
		_[12] = Value12;
		_[13] = Value13;
		_[14] = Value14;
		_[15] = Value15;
		
		return *this;
	}
	
	Matrix4f & Translate(float X, float Y, float Z)
	{
		_[12] += _[0] * X + _[4] * Y + _[8] * Z;
		_[13] += _[1] * X + _[5] * Y + _[9] * Z;
		_[14] += _[2] * X + _[6] * Y + _[10] * Z;
		_[15] += _[3] * X + _[7] * Y + _[11] * Z;
		
		return *this;
	}
	
	Matrix4f & Translate(const Vector3f & Translation);
	
	Matrix4f & Translate(const Vector4f & Translation);
	
	Matrix4f Translated(const Vector3f & Translation) const;
	
	Matrix4f Translated(const Vector4f & Translation) const;
	
	void Translation(float X, float Y, float Z)
	{
		_[0] = 1.0f;
		_[1] = 0.0f;
		_[2] = 0.0f;
		_[3] = 0.0f;
		_[4] = 0.0f;
		_[5] = 1.0f;
		_[6] = 0.0f;
		_[7] = 0.0f;
		_[8] = 0.0f;
		_[9] = 0.0f;
		_[10] = 1.0f;
		_[11] = 0.0f;
		_[12] = X;
		_[13] = Y;
		_[14] = Z;
		_[15] = 1.0f;
	}
	
	Matrix4f & Transpose(void)
	{
		std::swap(_[1], _[4]);
		std::swap(_[2], _[8]);
		std::swap(_[3], _[12]);
		std::swap(_[6], _[9]);
		std::swap(_[7], _[13]);
		std::swap(_[11], _[14]);
		
		return *this;
	}
	
	Matrix4f Transposed(void) const
	{
		Matrix4f Result;
		
		Result._[0] = _[0];
		Result._[1] = _[4];
		Result._[2] = _[8];
		Result._[3] = _[12];
		Result._[4] = _[1];
		Result._[5] = _[5];
		Result._[6] = _[9];
		Result._[7] = _[13];
		Result._[8] = _[2];
		Result._[9] = _[6];
		Result._[10] = _[10];
		Result._[11] = _[14];
		Result._[12] = _[3];
		Result._[13] = _[7];
		Result._[14] = _[11];
		Result._[15] = _[15];
		
		return Result;
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

/**
 * IMPLEMENTATION
 **/

#include "quaternion.h"
#include "vector3f.h"
#include "vector4f.h"

inline Matrix4f Matrix4f::CreateFromTranslationVector(const Vector3f & TranslationVector)
{
	Matrix4f Result;
	
	Result._[0] = 1.0f;
	Result._[1] = 0.0f;
	Result._[2] = 0.0f;
	Result._[3] = 0.0f;
	Result._[4] = 0.0f;
	Result._[5] = 1.0f;
	Result._[6] = 0.0f;
	Result._[7] = 0.0f;
	Result._[8] = 0.0f;
	Result._[9] = 0.0f;
	Result._[10] = 1.0f;
	Result._[11] = 0.0f;
	Result._[12] = TranslationVector._[0];
	Result._[13] = TranslationVector._[1];
	Result._[14] = TranslationVector._[2];
	Result._[15] = 1.0f;
	
	return Result;
}

inline Matrix4f Matrix4f::CreateFromTranslationVector(const Vector4f & TranslationVector)
{
	Matrix4f Result;
	
	Result._[0] = 1.0f;
	Result._[1] = 0.0f;
	Result._[2] = 0.0f;
	Result._[3] = 0.0f;
	Result._[4] = 0.0f;
	Result._[5] = 1.0f;
	Result._[6] = 0.0f;
	Result._[7] = 0.0f;
	Result._[8] = 0.0f;
	Result._[9] = 0.0f;
	Result._[10] = 1.0f;
	Result._[11] = 0.0f;
	Result._[12] = TranslationVector._[0];
	Result._[13] = TranslationVector._[1];
	Result._[14] = TranslationVector._[2];
	Result._[15] = 1.0f;
	
	return Result;
}

inline Matrix4f Matrix4f::CreateFromQuaternion(const Quaternion & Quaternion)
{
	Matrix4f Result;
	float WX(Quaternion._[0] * Quaternion._[1]);
	float WY(Quaternion._[0] * Quaternion._[2]);
	float WZ(Quaternion._[0] * Quaternion._[3]);
	float XX(Quaternion._[1] * Quaternion._[1]);
	float XY(Quaternion._[1] * Quaternion._[2]);
	float XZ(Quaternion._[1] * Quaternion._[3]);
	float YY(Quaternion._[2] * Quaternion._[2]);
	float YZ(Quaternion._[2] * Quaternion._[3]);
	float ZZ(Quaternion._[3] * Quaternion._[3]);
	
	Result._[0] = 1.0f - 2.0f * (YY + ZZ);
	Result._[1] = 2.0f * (XY + WZ);
	Result._[2] = 2.0f * (XZ - WY);
	Result._[3] = 0.0f;
	Result._[4] = 2.0f * (XY - WZ);
	Result._[5] = 1.0f - 2.0f * (XX + ZZ);
	Result._[6] = 2.0f * (YZ + WX);
	Result._[7] = 0.0f;
	Result._[8] = 2.0f * (XZ + WY);
	Result._[9] = 2.0f * (YZ - WX);
	Result._[10] = 1.0f - 2.0f * (XX + YY);
	Result._[11] = 0.0f;
	Result._[12] = 0.0f;
	Result._[13] = 0.0f;
	Result._[14] = 0.0f;
	Result._[15] = 1.0f;
	
	return Result;
}
	
inline Matrix4f Matrix4f::CreateRotationX(float Angle)
{
	Matrix4f Result;
	float Cos(cos(Angle));
	float Sin(sin(Angle));
	
	Result._[0] = 1.0f;
	Result._[1] = 0.0f;
	Result._[2] = 0.0f;
	Result._[3] = 0.0f;
	Result._[4] = 0.0f;
	Result._[5] = Cos;
	Result._[6] = Sin;
	Result._[7] = 0.0f;
	Result._[8] = 0.0f;
	Result._[9] = -Sin;
	Result._[10] = Cos;
	Result._[11] = 0.0f;
	Result._[12] = 0.0f;
	Result._[13] = 0.0f;
	Result._[14] = 0.0f;
	Result._[15] = 1.0f;
	
	return Result;
}

inline Matrix4f Matrix4f::CreateRotationY(float Angle)
{
	Matrix4f Result;
	float Cos(cos(Angle));
	float Sin(sin(Angle));
	
	Result._[0] = Cos;
	Result._[1] = 0.0f;
	Result._[2] = -Sin;
	Result._[3] = 0.0f;
	Result._[4] = 0.0f;
	Result._[5] = 1.0f;
	Result._[6] = 0.0f;
	Result._[7] = 0.0f;
	Result._[8] = Sin;
	Result._[9] = 0.0f;
	Result._[10] = Cos;
	Result._[11] = 0.0f;
	Result._[12] = 0.0f;
	Result._[13] = 0.0f;
	Result._[14] = 0.0f;
	Result._[15] = 1.0f;
	
	return Result;
}

inline Matrix4f Matrix4f::CreateRotationZ(float Angle)
{
	Matrix4f Result;
	float Cos(cos(Angle));
	float Sin(sin(Angle));
	
	Result._[0] = Cos;
	Result._[1] = Sin;
	Result._[2] = 0.0f;
	Result._[3] = 0.0f;
	Result._[4] = -Sin;
	Result._[5] = Cos;
	Result._[6] = 0.0f;
	Result._[7] = 0.0f;
	Result._[8] = 0.0f;
	Result._[9] = 0.0f;
	Result._[10] = 1.0f;
	Result._[11] = 0.0f;
	Result._[12] = 0.0f;
	Result._[13] = 0.0f;
	Result._[14] = 0.0f;
	Result._[15] = 1.0f;
	
	return Result;
}

inline Matrix4f & Matrix4f::Rotate(const Quaternion & Quaternion)
{
	float WX(Quaternion._[0] * Quaternion._[1]);
	float WY(Quaternion._[0] * Quaternion._[2]);
	float WZ(Quaternion._[0] * Quaternion._[3]);
	float XX(Quaternion._[1] * Quaternion._[1]);
	float XY(Quaternion._[1] * Quaternion._[2]);
	float XZ(Quaternion._[1] * Quaternion._[3]);
	float YY(Quaternion._[2] * Quaternion._[2]);
	float YZ(Quaternion._[2] * Quaternion._[3]);
	float ZZ(Quaternion._[3] * Quaternion._[3]);
	float Value4(_[4] + 2 * _[8] * WX - 2 * _[4] * XX + 2 * _[0] * XY - 2 * _[0] * WZ + 2 * _[8] * YZ - 2 * _[4] * ZZ);
	float Value5(_[5] + 2 * _[9] * WX - 2 * _[5] * XX + 2 * _[1] * XY - 2 * _[1] * WZ + 2 * _[9] * YZ - 2 * _[5] * ZZ);
	float Value6(_[6] + 2 * _[10] * WX - 2 * _[6] * XX + 2 * _[2] * XY - 2 * _[2] * WZ + 2 * _[10] * YZ - 2 * _[6] * ZZ);
	float Value7(_[7] + 2 * _[11] * WX - 2 * _[7] * XX + 2 * _[3] * XY - 2 * _[3] * WZ + 2 * _[11] * YZ - 2 * _[7] * ZZ);
	float Value8(_[8] - 2 * _[4] * WX - 2 * _[8] * XX + 2 * _[0] * WY - 2 * _[8] * YY + 2 * _[0] * XZ + 2 * _[4] * YZ);
	float Value9(_[9] - 2 * _[5] * WX - 2 * _[9] * XX + 2 * _[1] * WY - 2 * _[9] * YY + 2 * _[1] * XZ + 2 * _[5] * YZ);
	float Value10(_[10] - 2 * _[6] * WX - 2 * _[10] * XX + 2 * _[2] * WY - 2 * _[10] * YY + 2 * _[2] * XZ + 2 * _[6] * YZ);
	float Value11(_[11] - 2 * _[7] * WX - 2 * _[11] * XX + 2 * _[3] * WY - 2 * _[11] * YY + 2 * _[3] * XZ + 2 * _[7] * YZ);
	
	_[0] = _[0] - 2 * _[8] * WY + 2 * _[4] * XY - 2 * _[0] * YY + 2 * _[4] * WZ + 2 * _[8] * XZ - 2 * _[0] * ZZ;
	_[1] = _[1] - 2 * _[9] * WY + 2 * _[5] * XY - 2 * _[1] * YY + 2 * _[5] * WZ + 2 * _[9] * XZ - 2 * _[1] * ZZ;
	_[2] = _[2] - 2 * _[10] * WY + 2 * _[6] * XY - 2 * _[2] * YY + 2 * _[6] * WZ + 2 * _[10] * XZ - 2 * _[2] * ZZ;
	_[3] = _[3] - 2 * _[11] * WY + 2 * _[7] * XY - 2 * _[3] * YY + 2 * _[7] * WZ + 2 * _[11] * XZ - 2 * _[3] * ZZ;
	_[4] = Value4;
	_[5] = Value5;
	_[6] = Value6;
	_[7] = Value7;
	_[8] = Value8;
	_[9] = Value9;
	_[10] = Value10;
	_[11] = Value11;
	
	return *this;
}

inline Matrix4f & Matrix4f::Scale(const Vector3f & Scale)
{
	_[0] *= Scale._[0];
	_[1] *= Scale._[0];
	_[2] *= Scale._[0];
	_[3] *= Scale._[0];
	_[4] *= Scale._[1];
	_[5] *= Scale._[1];
	_[6] *= Scale._[1];
	_[7] *= Scale._[1];
	_[8] *= Scale._[2];
	_[9] *= Scale._[2];
	_[10] *= Scale._[2];
	_[11] *= Scale._[2];
	
	return *this;
}

inline Matrix4f & Matrix4f::Translate(const Vector3f & Translation)
{
	_[12] += _[0] * Translation._[0] + _[4] * Translation._[1] + _[8] * Translation._[2];
	_[13] += _[1] * Translation._[0] + _[5] * Translation._[1] + _[9] * Translation._[2];
	_[14] += _[2] * Translation._[0] + _[6] * Translation._[1] + _[10] * Translation._[2];
	_[15] += _[3] * Translation._[0] + _[7] * Translation._[1] + _[11] * Translation._[2];
	
	return *this;
}

inline Matrix4f & Matrix4f::Translate(const Vector4f & Translation)
{
	_[12] += _[0] * Translation._[0] + _[4] * Translation._[1] + _[8] * Translation._[2];
	_[13] += _[1] * Translation._[0] + _[5] * Translation._[1] + _[9] * Translation._[2];
	_[14] += _[2] * Translation._[0] + _[6] * Translation._[1] + _[10] * Translation._[2];
	_[15] += _[3] * Translation._[0] + _[7] * Translation._[1] + _[11] * Translation._[2];
	
	return *this;
}

inline Matrix4f Matrix4f::Translated(const Vector3f & Translation) const
{
	auto Result(*this);
	
	Result._[12] += _[0] * Translation._[0] + _[4] * Translation._[1] + _[8] * Translation._[2];
	Result._[13] += _[1] * Translation._[0] + _[5] * Translation._[1] + _[9] * Translation._[2];
	Result._[14] += _[2] * Translation._[0] + _[6] * Translation._[1] + _[10] * Translation._[2];
	Result._[15] += _[3] * Translation._[0] + _[7] * Translation._[1] + _[11] * Translation._[2];
	
	return Result;
}

inline Matrix4f Matrix4f::Translated(const Vector4f & Translation) const
{
	auto Result(*this);
	
	Result._[12] += _[0] * Translation._[0] + _[4] * Translation._[1] + _[8] * Translation._[2];
	Result._[13] += _[1] * Translation._[0] + _[5] * Translation._[1] + _[9] * Translation._[2];
	Result._[14] += _[2] * Translation._[0] + _[6] * Translation._[1] + _[10] * Translation._[2];
	Result._[15] += _[3] * Translation._[0] + _[7] * Translation._[1] + _[11] * Translation._[2];
	
	return Result;
}

#endif
