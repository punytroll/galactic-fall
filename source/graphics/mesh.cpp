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

#include "gl.h"
#include "mesh.h"

Graphics::Mesh::Mesh(const std::string & Identifier) :
	m_Identifier(Identifier),
	m_RadialSize(-1.0f)
{
}

void Graphics::Mesh::Draw(void) const
{
	GLBegin(GL_TRIANGLES);
	for(std::vector< Graphics::Mesh::Triangle >::size_type Triangle = 0; Triangle < m_Triangles.size(); ++Triangle)
	{
		GLNormal3fv(m_Triangles[Triangle].Normals[0].GetPointer());
		GLVertex3fv(m_Points[m_Triangles[Triangle].Points[0]].GetPointer());
		GLNormal3fv(m_Triangles[Triangle].Normals[1].GetPointer());
		GLVertex3fv(m_Points[m_Triangles[Triangle].Points[1]].GetPointer());
		GLNormal3fv(m_Triangles[Triangle].Normals[2].GetPointer());
		GLVertex3fv(m_Points[m_Triangles[Triangle].Points[2]].GetPointer());
	}
	GLEnd();
}

float Graphics::Mesh::GetRadialSize(void) const
{
	if(m_RadialSize < 0.0f)
	{
		for(std::vector< Vector4f >::const_iterator PointInterator = m_Points.begin(); PointInterator != m_Points.end(); ++PointInterator)
		{
			float RadialSquare(PointInterator->SquaredLength());
			
			if(m_RadialSize < RadialSquare)
			{
				m_RadialSize = RadialSquare;
			}
		}
		m_RadialSize = sqrt(m_RadialSize);
	}
	
	return m_RadialSize;
}

std::vector< Vector4f >::size_type Graphics::Mesh::AddPoint(const Vector4f & Point)
{
	m_Points.push_back(Point);
	
	return m_Points.size() - 1;
}

std::vector< Graphics::Mesh::Triangle >::size_type Graphics::Mesh::AddTriangle(std::vector< Vector4f >::size_type Point1Index, const Vector4f & Point1Normal, std::vector< Vector4f >::size_type Point2Index, const Vector4f & Point2Normal, std::vector< Vector4f >::size_type Point3Index, const Vector4f & Point3Normal)
{
	m_Triangles.push_back(Triangle());
	m_Triangles.back().Points[0] = Point1Index;
	m_Triangles.back().Normals[0] = Point1Normal;
	m_Triangles.back().Points[1] = Point2Index;
	m_Triangles.back().Normals[1] = Point2Normal;
	m_Triangles.back().Points[2] = Point3Index;
	m_Triangles.back().Normals[2] = Point3Normal;
	
	return m_Triangles.size() - 1;
}
