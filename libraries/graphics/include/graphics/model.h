/**
 * galactic-fall
 * Copyright (C) 2006-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__LIBRARIES__GRAPHICS__INCLUDE__GRAPHICS__MODEL_H
#define GALACTIC_FALL__LIBRARIES__GRAPHICS__INCLUDE__GRAPHICS__MODEL_H

#include <memory>
#include <optional>
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
			Graphics::Mesh const & Mesh;
			Vector3f Position;
			Quaternion Orientation;
		};
		
		Model(std::string Identifier);
		auto AddPart(std::string const & PartIdentifier, Graphics::Mesh const & PartMesh, Vector3f const & PartPosition, Quaternion const & PartOrientation) -> void;
		auto GetIdentifier() const -> std::string const &;
		auto GetParts() const -> std::unordered_map<std::string, std::unique_ptr<Graphics::Model::Part const>, Graphics::TransparentStringHash, std::equal_to<>> const &;
		auto GetRadialSize() const -> float;
		auto GetMarkerLength(std::string_view PartIdentifier, std::string_view MarkerIdentifier) const -> std::optional<float> const &;
		auto GetMarkerOrientation(std::string_view PartIdentifier, std::string_view MarkerIdentifier) const -> std::optional<Quaternion> const &;
		auto GetMarkerPosition(std::string_view PartIdentifier, std::string_view MarkerIdentifier) const -> std::optional<Vector3f> const &;
        
	private:
        auto m_GetPart(std::string_view PartIdentifier) const -> Graphics::Model::Part const &;
        
        /**
         * @brief The unique identifier of the model.
         **/
		std::string m_Identifier;
		std::unordered_map<std::string, std::unique_ptr<Graphics::Model::Part const>, Graphics::TransparentStringHash, std::equal_to<>> m_Parts;
		mutable std::optional<float> m_RadialSize;
	};
	
	inline auto Model::GetIdentifier() const -> std::string const &
	{
		return m_Identifier;
	}
	
	inline auto Model::GetParts() const -> std::unordered_map<std::string, std::unique_ptr<Graphics::Model::Part const>, Graphics::TransparentStringHash, std::equal_to<>> const &
	{
		return m_Parts;
	}
}

#endif
