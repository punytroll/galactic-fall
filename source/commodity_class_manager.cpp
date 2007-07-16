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

#include "commodity_class.h"
#include "commodity_class_manager.h"
#include "string_cast.h"
#include "xml_puny_dom.h"

CommodityClassManager::~CommodityClassManager(void)
{
	while(m_CommodityClasses.size() > 0)
	{
		Destroy(m_CommodityClasses.begin()->first);
	}
}

const CommodityClass * CommodityClassManager::Get(const std::string & Identifier) const
{
	std::map< std::string, CommodityClass * >::const_iterator CommodityClassIterator(m_CommodityClasses.find(Identifier));
	
	if(CommodityClassIterator == m_CommodityClasses.end())
	{
		return 0;
	}
	else
	{
		return CommodityClassIterator->second;
	}
}

CommodityClass * CommodityClassManager::Create(const std::string & Identifier)
{
	if(m_CommodityClasses.find(Identifier) != m_CommodityClasses.end())
	{
		return 0;
	}
	else
	{
		CommodityClass * NewCommodityClass(new CommodityClass(Identifier));
		
		m_CommodityClasses[NewCommodityClass->GetIdentifier()] = NewCommodityClass;
		
		return NewCommodityClass;
	}
}

void CommodityClassManager::Destroy(const std::string & Identifier)
{
	std::map< std::string, CommodityClass * >::iterator CommodityClassIterator(m_CommodityClasses.find(Identifier));
	
	if(CommodityClassIterator != m_CommodityClasses.end())
	{
		delete CommodityClassIterator->second;
		m_CommodityClasses.erase(CommodityClassIterator);
	}
}
