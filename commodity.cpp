#include "color.h"
#include "commodity.h"

Commodity::Commodity(const std::string & Identifier) :
	m_Identifier(Identifier),
	m_Color(0)
{
}

Commodity::~Commodity(void)
{
	delete m_Color;
	m_Color = 0;
}

void Commodity::SetName(const std::string & Name)
{
	m_Name = Name;
}

void Commodity::SetBasePrice(float BasePrice)
{
	m_BasePrice = BasePrice;
}

void Commodity::SetColor(const Color & NewColor)
{
	m_Color = new Color(NewColor);
}
