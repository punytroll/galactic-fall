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
 * This is part of version 1.5.4 of algebra.
 **/

#ifndef ALGEBRA_MATRIX4F_H
#define ALGEBRA_MATRIX4F_H

#include <utility>

#include "details.h"
#include "quaternion.h"
#include "vector3f.h"
#include "vector4f.h"

class Matrix4f
{
public:
	Array4f m_M[4];
	
	Matrix4f(void)
	{
	}
	
	Matrix4f(bool)
	{
		m_M[0].m_A[0] = 1;
		m_M[0].m_A[1] = 0;
		m_M[0].m_A[2] = 0;
		m_M[0].m_A[3] = 0;
		m_M[1].m_A[0] = 0;
		m_M[1].m_A[1] = 1;
		m_M[1].m_A[2] = 0;
		m_M[1].m_A[3] = 0;
		m_M[2].m_A[0] = 0;
		m_M[2].m_A[1] = 0;
		m_M[2].m_A[2] = 1;
		m_M[2].m_A[3] = 0;
		m_M[3].m_A[0] = 0;
		m_M[3].m_A[1] = 0;
		m_M[3].m_A[2] = 0;
		m_M[3].m_A[3] = 1;
	}
	
	Matrix4f(float X, float Y, float Z)
	{
		m_M[0].m_A[0] = 1;
		m_M[0].m_A[1] = 0;
		m_M[0].m_A[2] = 0;
		m_M[0].m_A[3] = X;
		m_M[1].m_A[0] = 0;
		m_M[1].m_A[1] = 1;
		m_M[1].m_A[2] = 0;
		m_M[1].m_A[3] = Y;
		m_M[2].m_A[0] = 0;
		m_M[2].m_A[1] = 0;
		m_M[2].m_A[2] = 1;
		m_M[2].m_A[3] = Z;
		m_M[3].m_A[0] = 0;
		m_M[3].m_A[1] = 0;
		m_M[3].m_A[2] = 0;
		m_M[3].m_A[3] = 1;
	}
	
	Matrix4f(const Vector4f & InitializeTranslation)
	{
		m_M[0].m_A[0] = 1;
		m_M[0].m_A[1] = 0;
		m_M[0].m_A[2] = 0;
		m_M[0].m_A[3] = InitializeTranslation.m_V.m_A[0];
		m_M[1].m_A[0] = 0;
		m_M[1].m_A[1] = 1;
		m_M[1].m_A[2] = 0;
		m_M[1].m_A[3] = InitializeTranslation.m_V.m_A[1];
		m_M[2].m_A[0] = 0;
		m_M[2].m_A[1] = 0;
		m_M[2].m_A[2] = 1;
		m_M[2].m_A[3] = InitializeTranslation.m_V.m_A[2];
		m_M[3].m_A[0] = 0;
		m_M[3].m_A[1] = 0;
		m_M[3].m_A[2] = 0;
		m_M[3].m_A[3] = 1;
	}
	
	Matrix4f(const Quaternion & Quaternion)
	{
		float WX(Quaternion.m_V.m_A[0] * Quaternion.m_V.m_A[1]);
		float WY(Quaternion.m_V.m_A[0] * Quaternion.m_V.m_A[2]);
		float WZ(Quaternion.m_V.m_A[0] * Quaternion.m_V.m_A[3]);
		float XX(Quaternion.m_V.m_A[1] * Quaternion.m_V.m_A[1]);
		float XY(Quaternion.m_V.m_A[1] * Quaternion.m_V.m_A[2]);
		float XZ(Quaternion.m_V.m_A[1] * Quaternion.m_V.m_A[3]);
		float YY(Quaternion.m_V.m_A[2] * Quaternion.m_V.m_A[2]);
		float YZ(Quaternion.m_V.m_A[2] * Quaternion.m_V.m_A[3]);
		float ZZ(Quaternion.m_V.m_A[3] * Quaternion.m_V.m_A[3]);
		
		m_M[0].m_A[0] = 1.0f - 2.0f * (YY + ZZ);
		m_M[0].m_A[1] = 2.0f * (XY - WZ);
		m_M[0].m_A[2] = 2.0f * (XZ + WY);
		m_M[0].m_A[3] = 0.0f;
		m_M[1].m_A[0] = 2.0f * (XY + WZ);
		m_M[1].m_A[1] = 1.0f - 2.0f * (XX + ZZ);
		m_M[1].m_A[2] = 2.0f * (YZ - WX);
		m_M[1].m_A[3] = 0.0f;
		m_M[2].m_A[0] = 2.0f * (XZ - WY);
		m_M[2].m_A[1] = 2.0f * (YZ + WX);
		m_M[2].m_A[2] = 1.0f - 2.0f * (XX + YY);
		m_M[2].m_A[3] = 0.0f;
		m_M[3].m_A[0] = 0.0f;
		m_M[3].m_A[1] = 0.0f;
		m_M[3].m_A[2] = 0.0f;
		m_M[3].m_A[3] = 1.0f;
	}
	
	Matrix4f & Identity(void)
	{
		m_M[0].m_A[0] = 1;
		m_M[0].m_A[1] = 0;
		m_M[0].m_A[2] = 0;
		m_M[0].m_A[3] = 0;
		m_M[1].m_A[0] = 0;
		m_M[1].m_A[1] = 1;
		m_M[1].m_A[2] = 0;
		m_M[1].m_A[3] = 0;
		m_M[2].m_A[0] = 0;
		m_M[2].m_A[1] = 0;
		m_M[2].m_A[2] = 1;
		m_M[2].m_A[3] = 0;
		m_M[3].m_A[0] = 0;
		m_M[3].m_A[1] = 0;
		m_M[3].m_A[2] = 0;
		m_M[3].m_A[3] = 1;
		
		return *this;
	}
	
	float * Matrix(void)
	{
		return reinterpret_cast< float * >(m_M);
	}
	
	const float * Matrix(void) const
	{
		return reinterpret_cast< const float * >(m_M);
	}
	
	Matrix4f & RotateX(float Radians)
	{
		float Sin(sin(Radians));
		float Cos(cos(Radians));
		float Value02(Cos * m_M[0].m_A[2] - Sin * m_M[0].m_A[1]);
		float Value12(Cos * m_M[1].m_A[2] - Sin * m_M[1].m_A[1]);
		float Value22(Cos * m_M[2].m_A[2] - Sin * m_M[2].m_A[1]);
		float Value32(Cos * m_M[3].m_A[2] - Sin * m_M[3].m_A[1]);
		
		m_M[0].m_A[1] = Cos * m_M[0].m_A[1] + Sin * m_M[0].m_A[2];
		m_M[0].m_A[2] = Value02;
		m_M[1].m_A[1] = Cos * m_M[1].m_A[1] + Sin * m_M[1].m_A[2];
		m_M[1].m_A[2] = Value12;
		m_M[2].m_A[1] = Cos * m_M[2].m_A[1] + Sin * m_M[2].m_A[2];
		m_M[2].m_A[2] = Value22;
		m_M[3].m_A[1] = Cos * m_M[3].m_A[1] + Sin * m_M[3].m_A[2];
		m_M[3].m_A[2] = Value32;
		
		return *this;
	}
	
