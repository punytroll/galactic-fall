#include "ship_class.h"

ShipClass::ShipClass(const std::string & Identifier) :
	m_Identifier(Identifier),
	m_CargoHoldSize(0.0f)
{
}

void ShipClass::SetCargoHoldSize(const float & CargoHoldSize)
{
	m_CargoHoldSize = CargoHoldSize;
}
