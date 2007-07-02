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

#include "buffer_reading.h"

Arxx::BufferReader & operator>>(Arxx::BufferReader & BufferReader, Color & Color)
{
	float Red;
	float Green;
	float Blue;
	float Alpha;
	
	BufferReader >> Red >> Green >> Blue >> Alpha;
	Color.Set(Red, Green, Blue, Alpha);
	
	return BufferReader;
}

Arxx::BufferReader & operator>>(Arxx::BufferReader & BufferReader, Quaternion & Quaternion)
{
	float W;
	float X;
	float Y;
	float Z;
	
	BufferReader >> W >> X >> Y >> Z;
	Quaternion.Set(W, X, Y, Z);
	
	return BufferReader;
}

Arxx::BufferReader & operator>>(Arxx::BufferReader & BufferReader, Vector2f & Vector)
{
	float x;
	float y;
	
	BufferReader >> x >> y;
	Vector.Set(x, y);
	
	return BufferReader;
}

Arxx::BufferReader & operator>>(Arxx::BufferReader & BufferReader, Vector3f & Vector)
{
	float x;
	float y;
	float z;
	
	BufferReader >> x >> y >> z;
	Vector.Set(x, y, z);
	
	return BufferReader;
}
