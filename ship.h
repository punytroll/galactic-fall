#ifndef SHIP_H
#define SHIP_H

#include <map>

#include <math3d/vector2f.h>

#include "physical_object.h"
#include "ship_class.h"

class Commodity;
class ShipClass;
class System;

class Ship : public PhysicalObject
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
	float GetAngularPosition(void) const;
	float GetFuel(void) const;
	float GetFuelCapacity(void) const;
	float GetMaximumSpeed(void) const;
	const ShipClass * GetShipClass(void) const;
	float GetTurnSpeed(void) const;
	const math3d::vector2f & GetVelocity(void) const;
	float GetFreeCargoHoldSize(void) const;
	float GetCommodityAmount(const Commodity * CargoCommodity) const;
	const std::map< const Commodity *, float > GetCommodities(void) const;
	PhysicalObject * GetTarget(void);
	System * GetLinkedSystemTarget(void);
	System * GetCurrentSystem(void);
	// setters
	void SetFuel(float Fuel);
	void SetVelocity(const math3d::vector2f & Velocity);
	void SetAngularPosition(float AngularPosition);
	void SetCommodities(const Commodity * Commodity, float Amount);
	void SetTarget(PhysicalObject * Target);
	void SetLinkedSystemTarget(System * LinkedSystem);
	void SetCurrentSystem(System * CurrentSystem);
	// modifiers
	bool AddCommodities(const Commodity * CargoCommodity, float Amount);
	bool RemoveCommodities(const Commodity * CargoCommodity, float Amount);
private:
	// ship class
	ShipClass * m_ShipClass;
	std::map< const Commodity *, float > m_Commodities;
	float m_Fuel;
	PhysicalObject * m_Target;
	System * m_LinkedSystemTarget;
	System * m_CurrentSystem;
};

inline float Ship::GetForwardThrust(void) const
{
	return m_ShipClass->GetForwardThrust();
}

inline float Ship::GetAngularPosition(void) const
{
	return m_AngularPosition;
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

inline float Ship::GetCommodityAmount(const Commodity * CargoCommodity) const
{
	std::map< const Commodity *, float >::const_iterator CommodityIterator(m_Commodities.find(CargoCommodity));
	
	return ((CommodityIterator == m_Commodities.end()) ? (0) : (CommodityIterator->second));
}

inline const std::map< const Commodity *, float > Ship::GetCommodities(void) const
{
	return m_Commodities;
}

inline PhysicalObject * Ship::GetTarget(void)
{
	return m_Target;
}

inline System * Ship::GetLinkedSystemTarget(void)
{
	return m_LinkedSystemTarget;
}

inline System * Ship::GetCurrentSystem(void)
{
	return m_CurrentSystem;
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

inline void Ship::SetCommodities(const Commodity * Commodity, float Amount)
{
	m_Commodities[Commodity] = Amount;
}

inline void Ship::SetTarget(PhysicalObject * Target)
{
	m_Target = Target;
}

inline void Ship::SetLinkedSystemTarget(System * LinkedSystem)
{
	m_LinkedSystemTarget = LinkedSystem;
}

inline void Ship::SetCurrentSystem(System * CurrentSystem)
{
	m_CurrentSystem = CurrentSystem;
}

#endif
