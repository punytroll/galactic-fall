#include <fstream>
#include <iostream>

#include "commodity_manager.h"
#include "planet.h"
#include "star.h"
#include "string_cast.h"
#include "system.h"
#include "system_manager.h"
#include "xml_puny_dom.h"

SystemManager::SystemManager(CommodityManager * CommodityManager) :
	m_CommodityManager(CommodityManager)
{
}

SystemManager::~SystemManager(void)
{
	while(m_Systems.size() > 0)
	{
		Destroy(m_Systems.begin()->first);
	}
}

System * SystemManager::Get(const std::string & Identifier)
{
	std::map< std::string, System * >::iterator SystemIterator(m_Systems.find(Identifier));
	
	if(SystemIterator == m_Systems.end())
	{
		return 0;
	}
	else
	{
		return SystemIterator->second;
	}
}

System * SystemManager::Create(const std::string & Identifier)
{
	if(m_Systems.find(Identifier) != m_Systems.end())
	{
		return 0;
	}
	else
	{
		System * NewSystem(new System(Identifier));
		
		m_Systems[NewSystem->GetIdentifier()] = NewSystem;
		
		return NewSystem;
	}
}

void SystemManager::Destroy(const std::string & Identifier)
{
	std::map< std::string, System * >::iterator SystemIterator(m_Systems.find(Identifier));
	
	if(SystemIterator != m_Systems.end())
	{
		delete SystemIterator->second;
		m_Systems.erase(SystemIterator);
	}
}

void LoadSystemFromElement(SystemManager * SystemManager, const Element * SystemElement)
{
	System * NewSystem(SystemManager->Create(SystemElement->GetAttribute("identifier")));
	
	if(NewSystem == 0)
	{
		return;
	}
	
	const std::vector< Element * > & SystemProperties(SystemElement->GetChilds());
	std::vector< Element * >::const_iterator SystemPropertyIterator(SystemProperties.begin());
	
	NewSystem->SetObjectIdentifier("::system(" + NewSystem->GetIdentifier() + ")");
	while(SystemPropertyIterator != SystemProperties.end())
	{
		Element * SystemProperty(*SystemPropertyIterator);
		
		if(SystemProperty->GetName() == "name")
		{
			NewSystem->SetName(SystemProperty->GetAttribute("value"));
		}
		else if(SystemProperty->GetName() == "position")
		{
			NewSystem->SetPosition(math3d::vector2f(from_string_cast< float >(SystemProperty->GetAttribute("x")), from_string_cast< float >(SystemProperty->GetAttribute("y"))));
		}
		else if(SystemProperty->GetName() == "star")
		{
			Star * NewStar(NewSystem->CreateStar());
			
			NewStar->SetObjectIdentifier("::system(" + NewSystem->GetIdentifier() + ")::star(" + SystemProperty->GetAttribute("identifier") + ")");
			if(NewStar != 0)
			{
				for(std::vector< Element * >::const_iterator StarChild = SystemProperty->GetChilds().begin(); StarChild != SystemProperty->GetChilds().end(); ++StarChild)
				{
					if((*StarChild)->GetName() == "position")
					{
						NewStar->SetPosition(math3d::vector2f(from_string_cast< float >((*StarChild)->GetAttribute("x")), from_string_cast< float >((*StarChild)->GetAttribute("y"))));
					}
					else if((*StarChild)->GetName() == "color")
					{
						NewStar->SetColor(Color(from_string_cast< float >((*StarChild)->GetAttribute("red")), from_string_cast< float >((*StarChild)->GetAttribute("green")), from_string_cast< float >((*StarChild)->GetAttribute("blue")), from_string_cast< float >((*StarChild)->GetAttribute("alpha"))));
					}
				}
			}
		}
		else if(SystemProperty->GetName() == "planet")
		{
			Planet * NewPlanet(NewSystem->CreatePlanet(SystemProperty->GetAttribute("identifier")));
			const std::vector< Element * > & PlanetProperties(SystemProperty->GetChilds());
			std::vector< Element * >::const_iterator PlanetPropertyIterator(PlanetProperties.begin());
			
			NewPlanet->SetObjectIdentifier("::system(" + NewSystem->GetIdentifier() + ")::planet(" + NewPlanet->GetIdentifier() + ")");
			while(PlanetPropertyIterator != PlanetProperties.end())
			{
				Element * PlanetProperty(*PlanetPropertyIterator);
				
				if(PlanetProperty->GetName() == "allow-refuelling")
				{
					NewPlanet->SetAllowRefuelling(from_string_cast< bool >(PlanetProperty->GetAttribute("value")));
				}
				else if(PlanetProperty->GetName() == "commodity")
				{
					Commodity * ManagedCommodity(SystemManager->GetCommodityManager()->Get(PlanetProperty->GetAttribute("identifier")));
					
					if(ManagedCommodity == 0)
					{
						std::cout << "failed to get managed commodity with identifier \"" << PlanetProperty->GetAttribute("identifier") << "\"" << std::endl;
					}
					else
					{
						PlanetCommodity * NewPlanetCommodity(NewPlanet->CreateCommodity(ManagedCommodity));
						
						NewPlanetCommodity->SetBasePriceModifier(from_string_cast< float >(PlanetProperty->GetAttribute("base-price-modifier")));
					}
				}
				else if(PlanetProperty->GetName() == "description")
				{
					NewPlanet->SetDescription(PlanetProperty->GetAttribute("value"));
				}
				else if(PlanetProperty->GetName() == "name")
				{
					NewPlanet->SetName(PlanetProperty->GetAttribute("value"));
				}
				else if(PlanetProperty->GetName() == "position")
				{
					NewPlanet->m_Position.set(from_string_cast< float >(PlanetProperty->GetAttribute("x")), from_string_cast< float >(PlanetProperty->GetAttribute("y")));
				}
				else if(PlanetProperty->GetName() == "size")
				{
					NewPlanet->SetSize(from_string_cast< float >(PlanetProperty->GetAttribute("value")));
				}
				else if(PlanetProperty->GetName() == "fuel-price")
				{
					NewPlanet->SetFuelPrice(from_string_cast< float >(PlanetProperty->GetAttribute("value")));
				}
				else if(PlanetProperty->GetName() == "landing-fee")
				{
					NewPlanet->SetLandingFee(from_string_cast< float >(PlanetProperty->GetAttribute("value")));
				}
				else if(PlanetProperty->GetName() == "color")
				{
					NewPlanet->SetColor(Color(from_string_cast< float >(PlanetProperty->GetAttribute("red")), from_string_cast< float >(PlanetProperty->GetAttribute("green")), from_string_cast< float >(PlanetProperty->GetAttribute("blue")), from_string_cast< float >(PlanetProperty->GetAttribute("alpha"))));
				}
				++PlanetPropertyIterator;
			}
		}
		++SystemPropertyIterator;
	}
}

