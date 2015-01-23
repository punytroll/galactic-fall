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

#ifndef GRAPHICS_LIGHT_H
#define GRAPHICS_LIGHT_H

#include <algebra/vector3f.h>

#include "color_rgb.h"

namespace Graphics
{
	class ColorRGB;
	
	class Light
	{
	public:
		enum class Type
		{
			Undefined,
			Directional,
			Positional
		};
		
		Light(void);
		// getters
		const Graphics::ColorRGB & GetColor(void) const;
		const Vector3f & GetDirection(void) const;
		const Vector3f & GetPosition(void) const;
		Graphics::Light::Type GetType(void) const;
		// setters
		void SetColor(const Graphics::ColorRGB & Color);
		void SetDirection(const Vector3f & Direction);
		void SetPosition(const Vector3f & Position);
		void SetType(Graphics::Light::Type Type);
	private:
		Graphics::ColorRGB _Color;
		Vector3f _Direction;
		Vector3f _Position;
		Graphics::Light::Type _Type;
	};
	
	inline const Graphics::ColorRGB & Graphics::Light::GetColor(void) const
	{
		return _Color;
	}
	
	inline Graphics::Light::Type Graphics::Light::GetType(void) const
	{
		return _Type;
	}
	
	inline void Graphics::Light::SetColor(const Graphics::ColorRGB & Color)
	{
		_Color = Color;
	}
	
	inline void Graphics::Light::SetType(Graphics::Light::Type Type)
	{
		_Type = Type;
	}
}

#endif
