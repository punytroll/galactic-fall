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

#include <fstream>

#include "asset_class.h"
#include "asset_class_manager.h"
#include "string_cast.h"
#include "xml_puny_dom.h"

AssetClassManager::~AssetClassManager(void)
{
	while(m_AssetClasses.size() > 0)
	{
		Destroy(m_AssetClasses.begin()->first);
	}
}

const AssetClass * AssetClassManager::Get(const std::string & Identifier) const
{
	std::map< std::string, AssetClass * >::const_iterator AssetClassIterator(m_AssetClasses.find(Identifier));
	
	if(AssetClassIterator == m_AssetClasses.end())
	{
		return 0;
	}
	else
	{
		return AssetClassIterator->second;
	}
}

AssetClass * AssetClassManager::Create(const std::string & Identifier)
{
	if(m_AssetClasses.find(Identifier) != m_AssetClasses.end())
	{
		return 0;
	}
	else
	{
		AssetClass * NewAssetClass(new AssetClass(Identifier));
		
		m_AssetClasses[NewAssetClass->GetIdentifier()] = NewAssetClass;
		
		return NewAssetClass;
	}
}

void AssetClassManager::Destroy(const std::string & Identifier)
{
	std::map< std::string, AssetClass * >::iterator AssetClassIterator(m_AssetClasses.find(Identifier));
	
	if(AssetClassIterator != m_AssetClasses.end())
	{
		delete AssetClassIterator->second;
		m_AssetClasses.erase(AssetClassIterator);
	}
}
