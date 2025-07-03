/**
 * galactic-fall
 * Copyright (C) 2006-2025  Hagen Möbius
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

#ifndef BUFFER_READING_H
#define BUFFER_READING_H

namespace Arxx
{
    class BufferReader;
}

namespace Graphics
{
	class ColorRGB;
	class ColorRGBO;
}

class Quaternion;
class Vector2f;
class Vector3f;
class VisualizationPrototype;

Arxx::BufferReader & operator>>(Arxx::BufferReader & BufferReader, Graphics::ColorRGB & Color);
Arxx::BufferReader & operator>>(Arxx::BufferReader & BufferReader, Graphics::ColorRGBO & Color);
Arxx::BufferReader & operator>>(Arxx::BufferReader & BufferReader, Quaternion & Quaternion);
Arxx::BufferReader & operator>>(Arxx::BufferReader & BufferReader, Vector2f & Vector);
Arxx::BufferReader & operator>>(Arxx::BufferReader & BufferReader, Vector3f & Vector);
Arxx::BufferReader & operator>>(Arxx::BufferReader & BufferReader, VisualizationPrototype & VisualizationPrototype);

#endif
