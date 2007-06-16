/**
 * galactic-fall
 * Copyright (C) 2006  Hagen Möbius
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

#include <GL/gl.h>

#include "perspective.h"

static void CalculatePerspectiveMatrix(float FieldOfView, float Aspect, float NearClippingPlane, float FarClippingPlane, math3d::matrix4f & Matrix)
{
	float Right, Top;
	
	Top = NearClippingPlane * tan(FieldOfView);
	Right = Top * Aspect;
	
	Matrix.m_M[1].m_A[0] = Matrix.m_M[2].m_A[0] = Matrix.m_M[3].m_A[0] = Matrix.m_M[0].m_A[1] = Matrix.m_M[2].m_A[1] = Matrix.m_M[3].m_A[1] = Matrix.m_M[0].m_A[2] = Matrix.m_M[1].m_A[2] = Matrix.m_M[0].m_A[3] = Matrix.m_M[1].m_A[3] = Matrix.m_M[3].m_A[3] = 0.0f;
	Matrix.m_M[0].m_A[0] = NearClippingPlane / Right;
	Matrix.m_M[1].m_A[1] = NearClippingPlane / Top;
	Matrix.m_M[2].m_A[2] = -(FarClippingPlane + NearClippingPlane) / (FarClippingPlane - NearClippingPlane);
	Matrix.m_M[3].m_A[2] = -(2.0f * FarClippingPlane * NearClippingPlane) / (FarClippingPlane - NearClippingPlane);
	Matrix.m_M[2].m_A[3] = -1.0f;
}

Perspective::Perspective(void) :
	m_Initialized(false),
	m_Aspect(0.0f),
	m_FieldOfView(0.0f),
	m_NearClippingPlane(0.0f),
	m_FarClippingPlane(0.0f),
	m_Matrix(true)
{
}

void Perspective::Draw(void) const
{
	if(m_Initialized == false)
	{
		CalculatePerspectiveMatrix(m_FieldOfView, m_Aspect, m_NearClippingPlane, m_FarClippingPlane, m_Matrix);
		m_Initialized = true;
	}
	glLoadMatrixf(m_Matrix.matrix());
}