	Matrix4f & RotateY(float Radians)
	{
		float Sin(sin(Radians));
		float Cos(cos(Radians));
		float Value00(Cos * m_M[0].m_A[0] - Sin * m_M[0].m_A[2]);
		float Value10(Cos * m_M[1].m_A[0] - Sin * m_M[1].m_A[2]);
		float Value20(Cos * m_M[2].m_A[0] - Sin * m_M[2].m_A[2]);
		float Value30(Cos * m_M[3].m_A[0] - Sin * m_M[3].m_A[2]);
		
		m_M[0].m_A[2] = Cos * m_M[0].m_A[2] + Sin * m_M[0].m_A[0];
		m_M[0].m_A[0] = Value00;
		m_M[1].m_A[2] = Cos * m_M[1].m_A[2] + Sin * m_M[1].m_A[0];
		m_M[1].m_A[0] = Value10;
		m_M[2].m_A[2] = Cos * m_M[2].m_A[2] + Sin * m_M[2].m_A[0];
		m_M[2].m_A[0] = Value20;
		m_M[3].m_A[2] = Cos * m_M[3].m_A[2] + Sin * m_M[3].m_A[0];
		m_M[3].m_A[0] = Value30;
		
		return *this;
	}
	
	Matrix4f & RotateZ(float Radians)
	{
		float Sin(sin(Radians));
		float Cos(cos(Radians));
		float Value01(Cos * m_M[0].m_A[1] - Sin * m_M[0].m_A[0]);
		float Value11(Cos * m_M[1].m_A[1] - Sin * m_M[1].m_A[0]);
		float Value21(Cos * m_M[2].m_A[1] - Sin * m_M[2].m_A[0]);
		float Value31(Cos * m_M[3].m_A[1] - Sin * m_M[3].m_A[0]);
		
		m_M[0].m_A[0] = Cos * m_M[0].m_A[0] + Sin * m_M[0].m_A[1];
		m_M[0].m_A[1] = Value01;
		m_M[1].m_A[0] = Cos * m_M[1].m_A[0] + Sin * m_M[1].m_A[1];
		m_M[1].m_A[1] = Value11;
		m_M[2].m_A[0] = Cos * m_M[2].m_A[0] + Sin * m_M[2].m_A[1];
		m_M[2].m_A[1] = Value21;
		m_M[3].m_A[0] = Cos * m_M[3].m_A[0] + Sin * m_M[3].m_A[1];
		m_M[3].m_A[1] = Value31;
		
		return *this;
	}
	
	Matrix4f & Transform(const Matrix4f & Other)
	{
		float Value00(m_M[0].m_A[0] * Other.m_M[0].m_A[0] + m_M[0].m_A[1] * Other.m_M[1].m_A[0] + m_M[0].m_A[2] * Other.m_M[2].m_A[0] + m_M[0].m_A[3] * Other.m_M[3].m_A[0]);
		float Value01(m_M[0].m_A[0] * Other.m_M[0].m_A[1] + m_M[0].m_A[1] * Other.m_M[1].m_A[1] + m_M[0].m_A[2] * Other.m_M[2].m_A[1] + m_M[0].m_A[3] * Other.m_M[3].m_A[1]);
		float Value02(m_M[0].m_A[0] * Other.m_M[0].m_A[2] + m_M[0].m_A[1] * Other.m_M[1].m_A[2] + m_M[0].m_A[2] * Other.m_M[2].m_A[2] + m_M[0].m_A[3] * Other.m_M[3].m_A[2]);
		float Value03(m_M[0].m_A[0] * Other.m_M[0].m_A[3] + m_M[0].m_A[1] * Other.m_M[1].m_A[3] + m_M[0].m_A[2] * Other.m_M[2].m_A[3] + m_M[0].m_A[3] * Other.m_M[3].m_A[3]);
		float Value10(m_M[1].m_A[0] * Other.m_M[0].m_A[0] + m_M[1].m_A[1] * Other.m_M[1].m_A[0] + m_M[1].m_A[2] * Other.m_M[2].m_A[0] + m_M[1].m_A[3] * Other.m_M[3].m_A[0]);
		float Value11(m_M[1].m_A[0] * Other.m_M[0].m_A[1] + m_M[1].m_A[1] * Other.m_M[1].m_A[1] + m_M[1].m_A[2] * Other.m_M[2].m_A[1] + m_M[1].m_A[3] * Other.m_M[3].m_A[1]);
		float Value12(m_M[1].m_A[0] * Other.m_M[0].m_A[2] + m_M[1].m_A[1] * Other.m_M[1].m_A[2] + m_M[1].m_A[2] * Other.m_M[2].m_A[2] + m_M[1].m_A[3] * Other.m_M[3].m_A[2]);
		float Value13(m_M[1].m_A[0] * Other.m_M[0].m_A[3] + m_M[1].m_A[1] * Other.m_M[1].m_A[3] + m_M[1].m_A[2] * Other.m_M[2].m_A[3] + m_M[1].m_A[3] * Other.m_M[3].m_A[3]);
		float Value20(m_M[2].m_A[0] * Other.m_M[0].m_A[0] + m_M[2].m_A[1] * Other.m_M[1].m_A[0] + m_M[2].m_A[2] * Other.m_M[2].m_A[0] + m_M[2].m_A[3] * Other.m_M[3].m_A[0]);
		float Value21(m_M[2].m_A[0] * Other.m_M[0].m_A[1] + m_M[2].m_A[1] * Other.m_M[1].m_A[1] + m_M[2].m_A[2] * Other.m_M[2].m_A[1] + m_M[2].m_A[3] * Other.m_M[3].m_A[1]);
		float Value22(m_M[2].m_A[0] * Other.m_M[0].m_A[2] + m_M[2].m_A[1] * Other.m_M[1].m_A[2] + m_M[2].m_A[2] * Other.m_M[2].m_A[2] + m_M[2].m_A[3] * Other.m_M[3].m_A[2]);
		float Value23(m_M[2].m_A[0] * Other.m_M[0].m_A[3] + m_M[2].m_A[1] * Other.m_M[1].m_A[3] + m_M[2].m_A[2] * Other.m_M[2].m_A[3] + m_M[2].m_A[3] * Other.m_M[3].m_A[3]);
		float Value30(m_M[3].m_A[0] * Other.m_M[0].m_A[0] + m_M[3].m_A[1] * Other.m_M[1].m_A[0] + m_M[3].m_A[2] * Other.m_M[2].m_A[0] + m_M[3].m_A[3] * Other.m_M[3].m_A[0]);
		float Value31(m_M[3].m_A[0] * Other.m_M[0].m_A[1] + m_M[3].m_A[1] * Other.m_M[1].m_A[1] + m_M[3].m_A[2] * Other.m_M[2].m_A[1] + m_M[3].m_A[3] * Other.m_M[3].m_A[1]);
		float Value32(m_M[3].m_A[0] * Other.m_M[0].m_A[2] + m_M[3].m_A[1] * Other.m_M[1].m_A[2] + m_M[3].m_A[2] * Other.m_M[2].m_A[2] + m_M[3].m_A[3] * Other.m_M[3].m_A[2]);
		float Value33(m_M[3].m_A[0] * Other.m_M[0].m_A[3] + m_M[3].m_A[1] * Other.m_M[1].m_A[3] + m_M[3].m_A[2] * Other.m_M[2].m_A[3] + m_M[3].m_A[3] * Other.m_M[3].m_A[3]);
		
		m_M[0].m_A[0] = Value00;
		m_M[0].m_A[1] = Value01;
		m_M[0].m_A[2] = Value02;
		m_M[0].m_A[3] = Value03;
		m_M[1].m_A[0] = Value10;
		m_M[1].m_A[1] = Value11;
		m_M[1].m_A[2] = Value12;
		m_M[1].m_A[3] = Value13;
		m_M[2].m_A[0] = Value20;
		m_M[2].m_A[1] = Value21;
		m_M[2].m_A[2] = Value22;
		m_M[2].m_A[3] = Value23;
		m_M[3].m_A[0] = Value30;
		m_M[3].m_A[1] = Value31;
		m_M[3].m_A[2] = Value32;
		m_M[3].m_A[3] = Value33;
		
		return *this;
	}
	
