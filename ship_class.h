#ifndef SHIP_CLASS_H
#define SHIP_CLASS_H

#include <string>

class Model;

class ShipClass
{
public:
	ShipClass(const std::string & Identifier);
	void SetCargoHoldSize(const float & CargoHoldSize);
	const std::string & GetIdentifier(void) const;
	const float & GetForwardThrust(void) const;
	const float & GetTurnSpeed(void) const;
	const float & GetMaximumVelocity(void) const;
	const float & GetCargoHoldSize(void) const;
	Model * m_Model;
	float m_ForwardThrust;
	float m_TurnSpeed;
	float m_MaximumVelocity;
private:
	std::string m_Identifier;
	float m_CargoHoldSize;
};

inline const std::string & ShipClass::GetIdentifier(void) const
{
	return m_Identifier;
}

inline const float & ShipClass::GetForwardThrust(void) const
{
	return m_ForwardThrust;
}

inline const float & ShipClass::GetTurnSpeed(void) const
{
	return m_TurnSpeed;
}

inline const float & ShipClass::GetMaximumVelocity(void) const
{
	return m_MaximumVelocity;
}

inline const float & ShipClass::GetCargoHoldSize(void) const
{
	return m_CargoHoldSize;
}

#endif
