/**
 * ARX - Advanced Resource Archives
 * Copyright (C) 2026  Hagen Möbius
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

#ifndef ARX__SOURCE__IO_H
#define ARX__SOURCE__IO_H

#include <bit>
#include <cstdint>
#include <istream>

namespace ARX
{
    template<std::integral ValueType>
    [[nodiscard]]
    inline auto ToNetworkByteOrder(ValueType Value) -> ValueType
    {
        if constexpr(std::endian::native == std::endian::little)
        {
            return std::byteswap(Value);
        }
        else
        {
            return Value;
        }
    }
    
    template<std::integral ValueType>
    [[nodiscard]]
    inline auto FromNetworkByteOrder(ValueType Value) -> ValueType
    {
        if constexpr(std::endian::native == std::endian::little)
        {
            return std::byteswap(Value);
        }
        else
        {
            return Value;
        }
    }
    
    [[nodiscard]]
    inline static auto ReadStringWithTermination(std::istream & IStream) -> std::string
    {
        auto Result = std::string{};
        auto Char = '\0';
        
        while(IStream.get(Char))
        {
            if(Char != '\0')
            {
                Result += Char;
            }
            else
            {
                break;
            }
        }
        
        return Result;
    }
    
    [[nodiscard]]
    inline auto ReadUnsignedInteger8Bit(std::istream & IStream) -> std::uint8_t
    {
        auto Result = std::uint8_t{};
        
        IStream.read(reinterpret_cast<std::istream::char_type *>(&Result), sizeof(Result));
        
        return ARX::FromNetworkByteOrder(Result);
    }
    
    [[nodiscard]]
    inline auto ReadUnsignedInteger32Bit(std::istream & IStream) -> std::uint32_t
    {
        auto Result = std::uint32_t{};
        
        IStream.read(reinterpret_cast<std::istream::char_type *>(&Result), sizeof(Result));
        
        return ARX::FromNetworkByteOrder(Result);
    }
    
    inline auto WriteUnsignedInteger8Bit(std::ostream & OStream, std::uint8_t Value) -> void
    {
        Value = ARX::ToNetworkByteOrder(Value);
		OStream.write(reinterpret_cast<std::ostream::char_type *>(&Value), sizeof(Value));
    }
    
    inline auto WriteUnsignedInteger32Bit(std::ostream & OStream, std::uint32_t Value) -> void
    {
        Value = ARX::ToNetworkByteOrder(Value);
		OStream.write(reinterpret_cast<std::ostream::char_type *>(&Value), sizeof(Value));
    }
}

#endif
