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

#include <graphics/orthogonal_2d_projection.h>

Graphics::Orthogonal2DProjection::Orthogonal2DProjection(void) :
	_Bottom(0.0f),
	_Left(0.0f),
	_Right(0.0f),
	_Top(0.0f)
{
}

Graphics::Orthogonal2DProjection::~Orthogonal2DProjection(void)
{
}

Matrix4f Graphics::Orthogonal2DProjection::CalculateMatrix(void) const
{
	Matrix4f Result;
	
	Result[0] = 2.0f / (_Right - _Left);
	Result[1] = 0.0f;
	Result[2] = 0.0f;
	Result[3] = 0.0f;
	Result[4] = 0.0f;
	Result[5] = 2.0f / (_Top - _Bottom);
	Result[6] = 0.0f;
	Result[7] = 0.0f;
	Result[8] = 0.0f;
	Result[9] = 0.0f;
	Result[10] = -1.0f;
	Result[11] = 0.0f;
	Result[12] = (_Left + _Right) / (_Left - _Right);
	Result[13] = (_Bottom + _Top) / (_Bottom - _Top);
	Result[14] = 0.0f;
	Result[15] = 1.0f;
	
	return Result;
}

void Graphics::Orthogonal2DProjection::SetBottom(float Bottom)
{
	if(_Bottom != Bottom)
	{
		_Bottom = Bottom;
		InvalidateMatrix();
	}
}

void Graphics::Orthogonal2DProjection::SetLeft(float Left)
{
	if(_Left != Left)
	{
		_Left = Left;
		InvalidateMatrix();
	}
}

void Graphics::Orthogonal2DProjection::SetRight(float Right)
{
	if(_Right != Right)
	{
		_Right = Right;
		InvalidateMatrix();
	}
}

void Graphics::Orthogonal2DProjection::SetTop(float Top)
{
	if(_Top != Top)
	{
		_Top = Top;
		InvalidateMatrix();
	}
}
