/**
 * galactic-fall
 * Copyright (C) 2006  Hagen MÃ¶bius
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

#include "object.h"
#include "real_time.h"
#include "string_cast.h"
#include "xml_stream.h"

std::set< Object * > Object::m_Objects;
std::map< std::string, Object * > Object::m_IdentifiedObjects;

Object::Object(void)
{
	m_Objects.insert(this);
}

Object::~Object(void)
{
	SetObjectIdentifier("");
	m_Objects.erase(m_Objects.find(this));
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
		SetObjectIdentifier(std::string("::") + typeid(*this).name() + "::" + to_string_cast(reinterpret_cast< void * >(this)) + "(" + to_string_cast(RealTime::GetTime()) + ")");
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
	XML << end;
}
