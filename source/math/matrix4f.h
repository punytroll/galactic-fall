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
 * This is part of version 1.4.2 of algebra.
 **/

#ifndef ALGEBRA_MATRIX4F_H
#define ALGEBRA_MATRIX4F_H

#include <utility>

#include "details.h"
#include "quaternion.h"

class Matrix4f
{
public:
	Array4f m_M[4];
	
	Matrix4f(void)
	{
	}
	
	Matrix4f(float X, float Y, float Z)
	{
		Translation(X, Y, Z);
	}
	
	Matrix4f(const Quaternion & Quaternion)
	{
		float XX(Quaternion.m_V.m_A[1] * Quaternion.m_V.m_A[1]);
		float XY(Quaternion.m_V.m_A[1] * Quaternion.m_V.m_A[2]);
		float XZ(Quaternion.m_V.m_A[1] * Quaternion.m_V.m_A[3]);
		float XW(Quaternion.m_V.m_A[1] * Quaternion.m_V.m_A[0]);
		float YY(Quaternion.m_V.m_A[2] * Quaternion.m_V.m_A[2]);
		float YZ(Quaternion.m_V.m_A[2] * Quaternion.m_V.m_A[3]);
		float YW(Quaternion.m_V.m_A[2] * Quaternion.m_V.m_A[0]);
		float ZZ(Quaternion.m_V.m_A[3] * Quaternion.m_V.m_A[3]);
		float ZW(Quaternion.m_V.m_A[3] * Quaternion.m_V.m_A[0]);
		
		this->m_M[0].m_A[0] = 1.0f - 2.0f * (YY + ZZ);
		this->m_M[0].m_A[1] = 2.0f * (XY - ZW);
		this->m_M[0].m_A[2] = 2.0f * (XZ + YW);
		this->m_M[1].m_A[0] = 2.0f * (XY + ZW);
		this->m_M[1].m_A[1] = 1.0f - 2.0f * (XX + ZZ);
		this->m_M[1].m_A[2] = 2.0f * (YZ - XW);
		this->m_M[2].m_A[0] = 2.0f * (XZ - YW);
		this->m_M[2].m_A[1] = 2.0f * (YZ + XW);
		this->m_M[2].m_A[2] = 1.0f - 2.0f * (XX + YY);
		this->m_M[0].m_A[3] = this->m_M[1].m_A[3] = this->m_M[2].m_A[3] = this->m_M[3].m_A[0] = this->m_M[3].m_A[1] = this->m_M[3].m_A[2] = 0.0f;
		this->m_M[3].m_A[3] = 1.0f;
	}
	
	float * Matrix(void)
	{
		return reinterpret_cast< float * >(m_M);
	}
	
	const float * Matrix(void) const
	{
		return reinterpret_cast< const float * >(m_M);
	}
	
	Matrix4f & Transform(const Matrix4f & Other)
	{
		// mathematica:
		// [
		//     [
		//         a00 b00 + a01 b10 + a02 b20 + a03 b30,
		//         a00 b01 + a01 b11 + a02 b21 + a03 b31,
		//         a00 b02 + a01 b12 + a02 b22 + a03 b32,
		//         a00 b03 + a01 b13 + a02 b23 + a03 b33
		//     ],
		//     [
		//         a10 b00 + a11 b10 + a12 b20 + a13 b30,
		//         a10 b01 + a11 b11 + a12 b21 + a13 b31,
		//         a10 b02 + a11 b12 + a12 b22 + a13 b32,
		//         a10 b03 + a11 b13 + a12 b23 + a13 b33
		//     ],
		//     [
		//         a20 b00 + a21 b10 + a22 b20 + a23 b30,
		//         a20 b01 + a21 b11 + a22 b21 + a23 b31,
		//         a20 b02 + a21 b12 + a22 b22 + a23 b32,
		//         a20 b03 + a21 b13 + a22 b23 + a23 b33
		//     ],
		//     [
		//         a30 b00 + a31 b10 + a32 b20 + a33 b30,
		//         a30 b01 + a31 b11 + a32 b21 + a33 b31,
		//         a30 b02 + a31 b12 + a32 b22 + a33 b32,
		//         a30 b03 + a31 b13 + a32 b23 + a33 b33
		//     ]
		// ]
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
};

#endif
