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
#include "type_definitions.h"

class Storage : public Object
{
public:
	// constructor & destructor
	Storage(void);
	virtual ~Storage(void) override;
	// getters
	unsigned_numeric GetAmount(const std::string & TypeIdentifier, const std::string & ClassIdentifier) const;
	unsigned_numeric GetSpace(void) const;
	unsigned_numeric GetSpaceCapacity(void) const;
	// setters
	void SetSpaceCapacity(unsigned_numeric SpaceCapacity);
private:
	void _OnAdded(Object * Content);
	void _OnRemoved(Object * Content);
	unsigned_numeric _Space;
	unsigned_numeric _SpaceCapacity;
};

inline unsigned_numeric Storage::GetSpace(void) const
{
	return _Space;
}

inline unsigned_numeric Storage::GetSpaceCapacity(void) const
{
	return _SpaceCapacity;
}

#endif
