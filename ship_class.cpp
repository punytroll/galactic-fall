#include "color.h"
#include "ship_class.h"

ShipClass::ShipClass(const std::string & Identifier) :
	m_Identifier(Identifier),
	m_CargoHoldSize(0.0f),
	m_Color(0),
	m_ExhaustOffset(true)
{
}

ShipClass::~ShipClass(void)
{
	delete m_Color;
	m_Color = 0;
}

void ShipClass::SetColor(const Color & NewColor)
{
	delete m_Color;
	m_Color = new Color(NewColor);
}
