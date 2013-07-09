/**
 * algebra
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

/**
 * This is part of version 1.5.3 of algebra.
 **/

#ifndef ALGEBRA_DETAILS_H
#define ALGEBRA_DETAILS_H

typedef float vector4float __attribute__ ((vector_size(16)));

union Array4f
{
	vector4float m_V;
	float m_A[4];
};

typedef float vector2float __attribute__ ((vector_size(8)));

union Array2f
{
	vector2float m_V;
	float m_A[2];
};

#endif
