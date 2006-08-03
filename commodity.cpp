#include "commodity.h"

Commodity::Commodity(const std::string & Identifier) :
	m_Identifier(Identifier)
{
}

void Commodity::SetName(const std::string & Name)
{
	m_Name = Name;
}

void Commodity::SetBasePrice(float BasePrice)
{
	m_BasePrice = BasePrice;
}
