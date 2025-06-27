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

#include <memory>

#include <graphics/color_rgb.h>
#include <graphics/color_rgbo.h>

Graphics::ColorRGB::ColorRGB(void)
{
}

Graphics::ColorRGB::ColorRGB(const Graphics::ColorRGB & Other) :
	m_Color{Other.m_Color}
{
}

Graphics::ColorRGB::ColorRGB(const Graphics::ColorRGBO & Other) :
	m_Color{Other.m_Color.Red, Other.m_Color.Green, Other.m_Color.Blue}
{
}

Graphics::ColorRGB::ColorRGB(float Red, float Green, float Blue) :
	m_Color{Red, Green, Blue}
{
}

Graphics::ColorRGB & Graphics::ColorRGB::operator=(const Graphics::ColorRGB & Other)
{
	if(this != std::addressof(Other))
	{
		m_Color.Red = Other.m_Color.Red;
		m_Color.Green = Other.m_Color.Green;
		m_Color.Blue = Other.m_Color.Blue;
	}
	
	return *this;
}

Graphics::ColorRGB & Graphics::ColorRGB::operator=(const Graphics::ColorRGBO & Other)
{
	m_Color.Red = Other.m_Color.Red;
	m_Color.Green = Other.m_Color.Green;
	m_Color.Blue = Other.m_Color.Blue;
	
	return *this;
}

void Graphics::ColorRGB::Set(float Red, float Green, float Blue)
{
	m_Color.Red = Red;
	m_Color.Green = Green;
	m_Color.Blue = Blue;
}
