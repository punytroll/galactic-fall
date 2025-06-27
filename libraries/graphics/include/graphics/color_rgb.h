/**
 * galactic-fall
 * Copyright (C) 2015-2025  Hagen Möbius
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

#ifndef GALACTIC_FALL__LIBRARY__GRAPHICS__INCLUDE__GRAPHICS__COLOR_RGB_H
#define GALACTIC_FALL__LIBRARY__GRAPHICS__INCLUDE__GRAPHICS__COLOR_RGB_H

namespace Graphics
{
	class ColorRGBO;
	
	class ColorRGB
	{
		friend class ColorRGBO;
	public:
		ColorRGB(void);
		ColorRGB(const Graphics::ColorRGB & Color);
		explicit ColorRGB(const Graphics::ColorRGBO & Color);
		ColorRGB(float Red, float Green, float Blue);
		float const * GetPointer(void) const;
		Graphics::ColorRGB & operator=(const Graphics::ColorRGB & Color);
		Graphics::ColorRGB & operator=(const Graphics::ColorRGBO & Color);
		void Set(float Red, float Green, float Blue);
	private:
		union
		{
			struct
			{
				float Red;
				float Green;
				float Blue;
			} m_Color;
			float m_Array[3];
		};
	};
	
	inline float const * Graphics::ColorRGB::GetPointer(void) const
	{
		return m_Array;
	}
}

#endif
