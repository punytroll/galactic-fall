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
	float GetMaximumSpeed(void) const;
	float GetCargoHoldSize(void) const;
	float GetFuelHoldSize(void) const;
	float GetJumpFuel(void) const;
	float GetForwardFuel(void) const;
	float GetTurnFuel(void) const;
	// modifiers
	void SetModel(const Model * Model);
	void SetForwardThrust(float ForwardThrust);
	void SetTurnSpeed(float TurnSpeed);
	void SetMaximumSpeed(float MaximumSpeed);
	void SetCargoHoldSize(float CargoHoldSize);
	void SetFuelHoldSize(float FuelHoldSize);
	void SetJumpFuel(float JumpFuel);
	void SetForwardFuel(float ForwardFuel);
	void SetTurnFuel(float TurnFuel);
private:
	std::string m_Identifier;
	const Model * m_Model;
	float m_ForwardThrust;
	float m_TurnSpeed;
	float m_MaximumSpeed;
	float m_CargoHoldSize;
	float m_FuelHoldSize;
	float m_JumpFuel;
	float m_ForwardFuel;
	float m_TurnFuel;
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

inline float ShipClass::GetMaximumSpeed(void) const
{
	return m_MaximumSpeed;
}

inline float ShipClass::GetCargoHoldSize(void) const
{
	return m_CargoHoldSize;
}

inline float ShipClass::GetFuelHoldSize(void) const
{
	return m_FuelHoldSize;
}

inline float ShipClass::GetJumpFuel(void) const
{
	return m_JumpFuel;
}

inline float ShipClass::GetForwardFuel(void) const
{
	return m_ForwardFuel;
}

inline float ShipClass::GetTurnFuel(void) const
{
	return m_TurnFuel;
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

inline void ShipClass::SetMaximumSpeed(float MaximumSpeed)
{
	m_MaximumSpeed = MaximumSpeed;
}

inline void ShipClass::SetCargoHoldSize(float CargoHoldSize)
{
	m_CargoHoldSize = CargoHoldSize;
}

inline void ShipClass::SetFuelHoldSize(float FuelHoldSize)
{
	m_FuelHoldSize = FuelHoldSize;
}

inline void ShipClass::SetJumpFuel(float JumpFuel)
{
	m_JumpFuel = JumpFuel;
}

inline void ShipClass::SetForwardFuel(float ForwardFuel)
{
	m_ForwardFuel = ForwardFuel;
}

inline void ShipClass::SetTurnFuel(float TurnFuel)
{
	m_TurnFuel = TurnFuel;
}

#endif
