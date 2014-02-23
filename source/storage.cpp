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

#include "object_aspect_object_container.h"
#include "object_aspect_physical.h"
#include "storage.h"

Storage::Storage(void) :
	_Space(0),
	_SpaceCapacity(0)
{
	// initialize object aspects
	AddAspectObjectContainer();
	GetAspectObjectContainer()->SetOnAddedCallback(Callback(this, &Storage::_OnAdded));
	GetAspectObjectContainer()->SetOnRemovedCallback(Callback(this, &Storage::_OnRemoved));
}

Storage::~Storage(void)
{
}

unsigned_numeric Storage::GetAmount(const std::string & TypeIdentifier, const std::string & ClassIdentifier) const
{
	assert(GetAspectObjectContainer() != nullptr);
	
	unsigned_numeric Amount(0);
	
	for(auto Content : GetAspectObjectContainer()->GetContent())
	{
		if((Content->GetTypeIdentifier() == TypeIdentifier) && (Content->GetClassIdentifier() == ClassIdentifier))
		{
			Amount += 1;
		}
	}
	
	return Amount;
}

void Storage::SetSpaceCapacity(unsigned_numeric SpaceCapacity)
{
	signed_numeric SpaceDifference(SpaceCapacity - _SpaceCapacity);
	
	_SpaceCapacity = SpaceCapacity;
	_Space += SpaceDifference;
}

void Storage::_OnAdded(Object * Content)
{
	if(Content->GetAspectPhysical() != nullptr)
	{
		assert(_Space >= Content->GetAspectPhysical()->GetSpaceRequirement());
		_Space -= Content->GetAspectPhysical()->GetSpaceRequirement();
	}
}

void Storage::_OnRemoved(Object * Content)
{
	if(Content->GetAspectPhysical() != nullptr)
	{
		_Space += Content->GetAspectPhysical()->GetSpaceRequirement();
	}
	assert(_Space <= _SpaceCapacity);
}
