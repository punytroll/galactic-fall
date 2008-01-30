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

#include "galaxy.h"
#include "system.h"

System * Galaxy::GetSystem(const std::string & SystemIdentifier)
{
	std::map< std::string, System * >::iterator System(m_Systems.find(SystemIdentifier));
	
	if(System != m_Systems.end())
	{
		return System->second;
	}
	else
	{
		return 0;
	}
}

bool Galaxy::IsAddingAllowed(Object * Content)
{
	System * TheSystem(dynamic_cast< System * >(Content));
	
	if(TheSystem != 0)
	{
		return m_Systems.find(TheSystem->GetIdentifier()) == m_Systems.end();
	}
	else
	{
		return false;
	}
}

bool Galaxy::IsRemovingAllowed(Object * Content)
{
	return Object::IsRemovingAllowed(Content);
}

void Galaxy::OnContentAdded(Object * Content)
{
	Object::OnContentAdded(Content);
	
	System * TheSystem(dynamic_cast< System * >(Content));
	
	assert(TheSystem != 0);
	assert(m_Systems.find(TheSystem->GetIdentifier()) == m_Systems.end());
	m_Systems[TheSystem->GetIdentifier()] = TheSystem;
}

void Galaxy::OnContentRemoved(Object * Content)
{
	System * TheSystem(dynamic_cast< System * >(Content));
	
	assert(TheSystem != 0);
	
	std::map< std::string, System * >::iterator SystemIterator(m_Systems.find(TheSystem->GetIdentifier()));
	
	assert(SystemIterator != m_Systems.end());
	m_Systems.erase(SystemIterator);
	Object::OnContentRemoved(Content);
}
