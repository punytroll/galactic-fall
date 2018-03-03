/**
 * galactic-fall
 * Copyright (C) 2006-2018  Hagen Möbius
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

Graphics::Mesh::Marker::Marker(void) :
	_Orientation(nullptr),
	_Position(nullptr)
{
}

Graphics::Mesh::Marker::~Marker(void)
{
	delete _Orientation;
	_Orientation = nullptr;
	delete _Position;
	_Position = nullptr;
}

void Graphics::Mesh::Marker::SetOrientation(const Quaternion & Orientation)
{
	delete _Orientation;
	_Orientation = new Quaternion(Orientation);
}

void Graphics::Mesh::Marker::SetPosition(const Vector3f & Position)
{
	delete _Position;
	_Position = new Vector3f(Position);
}

Graphics::Mesh::Mesh(const std::string & Identifier) :
	_Identifier(Identifier),
	_NumberOfIndices(0),
	_RadialSize(-1.0f),
	_VertexArray(0)
{
}

void Graphics::Mesh::BuildVertexArray(void)
{
	_NumberOfIndices = _Triangles.size() * 9;
	
	auto Vertices(new GLfloat[_NumberOfIndices]);
	auto Normals(new GLfloat[_NumberOfIndices]);
	int Index(0);
	
	for(auto & Triangle : _Triangles)
	{
		auto & Vertex1(_Points[Triangle.Points[0]]);
		auto & Normal1(Triangle.Normals[0]);
		
		Vertices[Index] = Vertex1[0];
		Normals[Index] = Normal1[0];
		++Index;
		Vertices[Index] = Vertex1[1];
		Normals[Index] = Normal1[1];
		++Index;
		Vertices[Index] = Vertex1[2];
		Normals[Index] = Normal1[2];
		++Index;
		
		auto & Vertex2(_Points[Triangle.Points[1]]);
		auto & Normal2(Triangle.Normals[1]);
		
		Vertices[Index] = Vertex2[0];
		Normals[Index] = Normal2[0];
		++Index;
		Vertices[Index] = Vertex2[1];
		Normals[Index] = Normal2[1];
		++Index;
		Vertices[Index] = Vertex2[2];
		Normals[Index] = Normal2[2];
		++Index;
		
		auto & Vertex3(_Points[Triangle.Points[2]]);
		auto & Normal3(Triangle.Normals[2]);
		
		Vertices[Index] = Vertex3[0];
		Normals[Index] = Normal3[0];
		++Index;
		Vertices[Index] = Vertex3[1];
		Normals[Index] = Normal3[1];
		++Index;
		Vertices[Index] = Vertex3[2];
		Normals[Index] = Normal3[2];
		++Index;
	}
	GLGenVertexArrays(1, &_VertexArray);
	GLBindVertexArray(_VertexArray);
	
	GLuint VertexBuffer(0);
	
	GLGenBuffers(1, &VertexBuffer);
	GLBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	GLBufferData(GL_ARRAY_BUFFER, _NumberOfIndices * sizeof(GLfloat), Vertices, GL_STATIC_DRAW);
	GLVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 0, nullptr);
	GLEnableVertexAttribArray(0);
	
	GLuint NormalBuffer(0);
	
	GLGenBuffers(1, &NormalBuffer);
	GLBindBuffer(GL_ARRAY_BUFFER, NormalBuffer);
	GLBufferData(GL_ARRAY_BUFFER, _NumberOfIndices * sizeof(GLfloat), Normals, GL_STATIC_DRAW);
	GLVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, nullptr);
	GLEnableVertexAttribArray(1);
	GLBindVertexArray(0);
	delete[] Normals;
	delete[] Vertices;
}

void Graphics::Mesh::Draw(Graphics::RenderContext * RenderContext) const
{
	assert(_VertexArray != 0);
	assert(_NumberOfIndices != 0);
	GLBindVertexArray(_VertexArray);
	GLDrawArrays(GL_TRIANGLES, 0, _NumberOfIndices);
	GLBindVertexArray(0);
}

const Quaternion * Graphics::Mesh::GetMarkerOrientation(const std::string MarkerIdentifier) const
{
	auto MarkerIterator{_Markers.find(MarkerIdentifier)};
	
	if(MarkerIterator != _Markers.end())
	{
		return MarkerIterator->second.GetOrientation();
	}
	else
	{
		return nullptr;
	}
}

const Vector3f * Graphics::Mesh::GetMarkerPosition(const std::string MarkerIdentifier) const
{
	auto MarkerIterator{_Markers.find(MarkerIdentifier)};
	
	if(MarkerIterator != _Markers.end())
	{
		return MarkerIterator->second.GetPosition();
	}
	else
	{
		return nullptr;
	}
}

float Graphics::Mesh::GetRadialSize(void) const
{
	if(_RadialSize < 0.0f)
	{
		for(auto & Point : _Points)
		{
			float RadialSquare(Point.SquaredLength());
			
			if(_RadialSize < RadialSquare)
			{
				_RadialSize = RadialSquare;
			}
		}
		_RadialSize = sqrt(_RadialSize);
	}
	
	return _RadialSize;
}

void Graphics::Mesh::AddMarker(const std::string & Identifier)
{
	assert(_Markers.find(Identifier) == _Markers.end());
	_Markers[Identifier];
}

void Graphics::Mesh::SetMarkerOrientation(const std::string & Identifier, const Quaternion & Orientation)
{
	assert(_Markers.find(Identifier) != _Markers.end());
	_Markers[Identifier].SetOrientation(Orientation);
}

void Graphics::Mesh::SetMarkerPosition(const std::string & Identifier, const Vector3f & Position)
{
	assert(_Markers.find(Identifier) != _Markers.end());
	_Markers[Identifier].SetPosition(Position);
}

std::vector< Vector3f >::size_type Graphics::Mesh::AddPoint(const Vector3f & Point)
{
	_Points.push_back(Point);
	
	return _Points.size() - 1;
}

std::vector< Graphics::Mesh::Triangle >::size_type Graphics::Mesh::AddTriangle(std::vector< Vector3f >::size_type Point1Index, const Vector3f & Point1Normal, std::vector< Vector3f >::size_type Point2Index, const Vector3f & Point2Normal, std::vector< Vector3f >::size_type Point3Index, const Vector3f & Point3Normal)
{
	_Triangles.push_back(Triangle());
	_Triangles.back().Points[0] = Point1Index;
	_Triangles.back().Normals[0] = Point1Normal;
	_Triangles.back().Points[1] = Point2Index;
	_Triangles.back().Normals[1] = Point2Normal;
	_Triangles.back().Points[2] = Point3Index;
	_Triangles.back().Normals[2] = Point3Normal;
	
	return _Triangles.size() - 1;
}
