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

#include <math3d/vector2f.h>

#include "physical_object.h"
#include "ship_class.h"

class Commodity;
class ShipClass;
class System;
class Weapon;

class Ship : public PhysicalObject
{
public:
	Ship(ShipClass * ShipClass);
	void Draw(void) const;
	void Update(float Seconds);
	// controls
	bool m_Accelerate;
	bool m_TurnLeft;
	bool m_TurnRight;
	bool m_Jettison;
	bool m_Jump;
	bool m_Land;
	bool m_Scoop;
	// current state
	math3d::vector2f m_Velocity;
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
	const math3d::vector2f & GetVelocity(void) const;
	float GetFreeCargoHoldSize(void) const;
	float GetCommodityAmount(const Commodity * CargoCommodity) const;
	Reference< PhysicalObject > & GetTarget(void);
	const Reference< PhysicalObject > & GetTarget(void) const;
	System * GetLinkedSystemTarget(void);
	System * GetCurrentSystem(void);
	const System * GetCurrentSystem(void) const;
	// setters
	void SetFire(bool Fire);
	void SetFuel(float Fuel);
	void SetHull(float Hull);
	void SetVelocity(const math3d::vector2f & Velocity);
	void SetAngularPosition(float AngularPosition);
	void SetTarget(Reference< PhysicalObject > Target);
	void SetLinkedSystemTarget(System * LinkedSystem);
	void SetCurrentSystem(System * CurrentSystem);
	// manifest
	bool AddObject(Object * Add);
	bool RemoveObject(Object * Remove);
	std::set< Object * > & GetManifest(void);
	const std::set< Object * > & GetManifest(void) const;
private:
	// ship class
	ShipClass * m_ShipClass;
	std::set< Object * > m_Manifest;
	std::vector< Weapon * > m_Weapons;
	float m_Fuel;
	float m_Hull;
	Reference< PhysicalObject > m_Target;
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

inline const math3d::vector2f & Ship::GetVelocity(void) const
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

inline void Ship::SetFuel(float Fuel)
{
	m_Fuel = Fuel;
}

inline void Ship::SetHull(float Hull)
{
	m_Hull = Hull;
}

inline void Ship::SetVelocity(const math3d::vector2f & Velocity)
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

inline std::set< Object * > & Ship::GetManifest(void)
{
	return m_Manifest;
}

inline const std::set< Object * > & Ship::GetManifest(void) const
{
	return m_Manifest;
}

#endif
