#include <fstream>

#include "commodity.h"
#include "commodity_manager.h"
#include "string_cast.h"
#include "xml_puny_dom.h"

CommodityManager::~CommodityManager(void)
{
	while(m_Commodities.size() > 0)
	{
		Destroy(m_Commodities.begin()->first);
	}
}

Commodity * CommodityManager::Get(const std::string & Identifier)
{
	std::map< std::string, Commodity * >::iterator CommodityIterator(m_Commodities.find(Identifier));
	
	if(CommodityIterator == m_Commodities.end())
	{
		return 0;
	}
	else
	{
		return CommodityIterator->second;
	}
}

Commodity * CommodityManager::Create(const std::string & Identifier)
{
	if(m_Commodities.find(Identifier) != m_Commodities.end())
	{
		return 0;
	}
	else
	{
		Commodity * NewCommodity(new Commodity(Identifier));
		
		m_Commodities[NewCommodity->GetIdentifier()] = NewCommodity;
		
		return NewCommodity;
	}
}

void CommodityManager::Destroy(const std::string & Identifier)
{
	std::map< std::string, Commodity * >::iterator CommodityIterator(m_Commodities.find(Identifier));
	
	if(CommodityIterator != m_Commodities.end())
	{
		delete CommodityIterator->second;
		m_Commodities.erase(CommodityIterator);
	}
}

void LoadCommodityFromElement(CommodityManager * CommodityManager, const Element * CommodityElement)
{
	Commodity * NewCommodity(CommodityManager->Create(CommodityElement->GetAttribute("identifier")));
	
	if(NewCommodity == 0)
	{
		return;
	}
	
	const std::vector< Element * > & CommodityProperties(CommodityElement->GetChilds());
	std::vector< Element * >::const_iterator CommodityPropertyIterator(CommodityProperties.begin());
	
	while(CommodityPropertyIterator != CommodityProperties.end())
	{
		Element * CommodityProperty(*CommodityPropertyIterator);
		
		if(CommodityProperty->GetName() == "name")
		{
			NewCommodity->SetName(CommodityProperty->GetAttribute("value"));
		}
		else if(CommodityProperty->GetName() == "base-price")
		{
			NewCommodity->SetBasePrice(from_string_cast< float >(CommodityProperty->GetAttribute("value")));
		}
		++CommodityPropertyIterator;
	}
}

void LoadCommoditiesFromFile(CommodityManager * CommodityManager, const std::string & FileName)
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
		if((*RootElementChild)->GetName() == "commodity")
		{
			LoadCommodityFromElement(CommodityManager, *RootElementChild);
		}
		++RootElementChild;
	}
}
