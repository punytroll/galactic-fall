/**
 * galactic-fall
 * Copyright (C) 2006-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#include <cassert>

#include <graphics/mesh.h>
#include <graphics/model.h>

Graphics::Model::Model(std::string Identifier) :
	m_Identifier{std::move(Identifier)}
{
}

auto Graphics::Model::AddPart(std::string const & PartIdentifier, Graphics::Mesh const & PartMesh, Vector3f const & PartPosition, Quaternion const & PartOrientation) -> void
{
    auto const & [Iterator, Successful] = m_Parts.try_emplace(PartIdentifier, std::make_unique<Graphics::Model::Part>(PartIdentifier, PartMesh, PartPosition, PartOrientation));
    
    assert(Successful == true);
    std::ignore = Iterator;
}

auto Graphics::Model::GetMarkerLength(std::string_view PartIdentifier, std::string_view MarkerIdentifier) const -> std::optional<float> const &
{
	return m_GetPart(PartIdentifier).Mesh.GetMarkerLength(MarkerIdentifier);
}

auto Graphics::Model::GetMarkerOrientation(std::string_view PartIdentifier, std::string_view MarkerIdentifier) const -> std::optional<Quaternion> const &
{
	return m_GetPart(PartIdentifier).Mesh.GetMarkerOrientation(MarkerIdentifier);
}

auto Graphics::Model::GetMarkerPosition(std::string_view PartIdentifier, std::string_view MarkerIdentifier) const -> std::optional<Vector3f> const &
{
	return m_GetPart(PartIdentifier).Mesh.GetMarkerPosition(MarkerIdentifier);
}

auto Graphics::Model::GetRadialSize() const -> float
{
	if(m_RadialSize.has_value() == false)
	{
		for(auto & PartIterator : m_Parts)
		{
            m_RadialSize = std::max(m_RadialSize.value_or(0.0f), PartIterator.second->Position.Length() + PartIterator.second->Mesh.GetRadialSize());
		}
	}
    assert(m_RadialSize.has_value() == true);
	
	return m_RadialSize.value();
}

auto Graphics::Model::m_GetPart(std::string_view PartIdentifier) const -> Graphics::Model::Part const &
{
	auto PartIterator = m_Parts.find(PartIdentifier);
	
	assert(PartIterator != m_Parts.end());
	
	return *(PartIterator->second);
}
