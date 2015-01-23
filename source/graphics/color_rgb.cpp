/**
 * galactic-fall
 * Copyright (C) 2015  Hagen Möbius
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

#include "color_rgb.h"
#include "color_rgbo.h"

Graphics::ColorRGB::ColorRGB(void)
{
}

Graphics::ColorRGB::ColorRGB(const Graphics::ColorRGB & Color) :
	_Red(Color._Red),
	_Green(Color._Green),
	_Blue(Color._Blue)
{
}

Graphics::ColorRGB::ColorRGB(const Graphics::ColorRGBO & Color) :
	_Red(Color._Red),
	_Green(Color._Green),
	_Blue(Color._Blue)
{
}

Graphics::ColorRGB::ColorRGB(float Red, float Green, float Blue) :
	_Red(Red),
	_Green(Green),
	_Blue(Blue)
{
}

Graphics::ColorRGB & Graphics::ColorRGB::operator=(const Graphics::ColorRGB & Color)
{
	if(this != &Color)
	{
		_Red = Color._Red;
		_Green = Color._Green;
		_Blue = Color._Blue;
	}
	
	return *this;
}

Graphics::ColorRGB & Graphics::ColorRGB::operator=(const Graphics::ColorRGBO & Color)
{
	_Red = Color._Red;
	_Green = Color._Green;
	_Blue = Color._Blue;
	
	return *this;
}

void Graphics::ColorRGB::Set(float Red, float Green, float Blue)
{
	_Red = Red;
	_Green = Green;
	_Blue = Blue;
}
