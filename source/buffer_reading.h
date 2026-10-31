/**
 * galactic-fall
 * Copyright (C) 2006-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__SOURCE__BUFFER_READING_H
#define GALACTIC_FALL__SOURCE__BUFFER_READING_H

namespace ARX
{
    class BufferReader;
    class Item;
}

namespace Graphics
{
	class ColorRGB;
	class ColorRGBO;
}

class Quaternion;
class Vector2f;
class Vector3f;
class VisualizationPrototype;

namespace GalacticFall
{
    class BufferReader
    {
    public:
        BufferReader(ARX::Item & Item);
        [[nodiscard]] auto GetCurrentData() -> std::byte const *;
        [[nodiscard]] auto ReadBoolean() -> bool;
        [[nodiscard]] auto ReadColourRGB() -> Graphics::ColorRGB;
        [[nodiscard]] auto ReadColourRGBO() -> Graphics::ColorRGBO;
        [[nodiscard]] auto ReadFloat() -> float;
        [[nodiscard]] auto ReadQuaternion() -> Quaternion;
        [[nodiscard]] auto ReadString() -> std::string;
        [[nodiscard]] auto ReadUnsignedInteger32Bit() -> std::uint32_t;
        [[nodiscard]] auto ReadVector2f() -> Vector2f;
        [[nodiscard]] auto ReadVector3f() -> Vector3f;
        [[nodiscard]] auto ReadVisualisationPrototype() -> VisualizationPrototype;
        
    private:
        ARX::BufferReader m_BufferReader;
    };
}

#endif
