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

#include "physical_object.h"

class Color;
class Commodity;

class PlanetCommodity
{
public:
	PlanetCommodity(Commodity * Commodity);
	virtual ~PlanetCommodity(void);
	const Commodity * GetCommodity(void) const;
	float GetPrice(void) const;
	void SetBasePriceModifier(float BasePriceModifier);
	float GetBasePriceModifier(void) const;
private:
	Commodity * m_Commodity;
	float m_BasePriceModifier;
};

inline void PlanetCommodity::SetBasePriceModifier(float BasePriceModifier)
{
	m_BasePriceModifier = BasePriceModifier;
}

inline float PlanetCommodity::GetBasePriceModifier(void) const
{
	return m_BasePriceModifier;
}

class Planet : public PhysicalObject
{
public:
	Planet(const std::string & Identifier);
	~Planet(void);
	// getters
	const std::string & GetIdentifier(void) const;
	const std::string & GetDescription(void) const;
	float GetSize(void) const;
	const std::vector< PlanetCommodity * > & GetCommodities(void) const;
	bool GetAllowRefuelling(void) const;
	float GetFuelPrice(void) const;
	float GetLandingFee(void) const;
	// setters
	void SetAllowRefuelling(bool AllowRefuelling);
	void SetFuelPrice(float FuelPrice);
	void SetLandingFee(float LandingFee);
	void SetDescription(const std::string & Description);
	void SetSize(const float & Size);
	void SetColor(const Color & Color);
	virtual void Draw(void) const;
	PlanetCommodity * CreateCommodity(Commodity * Commodity);
private:
	std::string m_Identifier;
	std::string m_Description;
	float m_Size;
	bool m_AllowRefuelling;
	float m_FuelPrice;
	float m_LandingFee;
	Color * m_Color;
	std::vector< PlanetCommodity * > m_Commodities;
};

inline const Commodity * PlanetCommodity::GetCommodity(void) const
{
	return m_Commodity;
}

inline const std::string & Planet::GetIdentifier(void) const
{
	return m_Identifier;
}

inline const std::string & Planet::GetDescription(void) const
{
	return m_Description;
}

inline float Planet::GetSize(void) const
{
	return m_Size;
}

inline const std::vector< PlanetCommodity * > & Planet::GetCommodities(void) const
{
	return m_Commodities;
}

inline bool Planet::GetAllowRefuelling(void) const
{
	return m_AllowRefuelling;
}

inline float Planet::GetFuelPrice(void) const
{
	return m_FuelPrice;
}

inline float Planet::GetLandingFee(void) const
{
	return m_LandingFee;
}

inline void Planet::SetFuelPrice(float FuelPrice)
{
	m_FuelPrice = FuelPrice;
}

inline void Planet::SetLandingFee(float LandingFee)
{
	m_LandingFee = LandingFee;
}

#endif
