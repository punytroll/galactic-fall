/**
 * galactic-fall
 * Copyright (C) 2018  Hagen Möbius
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

#ifndef AMMUNITION_CLASS_H
#define AMMUNITION_CLASS_H

#include <string>

class VisualizationPrototype;

class AmmunitionClass
{
public:
	// constructor & destructor
	AmmunitionClass(const std::string & Identifier);
	~AmmunitionClass(void);
	// getters
	std::uint32_t GetCartridgeSize(void) const;
	const std::string & GetIdentifier(void) const;
	const std::string & GetName(void) const;
	const std::string & GetSlotClassIdentifier(void) const;
	std::uint32_t GetSpaceRequirement(void) const;
	const VisualizationPrototype * GetVisualizationPrototype(void) const;
	// setters
	void SetCartridgeSize(std::uint32_t CartridgeSize);
	void SetName(const std::string & Name);
	void SetSpaceRequirement(std::uint32_t SpaceRequirement);
	void SetVisualizationPrototype(const VisualizationPrototype & VisualizationPrototype);
private:
	std::uint32_t _CartridgeSize;
	std::string _Identifier;
	std::string _Name;
	std::uint32_t _SpaceRequirement;
	VisualizationPrototype * _VisualizationPrototype;
};

inline std::uint32_t AmmunitionClass::GetCartridgeSize(void) const
{
	return _CartridgeSize;
}

inline const std::string & AmmunitionClass::GetIdentifier(void) const
{
	return _Identifier;
}

inline const std::string & AmmunitionClass::GetName(void) const
{
	return _Name;
}

inline std::uint32_t AmmunitionClass::GetSpaceRequirement(void) const
{
	return _SpaceRequirement;
}

inline const VisualizationPrototype * AmmunitionClass::GetVisualizationPrototype(void) const
{
	return _VisualizationPrototype;
}

inline void AmmunitionClass::SetCartridgeSize(std::uint32_t CartridgeSize)
{
	_CartridgeSize = CartridgeSize;
}

inline void AmmunitionClass::SetName(const std::string & Name)
{
	_Name = Name;
}

inline void AmmunitionClass::SetSpaceRequirement(std::uint32_t SpaceRequirement)
{
	_SpaceRequirement = SpaceRequirement;
}

#endif
