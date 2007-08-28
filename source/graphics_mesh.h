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

#ifndef GRAPHICS_MESH_H
#define GRAPHICS_MESH_H

#include <vector>
#include <string>

#include "math/vector4f.h"

namespace Graphics
{
	class Mesh
	{
	public:
		struct Triangle
		{
			std::vector< Vector4f >::size_type Points[3];
			Vector4f Normals[3];
		};
		
		Mesh(const std::string & Identifier);
		void Draw(void) const;
		const std::string & GetIdentifier(void) const;
		float GetRadialSize(void) const;
		std::vector< Vector4f >::size_type AddPoint(const Vector4f & Point);
		std::vector< Graphics::Mesh::Triangle >::size_type AddTriangle(std::vector< Vector4f >::size_type Point1Index, const Vector4f & Point1Normal, std::vector< Vector4f >::size_type Point2Index, const Vector4f & Point2Normal, std::vector< Vector4f >::size_type Point3Index, const Vector4f & Point3Normal);
	private:
		std::string m_Identifier;
		std::vector< Vector4f > m_Points;
		std::vector< Graphics::Mesh::Triangle > m_Triangles;
		mutable float m_RadialSize;
	};
}

inline const std::string & Graphics::Mesh::GetIdentifier(void) const
{
	return m_Identifier;
}

#endif
