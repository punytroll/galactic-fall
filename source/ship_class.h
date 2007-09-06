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

#include "math/vector3f.h"
#include "type_definitions.h"

class Slot;

namespace Graphics
{
	class Material;
	class Model;
}

class ShipClass
{
public:
	// constructor
	ShipClass(const std::string & Identifier);
	// destructor
	~ShipClass(void);
	// getters
	const std::string & GetIdentifier(void) const;
	const Graphics::Model * GetModel(void) const;
	float GetForwardThrust(void) const;
	float GetTurnSpeed(void) const;
	float GetMaximumSpeed(void) const;
	unsigned_numeric GetMaximumAvailableSpace(void) const;
	float GetFuelHoldSize(void) const;
	float GetJumpFuel(void) const;
	float GetForwardFuel(void) const;
	float GetTurnFuel(void) const;
	float GetHull(void) const;
	const Vector3f & GetExhaustOffset(void) const;
	float GetExhaustRadius(void) const;
	const std::map< std::string, Graphics::Material * > & GetPartMaterials(void) const;
	const std::map< std::string, Slot * > & GetSlots(void) const;
	// setters
	void SetModel(const Graphics::Model * Model);
	void SetForwardThrust(float ForwardThrust);
	void SetTurnSpeed(float TurnSpeed);
	void SetMaximumSpeed(float MaximumSpeed);
	void SetMaximumAvailableSpace(unsigned_numeric MaximumAvailableSpace);
	void SetFuelHoldSize(float FuelHoldSize);
	void SetJumpFuel(float JumpFuel);
	void SetForwardFuel(float ForwardFuel);
	void SetTurnFuel(float TurnFuel);
	void SetHull(float Hull);
	void SetExhaustOffset(const Vector3f & ExhaustOffset);
	void SetExhaustRadius(float ExhaustRadius);
	
	// modifiers
	/**
	 * @note Passes memory management responsibility for @a PartMaterial to the ship class.
	 **/
	bool AddPartMaterial(const std::string & Identifier, Graphics::Material * PartMaterial);
	
	/**
	 * @note Passes memory management responsibility for @a Slot to the ship class.
	 **/
	bool AddSlot(const std::string & SlotIdentifier, Slot * Slot);
private:
	std::string m_Identifier;
	const Graphics::Model * m_Model;
	float m_ForwardThrust;
	float m_TurnSpeed;
	float m_MaximumSpeed;
	unsigned_numeric m_MaximumAvailableSpace;
	float m_FuelHoldSize;
	float m_JumpFuel;
	float m_ForwardFuel;
	float m_TurnFuel;
	float m_Hull;
	Vector3f m_ExhaustOffset;
	float m_ExhaustRadius;
	std::map< std::string, Slot * > m_Slots;
	std::map< std::string, Graphics::Material * > m_PartMaterials;
};

inline const std::string & ShipClass::GetIdentifier(void) const
{
	return m_Identifier;
}

inline const Graphics::Model * ShipClass::GetModel(void) const
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

inline unsigned_numeric ShipClass::GetMaximumAvailableSpace(void) const
{
	return m_MaximumAvailableSpace;
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

inline float ShipClass::GetHull(void) const
{
	return m_Hull;
}

inline const Vector3f & ShipClass::GetExhaustOffset(void) const
{
	return m_ExhaustOffset;
}

inline float ShipClass::GetExhaustRadius(void) const
{
	return m_ExhaustRadius;
}

inline const std::map< std::string, Graphics::Material * > & ShipClass::GetPartMaterials(void) const
{
	return m_PartMaterials;
}

inline const std::map< std::string, Slot * > & ShipClass::GetSlots(void) const
{
	return m_Slots;
}

inline void ShipClass::SetModel(const Graphics::Model * Model)
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

inline void ShipClass::SetMaximumAvailableSpace(unsigned_numeric MaximumAvailableSpace)
{
	m_MaximumAvailableSpace = MaximumAvailableSpace;
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

inline void ShipClass::SetHull(float Hull)
{
	m_Hull = Hull;
}

inline void ShipClass::SetExhaustOffset(const Vector3f & ExhaustOffset)
{
	m_ExhaustOffset = ExhaustOffset;
}

inline void ShipClass::SetExhaustRadius(float ExhaustRadius)
{
	m_ExhaustRadius = ExhaustRadius;
}

#endif
