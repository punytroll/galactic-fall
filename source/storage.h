/**
 * galactic-fall
 * Copyright (C) 2008  Hagen Möbius
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

#ifndef STORAGE_H
#define STORAGE_H

#include "object.h"

class Storage : public Object
{
public:
	// constructor & destructor
	Storage(void);
	virtual ~Storage(void) override;
	// getters
	std::uint32_t GetSpace(void) const;
	std::uint32_t GetSpaceCapacity(void) const;
	// setters
	void SetSpaceCapacity(std::uint32_t SpaceCapacity);
private:
	void _OnAdded(Object * Content);
	void _OnRemoved(Object * Content);
	std::uint32_t _Space;
	std::uint32_t _SpaceCapacity;
};

inline std::uint32_t Storage::GetSpace(void) const
{
	return _Space;
}

inline std::uint32_t Storage::GetSpaceCapacity(void) const
{
	return _SpaceCapacity;
}

#endif
