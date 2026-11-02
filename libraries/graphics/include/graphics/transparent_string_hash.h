/**
 * galactic-fall
 * Copyright (C) 2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__LIBRARIES__GRAPHICS__INCLUDE__GRAPHICS__TRANSPARENT_STRING_HASH_H
#define GALACTIC_FALL__LIBRARIES__GRAPHICS__INCLUDE__GRAPHICS__TRANSPARENT_STRING_HASH_H

namespace Graphics
{
    struct TransparentStringHash
    {
        using is_transparent = void;
        
        [[nodiscard]] auto operator()(char const * String) const noexcept -> std::size_t
        {
            return std::hash<std::string_view>{}(String);
        }
        
        [[nodiscard]] auto operator()(std::string_view String) const noexcept -> std::size_t
        {
            return std::hash<std::string_view>{}(String);
        }
        
        [[nodiscard]] auto operator()(std::string const & String) const noexcept -> std::size_t
        {
            return std::hash<std::string>{}(String);
        }
    };
}

#endif
