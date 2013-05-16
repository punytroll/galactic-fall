/**
 * galactic-fall
 * Copyright (C) 2013  Hagen Möbius
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

#include "perspective_projection.h"

Graphics::PerspectiveProjection::PerspectiveProjection(void) :
	_Aspect(1.0f),
	_FarClippingPlane(10000.0f),
	_FieldOfView(0.392699082f),
	_NearClippingPlane(1.0f)
{
}

Graphics::PerspectiveProjection::~PerspectiveProjection(void)
{
}

Matrix4f Graphics::PerspectiveProjection::CalculateMatrix(void) const
{
	Matrix4f Result;
	float Top(_NearClippingPlane * tan(_FieldOfView));
	float Right(Top * _Aspect);
	
	Result.m_M[0].m_A[0] = _NearClippingPlane / Right;
	Result.m_M[0].m_A[1] = 0.0f;
	Result.m_M[0].m_A[2] = 0.0f;
	Result.m_M[0].m_A[3] = 0.0f;
	
	Result.m_M[1].m_A[0] = 0.0f;
	Result.m_M[1].m_A[1] = _NearClippingPlane / Top;
	Result.m_M[1].m_A[2] = 0.0f;
	Result.m_M[1].m_A[3] = 0.0f;
	
	Result.m_M[2].m_A[0] = 0.0f;
	Result.m_M[2].m_A[1] = 0.0f;
	Result.m_M[2].m_A[2] = -(_FarClippingPlane + _NearClippingPlane) / (_FarClippingPlane - _NearClippingPlane);
	Result.m_M[2].m_A[3] = -1.0f;
	
	Result.m_M[3].m_A[0] = 0.0f;
	Result.m_M[3].m_A[1] = 0.0f;
	Result.m_M[3].m_A[2] = -(2.0f * _FarClippingPlane * _NearClippingPlane) / (_FarClippingPlane - _NearClippingPlane);
	Result.m_M[3].m_A[3] = 0.0f;
	
	return Result;
}

void Graphics::PerspectiveProjection::SetAspect(float Aspect)
{
	if(_Aspect != Aspect)
	{
		_Aspect = Aspect;
		InvalidateMatrix();
	}
}

void Graphics::PerspectiveProjection::SetFarClippingPlane(float FarClippingPlane)
{
	if(_FarClippingPlane != FarClippingPlane)
	{
		_FarClippingPlane = FarClippingPlane;
		InvalidateMatrix();
	}
}

void Graphics::PerspectiveProjection::SetFieldOfView(float FieldOfView)
{
	if(_FieldOfView != FieldOfView)
	{
		_FieldOfView = FieldOfView;
		InvalidateMatrix();
	}
}

void Graphics::PerspectiveProjection::SetNearClippingPlane(float NearClippingPlane)
{
	if(_NearClippingPlane != NearClippingPlane)
	{
		_NearClippingPlane = NearClippingPlane;
		InvalidateMatrix();
	}
}
