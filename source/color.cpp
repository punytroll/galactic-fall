/**
 * galactic-fall
 * Copyright (C) 2006  Hagen Möbius
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

#include "color.h"

Color::Color(void)
{
}

Color::Color(const Color * Color) :
	_Color(Color->_Color)
{
}

Color::Color(float Red, float Green, float Blue, float Opacity) :
	_Color(Red, Green, Blue, Opacity)
{
}

void Color::Set(float Red, float Green, float Blue, float Opacity)
{
	_Color.Set(Red, Green, Blue, Opacity);
}
