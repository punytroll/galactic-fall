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

#ifndef PLANET_H
#define PLANET_H

#include <string>
#include <vector>

#include "object.h"
#include "type_definitions.h"

class AssetClass;
class Color;
class Faction;
class Ship;

class PlanetAssetClass
{
public:
	// constructor & destructor
	PlanetAssetClass(const AssetClass * AssetClass);
	virtual ~PlanetAssetClass(void);
	// getters
	const AssetClass * GetAssetClass(void) const;
	float GetBasePriceModifier(void) const;
	unsigned_numeric GetPrice(void) const;
	// setters
	void SetBasePriceModifier(float BasePriceModifier);
private:
	const AssetClass * m_AssetClass;
	float m_BasePriceModifier;
};

inline const AssetClass * PlanetAssetClass::GetAssetClass(void) const
{
	return m_AssetClass;
}

inline float PlanetAssetClass::GetBasePriceModifier(void) const
{
	return m_BasePriceModifier;
}

inline void PlanetAssetClass::SetBasePriceModifier(float BasePriceModifier)
{
	m_BasePriceModifier = BasePriceModifier;
}

class Planet : public Object
{
public:
	// constructor & destructor
	Planet(const std::string & Identifier);
	~Planet(void);
	// getters
	const Color & GetColor(void) const;
	const std::string & GetDescription(void) const;
	Reference< Faction > GetFaction(void) const;
	const std::string & GetIdentifier(void) const;
	float GetLandingFeePerSpace(void) const;
	const std::vector< PlanetAssetClass * > & GetPlanetAssetClasses(void) const;
	float GetSize(void) const;
	// setters
	void SetColor(const Color & Color);
	void SetDescription(const std::string & Description);
	void SetFaction(Reference< Faction > Faction);
	void SetLandingFeePerSpace(float LandingFeePerSpace);
	void SetSize(const float & Size);
	// modifiers
	PlanetAssetClass * CreatePlanetAssetClass(const AssetClass * AssetClass);
	void Land(Ship * Ship);
	void TakeOff(Ship * Ship);
private:
	Color * m_Color;
	std::string m_Description;
	Reference< Faction > m_Faction;
	std::string m_Identifier;
	float m_LandingFeePerSpace;
	std::vector< PlanetAssetClass * > m_PlanetAssetClasses;
	float m_Size;
};

inline const Color & Planet::GetColor(void) const
{
	return *m_Color;
}

inline const std::string & Planet::GetDescription(void) const
{
	return m_Description;
}

inline Reference< Faction > Planet::GetFaction(void) const
{
	return m_Faction;
}

inline const std::string & Planet::GetIdentifier(void) const
{
	return m_Identifier;
}

inline float Planet::GetLandingFeePerSpace(void) const
{
	return m_LandingFeePerSpace;
}

inline const std::vector< PlanetAssetClass * > & Planet::GetPlanetAssetClasses(void) const
{
	return m_PlanetAssetClasses;
}

inline float Planet::GetSize(void) const
{
	return m_Size;
}

inline void Planet::SetFaction(Reference< Faction > Faction)
{
	m_Faction = Faction;
}

inline void Planet::SetLandingFeePerSpace(float LandingFeePerSpace)
{
	m_LandingFeePerSpace = LandingFeePerSpace;
}

#endif
