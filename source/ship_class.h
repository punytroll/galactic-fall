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

#ifndef SHIP_CLASS_H
#define SHIP_CLASS_H

#include <map>
#include <string>

#include <algebra/vector3f.h>

#include "type_definitions.h"

class Slot;
class VisualizationPrototype;

class ShipClass
{
public:
	// constructor
	ShipClass(const std::string & Identifier);
	// destructor
	~ShipClass(void);
	// getters
	const Vector3f & GetExhaustOffset(void) const;
	float GetExhaustRadius(void) const;
	float GetFuelCapacity(void) const;
	const std::string & GetIdentifier(void) const;
	unsigned_numeric GetSpaceRequirement(void) const;
	float GetForwardThrust(void) const;
	float GetTurnSpeed(void) const;
	float GetMaximumSpeed(void) const;
	unsigned_numeric GetMaximumAvailableSpace(void) const;
	float GetJumpFuel(void) const;
	float GetForwardFuel(void) const;
	float GetTurnFuel(void) const;
	float GetHull(void) const;
	const std::map< std::string, Slot * > & GetSlots(void) const;
	VisualizationPrototype * GetVisualizationPrototype(void);
	const VisualizationPrototype * GetVisualizationPrototype(void) const;
	// setters
	void SetExhaustOffset(const Vector3f & ExhaustOffset);
	void SetExhaustRadius(float ExhaustRadius);
	void SetFuelCapacity(float FuelCapacity);
	void SetForwardThrust(float ForwardThrust);
	void SetSpaceRequirement(unsigned_numeric SpaceRequirement);
	void SetTurnSpeed(float TurnSpeed);
	void SetMaximumSpeed(float MaximumSpeed);
	void SetMaximumAvailableSpace(unsigned_numeric MaximumAvailableSpace);
	void SetJumpFuel(float JumpFuel);
	void SetForwardFuel(float ForwardFuel);
	void SetTurnFuel(float TurnFuel);
	void SetHull(float Hull);
	
	/**
	 * @note Passes memory management responsibility for @a Slot to the ship class.
	 **/
	bool AddSlot(const std::string & SlotIdentifier, Slot * Slot);
	void AddVisualizationPrototype(void);
private:
	Vector3f m_ExhaustOffset;
	float m_ExhaustRadius;
	float m_FuelCapacity;
	std::string m_Identifier;
	unsigned_numeric m_SpaceRequirement;
	float m_ForwardThrust;
	float m_TurnSpeed;
	float m_MaximumSpeed;
	unsigned_numeric m_MaximumAvailableSpace;
	float m_JumpFuel;
	float m_ForwardFuel;
	float m_TurnFuel;
	float m_Hull;
	std::map< std::string, Slot * > m_Slots;
	VisualizationPrototype * m_VisualizationPrototype;
};

inline const Vector3f & ShipClass::GetExhaustOffset(void) const
{
	return m_ExhaustOffset;
}

inline float ShipClass::GetExhaustRadius(void) const
{
	return m_ExhaustRadius;
}

inline float ShipClass::GetFuelCapacity(void) const
{
	return m_FuelCapacity;
}

inline const std::string & ShipClass::GetIdentifier(void) const
{
	return m_Identifier;
}

inline unsigned_numeric ShipClass::GetSpaceRequirement(void) const
{
	return m_SpaceRequirement;
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

inline unsigned_numeric ShipClass::GetMaximumAvailableSpace(void) const
{
	return m_MaximumAvailableSpace;
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

inline float ShipClass::GetHull(void) const
{
	return m_Hull;
}

inline const std::map< std::string, Slot * > & ShipClass::GetSlots(void) const
{
	return m_Slots;
}

inline VisualizationPrototype * ShipClass::GetVisualizationPrototype(void)
{
	return m_VisualizationPrototype;
}

inline const VisualizationPrototype * ShipClass::GetVisualizationPrototype(void) const
{
	return m_VisualizationPrototype;
}

inline void ShipClass::SetExhaustOffset(const Vector3f & ExhaustOffset)
{
	m_ExhaustOffset = ExhaustOffset;
}

inline void ShipClass::SetExhaustRadius(float ExhaustRadius)
{
	m_ExhaustRadius = ExhaustRadius;
}

inline void ShipClass::SetFuelCapacity(float FuelCapacity)
{
	m_FuelCapacity = FuelCapacity;
}

inline void ShipClass::SetForwardThrust(float ForwardThrust)
{
	m_ForwardThrust = ForwardThrust;
}

inline void ShipClass::SetSpaceRequirement(unsigned_numeric SpaceRequirement)
{
	m_SpaceRequirement = SpaceRequirement;
}

inline void ShipClass::SetTurnSpeed(float TurnSpeed)
{
	m_TurnSpeed = TurnSpeed;
}

inline void ShipClass::SetMaximumSpeed(float MaximumSpeed)
{
	m_MaximumSpeed = MaximumSpeed;
}

inline void ShipClass::SetMaximumAvailableSpace(unsigned_numeric MaximumAvailableSpace)
{
	m_MaximumAvailableSpace = MaximumAvailableSpace;
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

inline void ShipClass::SetHull(float Hull)
{
	m_Hull = Hull;
}

#endif
