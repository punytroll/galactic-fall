/**
 * galactic-fall
 * Copyright (C) 2015-2019  Hagen Möbius
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

#ifndef GRAPHICS_COLOR_RGB_H
#define GRAPHICS_COLOR_RGB_H

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
		const float * const GetPointer(void) const;
		Graphics::ColorRGB & operator=(const Graphics::ColorRGB & Color);
		Graphics::ColorRGB & operator=(const Graphics::ColorRGBO & Color);
		void Set(float Red, float Green, float Blue);
	private:
		union
		{
			struct
			{
				float _Red;
				float _Green;
				float _Blue;
			};
			float _[3];
		};
	};
	
	inline const float * const Graphics::ColorRGB::GetPointer(void) const
	{
		return _;
	}
}

#endif
