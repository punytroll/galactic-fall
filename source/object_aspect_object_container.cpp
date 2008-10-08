/**
 * galactic-fall
 * Copyright (C) 2008  Hagen Möbius
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

#include <assert.h>

#include "object.h"
#include "object_aspect_object_container.h"

ObjectAspectObjectContainer::ObjectAspectObjectContainer(Object * Object) :
	m_Object(Object)
{
	assert(m_Object != 0);
}

ObjectAspectObjectContainer::~ObjectAspectObjectContainer(void)
{
	assert(m_Content.empty() == true);
}

bool ObjectAspectObjectContainer::AddContent(Object * Content)
{
	assert(Content != 0);
	assert(Content->GetContainer() == 0);
	if((m_AllowAddingCallback.IsValid() == false) || (m_AllowAddingCallback(Content) == true))
	{
		std::pair< std::set< Object * >::iterator, bool > InsertionResult(m_Content.insert(Content));
		
		if(InsertionResult.second == true)
		{
			Content->SetContainer(GetObject());
			if(m_OnAddedCallback.IsValid() == true)
			{
				m_OnAddedCallback(Content);
			}
			
			return true;
		}
	}
	
	return false;
}

bool ObjectAspectObjectContainer::RemoveContent(Object * Content)
{
	assert(Content != 0);
	assert(Content->GetContainer() == GetObject());
	if((m_AllowRemovingCallback.IsValid() == false) || (m_AllowRemovingCallback(Content) == true))
	{
		std::set< Object * >::iterator ContentIterator(m_Content.find(Content));
		
		if(ContentIterator != m_Content.end())
		{
			Content->SetContainer(0);
			m_Content.erase(ContentIterator);
			if(m_OnRemovedCallback.IsValid() == true)
			{
				m_OnRemovedCallback(Content);
			}
			
			return true;
		}
	}
	
	return false;
}

void ObjectAspectObjectContainer::Destroy(void)
{
	// now delete and remove all content objects
	while(m_Content.empty() == false)
	{
		// save the pointer to Content because Destroy() will remove it from m_Content
		Object * Content(*(m_Content.begin()));
		
		Content->Destroy();
		delete Content;
	}
}
