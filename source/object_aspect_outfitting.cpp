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

#include <cassert>

#include "object_aspect_outfitting.h"
#include "slot.h"

ObjectAspectOutfitting::ObjectAspectOutfitting(void)
{
}

ObjectAspectOutfitting::~ObjectAspectOutfitting(void)
{
	while(m_Slots.empty() == false)
	{
		delete m_Slots.begin()->second;
		m_Slots.erase(m_Slots.begin());
	}
}

void ObjectAspectOutfitting::AddSlot(Slot * Slot)
{
	assert(Slot != 0);
	assert(m_Slots.find(Slot->GetIdentifier()) == m_Slots.end());
	m_Slots[Slot->GetIdentifier()] = Slot;
}
