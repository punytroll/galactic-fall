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

#include "cargo.h"
#include "commodity_class_manager.h"
#include "globals.h"
#include "object_factory.h"

Object * ObjectFactory::Create(const std::string & Type, const std::string & Class)
{
	if(Type == "cargo")
	{
		const CommodityClass * CommodityClass(g_CommodityClassManager.Get(Class));
		
		if(CommodityClass == 0)
		{
			throw std::runtime_error("Unknown object class '" + Class + "' for object type '" + Type + "'.");
		}
		
		return new Cargo(CommodityClass);
	}
	else
	{
		throw std::runtime_error("Unknown object type '" + Type + "'.");
	}
}
