/**
 * galactic-fall
 * Copyright (C) 2007  Hagen Möbius
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

#ifndef BATTERY_CLASS_H
#define BATTERY_CLASS_H

#include <string>

#include "type_definitions.h"

class BatteryClass
{
public:
	BatteryClass(const std::string & Identifier);
	// getters
	float GetEnergyCapacity(void) const;
	const std::string & GetIdentifier(void) const;
	const std::string & GetName(void) const;
	const std::string & GetSlotClassIdentifier(void) const;
	unsigned_numeric GetSpaceRequirement(void) const;
	// setters
	void SetEnergyCapacity(float EnergyCapacity);
	void SetName(const std::string & Name);
	void SetSlotClassIdentifier(const std::string & SlotClassIdentifier);
	void SetSpaceRequirement(unsigned_numeric SpaceRequirement);
private:
	float m_EnergyCapacity;
	std::string m_Identifier;
	std::string m_Name;
	std::string m_SlotClassIdentifier;
	unsigned_numeric m_SpaceRequirement;
};

inline float BatteryClass::GetEnergyCapacity(void) const
{
	return m_EnergyCapacity;
}

inline const std::string & BatteryClass::GetIdentifier(void) const
{
	return m_Identifier;
}

inline const std::string & BatteryClass::GetName(void) const
{
	return m_Name;
}

inline const std::string & BatteryClass::GetSlotClassIdentifier(void) const
{
	return m_SlotClassIdentifier;
}

inline unsigned_numeric BatteryClass::GetSpaceRequirement(void) const
{
	return m_SpaceRequirement;
}

inline void BatteryClass::SetEnergyCapacity(float EnergyCapacity)
{
	m_EnergyCapacity = EnergyCapacity;
}

inline void BatteryClass::SetName(const std::string & Name)
{
	m_Name = Name;
}

inline void BatteryClass::SetSlotClassIdentifier(const std::string & SlotClassIdentifier)
{
	m_SlotClassIdentifier = SlotClassIdentifier;
}

inline void BatteryClass::SetSpaceRequirement(unsigned_numeric SpaceRequirement)
{
	m_SpaceRequirement = SpaceRequirement;
}

#endif
