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

#ifndef COMMODITY_MANAGER_H
#define COMMODITY_MANAGER_H

#include <map>
#include <string>

class Element;
class Commodity;

class CommodityManager
{
public:
	~CommodityManager(void);
	const std::map< std::string, Commodity * > & GetCommodities(void) const;
	const Commodity * Get(const std::string & Identifier) const;
	Commodity * Create(const std::string & Identifier);
	void Destroy(const std::string & Identifier);
private:
	std::map< std::string, Commodity * > m_Commodities;
};

inline const std::map< std::string, Commodity * > & CommodityManager::GetCommodities(void) const
{
	return m_Commodities;
}

#endif
