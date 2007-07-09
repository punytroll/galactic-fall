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

#include "map_knowledge.h"
#include "system.h"

void MapKnowledge::AddExploredSystem(System * ExploredSystem)
{
	m_ExploredSystems.insert(ExploredSystem);
	
	std::set< System * >::iterator UnexploredSytemIterator(m_UnexploredSystems.find(ExploredSystem));
	
	if(UnexploredSytemIterator != m_UnexploredSystems.end())
	{
		m_UnexploredSystems.erase(UnexploredSytemIterator);
	}
	
	const std::list< System * > & LinkedSystems(ExploredSystem->GetLinkedSystems());
	
	for(std::list< System * >::const_iterator LinkedSystemIterator = LinkedSystems.begin(); LinkedSystemIterator != LinkedSystems.end(); ++LinkedSystemIterator)
	{
		if(m_ExploredSystems.find(*LinkedSystemIterator)  == m_ExploredSystems.end())
		{
			m_UnexploredSystems.insert(*LinkedSystemIterator);
		}
	}
}
