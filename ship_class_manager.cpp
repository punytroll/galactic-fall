#include <fstream>

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

void LoadShipClassFromElement(ShipClassManager * ShipClassManager, const Element * ShipClassElement)
{
	ShipClass * NewShipClass(ShipClassManager->Create(ShipClassElement->GetAttribute("identifier")));
	
	if(NewShipClass == 0)
	{
		return;
	}
	
	const std::vector< Element * > & Childs(ShipClassElement->GetChilds());
	std::vector< Element * >::const_iterator ChildIterator(Childs.begin());
	
	while(ChildIterator != Childs.end())
	{
		Element * Child(*ChildIterator);
		
		if(Child->GetName() == "cargo-hold-size")
		{
			NewShipClass->SetCargoHoldSize(string_cast< float >(Child->GetAttribute("value")));
		}
		else if(Child->GetName() == "forward-thrust")
		{
			NewShipClass->m_ForwardThrust = string_cast< float >(Child->GetAttribute("value"));
		}
		else if(Child->GetName() == "maximum-velocity")
		{
			NewShipClass->m_MaximumVelocity = string_cast< float >(Child->GetAttribute("value"));
		}
		else if(Child->GetName() == "model")
		{
			NewShipClass->m_Model = ShipClassManager->GetModelManager()->Get(Child->GetAttribute("identifier"));
		}
		else if(Child->GetName() == "turn-speed")
		{
			NewShipClass->m_TurnSpeed = string_cast< float >(Child->GetAttribute("value"));
		}
		++ChildIterator;
	}
}

void LoadShipClassesFromFile(ShipClassManager * ShipClassManager, const std::string & FileName)
{
	std::ifstream FileStream(FileName.c_str());
	
	if(FileStream == false)
	{
		return;
	}
	
	Document Document(FileStream);
	const Element * RootElement(Document.GetRootElement());
	
	if(RootElement == 0)
	{
		return;
	}
	
	const std::vector< Element * > & RootElementChilds(RootElement->GetChilds());
	std::vector< Element * >::const_iterator RootElementChild(RootElementChilds.begin());
	
	while(RootElementChild != RootElementChilds.end())
	{
		if((*RootElementChild)->GetName() == "ship-class")
		{
			LoadShipClassFromElement(ShipClassManager, *RootElementChild);
		}
		++RootElementChild;
	}
}
