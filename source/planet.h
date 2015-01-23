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
class Character;
class Faction;
class Hangar;
class Ship;
class Visualization;

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
	const AssetClass * _AssetClass;
	float _BasePriceModifier;
};

inline const AssetClass * PlanetAssetClass::GetAssetClass(void) const
{
	return _AssetClass;
}

inline float PlanetAssetClass::GetBasePriceModifier(void) const
{
	return _BasePriceModifier;
}

inline void PlanetAssetClass::SetBasePriceModifier(float BasePriceModifier)
{
	_BasePriceModifier = BasePriceModifier;
}

class Planet : public Object
{
public:
	// constructor & destructor
	Planet(void);
	virtual ~Planet(void) override;
	// getters
	const std::string & GetDescription(void) const;
	Reference< Faction > GetFaction(void) const;
	Hangar * GetHangar(Character * Character);
	float GetLandingFeePerSpace(void) const;
	bool GetOffersRecharging(void) const;
	bool GetOffersRepairing(void) const;
	const std::vector< PlanetAssetClass * > & GetPlanetAssetClasses(void) const;
	float GetSize(void) const;
	// setters
	void SetDescription(const std::string & Description);
	void SetFaction(Reference< Faction > Faction);
	void SetLandingFeePerSpace(float LandingFeePerSpace);
	void SetOffersRecharging(bool OffersRecharging);
	void SetOffersRepairing(bool OffersRepairing);
	void SetRechargingFeePerEnergy(float RechargingFeePerEnergy);
	void SetRepairingFeePerHull(float RepairingFeePerHull);
	void SetSize(const float & Size);
	// modifiers
	PlanetAssetClass * CreatePlanetAssetClass(const AssetClass * AssetClass);
	void Land(Ship * Ship, Character * Character);
	void Recharge(Ship * Ship, Character * Character);
	void Refuel(Ship * Ship, Character * Character);
	void Repair(Ship * Ship, Character * Character);
	void TakeOff(Ship * Ship, Character * Character);
private:
	// helper functions
	Hangar * _CreateHangar(Character * Character);
	void _UpdateVisualization(Visualization * Visualization);
	// member variables
	std::string _Description;
	Reference< Faction > _Faction;
	float _LandingFeePerSpace;
	bool _OffersRecharging;
	bool _OffersRepairing;
	std::vector< PlanetAssetClass * > _PlanetAssetClasses;
	float _RechargingFeePerEnergy;
	float _RepairingFeePerHull;
	float _Size;
};

inline const std::string & Planet::GetDescription(void) const
{
	return _Description;
}

inline Reference< Faction > Planet::GetFaction(void) const
{
	return _Faction;
}

inline float Planet::GetLandingFeePerSpace(void) const
{
	return _LandingFeePerSpace;
}

inline bool Planet::GetOffersRecharging(void) const
{
	return _OffersRecharging;
}

inline bool Planet::GetOffersRepairing(void) const
{
	return _OffersRepairing;
}

inline const std::vector< PlanetAssetClass * > & Planet::GetPlanetAssetClasses(void) const
{
	return _PlanetAssetClasses;
}

inline float Planet::GetSize(void) const
{
	return _Size;
}

inline void Planet::SetFaction(Reference< Faction > Faction)
{
	_Faction = Faction;
}

inline void Planet::SetLandingFeePerSpace(float LandingFeePerSpace)
{
	_LandingFeePerSpace = LandingFeePerSpace;
}

inline void Planet::SetOffersRecharging(bool OffersRecharging)
{
	_OffersRecharging = OffersRecharging;
}

inline void Planet::SetOffersRepairing(bool OffersRepairing)
{
	_OffersRepairing = OffersRepairing;
}

inline void Planet::SetRechargingFeePerEnergy(float RechargingFeePerEnergy)
{
	_RechargingFeePerEnergy = RechargingFeePerEnergy;
}

inline void Planet::SetRepairingFeePerHull(float RepairingFeePerHull)
{
	_RepairingFeePerHull = RepairingFeePerHull;
}

#endif
