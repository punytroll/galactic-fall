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

#include "light.h"

Graphics::Light::Light(void)
{
}

const Vector4f & Graphics::Light::GetPosition(void) const
{
	return _Position;
}

const Vector4f & Graphics::Light::GetDiffuseColor(void) const
{
	return _DiffuseColor;
}

void Graphics::Light::SetPosition(float X, float Y, float Z)
{
	_Position.m_V.m_A[0] = X;
	_Position.m_V.m_A[1] = Y;
	_Position.m_V.m_A[2] = Z;
	_Position.m_V.m_A[3] = 0.0f;
}

void Graphics::Light::SetDiffuseColor(float Red, float Green, float Blue, float Alpha)
{
	_DiffuseColor.m_V.m_A[0] = Red;
	_DiffuseColor.m_V.m_A[1] = Green;
	_DiffuseColor.m_V.m_A[2] = Blue;
	_DiffuseColor.m_V.m_A[3] = Alpha;
}
