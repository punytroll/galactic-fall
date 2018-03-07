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

#ifndef OBJECT_ASPECT_OBJECT_CONTAINER_H
#define OBJECT_ASPECT_OBJECT_CONTAINER_H

#include <functional>
#include <set>

#include "event.h"

class Object;

class ObjectAspectObjectContainer
{
public:
	// constructor & destructor
	ObjectAspectObjectContainer(Object * Object);
	~ObjectAspectObjectContainer(void);
	// getters
	const std::set< Object * > & GetContent(void) const;
	Object * GetObject(void);
	const Object * GetObject(void) const;
	std::uint32_t GetAmount(const std::string & TypeIdentifier, const std::string & ClassIdentifier) const;
	// modifiers
	void AddContent(Object * Content);
	void Destroy(void);
	void RemoveContent(Object * Content);
	// events
	Connection ConnectContentAddedCallback(std::function< void (Object *) > Callback);
	Connection ConnectContentRemovedCallback(std::function< void (Object *) > Callback);
	void DisconnectContentAddedCallback(Connection & Connection);
	void DisconnectContentRemovedCallback(Connection & Connection);
private:
	std::set< Object * > _Content;
	Object * _Object;
	::Event< void, Object * > _ContentAddedEvent;
	::Event< void, Object * > _ContentRemovedEvent;
};

inline const std::set< Object * > & ObjectAspectObjectContainer::GetContent(void) const
{
	return _Content;
}

inline Object * ObjectAspectObjectContainer::GetObject(void)
{
	return _Object;
}

inline const Object * ObjectAspectObjectContainer::GetObject(void) const
{
	return _Object;
}

#endif
