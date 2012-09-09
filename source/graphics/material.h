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
		/**
		 * @note This function transfers memory management responsibility for the @a DiffuseColor to the @a Graphics::Material.
		 **/
		void SetDiffuseColor(Color * DiffuseColor);
		void SetShininess(float Shininess);
		/**
		 * @note This function transfers memory management responsibility for the @a SpecularColor to the @a Graphics::Material.
		 **/
		void SetSpecularColor(Color * SpecularColor);
	private:
		Color * m_DiffuseColor;
		float m_Shininess;
		Color * m_SpecularColor;
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
		return m_DiffuseColor;
	}
	
	inline float Material::GetShininess(void) const
	{
		return m_Shininess;
	}
	
	inline const Color * Material::GetSpecularColor(void) const
	{
		return m_SpecularColor;
	}
	
	inline void Material::SetShininess(float Shininess)
	{
		m_Shininess = Shininess;
	}
}

#endif