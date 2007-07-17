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

#include <stdexcept>

#include "asset_class_manager.h"
#include "commodity.h"
#include "globals.h"
#include "object_factory.h"
#include "weapon.h"
#include "weapon_class_manager.h"

Object * ObjectFactory::Create(const std::string & Type, const std::string & Class)
{
	if(Type == "commodity")
	{
		const AssetClass * AssetClass(g_AssetClassManager->Get(Class));
		
		if(AssetClass == 0)
		{
			throw std::runtime_error("Unknown object class '" + Class + "' for object type '" + Type + "'.");
		}
		
		return new Commodity(AssetClass);
	}
	else if(Type == "weapon")
	{
		const WeaponClass * WeaponClass(g_WeaponClassManager->Get(Class));
		
		if(WeaponClass == 0)
		{
			throw std::runtime_error("Unknown object class '" + Class + "' for object type '" + Type + "'.");
		}
		
		return new Weapon(WeaponClass);
	}
	else
	{
		throw std::runtime_error("Unknown object type '" + Type + "'.");
	}
}