void LoadSystemsFromFile(SystemManager * SystemManager, const std::string & FileName)
{
	std::ifstream FileStream(FileName.c_str());
	
	if(FileStream == false)
	{
		return;
	}
	
	Document Document(FileStream);
	const Element * RootElement(Document.GetDocumentElement());
	
	if(RootElement == 0)
	{
		return;
	}
	
	const std::vector< Element * > & RootElementChilds(RootElement->GetChilds());
	std::vector< Element * >::const_iterator RootElementChild(RootElementChilds.begin());
	
	while(RootElementChild != RootElementChilds.end())
	{
		if((*RootElementChild)->GetName() == "system")
		{
			LoadSystemFromElement(SystemManager, *RootElementChild);
		}
		++RootElementChild;
	}
}

void LoadSystemLinkFromElement(SystemManager * SystemManager, const Element * SystemLinkElement)
{
	const std::vector< Element * > & SystemLinkProperties(SystemLinkElement->GetChilds());
	std::vector< Element * >::const_iterator SystemLinkPropertyIterator(SystemLinkProperties.begin());
	System * System1(0);
	System * System2(0);
	
	while(SystemLinkPropertyIterator != SystemLinkProperties.end())
	{
		Element * SystemLinkProperty(*SystemLinkPropertyIterator);
		
		if(SystemLinkProperty->GetName() == "system")
		{
			if(System1 == 0)
			{
				System1 = SystemManager->Get(SystemLinkProperty->GetAttribute("identifier"));
			}
			else
			{
				System2 = SystemManager->Get(SystemLinkProperty->GetAttribute("identifier"));
			}
		}
		++SystemLinkPropertyIterator;
	}
	if((System1 == 0) || (System2 == 0))
	{
		return;
	}
	System1->AddLinkedSystem(System2);
	System2->AddLinkedSystem(System1);
}

void LoadSystemLinksFromFile(SystemManager * SystemManager, const std::string & FileName)
{
	std::ifstream FileStream(FileName.c_str());
	
	if(FileStream == false)
	{
		return;
	}
	
	Document Document(FileStream);
	const Element * RootElement(Document.GetDocumentElement());
	
	if(RootElement == 0)
	{
		return;
	}
	
	const std::vector< Element * > & RootElementChilds(RootElement->GetChilds());
	std::vector< Element * >::const_iterator RootElementChild(RootElementChilds.begin());
	
	while(RootElementChild != RootElementChilds.end())
	{
		if((*RootElementChild)->GetName() == "system-link")
		{
			LoadSystemLinkFromElement(SystemManager, *RootElementChild);
		}
		++RootElementChild;
	}
}
