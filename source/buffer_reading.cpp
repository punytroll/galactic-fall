/**
 * galactic-fall
 * Copyright (C) 2006-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#include <cassert>
#include <cstdint>
#include <functional>

#include <arx/buffer_reader.h>
#include <arx/item.h>

#include <algebra/quaternion.h>
#include <algebra/vector2f.h>
#include <algebra/vector3f.h>

#include <graphics/color_rgb.h>
#include <graphics/color_rgbo.h>
#include <graphics/engine.h>
#include <graphics/model_manager.h>
#include <graphics/style.h>

#include "buffer_reading.h"
#include "globals.h"
#include "integer_handling.h"
#include "visualization_prototype.h"

namespace GalacticFall
{
    namespace
    {
        template<typename ValueType>
        auto ReadOptional(GalacticFall::BufferReader & BufferReader, auto BufferReaderFunction, std::uint32_t Skip) -> std::optional<ValueType>
        {
            auto Engaged = BufferReader.ReadBoolean();
            
            if(Engaged == true)
            {
                return (BufferReader.*BufferReaderFunction)();
            }
            else
            {
                BufferReader.Skip(Skip);
                
                return std::nullopt;
            }
        }
    }
}

GalacticFall::BufferReader::BufferReader(ARX::Item & Item) :
    m_BufferReader{Item}
{
}

auto GalacticFall::BufferReader::GetCurrentData() -> std::byte const *
{
    return m_BufferReader.GetBuffer().GetBegin() + m_BufferReader.GetPosition();
}

auto GalacticFall::BufferReader::ReadBoolean() -> bool
{
    auto Result = bool{};
    
    m_BufferReader.Read(sizeof(bool), reinterpret_cast<ARX::Buffer::pointer>(&Result));
    
    return Result;
}

auto GalacticFall::BufferReader::ReadColourRGB() -> Graphics::ColorRGB
{
	auto Red = ReadFloat();
	auto Green = ReadFloat();
	auto Blue = ReadFloat();
	
	return Graphics::ColorRGB{Red, Green, Blue};
}

auto GalacticFall::BufferReader::ReadColourRGBO() -> Graphics::ColorRGBO
{
	auto Red = ReadFloat();
	auto Green = ReadFloat();
	auto Blue = ReadFloat();
	auto Opacity = ReadFloat();
	
	return Graphics::ColorRGBO{Red, Green, Blue, Opacity};
}

auto GalacticFall::BufferReader::ReadFloat() -> float
{
    static_assert(sizeof(float) == sizeof(std::uint32_t));
    static_assert(std::numeric_limits<float>::is_iec559 == true);
    
    auto Result = ReadUnsignedInteger32Bit();
    
    return std::bit_cast<float>(Result);
}

auto GalacticFall::BufferReader::ReadOptionalColourRGBO() -> std::optional<Graphics::ColorRGBO>
{
    return GalacticFall::ReadOptional<Graphics::ColorRGBO>(*this, &GalacticFall::BufferReader::ReadColourRGBO, 16);
}

auto GalacticFall::BufferReader::ReadOptionalFloat() -> std::optional<float>
{
    return GalacticFall::ReadOptional<float>(*this, &GalacticFall::BufferReader::ReadFloat, 4);
}

auto GalacticFall::BufferReader::ReadOptionalQuaternion() -> std::optional<Quaternion>
{
    return GalacticFall::ReadOptional<Quaternion>(*this, &GalacticFall::BufferReader::ReadQuaternion, 16);
}

auto GalacticFall::BufferReader::ReadOptionalVector3f() -> std::optional<Vector3f>
{
    return GalacticFall::ReadOptional<Vector3f>(*this, &GalacticFall::BufferReader::ReadVector3f, 12);
}

auto GalacticFall::BufferReader::ReadQuaternion() -> Quaternion
{
	auto W = ReadFloat();
	auto X = ReadFloat();
	auto Y = ReadFloat();
	auto Z = ReadFloat();
    
    return Quaternion{W, X, Y, Z};
}

auto GalacticFall::BufferReader::ReadString() -> std::string
{
    auto Result = std::string{};
    auto const & Buffer = m_BufferReader.GetBuffer();
    auto Index = m_BufferReader.GetPosition();
    auto Length = Buffer.GetLength();
    
    while(Length > Index)
    {
        if(auto Byte = Buffer[Index++]; Byte == std::byte{0})
        {
            break;
        }
        else
        {
            Result += static_cast<std::string::value_type>(Byte);
        }
    }
    m_BufferReader.SetPosition(Index);
    
    return Result;
}

auto GalacticFall::BufferReader::ReadUnsignedInteger32Bit() -> std::uint32_t
{
    auto Result = std::uint32_t{};
    
    m_BufferReader.Read(sizeof(std::uint32_t), reinterpret_cast<ARX::Buffer::pointer>(&Result));
    
    return GalacticFall::FromBigEndian(Result);
}

auto GalacticFall::BufferReader::ReadVector2f() -> Vector2f
{
	auto X = ReadFloat();
	auto Y = ReadFloat();
    
    return Vector2f{X, Y};
}

auto GalacticFall::BufferReader::ReadVector3f() -> Vector3f
{
	auto X = ReadFloat();
	auto Y = ReadFloat();
	auto Z = ReadFloat();
    
    return Vector3f::CreateFromComponents(X, Y, Z);
}

auto GalacticFall::BufferReader::ReadVisualisationPrototype() -> VisualizationPrototype
{
    auto Result = VisualizationPrototype{};
	auto ModelIdentifier = ReadString();
    
	assert(g_GraphicsEngine != nullptr);
	assert(g_GraphicsEngine->GetModelManager() != nullptr);
	
	auto Model = g_GraphicsEngine->GetModelManager()->Get(ModelIdentifier);
	
	if(Model == nullptr)
	{
		throw std::runtime_error("Could not find the model \"" + ModelIdentifier + "\".");
	}
	Result.SetModel(Model);
	
	auto VisualisationPartCount = ReadUnsignedInteger32Bit();
	
	for(auto VisualisationPartIndex = std::uint32_t{0}; VisualisationPartIndex < VisualisationPartCount; ++VisualisationPartIndex)
	{
		auto PartStyle = new Graphics::Style{};
        auto PartIdentifier = ReadString();
        
		PartStyle->SetDiffuseColor(ReadColourRGBO());
        PartStyle->SetSpecularColor(ReadOptionalColourRGBO());
        PartStyle->SetShininess(ReadOptionalFloat());
		PartStyle->SetProgramIdentifier(ReadString());
		Result.SetPartStyle(PartIdentifier, PartStyle);
	}
	
	return Result;
}

auto GalacticFall::BufferReader::Skip(std::uint32_t Offset) -> void
{
    m_BufferReader.Skip(Offset);
}
