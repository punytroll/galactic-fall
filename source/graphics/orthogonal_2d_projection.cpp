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

#include "orthogonal_2d_projection.h"

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
	
	Result.m_M[0].m_A[0] = 2.0f / (_Right - _Left);
	Result.m_M[0].m_A[1] = 0.0f;
	Result.m_M[0].m_A[2] = 0.0f;
	Result.m_M[0].m_A[3] = 0.0f;
	
	Result.m_M[1].m_A[0] = 0.0f;
	Result.m_M[1].m_A[1] = 2.0f / (_Top - _Bottom);
	Result.m_M[1].m_A[2] = 0.0f;
	Result.m_M[1].m_A[3] = 0.0f;
	
	Result.m_M[2].m_A[0] = 0.0f;
	Result.m_M[2].m_A[1] = 0.0f;
	Result.m_M[2].m_A[2] = -1.0f;
	Result.m_M[2].m_A[3] = 0.0f;
	
	Result.m_M[3].m_A[0] = (_Left + _Right) / (_Left - _Right);
	Result.m_M[3].m_A[1] = (_Bottom + _Top) / (_Bottom - _Top);
	Result.m_M[3].m_A[2] = 0.0f;
	Result.m_M[3].m_A[3] = 1.0f;
	
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
