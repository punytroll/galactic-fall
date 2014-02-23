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

#ifndef OBJECT_H
#define OBJECT_H

#include <map>
#include <ostream>
#include <set>
#include <string>

#include "callbacks/events.h"
#include "referencing.h"

class Message;
class ObjectAspectAccessory;
class ObjectAspectMessages;
class ObjectAspectName;
class ObjectAspectObjectContainer;
class ObjectAspectOutfitting;
class ObjectAspectPhysical;
class ObjectAspectPosition;
class ObjectAspectUpdate;
class ObjectAspectVisualization;
class XMLStream;

namespace Graphics
{
	class Node;
}

class Object
{
public:
	Object(void);
	virtual ~Object(void);
	// getters
	ObjectAspectAccessory * GetAspectAccessory(void);
	const ObjectAspectAccessory * GetAspectAccessory(void) const;
	ObjectAspectMessages * GetAspectMessages(void);
	const ObjectAspectMessages * GetAspectMessages(void) const;
	ObjectAspectName * GetAspectName(void);
	const ObjectAspectName * GetAspectName(void) const;
	ObjectAspectObjectContainer * GetAspectObjectContainer(void);
	const ObjectAspectObjectContainer * GetAspectObjectContainer(void) const;
	ObjectAspectOutfitting * GetAspectOutfitting(void);
	const ObjectAspectOutfitting * GetAspectOutfitting(void) const;
	ObjectAspectPhysical * GetAspectPhysical(void);
	const ObjectAspectPhysical * GetAspectPhysical(void) const;
	ObjectAspectPosition * GetAspectPosition(void);
	const ObjectAspectPosition * GetAspectPosition(void) const;
	ObjectAspectUpdate * GetAspectUpdate(void);
	const ObjectAspectUpdate * GetAspectUpdate(void) const;
	ObjectAspectVisualization * GetAspectVisualization(void);
	const ObjectAspectVisualization * GetAspectVisualization(void) const;
	const std::string & GetClassIdentifier(void) const;
	Object * GetContainer(void);
	const Object * GetContainer(void) const;
	const std::string & GetObjectIdentifier(void) const;
	const Reference< Object > & GetReference(void) const;
	const std::string & GetTypeIdentifier(void) const;
	// setters
	void SetClassIdentifier(const std::string & ClassIdentifier);
	void SetContainer(Object * Container);
	void SetObjectIdentifier(const std::string & ObjectIdentifier);
	void SetTypeIdentifier(const std::string & TypeIdentifier);
	// modifiers
	void AddAspectAccessory(void);
	void AddAspectMessages(void);
	void AddAspectName(void);
	void AddAspectObjectContainer(void);
	void AddAspectOutfitting(void);
	void AddAspectPhysical(void);
	void AddAspectPosition(void);
	void AddAspectUpdate(void);
	void AddAspectVisualization(void);
	void Destroy(void);
	void GenerateObjectIdentifier(void);
	// events
	ConnectionHandle ConnectDestroyingCallback(Callback0< void > Callback);
	void DisconnectDestroyingCallback(ConnectionHandle & ConnectionHandle);
	// static methods
	static Object * GetObject(const std::string & ObjectIdentifier);
	static void Dump(std::ostream & OStream);
	static void Dump(XMLStream & XML);
private:
	// aspects
	ObjectAspectAccessory * _AspectAccessory;
	ObjectAspectMessages * _AspectMessages;
	ObjectAspectName * _AspectName;
	ObjectAspectObjectContainer * _AspectObjectContainer;
	ObjectAspectOutfitting * _AspectOutfitting;
	ObjectAspectPhysical * _AspectPhysical;
	ObjectAspectPosition * _AspectPosition;
	ObjectAspectUpdate * _AspectUpdate;
	ObjectAspectVisualization * _AspectVisualization;
	// other
	std::string _ClassIdentifier;
	Object * _Container;
	Event0< void > _DestroyingEvent;
	std::string _ObjectIdentifier;
	Reference< Object > _Reference;
	std::string _TypeIdentifier;
	
	static std::set< Object * > _Objects;
	static std::map< std::string, Object * > _IdentifiedObjects;
};

inline ObjectAspectAccessory * Object::GetAspectAccessory(void)
{
	return _AspectAccessory;
}

inline const ObjectAspectAccessory * Object::GetAspectAccessory(void) const
{
	return _AspectAccessory;
}

inline ObjectAspectMessages * Object::GetAspectMessages(void)
{
	return _AspectMessages;
}

inline const ObjectAspectMessages * Object::GetAspectMessages(void) const
{
	return _AspectMessages;
}

inline ObjectAspectName * Object::GetAspectName(void)
{
	return _AspectName;
}

inline const ObjectAspectName * Object::GetAspectName(void) const
{
	return _AspectName;
}

inline ObjectAspectObjectContainer * Object::GetAspectObjectContainer(void)
{
	return _AspectObjectContainer;
}

inline const ObjectAspectObjectContainer * Object::GetAspectObjectContainer(void) const
{
	return _AspectObjectContainer;
}

inline ObjectAspectOutfitting * Object::GetAspectOutfitting(void)
{
	return _AspectOutfitting;
}

inline const ObjectAspectOutfitting * Object::GetAspectOutfitting(void) const
{
	return _AspectOutfitting;
}

inline ObjectAspectPhysical * Object::GetAspectPhysical(void)
{
	return _AspectPhysical;
}

inline const ObjectAspectPhysical * Object::GetAspectPhysical(void) const
{
	return _AspectPhysical;
}

inline ObjectAspectPosition * Object::GetAspectPosition(void)
{
	return _AspectPosition;
}

inline const ObjectAspectPosition * Object::GetAspectPosition(void) const
{
	return _AspectPosition;
}

inline ObjectAspectUpdate * Object::GetAspectUpdate(void)
{
	return _AspectUpdate;
}

inline const ObjectAspectUpdate * Object::GetAspectUpdate(void) const
{
	return _AspectUpdate;
}

inline ObjectAspectVisualization * Object::GetAspectVisualization(void)
{
	return _AspectVisualization;
}

inline const ObjectAspectVisualization * Object::GetAspectVisualization(void) const
{
	return _AspectVisualization;
}

inline const std::string & Object::GetClassIdentifier(void) const
{
	return _ClassIdentifier;
}

inline Object * Object::GetContainer(void)
{
	return _Container;
}

inline const Object * Object::GetContainer(void) const
{
	return _Container;
}

inline const std::string & Object::GetObjectIdentifier(void) const
{
	return _ObjectIdentifier;
}

inline const Reference< Object > & Object::GetReference(void) const
{
	return _Reference;
}

inline const std::string & Object::GetTypeIdentifier(void) const
{
	return _TypeIdentifier;
}

inline void Object::SetClassIdentifier(const std::string & ClassIdentifier)
{
	_ClassIdentifier = ClassIdentifier;
}

inline void Object::SetTypeIdentifier(const std::string & TypeIdentifier)
{
	_TypeIdentifier = TypeIdentifier;
}

#endif
