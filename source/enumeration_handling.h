/**
 * galactic-fall
 * Copyright (C) 2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__SOURCE__ENUMERATION_HANDLING_H
#define GALACTIC_FALL__SOURCE__ENUMERATION_HANDLING_H

#include <format>
#include <meta>
#include <type_traits>

namespace std
{
    template<typename EnumerationType>
        requires std::is_enum_v<EnumerationType>
    struct formatter<EnumerationType, char>
    {
        constexpr auto parse(std::format_parse_context & FormatParseContext) -> std::format_parse_context::iterator
        {
            return FormatParseContext.begin();
        }
        
        auto format(EnumerationType EnumerationValue, std::format_context & FormatContext) const -> std::format_context::iterator
        {
            template for(constexpr auto EnumerationItemReflectionValue : std::define_static_array(std::meta::enumerators_of(^^EnumerationType)))
            {
                if(EnumerationValue == [:EnumerationItemReflectionValue:])
                {
                    return std::format_to(FormatContext.out(), "{}", std::meta::identifier_of(EnumerationItemReflectionValue));
                }
            }
            
            return std::format_to(FormatContext.out(), "<unknown>");
        }
    };
}

#endif
