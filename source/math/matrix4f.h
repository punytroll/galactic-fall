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
 * This is part of version 1.2.5 of algebra.
 **/

#ifndef ALGEBRA_MATRIX4F_H
#define ALGEBRA_MATRIX4F_H

#include "details.h"
#include "quaternion.h"

class Matrix4f
{
public:
	Array4f m_M[4];
	
	Matrix4f(void)
	{
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
	
	Matrix4f & Transpose(void)
	{
		float Temporary;
		
		Temporary = m_M[0].m_A[1];
		m_M[0].m_A[1] = m_M[1].m_A[0];
		m_M[1].m_A[0] = Temporary;
		Temporary = m_M[0].m_A[2];
		m_M[0].m_A[2] = m_M[2].m_A[0];
		m_M[2].m_A[0] = Temporary;
		Temporary = m_M[0].m_A[3];
		m_M[0].m_A[3] = m_M[3].m_A[0];
		m_M[3].m_A[0] = Temporary;
		
		Temporary = m_M[1].m_A[2];
		m_M[1].m_A[2] = m_M[2].m_A[1];
		m_M[2].m_A[1] = Temporary;
		Temporary = m_M[1].m_A[3];
		m_M[1].m_A[3] = m_M[3].m_A[1];
		m_M[3].m_A[1] = Temporary;
		
		Temporary = m_M[2].m_A[3];
		m_M[2].m_A[3] = m_M[3].m_A[2];
		m_M[3].m_A[2] = Temporary;
		
		return *this;
	}
};

#endif
