/**
 * galactic-fall
 * Copyright (C) 2006-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#include <cassert>

#include <graphics/mesh.h>
#include <graphics/model.h>

Graphics::Model::Model(const std::string & Identifier) :
	_Identifier(Identifier),
	_RadialSize(-1.0f)
{
}

Graphics::Model::~Model(void)
{
	while(m_Parts.empty() == false)
	{
		delete m_Parts.begin()->second;
		m_Parts.erase(m_Parts.begin());
	}
}

auto Graphics::Model::GetMarkerLength(std::string_view PartIdentifier, std::string_view MarkerIdentifier) const -> std::optional<float> const &
{
	auto PartIterator = m_Parts.find(PartIdentifier);
	
	assert(PartIterator != m_Parts.end());
	
	return PartIterator->second->Mesh->GetMarkerLength(MarkerIdentifier);
}

auto Graphics::Model::GetMarkerOrientation(std::string_view PartIdentifier, std::string_view MarkerIdentifier) const -> std::optional<Quaternion> const &
{
	auto PartIterator = m_Parts.find(PartIdentifier);
	
	assert(PartIterator != m_Parts.end());
	
	return PartIterator->second->Mesh->GetMarkerOrientation(MarkerIdentifier);
}

auto Graphics::Model::GetMarkerPosition(std::string_view PartIdentifier, std::string_view MarkerIdentifier) const -> std::optional<Vector3f> const &
{
	auto PartIterator = m_Parts.find(PartIdentifier);
	
	assert(PartIterator != m_Parts.end());
	
	return PartIterator->second->Mesh->GetMarkerPosition(MarkerIdentifier);
}

float Graphics::Model::GetRadialSize(void) const
{
	if(_RadialSize < 0.0f)
	{
		for(auto & PartIterator : m_Parts)
		{
			float RadialSize(PartIterator.second->Position.Length() + PartIterator.second->Mesh->GetRadialSize());
			
			if(_RadialSize < RadialSize)
			{
				_RadialSize = RadialSize;
			}
		}
	}
	
	return _RadialSize;
}

void Graphics::Model::AddPart(const std::string & PartIdentifier, const Graphics::Mesh * PartMesh, const Vector3f & PartPosition, const Quaternion & PartOrientation)
{
	assert(m_Parts.find(PartIdentifier) == m_Parts.end());
	
	auto Part(new Graphics::Model::Part());
	
	Part->Identifier = PartIdentifier;
	Part->Mesh = PartMesh;
	Part->Position = PartPosition;
	Part->Orientation = PartOrientation;
	m_Parts[PartIdentifier] = Part;
}
