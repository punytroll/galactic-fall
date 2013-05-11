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

#include "camera.h"

Graphics::Camera::Camera(void) :
	_Aspect(1.0f),
	_FarClippingPlane(10000.0f),
	_FieldOfView(0.392699082f),
	_NearClippingPlane(1.0f),
	_PerspectiveMatrixInitialized(false)
{
}

const Matrix4f & Graphics::Camera::GetPerspectiveMatrix(void) const
{
	if(_PerspectiveMatrixInitialized == false)
	{
		float Top(_NearClippingPlane * tan(_FieldOfView));
		float Right(Top * _Aspect);
		
		_PerspectiveMatrix.m_M[1].m_A[0] = _PerspectiveMatrix.m_M[2].m_A[0] = _PerspectiveMatrix.m_M[3].m_A[0] = _PerspectiveMatrix.m_M[0].m_A[1] = _PerspectiveMatrix.m_M[2].m_A[1] = _PerspectiveMatrix.m_M[3].m_A[1] = _PerspectiveMatrix.m_M[0].m_A[2] = _PerspectiveMatrix.m_M[1].m_A[2] = _PerspectiveMatrix.m_M[0].m_A[3] = _PerspectiveMatrix.m_M[1].m_A[3] = _PerspectiveMatrix.m_M[3].m_A[3] = 0.0f;
		_PerspectiveMatrix.m_M[0].m_A[0] = _NearClippingPlane / Right;
		_PerspectiveMatrix.m_M[1].m_A[1] = _NearClippingPlane / Top;
		_PerspectiveMatrix.m_M[2].m_A[2] = -(_FarClippingPlane + _NearClippingPlane) / (_FarClippingPlane - _NearClippingPlane);
		_PerspectiveMatrix.m_M[3].m_A[2] = -(2.0f * _FarClippingPlane * _NearClippingPlane) / (_FarClippingPlane - _NearClippingPlane);
		_PerspectiveMatrix.m_M[2].m_A[3] = -1.0f;
		_PerspectiveMatrixInitialized = true;
	}
	
	return _PerspectiveMatrix;
}

const Matrix4f & Graphics::Camera::GetSpacialMatrix(void) const
{
	return _SpacialMatrix;
}

void Graphics::Camera::SetAspect(float Aspect)
{
	if(_Aspect != Aspect)
	{
		_Aspect = Aspect;
		_PerspectiveMatrixInitialized = false;
	}
}

void Graphics::Camera::SetFarClippingPlane(float FarClippingPlane)
{
	if(_FarClippingPlane != FarClippingPlane)
	{
		_FarClippingPlane = FarClippingPlane;
		_PerspectiveMatrixInitialized = false;
	}
}

void Graphics::Camera::SetFieldOfView(float FieldOfView)
{
	if(_FieldOfView != FieldOfView)
	{
		_FieldOfView = FieldOfView;
		_PerspectiveMatrixInitialized = false;
	}
}

void Graphics::Camera::SetNearClippingPlane(float NearClippingPlane)
{
	if(_NearClippingPlane != NearClippingPlane)
	{
		_NearClippingPlane = NearClippingPlane;
		_PerspectiveMatrixInitialized = false;
	}
}

void Graphics::Camera::SetSpacialMatrix(const Matrix4f & SpacialMatrix)
{
	_SpacialMatrix = SpacialMatrix;
}
