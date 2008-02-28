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

#include <stdexcept>

#include "graphics_node.h"
#include "message.h"
#include "object.h"
#include "object_aspect_messages.h"
#include "object_aspect_name.h"
#include "object_aspect_position.h"
#include "object_aspect_update.h"
#include "real_time.h"
#include "string_cast.h"
#include "xml_stream.h"

std::set< Object * > Object::m_Objects;
std::map< std::string, Object * > Object::m_IdentifiedObjects;

Object::Object(void) :
	m_AspectMessages(0),
	m_AspectName(0),
	m_AspectPosition(0),
	m_AspectUpdate(0),
	m_Reference(*this),
	m_Container(0)
{
	m_Objects.insert(this);
}

Object::~Object(void)
{
	// make some object hierarchy integrity checks first
	assert(m_Container == 0);
	assert(m_Visualization.IsValid() == false);
	assert(m_Content.empty() == true);
	// aspects
	delete m_AspectMessages;
	m_AspectMessages = 0;
	delete m_AspectName;
	m_AspectName = 0;
	delete m_AspectPosition;
	m_AspectPosition = 0;
	delete m_AspectUpdate;
	m_AspectUpdate = 0;
	// invalidate reference first, so no one accesses this object
	m_Reference.Invalidate();
	SetObjectIdentifier("");
	m_Objects.erase(m_Objects.find(this));
}

void Object::AddAspectMessages(void)
{
	assert(m_AspectMessages == 0);
	m_AspectMessages = new ObjectAspectMessages();
}

void Object::AddAspectName(void)
{
	assert(m_AspectName == 0);
	m_AspectName = new ObjectAspectName();
}

void Object::AddAspectPosition(void)
{
	assert(m_AspectPosition == 0);
	m_AspectPosition = new ObjectAspectPosition();
}

void Object::AddAspectUpdate(void)
{
	assert(m_AspectUpdate == 0);
	m_AspectUpdate = new ObjectAspectUpdate();
}

void Object::SetObjectIdentifier(const std::string & ObjectIdentifier)
{
	if(m_ObjectIdentifier.empty() == false)
	{
		m_IdentifiedObjects.erase(m_IdentifiedObjects.find(m_ObjectIdentifier));
		m_ObjectIdentifier = "";
	}
	if((ObjectIdentifier.empty() == false) && (m_IdentifiedObjects.find(ObjectIdentifier) == m_IdentifiedObjects.end()))
	{
		m_ObjectIdentifier = ObjectIdentifier;
		m_IdentifiedObjects[m_ObjectIdentifier] = this;
	}
}

void Object::GenerateObjectIdentifier(void)
{
	if(m_ObjectIdentifier.empty() == true)
	{
		SetObjectIdentifier(std::string("::") + typeid(*this).name() + "::" + to_string_cast(reinterpret_cast< void * >(this)) + "(" + to_string_cast(RealTime::Get()) + ")");
	}
}

void Object::Destroy(void)
{
	// remove from object hierarchy
	if(m_Container != 0)
	{
		if(m_Container->RemoveContent(this) == false)
		{
			throw std::runtime_error("RemoveContent() must not fail during Destroy()");
		}
	}
	if(m_Visualization.IsValid() == true)
	{
		Reference< Graphics::Node > Visualization(GetVisualization());
		
		UnsetVisualization();
		Visualization->Remove();
		Visualization->Destroy();
	}
	// now delete and remove all content objects
	while(m_Content.empty() == false)
	{
		// save the pointer to Content because Destroy() will remove it from m_Content
		Object * Content(*(m_Content.begin()));
		
		Content->Destroy();
		delete Content;
	}
}

bool Object::AddContent(Object * Content)
{
	assert(Content != 0);
	assert(Content->m_Container == 0);
	if(IsAddingAllowed(Content) == true)
	{
		std::pair< std::set< Object * >::iterator, bool > InsertionResult(m_Content.insert(Content));
		
		if(InsertionResult.second == true)
		{
			Content->m_Container = this;
			OnContentAdded(Content);
			
			return true;
		}
	}
	
	return false;
}

