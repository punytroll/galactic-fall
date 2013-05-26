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
#include <typeinfo>

#include "message.h"
#include "object.h"
#include "object_aspect_accessory.h"
#include "object_aspect_messages.h"
#include "object_aspect_name.h"
#include "object_aspect_object_container.h"
#include "object_aspect_outfitting.h"
#include "object_aspect_physical.h"
#include "object_aspect_position.h"
#include "object_aspect_update.h"
#include "object_aspect_visualization.h"
#include "real_time.h"
#include "string_cast.h"
#include "xml_stream.h"

std::set< Object * > Object::m_Objects;
std::map< std::string, Object * > Object::m_IdentifiedObjects;

Object::Object(void) :
	m_AspectAccessory(0),
	m_AspectMessages(0),
	m_AspectName(0),
	m_AspectObjectContainer(0),
	m_AspectOutfitting(0),
	m_AspectPhysical(0),
	m_AspectPosition(0),
	m_AspectUpdate(0),
	m_AspectVisualization(0),
	m_Container(0),
	m_Reference(*this)
{
	m_Objects.insert(this);
}

Object::~Object(void)
{
	// make some object hierarchy integrity checks first
	assert(m_Container == 0);
	// aspects
	delete m_AspectAccessory;
	m_AspectAccessory = 0;
	delete m_AspectMessages;
	m_AspectMessages = 0;
	delete m_AspectName;
	m_AspectName = 0;
	delete m_AspectObjectContainer;
	m_AspectObjectContainer = 0;
	delete m_AspectOutfitting;
	m_AspectOutfitting = 0;
	delete m_AspectPhysical;
	m_AspectPhysical = 0;
	delete m_AspectPosition;
	m_AspectPosition = 0;
	delete m_AspectUpdate;
	m_AspectUpdate = 0;
	delete m_AspectVisualization;
	m_AspectVisualization = 0;
	// invalidate reference first, so no one accesses this object
	m_Reference.Invalidate();
	SetObjectIdentifier("");
	m_Objects.erase(m_Objects.find(this));
}

void Object::AddAspectAccessory(void)
{
	assert(m_AspectAccessory == 0);
	m_AspectAccessory = new ObjectAspectAccessory();
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

void Object::AddAspectObjectContainer(void)
{
	assert(m_AspectObjectContainer == 0);
	m_AspectObjectContainer = new ObjectAspectObjectContainer(this);
}

void Object::AddAspectOutfitting(void)
{
	assert(m_AspectOutfitting == 0);
	m_AspectOutfitting = new ObjectAspectOutfitting();
}

void Object::AddAspectPhysical(void)
{
	assert(m_AspectPhysical == 0);
	m_AspectPhysical = new ObjectAspectPhysical();
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

void Object::AddAspectVisualization(void)
{
	assert(m_AspectVisualization == 0);
	m_AspectVisualization = new ObjectAspectVisualization();
}

void Object::SetContainer(Object * Container)
{
	assert(((m_Container == 0) || (Container == 0)) && (m_Container != Container));
	m_Container = Container;
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
		assert(m_Container->GetAspectObjectContainer() != 0);
		if(m_Container->GetAspectObjectContainer()->RemoveContent(this) == false)
		{
			throw std::runtime_error("RemoveContent() must not fail during Destroy()");
		}
	}
	// call destroy on all relevant aspects
	if(m_AspectVisualization != 0)
	{
		m_AspectVisualization->Destroy();
	}
	if(m_AspectObjectContainer != 0)
	{
		m_AspectObjectContainer->Destroy();
	}
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

void DumpObjectHierarchy(XMLStream & XML, Object * Container)
{
	XML << element << "object" << attribute << "address" << value << Container << attribute << "type-identifier" << value << Container->GetTypeIdentifier() << attribute << "class-identifier" << value << Container->GetClassIdentifier() << attribute << "identifier" << value << Container->GetObjectIdentifier();
	if(Container->GetAspectObjectContainer() != 0)
	{
		for(std::set< Object * >::const_iterator ObjectIterator = Container->GetAspectObjectContainer()->GetContent().begin(); ObjectIterator != Container->GetAspectObjectContainer()->GetContent().end(); ++ObjectIterator)
		{
			DumpObjectHierarchy(XML, *ObjectIterator);
		}
	}
	XML << end;
}

void Object::Dump(XMLStream & XML)
{
	XML << element << "object-report";
	XML << element << "objects" << attribute << "count" << value << static_cast< int >(m_Objects.size());
	for(std::set< Object * >::const_iterator ObjectIterator = m_Objects.begin(); ObjectIterator != m_Objects.end(); ++ObjectIterator)
	{
		XML << element << "object" << attribute << "address" << value << *ObjectIterator << end;
	}
	XML << end;
	XML << element << "identified-objects" << attribute << "count" << value << static_cast< int >(m_IdentifiedObjects.size());
	for(std::map< std::string, Object * >::const_iterator ObjectIterator = m_IdentifiedObjects.begin(); ObjectIterator != m_IdentifiedObjects.end(); ++ObjectIterator)
	{
		XML << element << "object" << attribute << "address" << value << ObjectIterator->second << attribute << "identifier" << value << ObjectIterator->first << end;
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
