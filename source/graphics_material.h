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
		Material(void);
		Material(const Graphics::Material & Material);
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
		Color * m_DiffuseColor;
		float m_Shininess;
		Color * m_SpecularColor;
	};
}

inline const Color * Graphics::Material::GetDiffuseColor(void) const
{
	return m_DiffuseColor;
}

inline float Graphics::Material::GetShininess(void) const
{
	return m_Shininess;
}

inline const Color * Graphics::Material::GetSpecularColor(void) const
{
	return m_SpecularColor;
}

inline void Graphics::Material::SetShininess(float Shininess)
{
	m_Shininess = Shininess;
}

#endif