	Matrix4f & Transform(const Quaternion & Quaternion)
	{
		float WX(Quaternion.m_V.m_A[0] * Quaternion.m_V.m_A[1]);
		float WY(Quaternion.m_V.m_A[0] * Quaternion.m_V.m_A[2]);
		float WZ(Quaternion.m_V.m_A[0] * Quaternion.m_V.m_A[3]);
		float XX(Quaternion.m_V.m_A[1] * Quaternion.m_V.m_A[1]);
		float XY(Quaternion.m_V.m_A[1] * Quaternion.m_V.m_A[2]);
		float XZ(Quaternion.m_V.m_A[1] * Quaternion.m_V.m_A[3]);
		float YY(Quaternion.m_V.m_A[2] * Quaternion.m_V.m_A[2]);
		float YZ(Quaternion.m_V.m_A[2] * Quaternion.m_V.m_A[3]);
		float ZZ(Quaternion.m_V.m_A[3] * Quaternion.m_V.m_A[3]);
		float Value01(m_M[0].m_A[1] + 2 * m_M[0].m_A[2] * WX - 2 * m_M[0].m_A[1] * XX + 2 * m_M[0].m_A[0] * XY - 2 * m_M[0].m_A[0] * WZ + 2 * m_M[0].m_A[2] * YZ - 2 * m_M[0].m_A[1] * ZZ);
		float Value02(m_M[0].m_A[2] - 2 * m_M[0].m_A[1] * WX - 2 * m_M[0].m_A[2] * XX + 2 * m_M[0].m_A[0] * WY - 2 * m_M[0].m_A[2] * YY + 2 * m_M[0].m_A[0] * XZ + 2 * m_M[0].m_A[1] * YZ);
		float Value11(m_M[1].m_A[1] + 2 * m_M[1].m_A[2] * WX - 2 * m_M[1].m_A[1] * XX + 2 * m_M[1].m_A[0] * XY - 2 * m_M[1].m_A[0] * WZ + 2 * m_M[1].m_A[2] * YZ - 2 * m_M[1].m_A[1] * ZZ);
		float Value12(m_M[1].m_A[2] - 2 * m_M[1].m_A[1] * WX - 2 * m_M[1].m_A[2] * XX + 2 * m_M[1].m_A[0] * WY - 2 * m_M[1].m_A[2] * YY + 2 * m_M[1].m_A[0] * XZ + 2 * m_M[1].m_A[1] * YZ);
		float Value21(m_M[2].m_A[1] + 2 * m_M[2].m_A[2] * WX - 2 * m_M[2].m_A[1] * XX + 2 * m_M[2].m_A[0] * XY - 2 * m_M[2].m_A[0] * WZ + 2 * m_M[2].m_A[2] * YZ - 2 * m_M[2].m_A[1] * ZZ);
		float Value22(m_M[2].m_A[2] - 2 * m_M[2].m_A[1] * WX - 2 * m_M[2].m_A[2] * XX + 2 * m_M[2].m_A[0] * WY - 2 * m_M[2].m_A[2] * YY + 2 * m_M[2].m_A[0] * XZ + 2 * m_M[2].m_A[1] * YZ);
		float Value31(m_M[3].m_A[1] + 2 * m_M[3].m_A[2] * WX - 2 * m_M[3].m_A[1] * XX + 2 * m_M[3].m_A[0] * XY - 2 * m_M[3].m_A[0] * WZ + 2 * m_M[3].m_A[2] * YZ - 2 * m_M[3].m_A[1] * ZZ);
		float Value32(m_M[3].m_A[2] - 2 * m_M[3].m_A[1] * WX - 2 * m_M[3].m_A[2] * XX + 2 * m_M[3].m_A[0] * WY - 2 * m_M[3].m_A[2] * YY + 2 * m_M[3].m_A[0] * XZ + 2 * m_M[3].m_A[1] * YZ);
		
		m_M[0].m_A[0] = m_M[0].m_A[0] - 2 * m_M[0].m_A[2] * WY + 2 * m_M[0].m_A[1] * XY - 2 * m_M[0].m_A[0] * YY + 2 * m_M[0].m_A[1] * WZ + 2 * m_M[0].m_A[2] * XZ - 2 * m_M[0].m_A[0] * ZZ;
		m_M[0].m_A[1] = Value01;
		m_M[0].m_A[2] = Value02;
		m_M[1].m_A[0] = m_M[1].m_A[0] - 2 * m_M[1].m_A[2] * WY + 2 * m_M[1].m_A[1] * XY - 2 * m_M[1].m_A[0] * YY + 2 * m_M[1].m_A[1] * WZ + 2 * m_M[1].m_A[2] * XZ - 2 * m_M[1].m_A[0] * ZZ;
		m_M[1].m_A[1] = Value11;
		m_M[1].m_A[2] = Value12;
		m_M[2].m_A[0] = m_M[2].m_A[0] - 2 * m_M[2].m_A[2] * WY + 2 * m_M[2].m_A[1] * XY - 2 * m_M[2].m_A[0] * YY + 2 * m_M[2].m_A[1] * WZ + 2 * m_M[2].m_A[2] * XZ - 2 * m_M[2].m_A[0] * ZZ;
		m_M[2].m_A[1] = Value21;
		m_M[2].m_A[2] = Value22;
		m_M[3].m_A[0] = m_M[3].m_A[0] - 2 * m_M[3].m_A[2] * WY + 2 * m_M[3].m_A[1] * XY - 2 * m_M[3].m_A[0] * YY + 2 * m_M[3].m_A[1] * WZ + 2 * m_M[3].m_A[2] * XZ - 2 * m_M[3].m_A[0] * ZZ;
		m_M[3].m_A[1] = Value31;
		m_M[3].m_A[2] = Value32;
		
		return *this;
	}
	
