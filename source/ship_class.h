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
	std::uint32_t GetBasePrice(void) const;
	const std::string & GetDescription(void) const;
	const Vector3f & GetExhaustOffset(void) const;
	float GetExhaustRadius(void) const;
	float GetFuelCapacity(void) const;
	const std::string & GetIdentifier(void) const;
	const std::string & GetName(void) const;
	std::uint32_t GetSpaceRequirement(void) const;
	float GetForwardThrust(void) const;
	float GetTurnSpeed(void) const;
	float GetMaximumSpeed(void) const;
	std::uint32_t GetMaximumAvailableSpace(void) const;
	float GetJumpFuel(void) const;
	float GetForwardFuel(void) const;
	float GetTurnFuel(void) const;
	float GetHull(void) const;
	const std::map< std::string, Slot * > & GetSlots(void) const;
	const VisualizationPrototype * GetVisualizationPrototype(void) const;
	// setters
	void SetBasePrice(std::uint32_t BasePrice);
	void SetDescription(const std::string & Description);
	void SetExhaustOffset(const Vector3f & ExhaustOffset);
	void SetExhaustRadius(float ExhaustRadius);
	void SetFuelCapacity(float FuelCapacity);
	void SetForwardThrust(float ForwardThrust);
	void SetName(const std::string & Name);
	void SetSpaceRequirement(std::uint32_t SpaceRequirement);
	void SetTurnSpeed(float TurnSpeed);
	void SetMaximumSpeed(float MaximumSpeed);
	void SetMaximumAvailableSpace(std::uint32_t MaximumAvailableSpace);
	void SetJumpFuel(float JumpFuel);
	void SetForwardFuel(float ForwardFuel);
	void SetTurnFuel(float TurnFuel);
	void SetHull(float Hull);
	void SetVisualizationPrototype(const VisualizationPrototype & VisualizationPrototype);
	
	/**
	 * @note Passes memory management responsibility for @a Slot to the ship class.
	 **/
	bool AddSlot(const std::string & SlotIdentifier, Slot * Slot);
private:
	std::uint32_t _BasePrice;
	std::string _Description;
	Vector3f _ExhaustOffset;
	float _ExhaustRadius;
	float _FuelCapacity;
	std::string _Identifier;
	std::string _Name;
	std::uint32_t _SpaceRequirement;
	float _ForwardThrust;
	float _TurnSpeed;
	float _MaximumSpeed;
	std::uint32_t _MaximumAvailableSpace;
	float _JumpFuel;
	float _ForwardFuel;
	float _TurnFuel;
	float _Hull;
	std::map< std::string, Slot * > _Slots;
	VisualizationPrototype * _VisualizationPrototype;
};

inline std::uint32_t ShipClass::GetBasePrice(void) const
{
	return _BasePrice;
}

inline const std::string & ShipClass::GetDescription(void) const
{
	return _Description;
}

inline const Vector3f & ShipClass::GetExhaustOffset(void) const
{
	return _ExhaustOffset;
}

inline float ShipClass::GetExhaustRadius(void) const
{
	return _ExhaustRadius;
}

inline float ShipClass::GetFuelCapacity(void) const
{
	return _FuelCapacity;
}

inline const std::string & ShipClass::GetIdentifier(void) const
{
	return _Identifier;
}

inline const std::string & ShipClass::GetName(void) const
{
	return _Name;
}

inline std::uint32_t ShipClass::GetSpaceRequirement(void) const
{
	return _SpaceRequirement;
}

inline float ShipClass::GetForwardThrust(void) const
{
	return _ForwardThrust;
}

inline float ShipClass::GetTurnSpeed(void) const
{
	return _TurnSpeed;
}

inline float ShipClass::GetMaximumSpeed(void) const
{
	return _MaximumSpeed;
}

inline std::uint32_t ShipClass::GetMaximumAvailableSpace(void) const
{
	return _MaximumAvailableSpace;
}

inline float ShipClass::GetJumpFuel(void) const
{
	return _JumpFuel;
}

inline float ShipClass::GetForwardFuel(void) const
{
	return _ForwardFuel;
}

inline float ShipClass::GetTurnFuel(void) const
{
	return _TurnFuel;
}

inline float ShipClass::GetHull(void) const
{
	return _Hull;
}

inline const std::map< std::string, Slot * > & ShipClass::GetSlots(void) const
{
	return _Slots;
}

inline const VisualizationPrototype * ShipClass::GetVisualizationPrototype(void) const
{
	return _VisualizationPrototype;
}

inline void ShipClass::SetBasePrice(std::uint32_t BasePrice)
{
	_BasePrice = BasePrice;
}

inline void ShipClass::SetDescription(const std::string & Description)
{
	_Description = Description;
}

inline void ShipClass::SetExhaustOffset(const Vector3f & ExhaustOffset)
{
	_ExhaustOffset = ExhaustOffset;
}

inline void ShipClass::SetExhaustRadius(float ExhaustRadius)
{
	_ExhaustRadius = ExhaustRadius;
}

inline void ShipClass::SetFuelCapacity(float FuelCapacity)
{
	_FuelCapacity = FuelCapacity;
}

inline void ShipClass::SetForwardThrust(float ForwardThrust)
{
	_ForwardThrust = ForwardThrust;
}

inline void ShipClass::SetName(const std::string & Name)
{
	_Name = Name;
}

inline void ShipClass::SetSpaceRequirement(std::uint32_t SpaceRequirement)
{
	_SpaceRequirement = SpaceRequirement;
}

inline void ShipClass::SetTurnSpeed(float TurnSpeed)
{
	_TurnSpeed = TurnSpeed;
}

inline void ShipClass::SetMaximumSpeed(float MaximumSpeed)
{
	_MaximumSpeed = MaximumSpeed;
}

inline void ShipClass::SetMaximumAvailableSpace(std::uint32_t MaximumAvailableSpace)
{
	_MaximumAvailableSpace = MaximumAvailableSpace;
}

inline void ShipClass::SetJumpFuel(float JumpFuel)
{
	_JumpFuel = JumpFuel;
}

inline void ShipClass::SetForwardFuel(float ForwardFuel)
{
	_ForwardFuel = ForwardFuel;
}

inline void ShipClass::SetTurnFuel(float TurnFuel)
{
	_TurnFuel = TurnFuel;
}

inline void ShipClass::SetHull(float Hull)
{
	_Hull = Hull;
}

#endif
