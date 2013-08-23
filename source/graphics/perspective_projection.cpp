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
	_FieldOfViewY(1.0f),
	_NearClippingPlane(1.0f)
{
}

Graphics::PerspectiveProjection::~PerspectiveProjection(void)
{
}

Matrix4f Graphics::PerspectiveProjection::CalculateMatrix(void) const
{
	Matrix4f Result;
	float Top(_NearClippingPlane * tan(_FieldOfViewY / 2.0f));
	float Right(Top * _Aspect);
	
	Result[0] = _NearClippingPlane / Right;
	Result[1] = 0.0f;
	Result[2] = 0.0f;
	Result[3] = 0.0f;
	Result[4] = 0.0f;
	Result[5] = _NearClippingPlane / Top;
	Result[6] = 0.0f;
	Result[7] = 0.0f;
	Result[8] = 0.0f;
	Result[9] = 0.0f;
	Result[10] = -(_FarClippingPlane + _NearClippingPlane) / (_FarClippingPlane - _NearClippingPlane);
	Result[11] = -1.0f;
	Result[12] = 0.0f;
	Result[13] = 0.0f;
	Result[14] = -(2.0f * _FarClippingPlane * _NearClippingPlane) / (_FarClippingPlane - _NearClippingPlane);
	Result[15] = 0.0f;
	
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

void Graphics::PerspectiveProjection::SetFieldOfViewY(float FieldOfViewY)
{
	if(_FieldOfViewY != FieldOfViewY)
	{
		_FieldOfViewY = FieldOfViewY;
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
