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

#include <cassert>

#include "object.h"
#include "object_aspect_object_container.h"

ObjectAspectObjectContainer::ObjectAspectObjectContainer(Object * Object) :
	_Object(Object)
{
	assert(_Object != nullptr);
}

ObjectAspectObjectContainer::~ObjectAspectObjectContainer(void)
{
	assert(_Content.empty() == true);
}

void ObjectAspectObjectContainer::AddContent(Object * Content)
{
	assert(Content != nullptr);
	assert(Content->GetContainer() == nullptr);
	
	auto InsertionResult(_Content.insert(Content));
	
	assert(InsertionResult.second == true);
	Content->SetContainer(_Object);
	_ContentAddedEvent(Content);
}

Connection ObjectAspectObjectContainer::ConnectContentAddedCallback(std::function< void (Object *) > Callback)
{
	return _ContentAddedEvent.Connect(Callback);
}

Connection ObjectAspectObjectContainer::ConnectContentRemovedCallback(std::function< void (Object *) > Callback)
{
	return _ContentRemovedEvent.Connect(Callback);
}

void ObjectAspectObjectContainer::DisconnectContentAddedCallback(Connection & Connection)
{
	_ContentAddedEvent.Disconnect(Connection);
}

void ObjectAspectObjectContainer::DisconnectContentRemovedCallback(Connection & Connection)
{
	_ContentRemovedEvent.Disconnect(Connection);
}

std::uint32_t ObjectAspectObjectContainer::GetAmount(const std::string & TypeIdentifier, const std::string & SubTypeIdentifier) const
{
	std::uint32_t Amount{0};
	
	for(auto Content : _Content)
	{
		if((Content->GetTypeIdentifier() == TypeIdentifier) && (Content->GetSubTypeIdentifier() == SubTypeIdentifier))
		{
			Amount += 1;
		}
	}
	
	return Amount;
}

void ObjectAspectObjectContainer::RemoveContent(Object * Content)
{
	assert(Content != nullptr);
	assert(Content->GetContainer() == _Object);
	
	auto ContentIterator(_Content.find(Content));
		
	assert(ContentIterator != _Content.end());
	Content->SetContainer(0);
	_Content.erase(ContentIterator);
	_ContentRemovedEvent(Content);
}

void ObjectAspectObjectContainer::Destroy(void)
{
	// now delete and remove all content objects
	while(_Content.empty() == false)
	{
		// save the pointer to Content because Destroy() will remove it from m_Content
		auto Content(*(_Content.begin()));
		
		Content->Destroy();
		delete Content;
	}
}
