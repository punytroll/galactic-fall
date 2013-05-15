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

#include <assert.h>

#include "camera.h"

Graphics::Camera::Camera(void) :
	_Projection(0)
{
}

Graphics::Camera::~Camera(void)
{
	assert(_Projection == 0);
}

const Matrix4f & Graphics::Camera::GetSpacialMatrix(void) const
{
	return _SpacialMatrix;
}

void Graphics::Camera::SetProjection(Graphics::Projection * Projection)
{
	if(Projection == 0)
	{
		assert(_Projection != 0);
		_Projection = 0;
	}
	else
	{
		assert(_Projection == 0);
		_Projection = Projection;
	}
}

void Graphics::Camera::SetSpacialMatrix(const Matrix4f & SpacialMatrix)
{
	_SpacialMatrix = SpacialMatrix;
}
