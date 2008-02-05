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
class SlotClass;
class System;
class Weapon;

namespace Graphics
{
	class ParticleSystem;
}

class Ship : public PhysicalObject
{
public:
	Ship(const ShipClass * ShipClass);
	virtual ~Ship(void);
	void Update(float Seconds);
	// controls
	bool m_Jettison;
	bool m_Jump;
	bool m_Land;
	bool m_Scoop;
	// getters
	const Quaternion & GetAngularPosition(void) const;
	float GetAvailableSpace(void) const;
	unsigned_numeric GetContentAmount(const std::string & Type, const std::string & Class) const;
	System * GetCurrentSystem(void);
	const System * GetCurrentSystem(void) const;
	Reference< Graphics::ParticleSystem > & GetEngineGlowParticleSystem(void);
	float GetForwardThrust(void) const;
	float GetFuel(void) const;
	float GetFuelCapacity(void) const;
	float GetHull(void) const;
	System * GetLinkedSystemTarget(void);
	float GetMaximumSpeed(void) const;
	const ShipClass * GetShipClass(void) const;
	const std::map< std::string, Slot * > & GetSlots(void) const;
	const Reference< PhysicalObject > & GetTarget(void) const;
	float GetTurnSpeed(void) const;
	const Vector3f & GetVelocity(void) const;
	// setters
	void SetAccelerate(bool Accelerate);
	void SetAngularPosition(const Quaternion & AngularPosition);
	void SetCurrentSystem(System * CurrentSystem);
	void SetEngineGlowParticleSystem(Reference< Graphics::ParticleSystem > & EngineGlowParticleSystem);
	void SetFire(bool Fire);
	void SetFuel(float Fuel);
	void SetHull(float Hull);
	void SetLinkedSystemTarget(System * LinkedSystem);
	void SetRefuel(bool Refuel);
	void SetTarget(Reference< PhysicalObject > Target);
	void SetTurnLeft(float TurnLeft);
	void SetTurnRight(float TurnRight);
	void SetVelocity(const Vector3f & Velocity);
	// modifiers
	Slot * CreateSlot(const SlotClass * SlotClass, const std::string & SlotIdentifier);
	bool Mount(Object * Object, const std::string & SlotIdentifier);
	bool Unmount(const std::string & SlotIdentifier);
private:
	// ship class
	bool m_Accelerate;
	Quaternion m_AngularPosition;
	System * m_CurrentSystem;
	Reference< Graphics::ParticleSystem > m_EngineGlowParticleSystem;
	float m_Fuel;
	float m_Hull;
	System * m_LinkedSystemTarget;
	bool m_Refuel;
	const ShipClass * m_ShipClass;
	std::map< std::string, Slot * > m_Slots;
	Reference< PhysicalObject > m_Target;
	float m_TurnLeft;
	float m_TurnRight;
	Vector3f m_Velocity;
};

inline const Quaternion & Ship::GetAngularPosition(void) const
{
	return m_AngularPosition;
}

inline System * Ship::GetCurrentSystem(void)
{
	return m_CurrentSystem;
}

inline const System * Ship::GetCurrentSystem(void) const
{
	return m_CurrentSystem;
}

inline Reference< Graphics::ParticleSystem > & Ship::GetEngineGlowParticleSystem(void)
{
	return m_EngineGlowParticleSystem;
}

inline float Ship::GetForwardThrust(void) const
{
	return m_ShipClass->GetForwardThrust();
}

inline float Ship::GetFuel(void) const
{
	return m_Fuel;
}

inline float Ship::GetHull(void) const
{
	return m_Hull;
}

inline System * Ship::GetLinkedSystemTarget(void)
{
	return m_LinkedSystemTarget;
}

inline float Ship::GetMaximumSpeed(void) const
{
	return m_ShipClass->GetMaximumSpeed();
}

inline const ShipClass * Ship::GetShipClass(void) const
{
	return m_ShipClass;
}

inline const std::map< std::string, Slot * > & Ship::GetSlots(void) const
{
	return m_Slots;
}

inline const Reference< PhysicalObject > & Ship::GetTarget(void) const
{
	return m_Target;
}

inline float Ship::GetTurnSpeed(void) const
{
	return m_ShipClass->GetTurnSpeed();
}

inline const Vector3f & Ship::GetVelocity(void) const
{
	return m_Velocity;
}

inline void Ship::SetAccelerate(bool Accelerate)
{
	m_Accelerate = Accelerate;
}

inline void Ship::SetAngularPosition(const Quaternion & AngularPosition)
{
	m_AngularPosition = AngularPosition;
}

inline void Ship::SetCurrentSystem(System * CurrentSystem)
{
	m_CurrentSystem = CurrentSystem;
}

inline void Ship::SetEngineGlowParticleSystem(Reference< Graphics::ParticleSystem > & EngineGlowParticleSystem)
{
	m_EngineGlowParticleSystem = EngineGlowParticleSystem;
}

inline void Ship::SetFuel(float Fuel)
{
	m_Fuel = Fuel;
}

inline void Ship::SetHull(float Hull)
{
	m_Hull = Hull;
}

inline void Ship::SetLinkedSystemTarget(System * LinkedSystem)
{
	m_LinkedSystemTarget = LinkedSystem;
}

inline void Ship::SetRefuel(bool Refuel)
{
	m_Refuel = Refuel;
}

inline void Ship::SetTarget(Reference< PhysicalObject > Target)
{
	m_Target = Target;
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

inline void Ship::SetVelocity(const Vector3f & Velocity)
{
	m_Velocity = Velocity;
}

#endif
