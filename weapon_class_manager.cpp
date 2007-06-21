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

#include "weapon_class.h"
#include "weapon_class_manager.h"

WeaponClassManager::~WeaponClassManager(void)
{
	while(m_WeaponClasses.size() > 0)
	{
		Destroy(m_WeaponClasses.begin()->first);
	}
}

WeaponClass * WeaponClassManager::Get(const std::string & Identifier)
{
	std::map< std::string, WeaponClass * >::iterator WeaponClassIterator(m_WeaponClasses.find(Identifier));
	
	if(WeaponClassIterator == m_WeaponClasses.end())
	{
		return 0;
	}
	else
	{
		return WeaponClassIterator->second;
	}
}

WeaponClass * WeaponClassManager::Create(const std::string & Identifier)
{
	if(m_WeaponClasses.find(Identifier) != m_WeaponClasses.end())
	{
		return 0;
	}
	else
	{
		WeaponClass * NewWeaponClass(new WeaponClass(Identifier));
		
		m_WeaponClasses[NewWeaponClass->GetIdentifier()] = NewWeaponClass;
		
		return NewWeaponClass;
	}
}

void WeaponClassManager::Destroy(const std::string & Identifier)
{
	std::map< std::string, WeaponClass * >::iterator WeaponClassIterator(m_WeaponClasses.find(Identifier));
	
	if(WeaponClassIterator != m_WeaponClasses.end())
	{
		delete WeaponClassIterator->second;
		m_WeaponClasses.erase(WeaponClassIterator);
	}
}
