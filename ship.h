#ifndef SHIP_H
#define SHIP_H

#include <map>

#include <math3d/vector2f.h>

#include "position.h"
#include "ship_class.h"

class Commodity;
class ShipClass;

class Ship : public Position
{
public:
	Ship(ShipClass * ShipClass);
	void Draw(void) const;
	void Move(float Seconds);
	// enabler
	bool m_Accelerate;
	bool m_TurnLeft;
	bool m_TurnRight;
	// current state
	math3d::vector2f m_Velocity;
	float m_AngularPosition;
	// getters
	float GetForwardThrust(void) const;
	float GetFuel(void) const;
	float GetFuelCapacity(void) const;
	float GetMaximumSpeed(void) const;
	const ShipClass * GetShipClass(void) const;
	float GetTurnSpeed(void) const;
	const math3d::vector2f & GetVelocity(void) const;
	float GetFreeCargoHoldSize(void) const;
	float GetCargoAmount(const Commodity * CargoCommodity) const;
	const std::map< const Commodity *, float > GetCargo(void) const;
	// setters
	void SetFuel(float Fuel);
	void SetVelocity(const math3d::vector2f & Velocity);
	void SetAngularPosition(float AngularPosition);
	// modifiers
	bool AddCargo(const Commodity * CargoCommodity, float Amount);
	void Jump(void);
	void Refuel(float Fuel);
	bool RemoveCargo(const Commodity * CargoCommodity, float Amount);
private:
	// ship class
	ShipClass * m_ShipClass;
	std::map< const Commodity *, float > m_Cargo;
	float m_Fuel;
};

inline float Ship::GetForwardThrust(void) const
{
	return m_ShipClass->GetForwardThrust();
}

inline float Ship::GetFuel(void) const
{
	return m_Fuel;
}

inline float Ship::GetMaximumSpeed(void) const
{
	return m_ShipClass->GetMaximumSpeed();
}

inline const ShipClass * Ship::GetShipClass(void) const
{
	return m_ShipClass;
}

inline float Ship::GetTurnSpeed(void) const
{
	return m_ShipClass->GetTurnSpeed();
}

inline const math3d::vector2f & Ship::GetVelocity(void) const
{
	return m_Velocity;
}

inline float Ship::GetCargoAmount(const Commodity * CargoCommodity) const
{
	std::map< const Commodity *, float >::const_iterator CargoIterator(m_Cargo.find(CargoCommodity));
	
	return ((CargoIterator == m_Cargo.end()) ? (0) : (CargoIterator->second));
}

inline const std::map< const Commodity *, float > Ship::GetCargo(void) const
{
	return m_Cargo;
}

inline void Ship::SetFuel(float Fuel)
{
	m_Fuel = Fuel;
}

inline void Ship::SetVelocity(const math3d::vector2f & Velocity)
{
	m_Velocity = Velocity;
}

inline void Ship::SetAngularPosition(float AngularPosition)
{
	m_AngularPosition = AngularPosition;
}

#endif
