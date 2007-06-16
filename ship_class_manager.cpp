/**
 * galactic-fall
 * Copyright (C) 2006  Hagen MÃ¶bius
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

#include <fstream>

#include "color.h"
#include "model_manager.h"
#include "ship_class.h"
#include "ship_class_manager.h"
#include "string_cast.h"
#include "xml_puny_dom.h"

ShipClassManager::ShipClassManager(ModelManager * ModelManager) :
	m_ModelManager(ModelManager)
{
}

ShipClassManager::~ShipClassManager(void)
{
	while(m_ShipClasses.size() > 0)
	{
		Destroy(m_ShipClasses.begin()->first);
	}
}

ShipClass * ShipClassManager::Get(const std::string & Identifier)
{
	std::map< std::string, ShipClass * >::iterator ShipClassIterator(m_ShipClasses.find(Identifier));
	
	if(ShipClassIterator == m_ShipClasses.end())
	{
		return 0;
	}
	else
	{
		return ShipClassIterator->second;
	}
}

ShipClass * ShipClassManager::Create(const std::string & Identifier)
{
	if(m_ShipClasses.find(Identifier) != m_ShipClasses.end())
	{
		return 0;
	}
	else
	{
		ShipClass * NewShipClass(new ShipClass(Identifier));
		
		m_ShipClasses[NewShipClass->GetIdentifier()] = NewShipClass;
		
		return NewShipClass;
	}
}

void ShipClassManager::Destroy(const std::string & Identifier)
{
	std::map< std::string, ShipClass * >::iterator ShipClassIterator(m_ShipClasses.find(Identifier));
	
	if(ShipClassIterator != m_ShipClasses.end())
	{
		delete ShipClassIterator->second;
		m_ShipClasses.erase(ShipClassIterator);
	}
}
