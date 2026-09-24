/**
 * galactic-fall
 * Copyright (C) 2018-2026  Hagen Möbius
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

#ifndef AMMUNITION_H
#define AMMUNITION_H

#include <cstdint>

#include "object.h"

class Ammunition : public Object
{
public:
	Ammunition();
	virtual ~Ammunition() override = default;
	auto GetAmount() const -> std::uint32_t;
	auto GetCapacity() const -> std::uint32_t;
	auto SetAmount(std::uint32_t Amount) -> void;
	auto SetCapacity(std::uint32_t Capacity) -> void;
private:
	std::uint32_t m_Amount{0};
	std::uint32_t m_Capacity{0};
};

inline auto Ammunition::GetAmount() const -> std::uint32_t
{
	return m_Amount;
}

inline auto Ammunition::GetCapacity() const -> std::uint32_t
{
	return m_Capacity;
}

inline auto Ammunition::SetAmount(std::uint32_t Amount) -> void
{
	m_Amount = Amount;
}

inline auto Ammunition::SetCapacity(std::uint32_t Capacity) -> void
{
	m_Capacity = Capacity;
}

#endif
