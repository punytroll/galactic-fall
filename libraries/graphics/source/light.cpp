/**
 * galactic-fall
 * Copyright (C) 2013-2019  Hagen Möbius
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

#include <graphics/light.h>

Graphics::Light::Light(void) :
	_Type(Graphics::Light::Type::Undefined)
{
}

const Vector3f & Graphics::Light::GetDirection(void) const
{
	assert(_Type == Graphics::Light::Type::Directional);
	
	return _Direction;
}

const Vector3f & Graphics::Light::GetPosition(void) const
{
	assert(_Type == Graphics::Light::Type::Positional);
	
	return _Position;
}

void Graphics::Light::SetDirection(const Vector3f & Direction)
{
	assert(_Type == Graphics::Light::Type::Directional);
	_Direction = Direction.Normalized();
}

void Graphics::Light::SetPosition(const Vector3f & Position)
{
	assert(_Type == Graphics::Light::Type::Positional);
	_Position = Position;
}
