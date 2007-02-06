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
