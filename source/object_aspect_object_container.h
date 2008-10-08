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

#include <set>

#include "callbacks/callbacks.h"

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
	// modifiers
	bool AddContent(Object * Content);
	void Destroy(void);
	bool RemoveContent(Object * Content);
	// setters
	void SetAllowAddingCallback(Callback1< bool, Object * > AllowAddingCallback);
	void SetAllowRemovingCallback(Callback1< bool, Object * > AllowRemovingCallback);
	void SetOnAddedCallback(Callback1< void, Object * > OnAddedCallback);
	void SetOnRemovedCallback(Callback1< void, Object * > OnRemovedCallback);
private:
	Callback1< bool, Object * > m_AllowAddingCallback;
	Callback1< bool, Object * > m_AllowRemovingCallback;
	std::set< Object * > m_Content;
	Object * m_Object;
	Callback1< void, Object * > m_OnAddedCallback;
	Callback1< void, Object * > m_OnRemovedCallback;
};

inline const std::set< Object * > & ObjectAspectObjectContainer::GetContent(void) const
{
	return m_Content;
}

inline Object * ObjectAspectObjectContainer::GetObject(void)
{
	return m_Object;
}

inline const Object * ObjectAspectObjectContainer::GetObject(void) const
{
	return m_Object;
}

inline void ObjectAspectObjectContainer::SetAllowAddingCallback(Callback1< bool, Object * > AllowAddingCallback)
{
	m_AllowAddingCallback = AllowAddingCallback;
}

inline void ObjectAspectObjectContainer::SetAllowRemovingCallback(Callback1< bool, Object * > AllowRemovingCallback)
{
	m_AllowRemovingCallback = AllowRemovingCallback;
}

inline void ObjectAspectObjectContainer::SetOnAddedCallback(Callback1< void, Object * > OnAddedCallback)
{
	m_OnAddedCallback = OnAddedCallback;
}

inline void ObjectAspectObjectContainer::SetOnRemovedCallback(Callback1< void, Object * > OnRemovedCallback)
{
	m_OnRemovedCallback = OnRemovedCallback;
}

#endif
