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

#include "commodity.h"
#include "commodity_manager.h"
#include "string_cast.h"
#include "xml_puny_dom.h"

CommodityManager::~CommodityManager(void)
{
	while(m_Commodities.size() > 0)
	{
		Destroy(m_Commodities.begin()->first);
	}
}

Commodity * CommodityManager::Get(const std::string & Identifier)
{
	std::map< std::string, Commodity * >::iterator CommodityIterator(m_Commodities.find(Identifier));
	
	if(CommodityIterator == m_Commodities.end())
	{
		return 0;
	}
	else
	{
		return CommodityIterator->second;
	}
}

Commodity * CommodityManager::Create(const std::string & Identifier)
{
	if(m_Commodities.find(Identifier) != m_Commodities.end())
	{
		return 0;
	}
	else
	{
		Commodity * NewCommodity(new Commodity(Identifier));
		
		m_Commodities[NewCommodity->GetIdentifier()] = NewCommodity;
		
		return NewCommodity;
	}
}

void CommodityManager::Destroy(const std::string & Identifier)
{
	std::map< std::string, Commodity * >::iterator CommodityIterator(m_Commodities.find(Identifier));
	
	if(CommodityIterator != m_Commodities.end())
	{
		delete CommodityIterator->second;
		m_Commodities.erase(CommodityIterator);
	}
}
