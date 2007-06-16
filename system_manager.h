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

#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include <map>
#include <string>

class CommodityManager;
class Element;
class System;

class SystemManager
{
public:
	SystemManager(CommodityManager * CommodityManager);
	~SystemManager(void);
	const std::map< std::string, System * > & GetSystems(void) const;
	System * Get(const std::string & Identifier);
	System * Create(const std::string & Identifier);
	void Destroy(const std::string & Identifier);
	CommodityManager * GetCommodityManager(void);
private:
	CommodityManager * m_CommodityManager;
	std::map< std::string, System * > m_Systems;
};

inline const std::map< std::string, System * > & SystemManager::GetSystems(void) const
{
	return m_Systems;
}

inline CommodityManager * SystemManager::GetCommodityManager(void)
{
	return m_CommodityManager;
}

#endif
