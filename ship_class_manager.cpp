#include <fstream>

#include "color.h"
#include "model_manager.h"
#include "ship_class.h"
#include "ship_class_manager.h"
#include "string_cast.h"
#include "xml_puny_dom.h"

ShipClassManager::ShipClassManager(ModelManager * ModelManager) :
	m_ModelManager(ModelManager)
{
}

ShipClassManager::~ShipClassManager(void)
{
	while(m_ShipClasses.size() > 0)
	{
		Destroy(m_ShipClasses.begin()->first);
	}
}

ShipClass * ShipClassManager::Get(const std::string & Identifier)
{
	std::map< std::string, ShipClass * >::iterator ShipClassIterator(m_ShipClasses.find(Identifier));
	
	if(ShipClassIterator == m_ShipClasses.end())
	{
		return 0;
	}
	else
	{
		return ShipClassIterator->second;
	}
}

ShipClass * ShipClassManager::Create(const std::string & Identifier)
{
	if(m_ShipClasses.find(Identifier) != m_ShipClasses.end())
	{
		return 0;
	}
	else
	{
		ShipClass * NewShipClass(new ShipClass(Identifier));
		
		m_ShipClasses[NewShipClass->GetIdentifier()] = NewShipClass;
		
		return NewShipClass;
	}
}

void ShipClassManager::Destroy(const std::string & Identifier)
{
	std::map< std::string, ShipClass * >::iterator ShipClassIterator(m_ShipClasses.find(Identifier));
	
	if(ShipClassIterator != m_ShipClasses.end())
	{
		delete ShipClassIterator->second;
		m_ShipClasses.erase(ShipClassIterator);
	}
}