	Matrix4f & Translate(float X, float Y, float Z)
	{
		m_M[0].m_A[3] += m_M[0].m_A[0] * X + m_M[0].m_A[1] * Y + m_M[0].m_A[2] * Z;
		m_M[1].m_A[3] += m_M[1].m_A[0] * X + m_M[1].m_A[1] * Y + m_M[1].m_A[2] * Z;
		m_M[2].m_A[3] += m_M[2].m_A[0] * X + m_M[2].m_A[1] * Y + m_M[2].m_A[2] * Z;
		m_M[3].m_A[3] += m_M[3].m_A[0] * X + m_M[3].m_A[1] * Y + m_M[3].m_A[2] * Z;
		
		return *this;
	}
	
	Matrix4f & Translate(const Vector3f & Translation)
	{
		m_M[0].m_A[3] += m_M[0].m_A[0] * Translation.m_V.m_A[0] + m_M[0].m_A[1] * Translation.m_V.m_A[1] + m_M[0].m_A[2] * Translation.m_V.m_A[2];
		m_M[1].m_A[3] += m_M[1].m_A[0] * Translation.m_V.m_A[0] + m_M[1].m_A[1] * Translation.m_V.m_A[1] + m_M[1].m_A[2] * Translation.m_V.m_A[2];
		m_M[2].m_A[3] += m_M[2].m_A[0] * Translation.m_V.m_A[0] + m_M[2].m_A[1] * Translation.m_V.m_A[1] + m_M[2].m_A[2] * Translation.m_V.m_A[2];
		m_M[3].m_A[3] += m_M[3].m_A[0] * Translation.m_V.m_A[0] + m_M[3].m_A[1] * Translation.m_V.m_A[1] + m_M[3].m_A[2] * Translation.m_V.m_A[2];
		
		return *this;
	}
	
	Matrix4f & Translate(const Vector4f & Translation)
	{
		m_M[0].m_A[3] += m_M[0].m_A[0] * Translation.m_V.m_A[0] + m_M[0].m_A[1] * Translation.m_V.m_A[1] + m_M[0].m_A[2] * Translation.m_V.m_A[2];
		m_M[1].m_A[3] += m_M[1].m_A[0] * Translation.m_V.m_A[0] + m_M[1].m_A[1] * Translation.m_V.m_A[1] + m_M[1].m_A[2] * Translation.m_V.m_A[2];
		m_M[2].m_A[3] += m_M[2].m_A[0] * Translation.m_V.m_A[0] + m_M[2].m_A[1] * Translation.m_V.m_A[1] + m_M[2].m_A[2] * Translation.m_V.m_A[2];
		m_M[3].m_A[3] += m_M[3].m_A[0] * Translation.m_V.m_A[0] + m_M[3].m_A[1] * Translation.m_V.m_A[1] + m_M[3].m_A[2] * Translation.m_V.m_A[2];
		
		return *this;
	}
	
	void Translation(float X, float Y, float Z)
	{
		m_M[0].m_A[0] = 1;
		m_M[0].m_A[1] = 0;
		m_M[0].m_A[2] = 0;
		m_M[0].m_A[3] = X;
		m_M[1].m_A[0] = 0;
		m_M[1].m_A[1] = 1;
		m_M[1].m_A[2] = 0;
		m_M[1].m_A[3] = Y;
		m_M[2].m_A[0] = 0;
		m_M[2].m_A[1] = 0;
		m_M[2].m_A[2] = 1;
		m_M[2].m_A[3] = Z;
		m_M[3].m_A[0] = 0;
		m_M[3].m_A[1] = 0;
		m_M[3].m_A[2] = 0;
		m_M[3].m_A[3] = 1;
	}
	
	Matrix4f & Transpose(void)
	{
		std::swap(m_M[0].m_A[1], m_M[1].m_A[0]);
		std::swap(m_M[0].m_A[2], m_M[2].m_A[0]);
		std::swap(m_M[0].m_A[3], m_M[3].m_A[0]);
		std::swap(m_M[1].m_A[2], m_M[2].m_A[1]);
		std::swap(m_M[1].m_A[3], m_M[3].m_A[1]);
		std::swap(m_M[2].m_A[3], m_M[3].m_A[2]);
		
		return *this;
	}
	
	Matrix4f Transposed(void) const
	{
		Matrix4f Result;
		
		Result.m_M[0].m_A[0] = m_M[0].m_A[0];
		Result.m_M[0].m_A[1] = m_M[1].m_A[0];
		Result.m_M[0].m_A[2] = m_M[2].m_A[0];
		Result.m_M[0].m_A[3] = m_M[3].m_A[0];
		Result.m_M[1].m_A[0] = m_M[0].m_A[1];
		Result.m_M[1].m_A[1] = m_M[1].m_A[1];
		Result.m_M[1].m_A[2] = m_M[2].m_A[1];
		Result.m_M[1].m_A[3] = m_M[3].m_A[1];
		Result.m_M[2].m_A[0] = m_M[0].m_A[2];
		Result.m_M[2].m_A[1] = m_M[1].m_A[2];
		Result.m_M[2].m_A[2] = m_M[2].m_A[2];
		Result.m_M[2].m_A[3] = m_M[3].m_A[2];
		Result.m_M[3].m_A[0] = m_M[0].m_A[3];
		Result.m_M[3].m_A[1] = m_M[1].m_A[3];
		Result.m_M[3].m_A[2] = m_M[2].m_A[3];
		Result.m_M[3].m_A[3] = m_M[3].m_A[3];
		
		return Result;
	}
	