bool Object::RemoveContent(Object * Content)
{
	assert(Content != 0);
	assert(Content->m_Container == this);
	if(IsRemovingAllowed(Content) == true)
	{
		std::set< Object * >::iterator ContentIterator(m_Content.find(Content));
		
		if(ContentIterator != m_Content.end())
		{
			Content->m_Container = 0;
			m_Content.erase(ContentIterator);
			OnContentRemoved(Content);
			
			return true;
		}
	}
	
	return false;
}

bool Object::IsAddingAllowed(Object * Content)
{
	//standard implementation always allows adding
	return true;
}

bool Object::IsRemovingAllowed(Object * Content)
{
	// standard implementation always allows removing
	return true;
}

void Object::OnContentAdded(Object * Content)
{
	// intentionally left empty
}

void Object::OnContentRemoved(Object * Content)
{
	// intentionally left empty
}

Object * Object::GetObject(const std::string & ObjectIdentifier)
{
	std::map< std::string, Object * >::iterator ObjectIterator(m_IdentifiedObjects.find(ObjectIdentifier));
	
	if(ObjectIterator != m_IdentifiedObjects.end())
	{
		return ObjectIterator->second;
	}
	else
	{
		return 0;
	}
}

void Object::Dump(std::ostream & OStream)
{
	OStream << "Objects (Count " << m_Objects.size() << "): \n";
	
	for(std::set< Object * >::const_iterator ObjectIterator = m_Objects.begin(); ObjectIterator != m_Objects.end(); ++ObjectIterator)
	{
		OStream << "  " << *ObjectIterator << '\n';
	}
	OStream << '\n';
	OStream << "Identified Objects (Count " << m_IdentifiedObjects.size() << "): \n";
	for(std::map< std::string, Object * >::const_iterator ObjectIterator = m_IdentifiedObjects.begin(); ObjectIterator != m_IdentifiedObjects.end(); ++ObjectIterator)
	{
		OStream << "  " << ObjectIterator->second << " = '" << ObjectIterator->first << "'\n";
	}
	OStream << std::endl;
}

void DumpObjectHierarchy(XMLStream & XML, Object * Conatiner)
{
	XML << element << "object" << attribute << "type-name" << value << typeid(*Conatiner).name() << attribute << "identifier" << value << Conatiner->GetObjectIdentifier();
	for(std::set< Object * >::const_iterator ObjectIterator = Conatiner->GetContent().begin(); ObjectIterator != Conatiner->GetContent().end(); ++ObjectIterator)
	{
		DumpObjectHierarchy(XML, *ObjectIterator);
	}
	XML << end;
}

void Object::Dump(XMLStream & XML)
{
	XML << element << "object-report";
	XML << element << "objects" << attribute << "count" << value << static_cast< int >(m_Objects.size());
	for(std::set< Object * >::const_iterator ObjectIterator = m_Objects.begin(); ObjectIterator != m_Objects.end(); ++ObjectIterator)
	{
		XML << element << "object" << attribute << "address" << value << *ObjectIterator << attribute << "type-name" << value << typeid(**ObjectIterator).name() << end;
	}
	XML << end;
	XML << element << "identified-objects" << attribute << "count" << value << static_cast< int >(m_IdentifiedObjects.size());
	for(std::map< std::string, Object * >::const_iterator ObjectIterator = m_IdentifiedObjects.begin(); ObjectIterator != m_IdentifiedObjects.end(); ++ObjectIterator)
	{
		XML << element << "object" << attribute << "address" << value << ObjectIterator->second << attribute << "identifier" << value << ObjectIterator->first << attribute << "type-name" << value << typeid(*(ObjectIterator->second)).name() << end;
	}
	XML << end;
	XML << element << "object-hierarchy";
	for(std::set< Object * >::const_iterator ObjectIterator = m_Objects.begin(); ObjectIterator != m_Objects.end(); ++ObjectIterator)
	{
		if((*ObjectIterator)->GetContainer() == 0)
		{
			DumpObjectHierarchy(XML, *ObjectIterator);
		}
	}
	XML << end;
	XML << end;
}
