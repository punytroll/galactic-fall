#include "object.h"
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
