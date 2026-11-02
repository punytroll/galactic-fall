/**
 * galactic-fall
 * Copyright (C) 2006-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__LIBRARIES__GRAPHICS__INCLUDE__GRAPHICS__MESH_H
#define GALACTIC_FALL__LIBRARIES__GRAPHICS__INCLUDE__GRAPHICS__MESH_H

#include <GL/gl.h>

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
        auto GetMarkerLength(std::string_view MarkerIdentifier) const -> float const *;
        auto GetMarkerOrientation(std::string_view MarkerIdentifier) const -> Quaternion const *;
        auto GetMarkerPosition(std::string_view MarkerIdentifier) const -> Vector3f const *;
        float GetRadialSize(void) const;
        auto AddMarker(std::string_view Identifier) -> void;
        auto SetMarkerLength(std::string_view MarkerIdentifier, float Length) -> void;
        auto SetMarkerOrientation(std::string_view MarkerIdentifier, Quaternion const& Orientation) -> void;
        auto SetMarkerPosition(std::string_view MarkerIdentifier, Vector3f const & Position) -> void;
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
