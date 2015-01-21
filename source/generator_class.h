/**
 * galactic-fall
 * Copyright (C) 2009  Hagen Möbius
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

#ifndef GENERATOR_CLASS_H
#define GENERATOR_CLASS_H

#include <string>

#include "type_definitions.h"

class GeneratorClass
{
public:
	GeneratorClass(const std::string & Identifier);
	// getters
	float GetEnergyProvisionPerSecond(void) const;
	const std::string & GetIdentifier(void) const;
	const std::string & GetName(void) const;
	const std::string & GetSlotClassIdentifier(void) const;
	unsigned_numeric GetSpaceRequirement(void) const;
	// setters
	void SetEnergyProvisionPerSecond(float EnergyProvisionPerSecond);
	void SetName(const std::string & Name);
	void SetSlotClassIdentifier(const std::string & SlotClassIdentifier);
	void SetSpaceRequirement(unsigned_numeric SpaceRequirement);
private:
	float _EnergyProvisionPerSecond;
	std::string _Identifier;
	std::string _Name;
	std::string _SlotClassIdentifier;
	unsigned_numeric _SpaceRequirement;
};

inline float GeneratorClass::GetEnergyProvisionPerSecond(void) const
{
	return _EnergyProvisionPerSecond;
}

inline const std::string & GeneratorClass::GetIdentifier(void) const
{
	return _Identifier;
}

inline const std::string & GeneratorClass::GetName(void) const
{
	return _Name;
}

inline const std::string & GeneratorClass::GetSlotClassIdentifier(void) const
{
	return _SlotClassIdentifier;
}

inline unsigned_numeric GeneratorClass::GetSpaceRequirement(void) const
{
	return _SpaceRequirement;
}

inline void GeneratorClass::SetEnergyProvisionPerSecond(float EnergyProvisionPerSecond)
{
	_EnergyProvisionPerSecond = EnergyProvisionPerSecond;
}

inline void GeneratorClass::SetName(const std::string & Name)
{
	_Name = Name;
}

inline void GeneratorClass::SetSlotClassIdentifier(const std::string & SlotClassIdentifier)
{
	_SlotClassIdentifier = SlotClassIdentifier;
}

inline void GeneratorClass::SetSpaceRequirement(unsigned_numeric SpaceRequirement)
{
	_SpaceRequirement = SpaceRequirement;
}

#endif
