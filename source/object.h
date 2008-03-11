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

#include "referencing.h"

class Message;
class ObjectAspectAccessory;
class ObjectAspectMessages;
class ObjectAspectName;
class ObjectAspectObjectContainer;
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
	// setters
	void SetContainer(Object * Container);
	void SetObjectIdentifier(const std::string & ObjectIdentifier);
	// getters
	Object * GetContainer(void);
	const Object * GetContainer(void) const;
	ObjectAspectAccessory * GetAspectAccessory(void);
	const ObjectAspectAccessory * GetAspectAccessory(void) const;
	ObjectAspectMessages * GetAspectMessages(void);
	const ObjectAspectMessages * GetAspectMessages(void) const;
	ObjectAspectName * GetAspectName(void);
	const ObjectAspectName * GetAspectName(void) const;
	ObjectAspectObjectContainer * GetAspectObjectContainer(void);
	const ObjectAspectObjectContainer * GetAspectObjectContainer(void) const;
	ObjectAspectPosition * GetAspectPosition(void);
	const ObjectAspectPosition * GetAspectPosition(void) const;
	ObjectAspectUpdate * GetAspectUpdate(void);
	const ObjectAspectUpdate * GetAspectUpdate(void) const;
	ObjectAspectVisualization * GetAspectVisualization(void);
	const ObjectAspectVisualization * GetAspectVisualization(void) const;
	const std::string & GetObjectIdentifier(void) const;
	const Reference< Object > & GetReference(void) const;
	// modifiers
	void AddAspectAccessory(void);
	void AddAspectMessages(void);
	void AddAspectName(void);
	void AddAspectObjectContainer(void);
	void AddAspectPosition(void);
	void AddAspectUpdate(void);
	void AddAspectVisualization(void);
	void Destroy(void);
	void GenerateObjectIdentifier(void);
	
	// static methods
	static Object * GetObject(const std::string & ObjectIdentifier);
	static void Dump(std::ostream & OStream);
	static void Dump(XMLStream & XML);
private:
	// aspects
	ObjectAspectAccessory * m_AspectAccessory;
	ObjectAspectMessages * m_AspectMessages;
	ObjectAspectName * m_AspectName;
	ObjectAspectObjectContainer * m_AspectObjectContainer;
	ObjectAspectPosition * m_AspectPosition;
	ObjectAspectUpdate * m_AspectUpdate;
	ObjectAspectVisualization * m_AspectVisualization;
	// other
	std::string m_ObjectIdentifier;
	Reference< Object > m_Reference;
	Object * m_Container;
	
	static std::set< Object * > m_Objects;
	static std::map< std::string, Object * > m_IdentifiedObjects;
};

inline Object * Object::GetContainer(void)
{
	return m_Container;
}

inline const Object * Object::GetContainer(void) const
{
	return m_Container;
}

inline const std::string & Object::GetObjectIdentifier(void) const
{
	return m_ObjectIdentifier;
}

inline ObjectAspectAccessory * Object::GetAspectAccessory(void)
{
	return m_AspectAccessory;
}

inline const ObjectAspectAccessory * Object::GetAspectAccessory(void) const
{
	return m_AspectAccessory;
}

inline ObjectAspectMessages * Object::GetAspectMessages(void)
{
	return m_AspectMessages;
}

inline const ObjectAspectMessages * Object::GetAspectMessages(void) const
{
	return m_AspectMessages;
}

inline ObjectAspectName * Object::GetAspectName(void)
{
	return m_AspectName;
}

inline const ObjectAspectName * Object::GetAspectName(void) const
{
	return m_AspectName;
}

inline ObjectAspectObjectContainer * Object::GetAspectObjectContainer(void)
{
	return m_AspectObjectContainer;
}

inline const ObjectAspectObjectContainer * Object::GetAspectObjectContainer(void) const
{
	return m_AspectObjectContainer;
}

inline ObjectAspectPosition * Object::GetAspectPosition(void)
{
	return m_AspectPosition;
}

inline const ObjectAspectPosition * Object::GetAspectPosition(void) const
{
	return m_AspectPosition;
}

inline ObjectAspectUpdate * Object::GetAspectUpdate(void)
{
	return m_AspectUpdate;
}

inline const ObjectAspectUpdate * Object::GetAspectUpdate(void) const
{
	return m_AspectUpdate;
}

inline ObjectAspectVisualization * Object::GetAspectVisualization(void)
{
	return m_AspectVisualization;
}

inline const ObjectAspectVisualization * Object::GetAspectVisualization(void) const
{
	return m_AspectVisualization;
}

inline const Reference< Object > & Object::GetReference(void) const
{
	return m_Reference;
}

#endif
