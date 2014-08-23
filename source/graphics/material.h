/**
 * galactic-fall
 * Copyright (C) 2007  Hagen Möbius
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

#ifndef GRAPHICS_MATERIAL_H
#define GRAPHICS_MATERIAL_H

#include <string>

class Color;

namespace Graphics
{
	class Material
	{
	public:
		// constructor & destructor
		Material(void);
		explicit Material(const Graphics::Material * Material);
		~Material(void);
		// getters
		const Color * GetDiffuseColor(void) const;
		float GetShininess(void) const;
		const Color * GetSpecularColor(void) const;
		// setters
		void SetDiffuseColor(const Color & DiffuseColor);
		void SetShininess(float Shininess);
		void SetSpecularColor(const Color & SpecularColor);
	private:
		Color * _DiffuseColor;
		float _Shininess;
		Color * _SpecularColor;
	private:
		/**
		 * @brief Hidden copy constructor.
		 **/
		Material(const Graphics::Material & Material);
		
		/**
		 * @brief Hidden assignment operator.
		 **/
		Graphics::Material & operator=(const Graphics::Material & Material);
	};
	
	inline const Color * Material::GetDiffuseColor(void) const
	{
		return _DiffuseColor;
	}
	
	inline float Material::GetShininess(void) const
	{
		return _Shininess;
	}
	
	inline const Color * Material::GetSpecularColor(void) const
	{
		return _SpecularColor;
	}
	
	inline void Material::SetShininess(float Shininess)
	{
		_Shininess = Shininess;
	}
}

#endif
