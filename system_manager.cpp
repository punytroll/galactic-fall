/**
 * galactic-fall
 * Copyright (C) 2006  Hagen Möbius
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
#include <iostream>

#include "commodity_manager.h"
#include "planet.h"
#include "star.h"
#include "string_cast.h"
#include "system.h"
#include "system_manager.h"
#include "xml_puny_dom.h"

SystemManager::SystemManager(CommodityManager * CommodityManager) :
	m_CommodityManager(CommodityManager)
{
}

SystemManager::~SystemManager(void)
{
	while(m_Systems.size() > 0)
	{
		Destroy(m_Systems.begin()->first);
	}
}

System * SystemManager::Get(const std::string & Identifier)
{
	std::map< std::string, System * >::iterator SystemIterator(m_Systems.find(Identifier));
	
	if(SystemIterator == m_Systems.end())
	{
		return 0;
	}
	else
	{
		return SystemIterator->second;
	}
}

System * SystemManager::Create(const std::string & Identifier)
{
	if(m_Systems.find(Identifier) != m_Systems.end())
	{
		return 0;
	}
	else
	{
		System * NewSystem(new System(Identifier));
		
		m_Systems[NewSystem->GetIdentifier()] = NewSystem;
		
		return NewSystem;
	}
}

void SystemManager::Destroy(const std::string & Identifier)
{
	std::map< std::string, System * >::iterator SystemIterator(m_Systems.find(Identifier));
	
	if(SystemIterator != m_Systems.end())
	{
		delete SystemIterator->second;
		m_Systems.erase(SystemIterator);
	}
}
