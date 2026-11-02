/**
 * galactic-fall
 * Copyright (C) 2006-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__LIBRARIES__GRAPHICS__INCLUDE__GRAPHICS__MODEL_H
#define GALACTIC_FALL__LIBRARIES__GRAPHICS__INCLUDE__GRAPHICS__MODEL_H

#include <string>
#include <unordered_map>

#include <algebra/quaternion.h>
#include <algebra/vector3f.h>

#include <graphics/transparent_string_hash.h>

namespace Graphics
{
	class Mesh;
	
	class Model
	{
	public:
		struct Part
		{
			std::string Identifier;
			const Graphics::Mesh * Mesh;
			Vector3f Position;
			Quaternion Orientation;
		};
		
		// constructor
		Model(const std::string & Identifier);
		~Model(void);
		// getters
		const std::string & GetIdentifier(void) const;
		auto GetParts() const -> std::unordered_map<std::string, Graphics::Model::Part const *, Graphics::TransparentStringHash, std::equal_to<>> const &;
		float GetRadialSize(void) const;
		auto GetMarkerLength(std::string_view PartIdentifier, std::string_view MarkerIdentifier) const -> float const *;
		auto GetMarkerOrientation(std::string_view PartIdentifier, std::string_view MarkerIdentifier) const -> Quaternion const *;
		auto GetMarkerPosition(std::string_view PartIdentifier, std::string_view MarkerIdentifier) const -> Vector3f const *;
		// modifiers
		void AddPart(const std::string & PartIdentifier, const Graphics::Mesh * PartMesh, const Vector3f & PartPosition, const Quaternion & PartOrientation);
	private:
		std::string _Identifier;
		std::unordered_map<std::string, Graphics::Model::Part const *, Graphics::TransparentStringHash, std::equal_to<>> m_Parts;
		mutable float _RadialSize;
	};
	
	inline const std::string & Model::GetIdentifier(void) const
	{
		return _Identifier;
	}
	
	inline auto Model::GetParts() const -> std::unordered_map<std::string, Graphics::Model::Part const *, Graphics::TransparentStringHash, std::equal_to<>> const &
	{
		return m_Parts;
	}
}

#endif