	Matrix4f & Invert(void)
	{
		float Determinant = m_M[0].m_A[3] * m_M[1].m_A[2] * m_M[2].m_A[1] * m_M[3].m_A[0] - m_M[0].m_A[2] * m_M[1].m_A[3] * m_M[2].m_A[1] * m_M[3].m_A[0] - m_M[0].m_A[3] * m_M[1].m_A[1] * m_M[2].m_A[2] * m_M[3].m_A[0] + m_M[0].m_A[1] * m_M[1].m_A[3] * m_M[2].m_A[2] * m_M[3].m_A[0] + m_M[0].m_A[2] * m_M[1].m_A[1] * m_M[2].m_A[3] * m_M[3].m_A[0] - m_M[0].m_A[1] * m_M[1].m_A[2] * m_M[2].m_A[3] * m_M[3].m_A[0] - m_M[0].m_A[3] * m_M[1].m_A[2] * m_M[2].m_A[0] * m_M[3].m_A[1] + m_M[0].m_A[2] * m_M[1].m_A[3] * m_M[2].m_A[0] * m_M[3].m_A[1] + m_M[0].m_A[3] * m_M[1].m_A[0] * m_M[2].m_A[2] * m_M[3].m_A[1] - m_M[0].m_A[0] * m_M[1].m_A[3] * m_M[2].m_A[2] * m_M[3].m_A[1] - m_M[0].m_A[2] * m_M[1].m_A[0] * m_M[2].m_A[3] * m_M[3].m_A[1] + m_M[0].m_A[0] * m_M[1].m_A[2] * m_M[2].m_A[3] * m_M[3].m_A[1] + m_M[0].m_A[3] * m_M[1].m_A[1] * m_M[2].m_A[0] * m_M[3].m_A[2] - m_M[0].m_A[1] * m_M[1].m_A[3] * m_M[2].m_A[0] * m_M[3].m_A[2] - m_M[0].m_A[3] * m_M[1].m_A[0] * m_M[2].m_A[1] * m_M[3].m_A[2] + m_M[0].m_A[0] * m_M[1].m_A[3] * m_M[2].m_A[1] * m_M[3].m_A[2] + m_M[0].m_A[1] * m_M[1].m_A[0] * m_M[2].m_A[3] * m_M[3].m_A[2] - m_M[0].m_A[0] * m_M[1].m_A[1] * m_M[2].m_A[3] * m_M[3].m_A[2] - m_M[0].m_A[2] * m_M[1].m_A[1] * m_M[2].m_A[0] * m_M[3].m_A[3] + m_M[0].m_A[1] * m_M[1].m_A[2] * m_M[2].m_A[0] * m_M[3].m_A[3] + m_M[0].m_A[2] * m_M[1].m_A[0] * m_M[2].m_A[1] * m_M[3].m_A[3] - m_M[0].m_A[0] * m_M[1].m_A[2] * m_M[2].m_A[1] * m_M[3].m_A[3] - m_M[0].m_A[1] * m_M[1].m_A[0] * m_M[2].m_A[2] * m_M[3].m_A[3] + m_M[0].m_A[0] * m_M[1].m_A[1] * m_M[2].m_A[2] * m_M[3].m_A[3];
		float Value00 = -m_M[1].m_A[3] * m_M[2].m_A[2] * m_M[3].m_A[1] + m_M[1].m_A[2] * m_M[2].m_A[3] * m_M[3].m_A[1] + m_M[1].m_A[3] * m_M[2].m_A[1] * m_M[3].m_A[2] - m_M[1].m_A[1] * m_M[2].m_A[3] * m_M[3].m_A[2] - m_M[1].m_A[2] * m_M[2].m_A[1] * m_M[3].m_A[3] + m_M[1].m_A[1] * m_M[2].m_A[2] * m_M[3].m_A[3];
		float Value01 = m_M[0].m_A[3] * m_M[2].m_A[2] * m_M[3].m_A[1] - m_M[0].m_A[2] * m_M[2].m_A[3] * m_M[3].m_A[1] - m_M[0].m_A[3] * m_M[2].m_A[1] * m_M[3].m_A[2] + m_M[0].m_A[1] * m_M[2].m_A[3] * m_M[3].m_A[2] + m_M[0].m_A[2] * m_M[2].m_A[1] * m_M[3].m_A[3] - m_M[0].m_A[1] * m_M[2].m_A[2] * m_M[3].m_A[3];
		float Value02 = -m_M[0].m_A[3] * m_M[1].m_A[2] * m_M[3].m_A[1] + m_M[0].m_A[2] * m_M[1].m_A[3] * m_M[3].m_A[1] + m_M[0].m_A[3] * m_M[1].m_A[1] * m_M[3].m_A[2] - m_M[0].m_A[1] * m_M[1].m_A[3] * m_M[3].m_A[2] - m_M[0].m_A[2] * m_M[1].m_A[1] * m_M[3].m_A[3] + m_M[0].m_A[1] * m_M[1].m_A[2] * m_M[3].m_A[3];
		float Value03 = m_M[0].m_A[3] * m_M[1].m_A[2] * m_M[2].m_A[1] - m_M[0].m_A[2] * m_M[1].m_A[3] * m_M[2].m_A[1] - m_M[0].m_A[3] * m_M[1].m_A[1] * m_M[2].m_A[2] + m_M[0].m_A[1] * m_M[1].m_A[3] * m_M[2].m_A[2] + m_M[0].m_A[2] * m_M[1].m_A[1] * m_M[2].m_A[3] - m_M[0].m_A[1] * m_M[1].m_A[2] * m_M[2].m_A[3];
		float Value10 = m_M[1].m_A[3] * m_M[2].m_A[2] * m_M[3].m_A[0] - m_M[1].m_A[2] * m_M[2].m_A[3] * m_M[3].m_A[0] - m_M[1].m_A[3] * m_M[2].m_A[0] * m_M[3].m_A[2] + m_M[1].m_A[0] * m_M[2].m_A[3] * m_M[3].m_A[2] + m_M[1].m_A[2] * m_M[2].m_A[0] * m_M[3].m_A[3] - m_M[1].m_A[0] * m_M[2].m_A[2] * m_M[3].m_A[3];
		float Value11 = -m_M[0].m_A[3] * m_M[2].m_A[2] * m_M[3].m_A[0] + m_M[0].m_A[2] * m_M[2].m_A[3] * m_M[3].m_A[0] + m_M[0].m_A[3] * m_M[2].m_A[0] * m_M[3].m_A[2] - m_M[0].m_A[0] * m_M[2].m_A[3] * m_M[3].m_A[2] - m_M[0].m_A[2] * m_M[2].m_A[0] * m_M[3].m_A[3] + m_M[0].m_A[0] * m_M[2].m_A[2] * m_M[3].m_A[3];
		float Value12 = m_M[0].m_A[3] * m_M[1].m_A[2] * m_M[3].m_A[0] - m_M[0].m_A[2] * m_M[1].m_A[3] * m_M[3].m_A[0] - m_M[0].m_A[3] * m_M[1].m_A[0] * m_M[3].m_A[2] + m_M[0].m_A[0] * m_M[1].m_A[3] * m_M[3].m_A[2] + m_M[0].m_A[2] * m_M[1].m_A[0] * m_M[3].m_A[3] - m_M[0].m_A[0] * m_M[1].m_A[2] * m_M[3].m_A[3];
		float Value13 = -m_M[0].m_A[3] * m_M[1].m_A[2] * m_M[2].m_A[0] + m_M[0].m_A[2] * m_M[1].m_A[3] * m_M[2].m_A[0] + m_M[0].m_A[3] * m_M[1].m_A[0] * m_M[2].m_A[2] - m_M[0].m_A[0] * m_M[1].m_A[3] * m_M[2].m_A[2] - m_M[0].m_A[2] * m_M[1].m_A[0] * m_M[2].m_A[3] + m_M[0].m_A[0] * m_M[1].m_A[2] * m_M[2].m_A[3];
		float Value20 = -m_M[1].m_A[3] * m_M[2].m_A[1] * m_M[3].m_A[0] + m_M[1].m_A[1] * m_M[2].m_A[3] * m_M[3].m_A[0] + m_M[1].m_A[3] * m_M[2].m_A[0] * m_M[3].m_A[1] - m_M[1].m_A[0] * m_M[2].m_A[3] * m_M[3].m_A[1] - m_M[1].m_A[1] * m_M[2].m_A[0] * m_M[3].m_A[3] + m_M[1].m_A[0] * m_M[2].m_A[1] * m_M[3].m_A[3];
		float Value21 = m_M[0].m_A[3] * m_M[2].m_A[1] * m_M[3].m_A[0] - m_M[0].m_A[1] * m_M[2].m_A[3] * m_M[3].m_A[0] - m_M[0].m_A[3] * m_M[2].m_A[0] * m_M[3].m_A[1] + m_M[0].m_A[0] * m_M[2].m_A[3] * m_M[3].m_A[1] + m_M[0].m_A[1] * m_M[2].m_A[0] * m_M[3].m_A[3] - m_M[0].m_A[0] * m_M[2].m_A[1] * m_M[3].m_A[3];
		float Value22 = -m_M[0].m_A[3] * m_M[1].m_A[1] * m_M[3].m_A[0] + m_M[0].m_A[1] * m_M[1].m_A[3] * m_M[3].m_A[0] + m_M[0].m_A[3] * m_M[1].m_A[0] * m_M[3].m_A[1] - m_M[0].m_A[0] * m_M[1].m_A[3] * m_M[3].m_A[1] - m_M[0].m_A[1] * m_M[1].m_A[0] * m_M[3].m_A[3] + m_M[0].m_A[0] * m_M[1].m_A[1] * m_M[3].m_A[3];
		float Value23 = m_M[0].m_A[3] * m_M[1].m_A[1] * m_M[2].m_A[0] - m_M[0].m_A[1] * m_M[1].m_A[3] * m_M[2].m_A[0] - m_M[0].m_A[3] * m_M[1].m_A[0] * m_M[2].m_A[1] + m_M[0].m_A[0] * m_M[1].m_A[3] * m_M[2].m_A[1] + m_M[0].m_A[1] * m_M[1].m_A[0] * m_M[2].m_A[3] - m_M[0].m_A[0] * m_M[1].m_A[1] * m_M[2].m_A[3];
		float Value30 = m_M[1].m_A[2] * m_M[2].m_A[1] * m_M[3].m_A[0] - m_M[1].m_A[1] * m_M[2].m_A[2] * m_M[3].m_A[0] - m_M[1].m_A[2] * m_M[2].m_A[0] * m_M[3].m_A[1] + m_M[1].m_A[0] * m_M[2].m_A[2] * m_M[3].m_A[1] + m_M[1].m_A[1] * m_M[2].m_A[0] * m_M[3].m_A[2] - m_M[1].m_A[0] * m_M[2].m_A[1] * m_M[3].m_A[2];
		float Value31 = -m_M[0].m_A[2] * m_M[2].m_A[1] * m_M[3].m_A[0] + m_M[0].m_A[1] * m_M[2].m_A[2] * m_M[3].m_A[0] + m_M[0].m_A[2] * m_M[2].m_A[0] * m_M[3].m_A[1] - m_M[0].m_A[0] * m_M[2].m_A[2] * m_M[3].m_A[1] - m_M[0].m_A[1] * m_M[2].m_A[0] * m_M[3].m_A[2] + m_M[0].m_A[0] * m_M[2].m_A[1] * m_M[3].m_A[2];
		float Value32 = m_M[0].m_A[2] * m_M[1].m_A[1] * m_M[3].m_A[0] - m_M[0].m_A[1] * m_M[1].m_A[2] * m_M[3].m_A[0] - m_M[0].m_A[2] * m_M[1].m_A[0] * m_M[3].m_A[1] + m_M[0].m_A[0] * m_M[1].m_A[2] * m_M[3].m_A[1] + m_M[0].m_A[1] * m_M[1].m_A[0] * m_M[3].m_A[2] - m_M[0].m_A[0] * m_M[1].m_A[1] * m_M[3].m_A[2];
		float Value33 = -m_M[0].m_A[2] * m_M[1].m_A[1] * m_M[2].m_A[0] + m_M[0].m_A[1] * m_M[1].m_A[2] * m_M[2].m_A[0] + m_M[0].m_A[2] * m_M[1].m_A[0] * m_M[2].m_A[1] - m_M[0].m_A[0] * m_M[1].m_A[2] * m_M[2].m_A[1] - m_M[0].m_A[1] * m_M[1].m_A[0] * m_M[2].m_A[2] + m_M[0].m_A[0] * m_M[1].m_A[1] * m_M[2].m_A[2];
		
		m_M[0].m_A[0] = Value00 / Determinant;
		m_M[0].m_A[1] = Value01 / Determinant;
		m_M[0].m_A[2] = Value02 / Determinant;
		m_M[0].m_A[3] = Value03 / Determinant;
		
		m_M[1].m_A[0] = Value10 / Determinant;
		m_M[1].m_A[1] = Value11 / Determinant;
		m_M[1].m_A[2] = Value12 / Determinant;
		m_M[1].m_A[3] = Value13 / Determinant;
		
		m_M[2].m_A[0] = Value20 / Determinant;
		m_M[2].m_A[1] = Value21 / Determinant;
		m_M[2].m_A[2] = Value22 / Determinant;
		m_M[2].m_A[3] = Value23 / Determinant;
		
		m_M[3].m_A[0] = Value30 / Determinant;
		m_M[3].m_A[1] = Value31 / Determinant;
		m_M[3].m_A[2] = Value32 / Determinant;
		m_M[3].m_A[3] = Value33 / Determinant;
		
		return *this;
	}
	
