#include "color.h"
#include "ship_class.h"

ShipClass::ShipClass(const std::string & Identifier) :
	m_Identifier(Identifier),
	m_CargoHoldSize(0.0f),
	m_Color(0)
{
}

ShipClass::~ShipClass(void)
{
	delete m_Color;
	m_Color = 0;
}

void ShipClass::SetColor(const Color & NewColor)
{
	m_Color = new Color(NewColor);
}
