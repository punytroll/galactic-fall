/**
 * galactic-fall
 * Copyright (C) 2007-2018  Hagen Möbius
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

class VisualizationPrototype;

class BatteryClass
{
public:
	// constructor & destructor
	BatteryClass(const std::string & Identifier);
	~BatteryClass(void);
	// getters
	std::uint32_t GetBasePrice(void) const;
	const std::string & GetDescription(void) const;
	float GetEnergyCapacity(void) const;
	const std::string & GetIdentifier(void) const;
	const std::string & GetName(void) const;
	const std::string & GetSlotClassIdentifier(void) const;
	std::uint32_t GetSpaceRequirement(void) const;
	const VisualizationPrototype * GetVisualizationPrototype(void) const;
	// setters
	void SetBasePrice(std::uint32_t BasePrice);
	void SetDescription(const std::string & Description);
	void SetEnergyCapacity(float EnergyCapacity);
	void SetName(const std::string & Name);
	void SetSlotClassIdentifier(const std::string & SlotClassIdentifier);
	void SetSpaceRequirement(std::uint32_t SpaceRequirement);
	void SetVisualizationPrototype(const VisualizationPrototype & VisualizationPrototype);
private:
	std::uint32_t _BasePrice;
	std::string _Description;
	float _EnergyCapacity;
	std::string _Identifier;
	std::string _Name;
	std::string _SlotClassIdentifier;
	std::uint32_t _SpaceRequirement;
	VisualizationPrototype * _VisualizationPrototype;
};

inline std::uint32_t BatteryClass::GetBasePrice(void) const
{
	return _BasePrice;
}

inline const std::string & BatteryClass::GetDescription(void) const
{
	return _Description;
}

inline float BatteryClass::GetEnergyCapacity(void) const
{
	return _EnergyCapacity;
}

inline const std::string & BatteryClass::GetIdentifier(void) const
{
	return _Identifier;
}

inline const std::string & BatteryClass::GetName(void) const
{
	return _Name;
}

inline const std::string & BatteryClass::GetSlotClassIdentifier(void) const
{
	return _SlotClassIdentifier;
}

inline std::uint32_t BatteryClass::GetSpaceRequirement(void) const
{
	return _SpaceRequirement;
}

inline const VisualizationPrototype * BatteryClass::GetVisualizationPrototype(void) const
{
	return _VisualizationPrototype;
}

inline void BatteryClass::SetBasePrice(std::uint32_t BasePrice)
{
	_BasePrice = BasePrice;
}

inline void BatteryClass::SetDescription(const std::string & Description)
{
	_Description = Description;
}

inline void BatteryClass::SetEnergyCapacity(float EnergyCapacity)
{
	_EnergyCapacity = EnergyCapacity;
}

inline void BatteryClass::SetName(const std::string & Name)
{
	_Name = Name;
}

inline void BatteryClass::SetSlotClassIdentifier(const std::string & SlotClassIdentifier)
{
	_SlotClassIdentifier = SlotClassIdentifier;
}

inline void BatteryClass::SetSpaceRequirement(std::uint32_t SpaceRequirement)
{
	_SpaceRequirement = SpaceRequirement;
}

#endif
