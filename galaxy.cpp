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

bool Galaxy::OnAddContent(Object * Content)
{
	System * TheSystem(dynamic_cast< System * >(Content));
	
	if(TheSystem != 0)
	{
		if(m_Systems.find(TheSystem->GetIdentifier()) == m_Systems.end())
		{
			if(Object::OnAddContent(Content) == true)
			{
				m_Systems[TheSystem->GetIdentifier()] = TheSystem;
				
				return true;
			}
		}
		
		return false;
	}
	
	return Object::OnAddContent(Content);
}

bool Galaxy::OnRemoveContent(Object * Content)
{
	System * TheSystem(dynamic_cast< System * >(Content));
	
	if(TheSystem != 0)
	{
		std::map< std::string, System * >::iterator SystemIterator(m_Systems.find(TheSystem->GetIdentifier()));
		
		if(SystemIterator == m_Systems.end())
		{
			if(Object::OnRemoveContent(Content) == true)
			{
				m_Systems.erase(SystemIterator);
				
				return true;
			}
		}
		
		return false;
	}
	
	return Object::OnRemoveContent(Content);
}
