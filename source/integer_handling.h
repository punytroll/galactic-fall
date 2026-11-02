/**
 * galactic-fall
 * Copyright (C) 2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__SOURCE__INTEGER_HANDLING_H
#define GALACTIC_FALL__SOURCE__INTEGER_HANDLING_H

namespace GalacticFall
{
    template<typename Type>
    concept Integer = (std::integral<Type> == true) && (std::same_as<Type, bool> == false);
    
    template<GalacticFall::Integer IntegerType>
    constexpr auto FromBigEndian(IntegerType Value) noexcept -> IntegerType
    {
        static_assert((std::endian::native == std::endian::little) || (std::endian::native == std::endian::big));
        
        if constexpr(std::endian::native == std::endian::big)
        {
            return Value;
        }
        else
        {
            return std::byteswap(Value);
        }
    }
}

#endif
