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

#ifndef MAP_KNOWLEDGE_H
#define MAP_KNOWLEDGE_H

#include <set>

class System;

class MapKnowledge
{
public:
	void AddExploredSystem(System * ExploredSystem);
	const std::set< System * > & GetExploredSystems(void) const;
	const std::set< System * > & GetUnexploredSystems(void) const;
private:
	std::set< System * > m_ExploredSystems;
	std::set< System * > m_UnexploredSystems;
};

inline const std::set< System * > & MapKnowledge::GetExploredSystems(void) const
{
	return m_ExploredSystems;
}

inline const std::set< System * > & MapKnowledge::GetUnexploredSystems(void) const
{
	return m_UnexploredSystems;
}

#endif
