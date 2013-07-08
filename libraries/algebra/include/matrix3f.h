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
 * This is part of version 1.5.2 of algebra.
 **/

#ifndef ALGEBRA_MATRIX3F_H
#define ALGEBRA_MATRIX3F_H

#include <math.h>

#include "details.h"

class Matrix3f
{
public:
	enum Initializer
	{
		InitializeZero,
		InitializeIdentity,
		InitializeRotationX,
		InitializeRotationY,
		InitializeRotationZ
	};
	
	Array4f m_M[4];
	
	Matrix3f(void)
	{
	}
	
	Matrix3f(Initializer Initializer)
	{
		if(Initializer == InitializeZero)
		{
			Zero();
		}
		else if(Initializer == InitializeIdentity)
		{
			Identity();
		}
	}
	
	Matrix3f(float Angle, Initializer Initializer)
	{
		if(Initializer == InitializeRotationX)
		{
			RotationX(Angle);
		}
		else if(Initializer == InitializeRotationY)
		{
			RotationY(Angle);
		}
		else if(Initializer == InitializeRotationZ)
		{
			RotationZ(Angle);
		}
	}
	
	void Zero(void)
	{
		m_M[0].m_A[0] = m_M[0].m_A[1] = m_M[0].m_A[2] = m_M[1].m_A[0] = m_M[1].m_A[1] = m_M[1].m_A[2] = m_M[2].m_A[0] = m_M[2].m_A[1] = m_M[2].m_A[2] = 0.0f;
	}
	
	void Identity(void)
	{
		m_M[0].m_A[1] = m_M[0].m_A[2] = m_M[1].m_A[0] = m_M[1].m_A[2] = m_M[2].m_A[0] = m_M[2].m_A[1] = 0.0f;
		m_M[0].m_A[0] = m_M[1].m_A[1] = m_M[2].m_A[2] = 1.0f;
	}
	
	void RotationX(float Angle)
	{
		m_M[0].m_A[0] = 1.0f;
		m_M[0].m_A[1] = m_M[0].m_A[2] = m_M[1].m_A[0] = m_M[2].m_A[0] = 0.0f;
		m_M[1].m_A[1] = m_M[2].m_A[2] = cos(Angle);
		m_M[1].m_A[2] = -(m_M[2].m_A[1] = sin(Angle));
	}
	
	void RotationY(float Angle)
	{
		m_M[0].m_A[1] = m_M[1].m_A[0] = m_M[1].m_A[2] = m_M[2].m_A[1] = 0.0f;
		m_M[1].m_A[1] = 1.0f;
		m_M[0].m_A[0] = m_M[2].m_A[2] = cos(Angle);
		m_M[2].m_A[0] = -(m_M[0].m_A[2] = sin(Angle));
	}
	
	void RotationZ(float Angle)
	{
		m_M[0].m_A[2] = m_M[1].m_A[2] = m_M[2].m_A[0] = m_M[2].m_A[1] = 0.0f;
		m_M[2].m_A[2] = 1.0f;
		m_M[0].m_A[0] = m_M[1].m_A[1] = cos(Angle);
		m_M[0].m_A[1] = -(m_M[1].m_A[0] = sin(Angle));
	}
	
	Matrix3f & operator+=(const Matrix3f & Other)
	{
		m_M[0].m_A[0] += Other.m_M[0].m_A[0];
		m_M[0].m_A[1] += Other.m_M[0].m_A[1];
		m_M[0].m_A[2] += Other.m_M[0].m_A[2];
		m_M[1].m_A[0] += Other.m_M[1].m_A[0];
		m_M[1].m_A[1] += Other.m_M[1].m_A[1];
		m_M[1].m_A[2] += Other.m_M[1].m_A[2];
		m_M[2].m_A[0] += Other.m_M[2].m_A[0];
		m_M[2].m_A[1] += Other.m_M[2].m_A[1];
		m_M[2].m_A[2] += Other.m_M[2].m_A[2];
		
		return *this;
	}
	
	Matrix3f & operator-=(const Matrix3f & Other)
	{
		m_M[0].m_A[0] -= Other.m_M[0].m_A[0];
		m_M[0].m_A[1] -= Other.m_M[0].m_A[1];
		m_M[0].m_A[2] -= Other.m_M[0].m_A[2];
		m_M[1].m_A[0] -= Other.m_M[1].m_A[0];
		m_M[1].m_A[1] -= Other.m_M[1].m_A[1];
		m_M[1].m_A[2] -= Other.m_M[1].m_A[2];
		m_M[2].m_A[0] -= Other.m_M[2].m_A[0];
		m_M[2].m_A[1] -= Other.m_M[2].m_A[1];
		m_M[2].m_A[2] -= Other.m_M[2].m_A[2];
		
		return *this;
	}
	
	Matrix3f & operator*=(const Matrix3f & Other)
	{
		float Value00(m_M[0].m_A[0] * Other.m_M[0].m_A[0] + m_M[0].m_A[1] * Other.m_M[1].m_A[0] + m_M[0].m_A[2] * Other.m_M[2].m_A[0]);
		float Value01(m_M[0].m_A[0] * Other.m_M[0].m_A[1] + m_M[0].m_A[1] * Other.m_M[1].m_A[1] + m_M[0].m_A[2] * Other.m_M[2].m_A[1]);
		float Value02(m_M[0].m_A[0] * Other.m_M[0].m_A[2] + m_M[0].m_A[1] * Other.m_M[1].m_A[2] + m_M[0].m_A[2] * Other.m_M[2].m_A[2]);
		float Value10(m_M[1].m_A[0] * Other.m_M[0].m_A[0] + m_M[1].m_A[1] * Other.m_M[1].m_A[0] + m_M[1].m_A[2] * Other.m_M[2].m_A[0]);
		float Value11(m_M[1].m_A[0] * Other.m_M[0].m_A[1] + m_M[1].m_A[1] * Other.m_M[1].m_A[1] + m_M[1].m_A[2] * Other.m_M[2].m_A[1]);
		float Value12(m_M[1].m_A[0] * Other.m_M[0].m_A[2] + m_M[1].m_A[1] * Other.m_M[1].m_A[2] + m_M[1].m_A[2] * Other.m_M[2].m_A[2]);
		float Value20(m_M[2].m_A[0] * Other.m_M[0].m_A[0] + m_M[2].m_A[1] * Other.m_M[1].m_A[0] + m_M[2].m_A[2] * Other.m_M[2].m_A[0]);
		float Value21(m_M[2].m_A[0] * Other.m_M[0].m_A[1] + m_M[2].m_A[1] * Other.m_M[1].m_A[1] + m_M[2].m_A[2] * Other.m_M[2].m_A[1]);
		float Value22(m_M[2].m_A[0] * Other.m_M[0].m_A[2] + m_M[2].m_A[1] * Other.m_M[1].m_A[2] + m_M[2].m_A[2] * Other.m_M[2].m_A[2]);
		
		m_M[0].m_A[0] = Value00;
		m_M[0].m_A[1] = Value01;
		m_M[0].m_A[2] = Value02;
		m_M[1].m_A[0] = Value10;
		m_M[1].m_A[1] = Value11;
		m_M[1].m_A[2] = Value12;
		m_M[2].m_A[0] = Value20;
		m_M[2].m_A[1] = Value21;
		m_M[2].m_A[2] = Value22;
		
		return *this;
	}
};

#endif
