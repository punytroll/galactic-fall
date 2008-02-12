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

#include <deque>
#include <map>
#include <ostream>
#include <set>
#include <string>

#include "referencing.h"

class Message;
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
	void SetAcceptMessages(bool AcceptMessages);
	void SetObjectIdentifier(const std::string & ObjectIdentifier);
	// getters
	Object * GetContainer(void);
	const Object * GetContainer(void) const;
	const std::set< Object * > & GetContent(void) const;
	const std::string & GetObjectIdentifier(void) const;
	const Reference< Object > & GetReference(void) const;
	Reference< Graphics::Node > & GetVisualization(void);
	// modifiers
	void Destroy(void);
	Message * PopMessage(void);
	void PushMessage(Message * Message);
	bool AddContent(Object * Content);
	bool RemoveContent(Object * Content);
	void GenerateObjectIdentifier(void);
	void SetVisualization(Reference< Graphics::Node > & Visualization);
	void UnsetVisualization(void);
	
	// static methods
	static Object * GetObject(const std::string & ObjectIdentifier);
	static void Dump(std::ostream & OStream);
	static void Dump(XMLStream & XML);
protected:
	virtual bool IsAddingAllowed(Object * Content);
	virtual bool IsRemovingAllowed(Object * Content);
	virtual void OnContentAdded(Object * Content);
	virtual void OnContentRemoved(Object * Content);
private:
	std::deque< Message * > * m_Messages;
	std::string m_ObjectIdentifier;
	Reference< Object > m_Reference;
	Object * m_Container;
	std::set< Object * > m_Content;
	Reference< Graphics::Node > m_Visualization;
	
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

inline const std::set< Object * > & Object::GetContent(void) const
{
	return m_Content;
}

inline const std::string & Object::GetObjectIdentifier(void) const
{
	return m_ObjectIdentifier;
}

inline const Reference< Object > & Object::GetReference(void) const
{
	return m_Reference;
}

inline Reference< Graphics::Node > & Object::GetVisualization(void)
{
	return m_Visualization;
}

inline void Object::SetVisualization(Reference< Graphics::Node > & Visualization)
{
	assert(Visualization.IsValid() == true);
	assert(m_Visualization.IsValid() == false);
	m_Visualization = Visualization;
}

inline void Object::UnsetVisualization(void)
{
	assert(m_Visualization.IsValid() == true);
	m_Visualization.Clear();
}

#endif
