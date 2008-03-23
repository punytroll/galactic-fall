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
class Ship;

class PlanetAssetClass
{
public:
	PlanetAssetClass(const AssetClass * AssetClass);
	virtual ~PlanetAssetClass(void);
	const AssetClass * GetAssetClass(void) const;
	unsigned_numeric GetPrice(void) const;
	void SetBasePriceModifier(float BasePriceModifier);
	float GetBasePriceModifier(void) const;
private:
	const AssetClass * m_AssetClass;
	float m_BasePriceModifier;
};

inline void PlanetAssetClass::SetBasePriceModifier(float BasePriceModifier)
{
	m_BasePriceModifier = BasePriceModifier;
}

inline float PlanetAssetClass::GetBasePriceModifier(void) const
{
	return m_BasePriceModifier;
}

class Planet : public Object
{
public:
	Planet(const std::string & Identifier);
	~Planet(void);
	// getters
	const std::string & GetIdentifier(void) const;
	const Color & GetColor(void) const;
	const std::string & GetDescription(void) const;
	float GetSize(void) const;
	const std::vector< PlanetAssetClass * > & GetPlanetAssetClasses(void) const;
	unsigned_numeric GetLandingFee(void) const;
	// setters
	void SetLandingFee(unsigned_numeric LandingFee);
	void SetDescription(const std::string & Description);
	void SetSize(const float & Size);
	void SetColor(const Color & Color);
	// modifiers
	PlanetAssetClass * CreatePlanetAssetClass(const AssetClass * AssetClass);
	void Land(Ship * Ship);
	void TakeOff(Ship * Ship);
private:
	std::string m_Identifier;
	std::string m_Description;
	float m_Size;
	unsigned_numeric m_LandingFee;
	Color * m_Color;
	std::vector< PlanetAssetClass * > m_PlanetAssetClasses;
};

inline const AssetClass * PlanetAssetClass::GetAssetClass(void) const
{
	return m_AssetClass;
}

inline const std::string & Planet::GetIdentifier(void) const
{
	return m_Identifier;
}

inline const Color & Planet::GetColor(void) const
{
	return *m_Color;
}

inline const std::string & Planet::GetDescription(void) const
{
	return m_Description;
}

inline float Planet::GetSize(void) const
{
	return m_Size;
}

inline const std::vector< PlanetAssetClass * > & Planet::GetPlanetAssetClasses(void) const
{
	return m_PlanetAssetClasses;
}

inline unsigned_numeric Planet::GetLandingFee(void) const
{
	return m_LandingFee;
}

inline void Planet::SetLandingFee(unsigned_numeric LandingFee)
{
	m_LandingFee = LandingFee;
}

#endif
