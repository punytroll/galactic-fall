/**
 * galactic-fall
 * Copyright (C) 2007  Hagen Möbius
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

#include "slot_class.h"
#include "slot_class_manager.h"

SlotClassManager::~SlotClassManager(void)
{
	while(m_SlotClasses.size() > 0)
	{
		Destroy(m_SlotClasses.begin()->first);
	}
}

const SlotClass * SlotClassManager::Get(const std::string & Identifier) const
{
	std::map< std::string, SlotClass * >::const_iterator SlotClassIterator(m_SlotClasses.find(Identifier));
	
	if(SlotClassIterator == m_SlotClasses.end())
	{
		return 0;
	}
	else
	{
		return SlotClassIterator->second;
	}
}

SlotClass * SlotClassManager::Create(const std::string & Identifier)
{
	if(m_SlotClasses.find(Identifier) != m_SlotClasses.end())
	{
		return 0;
	}
	else
	{
		SlotClass * NewSlotClass(new SlotClass(Identifier));
		
		m_SlotClasses[NewSlotClass->GetIdentifier()] = NewSlotClass;
		
		return NewSlotClass;
	}
}

void SlotClassManager::Destroy(const std::string & Identifier)
{
	std::map< std::string, SlotClass * >::iterator SlotClassIterator(m_SlotClasses.find(Identifier));
	
	if(SlotClassIterator != m_SlotClasses.end())
	{
		delete SlotClassIterator->second;
		m_SlotClasses.erase(SlotClassIterator);
	}
}
