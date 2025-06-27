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

#ifndef GALACTIC_FALL__LIBRARY__GRAPHICS__INCLUDE__GRAPHICS__COLOR_RGBO_H
#define GALACTIC_FALL__LIBRARY__GRAPHICS__INCLUDE__GRAPHICS__COLOR_RGBO_H

namespace Graphics
{
	class ColorRGB;
	
	class ColorRGBO
	{
		friend class ColorRGB;
	public:
		ColorRGBO(void);
		ColorRGBO(const Graphics::ColorRGBO & Color);
		ColorRGBO(float Red, float Green, float Blue, float Opacity);
		float GetBlue(void) const;
		float GetGreen(void) const;
		float GetOpacity(void) const;
		float const * GetPointer(void) const;
		float GetRed(void) const;
		Graphics::ColorRGBO & operator=(const Graphics::ColorRGBO & Color);
		void Set(float Red, float Green, float Blue, float Opacity);
	private:
		union
		{
			struct
			{
				float Red;
				float Green;
				float Blue;
				float Opacity;
			} m_Color;
			float m_Array[4];
		};
	};
	
	inline float Graphics::ColorRGBO::GetBlue(void) const
	{
		return m_Color.Blue;
	}
	
	inline float Graphics::ColorRGBO::GetGreen(void) const
	{
		return m_Color.Green;
	}
	
	inline float Graphics::ColorRGBO::GetOpacity(void) const
	{
		return m_Color.Opacity;
	}
	
	inline float const * Graphics::ColorRGBO::GetPointer(void) const
	{
		return m_Array;
	}
	
	inline float Graphics::ColorRGBO::GetRed(void) const
	{
		return m_Color.Red;
	}
}

#endif