	Matrix4f Inverted(void) const
	{
		float Determinant = m_M[0].m_A[3] * m_M[1].m_A[2] * m_M[2].m_A[1] * m_M[3].m_A[0] - m_M[0].m_A[2] * m_M[1].m_A[3] * m_M[2].m_A[1] * m_M[3].m_A[0] - m_M[0].m_A[3] * m_M[1].m_A[1] * m_M[2].m_A[2] * m_M[3].m_A[0] + m_M[0].m_A[1] * m_M[1].m_A[3] * m_M[2].m_A[2] * m_M[3].m_A[0] + m_M[0].m_A[2] * m_M[1].m_A[1] * m_M[2].m_A[3] * m_M[3].m_A[0] - m_M[0].m_A[1] * m_M[1].m_A[2] * m_M[2].m_A[3] * m_M[3].m_A[0] - m_M[0].m_A[3] * m_M[1].m_A[2] * m_M[2].m_A[0] * m_M[3].m_A[1] + m_M[0].m_A[2] * m_M[1].m_A[3] * m_M[2].m_A[0] * m_M[3].m_A[1] + m_M[0].m_A[3] * m_M[1].m_A[0] * m_M[2].m_A[2] * m_M[3].m_A[1] - m_M[0].m_A[0] * m_M[1].m_A[3] * m_M[2].m_A[2] * m_M[3].m_A[1] - m_M[0].m_A[2] * m_M[1].m_A[0] * m_M[2].m_A[3] * m_M[3].m_A[1] + m_M[0].m_A[0] * m_M[1].m_A[2] * m_M[2].m_A[3] * m_M[3].m_A[1] + m_M[0].m_A[3] * m_M[1].m_A[1] * m_M[2].m_A[0] * m_M[3].m_A[2] - m_M[0].m_A[1] * m_M[1].m_A[3] * m_M[2].m_A[0] * m_M[3].m_A[2] - m_M[0].m_A[3] * m_M[1].m_A[0] * m_M[2].m_A[1] * m_M[3].m_A[2] + m_M[0].m_A[0] * m_M[1].m_A[3] * m_M[2].m_A[1] * m_M[3].m_A[2] + m_M[0].m_A[1] * m_M[1].m_A[0] * m_M[2].m_A[3] * m_M[3].m_A[2] - m_M[0].m_A[0] * m_M[1].m_A[1] * m_M[2].m_A[3] * m_M[3].m_A[2] - m_M[0].m_A[2] * m_M[1].m_A[1] * m_M[2].m_A[0] * m_M[3].m_A[3] + m_M[0].m_A[1] * m_M[1].m_A[2] * m_M[2].m_A[0] * m_M[3].m_A[3] + m_M[0].m_A[2] * m_M[1].m_A[0] * m_M[2].m_A[1] * m_M[3].m_A[3] - m_M[0].m_A[0] * m_M[1].m_A[2] * m_M[2].m_A[1] * m_M[3].m_A[3] - m_M[0].m_A[1] * m_M[1].m_A[0] * m_M[2].m_A[2] * m_M[3].m_A[3] + m_M[0].m_A[0] * m_M[1].m_A[1] * m_M[2].m_A[2] * m_M[3].m_A[3];
		Matrix4f Result;
		
		Result.m_M[0].m_A[0] = (-m_M[1].m_A[3] * m_M[2].m_A[2] * m_M[3].m_A[1] + m_M[1].m_A[2] * m_M[2].m_A[3] * m_M[3].m_A[1] + m_M[1].m_A[3] * m_M[2].m_A[1] * m_M[3].m_A[2] - m_M[1].m_A[1] * m_M[2].m_A[3] * m_M[3].m_A[2] - m_M[1].m_A[2] * m_M[2].m_A[1] * m_M[3].m_A[3] + m_M[1].m_A[1] * m_M[2].m_A[2] * m_M[3].m_A[3]) / Determinant;
		Result.m_M[0].m_A[1] = (m_M[0].m_A[3] * m_M[2].m_A[2] * m_M[3].m_A[1] - m_M[0].m_A[2] * m_M[2].m_A[3] * m_M[3].m_A[1] - m_M[0].m_A[3] * m_M[2].m_A[1] * m_M[3].m_A[2] + m_M[0].m_A[1] * m_M[2].m_A[3] * m_M[3].m_A[2] + m_M[0].m_A[2] * m_M[2].m_A[1] * m_M[3].m_A[3] - m_M[0].m_A[1] * m_M[2].m_A[2] * m_M[3].m_A[3]) / Determinant;
		Result.m_M[0].m_A[2] = (-m_M[0].m_A[3] * m_M[1].m_A[2] * m_M[3].m_A[1] + m_M[0].m_A[2] * m_M[1].m_A[3] * m_M[3].m_A[1] + m_M[0].m_A[3] * m_M[1].m_A[1] * m_M[3].m_A[2] - m_M[0].m_A[1] * m_M[1].m_A[3] * m_M[3].m_A[2] - m_M[0].m_A[2] * m_M[1].m_A[1] * m_M[3].m_A[3] + m_M[0].m_A[1] * m_M[1].m_A[2] * m_M[3].m_A[3]) / Determinant;
		Result.m_M[0].m_A[3] = (m_M[0].m_A[3] * m_M[1].m_A[2] * m_M[2].m_A[1] - m_M[0].m_A[2] * m_M[1].m_A[3] * m_M[2].m_A[1] - m_M[0].m_A[3] * m_M[1].m_A[1] * m_M[2].m_A[2] + m_M[0].m_A[1] * m_M[1].m_A[3] * m_M[2].m_A[2] + m_M[0].m_A[2] * m_M[1].m_A[1] * m_M[2].m_A[3] - m_M[0].m_A[1] * m_M[1].m_A[2] * m_M[2].m_A[3]) / Determinant;
		Result.m_M[1].m_A[0] = (m_M[1].m_A[3] * m_M[2].m_A[2] * m_M[3].m_A[0] - m_M[1].m_A[2] * m_M[2].m_A[3] * m_M[3].m_A[0] - m_M[1].m_A[3] * m_M[2].m_A[0] * m_M[3].m_A[2] + m_M[1].m_A[0] * m_M[2].m_A[3] * m_M[3].m_A[2] + m_M[1].m_A[2] * m_M[2].m_A[0] * m_M[3].m_A[3] - m_M[1].m_A[0] * m_M[2].m_A[2] * m_M[3].m_A[3]) / Determinant;
		Result.m_M[1].m_A[1] = (-m_M[0].m_A[3] * m_M[2].m_A[2] * m_M[3].m_A[0] + m_M[0].m_A[2] * m_M[2].m_A[3] * m_M[3].m_A[0] + m_M[0].m_A[3] * m_M[2].m_A[0] * m_M[3].m_A[2] - m_M[0].m_A[0] * m_M[2].m_A[3] * m_M[3].m_A[2] - m_M[0].m_A[2] * m_M[2].m_A[0] * m_M[3].m_A[3] + m_M[0].m_A[0] * m_M[2].m_A[2] * m_M[3].m_A[3]) / Determinant;
		Result.m_M[1].m_A[2] = (m_M[0].m_A[3] * m_M[1].m_A[2] * m_M[3].m_A[0] - m_M[0].m_A[2] * m_M[1].m_A[3] * m_M[3].m_A[0] - m_M[0].m_A[3] * m_M[1].m_A[0] * m_M[3].m_A[2] + m_M[0].m_A[0] * m_M[1].m_A[3] * m_M[3].m_A[2] + m_M[0].m_A[2] * m_M[1].m_A[0] * m_M[3].m_A[3] - m_M[0].m_A[0] * m_M[1].m_A[2] * m_M[3].m_A[3]) / Determinant;
		Result.m_M[1].m_A[3] = (-m_M[0].m_A[3] * m_M[1].m_A[2] * m_M[2].m_A[0] + m_M[0].m_A[2] * m_M[1].m_A[3] * m_M[2].m_A[0] + m_M[0].m_A[3] * m_M[1].m_A[0] * m_M[2].m_A[2] - m_M[0].m_A[0] * m_M[1].m_A[3] * m_M[2].m_A[2] - m_M[0].m_A[2] * m_M[1].m_A[0] * m_M[2].m_A[3] + m_M[0].m_A[0] * m_M[1].m_A[2] * m_M[2].m_A[3]) / Determinant;
		Result.m_M[2].m_A[0] = (-m_M[1].m_A[3] * m_M[2].m_A[1] * m_M[3].m_A[0] + m_M[1].m_A[1] * m_M[2].m_A[3] * m_M[3].m_A[0] + m_M[1].m_A[3] * m_M[2].m_A[0] * m_M[3].m_A[1] - m_M[1].m_A[0] * m_M[2].m_A[3] * m_M[3].m_A[1] - m_M[1].m_A[1] * m_M[2].m_A[0] * m_M[3].m_A[3] + m_M[1].m_A[0] * m_M[2].m_A[1] * m_M[3].m_A[3]) / Determinant;
		Result.m_M[2].m_A[1] = (m_M[0].m_A[3] * m_M[2].m_A[1] * m_M[3].m_A[0] - m_M[0].m_A[1] * m_M[2].m_A[3] * m_M[3].m_A[0] - m_M[0].m_A[3] * m_M[2].m_A[0] * m_M[3].m_A[1] + m_M[0].m_A[0] * m_M[2].m_A[3] * m_M[3].m_A[1] + m_M[0].m_A[1] * m_M[2].m_A[0] * m_M[3].m_A[3] - m_M[0].m_A[0] * m_M[2].m_A[1] * m_M[3].m_A[3]) / Determinant;
		Result.m_M[2].m_A[2] = (-m_M[0].m_A[3] * m_M[1].m_A[1] * m_M[3].m_A[0] + m_M[0].m_A[1] * m_M[1].m_A[3] * m_M[3].m_A[0] + m_M[0].m_A[3] * m_M[1].m_A[0] * m_M[3].m_A[1] - m_M[0].m_A[0] * m_M[1].m_A[3] * m_M[3].m_A[1] - m_M[0].m_A[1] * m_M[1].m_A[0] * m_M[3].m_A[3] + m_M[0].m_A[0] * m_M[1].m_A[1] * m_M[3].m_A[3]) / Determinant;
		Result.m_M[2].m_A[3] = (m_M[0].m_A[3] * m_M[1].m_A[1] * m_M[2].m_A[0] - m_M[0].m_A[1] * m_M[1].m_A[3] * m_M[2].m_A[0] - m_M[0].m_A[3] * m_M[1].m_A[0] * m_M[2].m_A[1] + m_M[0].m_A[0] * m_M[1].m_A[3] * m_M[2].m_A[1] + m_M[0].m_A[1] * m_M[1].m_A[0] * m_M[2].m_A[3] - m_M[0].m_A[0] * m_M[1].m_A[1] * m_M[2].m_A[3]) / Determinant;
		Result.m_M[3].m_A[0] = (m_M[1].m_A[2] * m_M[2].m_A[1] * m_M[3].m_A[0] - m_M[1].m_A[1] * m_M[2].m_A[2] * m_M[3].m_A[0] - m_M[1].m_A[2] * m_M[2].m_A[0] * m_M[3].m_A[1] + m_M[1].m_A[0] * m_M[2].m_A[2] * m_M[3].m_A[1] + m_M[1].m_A[1] * m_M[2].m_A[0] * m_M[3].m_A[2] - m_M[1].m_A[0] * m_M[2].m_A[1] * m_M[3].m_A[2]) / Determinant;
		Result.m_M[3].m_A[1] = (-m_M[0].m_A[2] * m_M[2].m_A[1] * m_M[3].m_A[0] + m_M[0].m_A[1] * m_M[2].m_A[2] * m_M[3].m_A[0] + m_M[0].m_A[2] * m_M[2].m_A[0] * m_M[3].m_A[1] - m_M[0].m_A[0] * m_M[2].m_A[2] * m_M[3].m_A[1] - m_M[0].m_A[1] * m_M[2].m_A[0] * m_M[3].m_A[2] + m_M[0].m_A[0] * m_M[2].m_A[1] * m_M[3].m_A[2]) / Determinant;
		Result.m_M[3].m_A[2] = (m_M[0].m_A[2] * m_M[1].m_A[1] * m_M[3].m_A[0] - m_M[0].m_A[1] * m_M[1].m_A[2] * m_M[3].m_A[0] - m_M[0].m_A[2] * m_M[1].m_A[0] * m_M[3].m_A[1] + m_M[0].m_A[0] * m_M[1].m_A[2] * m_M[3].m_A[1] + m_M[0].m_A[1] * m_M[1].m_A[0] * m_M[3].m_A[2] - m_M[0].m_A[0] * m_M[1].m_A[1] * m_M[3].m_A[2]) / Determinant;
		Result.m_M[3].m_A[3] = (-m_M[0].m_A[2] * m_M[1].m_A[1] * m_M[2].m_A[0] + m_M[0].m_A[1] * m_M[1].m_A[2] * m_M[2].m_A[0] + m_M[0].m_A[2] * m_M[1].m_A[0] * m_M[2].m_A[1] - m_M[0].m_A[0] * m_M[1].m_A[2] * m_M[2].m_A[1] - m_M[0].m_A[1] * m_M[1].m_A[0] * m_M[2].m_A[2] + m_M[0].m_A[0] * m_M[1].m_A[1] * m_M[2].m_A[2]) / Determinant;
		
		return Result;
	}
};

#endif
