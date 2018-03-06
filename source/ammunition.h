/**
 * galactic-fall
 * Copyright (C) 2018  Hagen Möbius
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

#include "object.h"

class Ammunition : public Object
{
public:
	Ammunition(void);
	virtual ~Ammunition(void) override;
	// getters
	std::uint32_t GetAmount(void) const;
	std::uint32_t GetCapacity(void) const;
	// setters
	void SetAmount(std::uint32_t Amount);
	void SetCapacity(std::uint32_t Capacity);
private:
	std::uint32_t _Amount;
	std::uint32_t _Capacity;
};

inline std::uint32_t Ammunition::GetAmount(void) const
{
	return _Amount;
}

inline std::uint32_t Ammunition::GetCapacity(void) const
{
	return _Capacity;
}

inline void Ammunition::SetAmount(std::uint32_t Amount)
{
	_Amount = Amount;
}

inline void Ammunition::SetCapacity(std::uint32_t Capacity)
{
	_Capacity = Capacity;
}

#endif
