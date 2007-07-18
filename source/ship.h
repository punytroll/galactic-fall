/**
 * galactic-fall
 * Copyright (C) 2006  Hagen Möbius
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**/

#ifndef SHIP_H
#define SHIP_H

#include <map>
#include <vector>

#include "math/vector2f.h"
#include "physical_object.h"
#include "ship_class.h"
#include "type_definitions.h"

class CommodityClass;
class ShipClass;
class System;
class Weapon;

class Ship : public PhysicalObject
{
public:
	Ship(const ShipClass * ShipClass);
	void Draw(void) const;
	void Update(float Seconds);
	// controls
	bool m_Jettison;
	bool m_Jump;
	bool m_Land;
	bool m_Scoop;
	// current state
	float m_AngularPosition;
	// getters
	float GetForwardThrust(void) const;
	float GetAngularPosition(void) const;
	float GetFuel(void) const;
	float GetHull(void) const;
	float GetFuelCapacity(void) const;
	float GetMaximumSpeed(void) const;
	const ShipClass * GetShipClass(void) const;
	float GetTurnSpeed(void) const;
	const Vector2f & GetVelocity(void) const;
	float GetAvailableSpace(void) const;
	unsigned_numeric GetContentAmount(const std::string & Type, const std::string & Class) const;
	Reference< PhysicalObject > & GetTarget(void);
	const Reference< PhysicalObject > & GetTarget(void) const;
	System * GetLinkedSystemTarget(void);
	System * GetCurrentSystem(void);
	const System * GetCurrentSystem(void) const;
	const std::map< std::string, Slot * > & GetSlots(void) const;
	// setters
	void SetAccelerate(bool Accelerate);
	void SetFire(bool Fire);
	void SetFuel(float Fuel);
	void SetHull(float Hull);
	void SetRefuel(bool Refuel);
	void SetTurnLeft(float TurnLeft);
	void SetTurnRight(float TurnRight);
	void SetVelocity(const Vector2f & Velocity);
	void SetAngularPosition(float AngularPosition);
	void SetTarget(Reference< PhysicalObject > Target);
	void SetLinkedSystemTarget(System * LinkedSystem);
	void SetCurrentSystem(System * CurrentSystem);
	// modifiers
	Slot * CreateSlot(const std::string & SlotIdentifier);
	bool Mount(Object * Object, const std::string & SlotIdentifier);
protected:
	virtual bool OnAddContent(Object * Content);
	virtual bool OnRemoveContent(Object * Content);
private:
	// ship class
	const ShipClass * m_ShipClass;
	std::map< std::string, Slot * > m_Slots;
	std::vector< Weapon * > m_Weapons;
	bool m_Accelerate;
	bool m_Accelerating;
	float m_Fuel;
	float m_Hull;
	bool m_Refuel;
	float m_TurnLeft;
	float m_TurnRight;
	Reference< PhysicalObject > m_Target;
	System * m_LinkedSystemTarget;
	System * m_CurrentSystem;
	Vector2f m_Velocity;
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

inline float Ship::GetHull(void) const
{
	return m_Hull;
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

inline const Vector2f & Ship::GetVelocity(void) const
{
	return m_Velocity;
}

inline Reference< PhysicalObject > & Ship::GetTarget(void)
{
	return m_Target;
}

inline const Reference< PhysicalObject > & Ship::GetTarget(void) const
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

inline const System * Ship::GetCurrentSystem(void) const
{
	return m_CurrentSystem;
}

inline const std::map< std::string, Slot * > & Ship::GetSlots(void) const
{
	return m_Slots;
}

inline void Ship::SetAccelerate(bool Accelerate)
{
	m_Accelerate = Accelerate;
}

inline void Ship::SetFuel(float Fuel)
{
	m_Fuel = Fuel;
}

inline void Ship::SetHull(float Hull)
{
	m_Hull = Hull;
}

inline void Ship::SetRefuel(bool Refuel)
{
	m_Refuel = Refuel;
}

inline void Ship::SetTurnLeft(float TurnLeft)
{
	assert((TurnLeft >= 0.0f) && (TurnLeft <= 1.0f));
	m_TurnLeft = TurnLeft;
}

inline void Ship::SetTurnRight(float TurnRight)
{
	assert((TurnRight >= 0.0f) && (TurnRight <= 1.0f));
	m_TurnRight = TurnRight;
}

inline void Ship::SetVelocity(const Vector2f & Velocity)
{
	m_Velocity = Velocity;
}

inline void Ship::SetAngularPosition(float AngularPosition)
{
	m_AngularPosition = AngularPosition;
}

inline void Ship::SetTarget(Reference< PhysicalObject > Target)
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
