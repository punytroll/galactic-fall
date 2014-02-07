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
	m_Space(0),
	m_SpaceCapacity(0)
{
	// initialize object aspects
	AddAspectObjectContainer();
	GetAspectObjectContainer()->SetAllowAddingCallback(Callback(this, &Storage::AllowAdding));
	GetAspectObjectContainer()->SetAllowRemovingCallback(Callback(this, &Storage::AllowRemoving));
	GetAspectObjectContainer()->SetOnAddedCallback(Callback(this, &Storage::OnAdded));
	GetAspectObjectContainer()->SetOnRemovedCallback(Callback(this, &Storage::OnRemoved));
}

Storage::~Storage(void)
{
}

unsigned_numeric Storage::GetAmount(const std::string & TypeIdentifier, const std::string & ClassIdentifier) const
{
	assert(GetAspectObjectContainer() != 0);
	
	unsigned_numeric Amount(0);
	const std::set< Object * > & Content(GetAspectObjectContainer()->GetContent());
	
	for(std::set< Object * >::const_iterator ContentIterator = Content.begin(); ContentIterator != Content.end(); ++ContentIterator)
	{
		Object * Content(*ContentIterator);
		
		if((Content->GetTypeIdentifier() == TypeIdentifier) && (Content->GetClassIdentifier() == ClassIdentifier))
		{
			Amount += 1;
		}
	}
	
	return Amount;
}

void Storage::SetSpaceCapacity(unsigned_numeric SpaceCapacity)
{
	signed_numeric SpaceDifference(SpaceCapacity - GetSpaceCapacity());
	
	m_SpaceCapacity = SpaceCapacity;
	m_Space += SpaceDifference;
}

bool Storage::AllowAdding(Object * Content)
{
	return true;
}

bool Storage::AllowRemoving(Object * Content)
{
	return true;
}

void Storage::OnAdded(Object * Content)
{
	if(Content->GetAspectPhysical() != nullptr)
	{
		assert(GetSpace() >= Content->GetAspectPhysical()->GetSpaceRequirement());
		m_Space -= Content->GetAspectPhysical()->GetSpaceRequirement();
	}
}

void Storage::OnRemoved(Object * Content)
{
	if(Content->GetAspectPhysical() != nullptr)
	{
		m_Space += Content->GetAspectPhysical()->GetSpaceRequirement();
	}
	assert(GetSpace() <= GetSpaceCapacity());
}
