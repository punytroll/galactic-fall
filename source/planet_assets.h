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

#ifndef PLANET_ASSETS_H
#define PLANET_ASSETS_H

#include <string>

class VisualizationPrototype;

class PlanetAssets
{
public:
	// constructor & destructor
	PlanetAssets(const std::string & TypeIdentifier, const std::string & SubTypeIdentifier);
	~PlanetAssets(void);
	// getters
	float GetBasePriceModifier(void) const;
	const std::string & GetDescription(void) const;
	const std::string & GetName(void) const;
	std::uint32_t GetPrice(void) const;
	std::uint32_t GetSpaceRequirement(void) const;
	const std::string & GetSubTypeIdentifier(void) const;
	const std::string & GetTypeIdentifier(void) const;
	const VisualizationPrototype * GetVisualizationPrototype(void) const;
	// setters
	void SetBasePriceModifier(float BasePriceModifier);
private:
	std::uint32_t _BasePrice;
	float _BasePriceModifier;
	std::string _SubTypeIdentifier;
	std::string _TypeIdentifier;
};

inline float PlanetAssets::GetBasePriceModifier(void) const
{
	return _BasePriceModifier;
}

inline void PlanetAssets::SetBasePriceModifier(float BasePriceModifier)
{
	_BasePriceModifier = BasePriceModifier;
}

inline const std::string & PlanetAssets::GetSubTypeIdentifier(void) const
{
	return _SubTypeIdentifier;
}

inline const std::string & PlanetAssets::GetTypeIdentifier(void) const
{
	return _TypeIdentifier;
}

#endif
