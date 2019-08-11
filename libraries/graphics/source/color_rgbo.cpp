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

#include <graphics/color_rgbo.h>

Graphics::ColorRGBO::ColorRGBO(void)
{
}

Graphics::ColorRGBO::ColorRGBO(const Graphics::ColorRGBO & Color) :
	_Red(Color._Red),
	_Green(Color._Green),
	_Blue(Color._Blue),
	_Opacity(Color._Opacity)
{
}

Graphics::ColorRGBO::ColorRGBO(float Red, float Green, float Blue, float Opacity) :
	_Red(Red),
	_Green(Green),
	_Blue(Blue),
	_Opacity(Opacity)
{
}

Graphics::ColorRGBO & Graphics::ColorRGBO::operator=(const Graphics::ColorRGBO & Color)
{
	if(this != &Color)
	{
		_Red = Color._Red;
		_Green = Color._Green;
		_Blue = Color._Blue;
		_Opacity = Color._Opacity;
	}
	
	return *this;
}

void Graphics::ColorRGBO::Set(float Red, float Green, float Blue, float Opacity)
{
	_Red = Red;
	_Green = Green;
	_Blue = Blue;
	_Opacity = Opacity;
}
