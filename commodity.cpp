#include "color.h"
#include "commodity.h"

Commodity::Commodity(const std::string & Identifier) :
	m_Color(0),
	m_Identifier(Identifier),
	m_Model(0)
{
}

Commodity::~Commodity(void)
{
	delete m_Color;
	m_Color = 0;
}

void Commodity::SetColor(const Color & NewColor)
{
	m_Color = new Color(NewColor);
}
