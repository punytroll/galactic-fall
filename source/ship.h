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
#include "object.h"
#include "ship_class.h"
#include "type_definitions.h"

class ShipClass;
class SlotClass;
class System;
class Weapon;

namespace Graphics
{
	class ParticleSystem;
}

class Ship : public Object
{
public:
	Ship(const ShipClass * ShipClass);
	virtual ~Ship(void);
	// getters
	float GetAvailableSpace(void) const;
	unsigned_numeric GetContentAmount(const std::string & TypeIdentifier, const std::string & ClassIdentifier) const;
	System * GetCurrentSystem(void);
	const System * GetCurrentSystem(void) const;
	Reference< Graphics::ParticleSystem > & GetEngineGlowParticleSystem(void);
	float GetFuel(void) const;
	float GetFuelCapacity(void) const;
	float GetFuelNeededToJump(void) const;
	float GetHull(void) const;
	float GetHullCapacity(void) const;
	System * GetLinkedSystemTarget(void);
	const System * GetLinkedSystemTarget(void) const;
	float GetMaximumForwardThrust(void) const;
	float GetMaximumSpeed(void) const;
	float GetMaximumTurnSpeed(void) const;
	const ShipClass * GetShipClass(void) const;
	const std::map< std::string, Slot * > & GetSlots(void) const;
	Reference< Object > & GetTarget(void);
	const Reference< Object > & GetTarget(void) const;
	const Vector3f & GetVelocity(void) const;
	// setters
	void SetAccelerate(bool Accelerate);
	void SetCurrentSystem(System * CurrentSystem);
	void SetEngineGlowParticleSystem(Reference< Graphics::ParticleSystem > & EngineGlowParticleSystem);
	void SetFire(bool Fire);
	void SetFuel(float Fuel);
	void SetFuelCapacity(float FuelCapacity);
	void SetFuelNeededToJump(float FuelNeededToJump);
	void SetHull(float Hull);
	void SetHullCapacity(float HullCapacity);
	void SetJettison(bool Jettison);
	void SetJump(bool Jump);
	void SetLand(bool Land);
	void SetLinkedSystemTarget(System * LinkedSystem);
	void SetMaximumForwardThrust(float MaximumForwardThrust);
	void SetMaximumSpeed(float MaximumSpeed);
	void SetMaximumTurnSpeed(float MaximumTurnSpeed);
	void SetRefuel(bool Refuel);
	void SetScoop(bool Scoop);
	void SetTakeOff(bool TakeOff);
	void SetTarget(Reference< Object > Target);
	void SetTurnLeft(float TurnLeft);
	void SetTurnRight(float TurnRight);
	void SetVelocity(const Vector3f & Velocity);
	// modifiers
	Slot * CreateSlot(const SlotClass * SlotClass, const std::string & SlotIdentifier);
	void Mount(Object * Object, const std::string & SlotIdentifier);
	void Unmount(const std::string & SlotIdentifier);
private:
	// slot for the object aspect Update
	bool Update(float Seconds);
private:
	// ship class
	bool m_Accelerate;
	System * m_CurrentSystem;
	Reference< Graphics::ParticleSystem > m_EngineGlowParticleSystem;
	float m_Fuel;
	float m_FuelCapacity;
	float m_FuelNeededToJump;
	float m_Hull;
	float m_HullCapacity;
	bool m_Jettison;
	bool m_Jump;
	bool m_Land;
	System * m_LinkedSystemTarget;
	float m_MaximumForwardThrust;
	float m_MaximumSpeed;
	float m_MaximumTurnSpeed;
	bool m_Refuel;
	bool m_Scoop;
	const ShipClass * m_ShipClass;
	std::map< std::string, Slot * > m_Slots;
	bool m_TakeOff;
	Reference< Object > m_Target;
	float m_TurnLeft;
	float m_TurnRight;
	Vector3f m_Velocity;
};

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

inline float Ship::GetFuel(void) const
{
	return m_Fuel;
}

inline float Ship::GetFuelCapacity(void) const
{
	return m_FuelCapacity;
}

inline float Ship::GetFuelNeededToJump(void) const
{
	return m_FuelNeededToJump;
}

inline float Ship::GetHull(void) const
{
	return m_Hull;
}

inline float Ship::GetHullCapacity(void) const
{
	return m_HullCapacity;
}

inline System * Ship::GetLinkedSystemTarget(void)
{
	return m_LinkedSystemTarget;
}

inline float Ship::GetMaximumForwardThrust(void) const
{
	return m_MaximumForwardThrust;
}

inline float Ship::GetMaximumSpeed(void) const
{
	return m_MaximumSpeed;
}

inline float Ship::GetMaximumTurnSpeed(void) const
{
	return m_MaximumTurnSpeed;
}

inline const System * Ship::GetLinkedSystemTarget(void) const
{
	return m_LinkedSystemTarget;
}

inline const ShipClass * Ship::GetShipClass(void) const
{
	return m_ShipClass;
}

inline const std::map< std::string, Slot * > & Ship::GetSlots(void) const
{
	return m_Slots;
}

inline Reference< Object > & Ship::GetTarget(void)
{
	return m_Target;
}

inline const Reference< Object > & Ship::GetTarget(void) const
{
	return m_Target;
}

inline const Vector3f & Ship::GetVelocity(void) const
{
	return m_Velocity;
}

inline void Ship::SetAccelerate(bool Accelerate)
{
	m_Accelerate = Accelerate;
}

inline void Ship::SetCurrentSystem(System * CurrentSystem)
{
	m_CurrentSystem = CurrentSystem;
}

inline void Ship::SetEngineGlowParticleSystem(Reference< Graphics::ParticleSystem > & EngineGlowParticleSystem)
{
	m_EngineGlowParticleSystem = EngineGlowParticleSystem;
}

inline void Ship::SetFuelCapacity(float FuelCapacity)
{
	m_FuelCapacity = FuelCapacity;
}

inline void Ship::SetFuelNeededToJump(float FuelNeededToJump)
{
	m_FuelNeededToJump = FuelNeededToJump;
}

inline void Ship::SetHullCapacity(float HullCapacity)
{
	m_HullCapacity = HullCapacity;
}

inline void Ship::SetJettison(bool Jettison)
{
	m_Jettison = Jettison;
}

inline void Ship::SetJump(bool Jump)
{
	m_Jump = Jump;
}

inline void Ship::SetLand(bool Land)
{
	m_Land = Land;
}

inline void Ship::SetLinkedSystemTarget(System * LinkedSystem)
{
	m_LinkedSystemTarget = LinkedSystem;
}

inline void Ship::SetMaximumForwardThrust(float MaximumForwardThrust)
{
	m_MaximumForwardThrust = MaximumForwardThrust;
}

inline void Ship::SetMaximumSpeed(float MaximumSpeed)
{
	m_MaximumSpeed = MaximumSpeed;
}

inline void Ship::SetMaximumTurnSpeed(float MaximumTurnSpeed)
{
	m_MaximumTurnSpeed = MaximumTurnSpeed;
}

inline void Ship::SetRefuel(bool Refuel)
{
	m_Refuel = Refuel;
}

inline void Ship::SetScoop(bool Scoop)
{
	m_Scoop = Scoop;
}

inline void Ship::SetTakeOff(bool TakeOff)
{
	m_TakeOff = TakeOff;
}

inline void Ship::SetTarget(Reference< Object > Target)
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
