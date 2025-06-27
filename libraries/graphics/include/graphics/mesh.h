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

#ifndef GALACTIC_FALL__LIBRARY__GRAPHICS__INCLUDE__GRAPHICS__MESH_H
#define GALACTIC_FALL__LIBRARY__GRAPHICS__INCLUDE__GRAPHICS__MESH_H

#include <GL/gl.h>

#include <map>
#include <string>
#include <vector>

#include <algebra/vector3f.h>

namespace Graphics
{
	class RenderContext;
	
	class Mesh
	{
	public:
		struct Triangle
		{
			std::vector< Vector3f >::size_type Points[3];
			Vector3f Normals[3];
		};
		
		class Marker
		{
		public:
			// constructor and destructor
			Marker(void);
			~Marker(void);
			// queries
			const float * GetLength(void) const;
			const Quaternion * GetOrientation(void) const;
			const Vector3f * GetPosition(void) const;
			// modifications
			void SetLength(float Length);
			void SetOrientation(const Quaternion & Orientation);
			void SetPosition(const Vector3f & Position);
		private:
			float * _Length;
			Quaternion * _Orientation;
			Vector3f * _Position;
		};
		
		Mesh(const std::string & Identifier);
		void Draw(Graphics::RenderContext * RenderContext) const;
		const std::string & GetIdentifier(void) const;
		const float * GetMarkerLength(const std::string & MarkerIdentifier) const;
		const Quaternion * GetMarkerOrientation(const std::string & MarkerIdentifier) const;
		const Vector3f * GetMarkerPosition(const std::string & MarkerIdentifier) const;
		float GetRadialSize(void) const;
		void AddMarker(const std::string & Identifier);
		void SetMarkerLength(const std::string & MarkerIdentifier, float Length);
		void SetMarkerOrientation(const std::string & MarkerIdentifier, const Quaternion & Orientation);
		void SetMarkerPosition(const std::string & MarkerIdentifier, const Vector3f & Position);
		std::vector< Vector3f >::size_type AddPoint(const Vector3f & Point);
		std::vector< Graphics::Mesh::Triangle >::size_type AddTriangle(std::vector< Vector3f >::size_type Point1Index, const Vector3f & Point1Normal, std::vector< Vector3f >::size_type Point2Index, const Vector3f & Point2Normal, std::vector< Vector3f >::size_type Point3Index, const Vector3f & Point3Normal);
		void BuildVertexArray(void);
	private:
		std::string _Identifier;
		std::map< std::string, Graphics::Mesh::Marker > _Markers;
		GLsizei _NumberOfIndices;
		std::vector< Vector3f > _Points;
		mutable float _RadialSize;
		std::vector< Graphics::Mesh::Triangle > _Triangles;
		GLuint _VertexArray;
	};
	
	inline const float * Mesh::Marker::GetLength(void) const
	{
		return _Length;
	}
	
	inline const Quaternion * Mesh::Marker::GetOrientation(void) const
	{
		return _Orientation;
	}
	
	inline const Vector3f * Mesh::Marker::GetPosition(void) const
	{
		return _Position;
	}
	
	inline const std::string & Mesh::GetIdentifier(void) const
	{
		return _Identifier;
	}
}

#endif
