#ifndef SHIP_CLASS_H
#define SHIP_CLASS_H

#include <string>

class Model;

class ShipClass
{
public:
	// constructor
	ShipClass(const std::string & Identifier);
	// getters
	const std::string & GetIdentifier(void) const;
	const Model * GetModel(void) const;
	float GetForwardThrust(void) const;
	float GetTurnSpeed(void) const;
	float GetMaximumVelocity(void) const;
	float GetCargoHoldSize(void) const;
	float GetFuelHoldSize(void) const;
	// modifiers
	void SetModel(const Model * Model);
	void SetForwardThrust(float ForwardThrust);
	void SetTurnSpeed(float TurnSpeed);
	void SetMaximumVelocity(float MaximumVelocity);
	void SetCargoHoldSize(float CargoHoldSize);
	void SetFuelHoldSize(float FuelHoldSize);
private:
	std::string m_Identifier;
	const Model * m_Model;
	float m_ForwardThrust;
	float m_TurnSpeed;
	float m_MaximumVelocity;
	float m_CargoHoldSize;
	float m_FuelHoldSize;
};

inline const std::string & ShipClass::GetIdentifier(void) const
{
	return m_Identifier;
}

inline const Model * ShipClass::GetModel(void) const
{
	return m_Model;
}

inline float ShipClass::GetForwardThrust(void) const
{
	return m_ForwardThrust;
}

inline float ShipClass::GetTurnSpeed(void) const
{
	return m_TurnSpeed;
}

inline float ShipClass::GetMaximumVelocity(void) const
{
	return m_MaximumVelocity;
}

inline float ShipClass::GetCargoHoldSize(void) const
{
	return m_CargoHoldSize;
}

inline float ShipClass::GetFuelHoldSize(void) const
{
	return m_FuelHoldSize;
}

inline void ShipClass::SetModel(const Model * Model)
{
	m_Model = Model;
}

inline void ShipClass::SetForwardThrust(float ForwardThrust)
{
	m_ForwardThrust = ForwardThrust;
}

inline void ShipClass::SetTurnSpeed(float TurnSpeed)
{
	m_TurnSpeed = TurnSpeed;
}

inline void ShipClass::SetMaximumVelocity(float MaximumVelocity)
{
	m_MaximumVelocity = MaximumVelocity;
}

inline void ShipClass::SetCargoHoldSize(float CargoHoldSize)
{
	m_CargoHoldSize = CargoHoldSize;
}

inline void ShipClass::SetFuelHoldSize(float FuelHoldSize)
{
	m_FuelHoldSize = FuelHoldSize;
}

#endif
