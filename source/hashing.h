/**
 * galactic-fall
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

#ifndef HASHING_H
#define HASHING_H

template<typename ValueType, typename... RestType>
inline auto HashCombine(std::size_t & Seed, ValueType const & Value, RestType &&... Rest) -> void
{
    static_assert(sizeof(std::size_t) == 8);
    Seed ^= std::hash<ValueType>{}(Value) + 0x517cc1b727220a95 + (Seed << 6) + (Seed >> 2);
    (HashCombine(Seed, Rest), ...);
}

/**
 * @brief An implementation of hashing a std::pair.
 * @note This class can be used to create unordered sets or maps with std::pair keys.
 **/
struct PairHash
{
    template<class Type1, class Type2>
    auto operator()(std::pair<Type1, Type2> const & Pair) const -> std::size_t
    {
        auto Seed = static_cast<std::size_t>(0);
        
        HashCombine(Seed, Pair.first, Pair.second);
        
        return Seed;
    }
};

#endif
