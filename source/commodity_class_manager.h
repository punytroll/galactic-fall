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

#ifndef COMMODITY_CLASS_MANAGER_H
#define COMMODITY_CLASS_MANAGER_H

#include <map>
#include <string>

class CommodityClass;

class CommodityClassManager
{
public:
	~CommodityClassManager(void);
	const std::map< std::string, CommodityClass * > & GetCommodityClasses(void) const;
	const CommodityClass * Get(const std::string & Identifier) const;
	CommodityClass * Create(const std::string & Identifier);
	void Destroy(const std::string & Identifier);
private:
	std::map< std::string, CommodityClass * > m_CommodityClasses;
};

inline const std::map< std::string, CommodityClass * > & CommodityClassManager::GetCommodityClasses(void) const
{
	return m_CommodityClasses;
}

#endif
