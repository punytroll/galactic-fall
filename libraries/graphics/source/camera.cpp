/**
 * galactic-fall
 * Copyright (C) 2013-2025  Hagen Möbius
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

#include <cassert>

#include <graphics/camera.h>

Graphics::Camera::Camera(void) :
	_Projection(nullptr)
{
}

Graphics::Camera::~Camera(void)
{
	assert(_Projection == nullptr);
}

const Matrix4f & Graphics::Camera::GetSpacialMatrix(void) const
{
	return _SpacialMatrix;
}

void Graphics::Camera::SetProjection(Graphics::Projection * Projection)
{
	if(Projection == nullptr)
	{
		assert(_Projection != nullptr);
		_Projection = nullptr;
	}
	else
	{
		assert(_Projection == nullptr);
		_Projection = Projection;
	}
}

void Graphics::Camera::SetSpacialMatrix(const Matrix4f & SpacialMatrix)
{
	_SpacialMatrix = SpacialMatrix;
}
