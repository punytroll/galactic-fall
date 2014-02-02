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

#ifndef COLOR_H
#define COLOR_H

#include <algebra/vector4f.h>

class Color
{
public:
	Color(void);
	explicit Color(const Color * Color);
	Color(float Red, float Green, float Blue, float Opacity);
	const Vector4f & GetColor(void) const;
	void Set(float Red, float Green, float Blue, float Opacity);
private:
	Vector4f _Color;
};

inline const Vector4f & Color::GetColor(void) const
{
	return _Color;
}

#endif
