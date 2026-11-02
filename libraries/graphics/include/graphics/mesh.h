/**
 * galactic-fall
 * Copyright (C) 2006-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__LIBRARIES__GRAPHICS__INCLUDE__GRAPHICS__MESH_H
#define GALACTIC_FALL__LIBRARIES__GRAPHICS__INCLUDE__GRAPHICS__MESH_H

#include <GL/gl.h>

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include <algebra/vector3f.h>

#include <graphics/transparent_string_hash.h>

namespace Graphics
{
    class RenderContext;
    
    class Mesh
    {
    private:
        struct Marker
        {
            std::optional<float> Length;
            std::optional<Quaternion> Orientation;
            std::optional<Vector3f> Position;
        };
        
    public:
        struct Triangle
        {
            std::vector< Vector3f >::size_type Points[3];
            Vector3f Normals[3];
        };
        
        Mesh(const std::string & Identifier);
        void Draw(Graphics::RenderContext * RenderContext) const;
        const std::string & GetIdentifier(void) const;
        auto GetMarkerLength(std::string_view MarkerIdentifier) const -> std::optional<float> const &;
        auto GetMarkerOrientation(std::string_view MarkerIdentifier) const -> std::optional<Quaternion> const &;
        auto GetMarkerPosition(std::string_view MarkerIdentifier) const -> std::optional<Vector3f> const &;
        float GetRadialSize(void) const;
        auto AddMarker(std::string_view Identifier) -> void;
        auto SetMarkerLength(std::string_view MarkerIdentifier, std::optional<float> Length) -> void;
        auto SetMarkerOrientation(std::string_view MarkerIdentifier, std::optional<Quaternion> Orientation) -> void;
        auto SetMarkerPosition(std::string_view MarkerIdentifier, std::optional<Vector3f> Position) -> void;
        std::vector< Vector3f >::size_type AddPoint(const Vector3f & Point);
        std::vector< Graphics::Mesh::Triangle >::size_type AddTriangle(std::vector< Vector3f >::size_type Point1Index, const Vector3f & Point1Normal, std::vector< Vector3f >::size_type Point2Index, const Vector3f & Point2Normal, std::vector< Vector3f >::size_type Point3Index, const Vector3f & Point3Normal);
        void BuildVertexArray(void);
    private:
        std::string _Identifier;
        std::unordered_map<std::string, Graphics::Mesh::Marker, Graphics::TransparentStringHash, std::equal_to<>> m_Markers;
        GLsizei _NumberOfIndices;
        std::vector< Vector3f > _Points;
        mutable float _RadialSize;
        std::vector< Graphics::Mesh::Triangle > _Triangles;
        GLuint _VertexArray;
    };
    
    inline const std::string & Mesh::GetIdentifier(void) const
    {
        return _Identifier;
    }
}

#endif
