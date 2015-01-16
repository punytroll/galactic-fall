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

#ifndef OBJECT_FACTORY_H
#define OBJECT_FACTORY_H

#include <string>

template< class Class >
class ClassManager;

class BatteryClass;
class CommodityClass;
class GeneratorClass;
class Object;
class ShipClass;
class TurretClass;
class VisualizationPrototype;
class WeaponClass;

class ObjectFactory
{
public:
	// constructor & destructor
	ObjectFactory(void);
	~ObjectFactory(void);
	// getters
	Object * Create(const std::string & TypeIdentifier, const std::string & ClassIdentifier, bool CreateNestedObjects) const;
	unsigned_numeric GetSpaceRequirement(const std::string & TypeIdentifier, const std::string & ClassIdentifier) const;
	const VisualizationPrototype * GetVisualizationPrototype(const std::string & TypeIdentifier, const std::string & ClassIdentifier) const;
	ClassManager< BatteryClass > * GetBatteryClassManager(void);
	ClassManager< CommodityClass > * GetCommodityClassManager(void);
	ClassManager< GeneratorClass > * GetGeneratorClassManager(void);
	ClassManager< ShipClass > * GetShipClassManager(void);
	ClassManager< TurretClass > * GetTurretClassManager(void);
	ClassManager< WeaponClass > * GetWeaponClassManager(void);
private:
	ClassManager< BatteryClass > * _BatteryClassManager;
	ClassManager< CommodityClass > * _CommodityClassManager;
	ClassManager< GeneratorClass > * _GeneratorClassManager;
	ClassManager< ShipClass > * _ShipClassManager;
	ClassManager< TurretClass > * _TurretClassManager;
	ClassManager< WeaponClass > * _WeaponClassManager;
};

inline ClassManager< BatteryClass > * ObjectFactory::GetBatteryClassManager(void)
{
	return _BatteryClassManager;
}

inline ClassManager< CommodityClass > * ObjectFactory::GetCommodityClassManager(void)
{
	return _CommodityClassManager;
}

inline ClassManager< GeneratorClass > * ObjectFactory::GetGeneratorClassManager(void)
{
	return _GeneratorClassManager;
}

inline ClassManager< ShipClass > * ObjectFactory::GetShipClassManager(void)
{
	return _ShipClassManager;
}

inline ClassManager< TurretClass > * ObjectFactory::GetTurretClassManager(void)
{
	return _TurretClassManager;
}

inline ClassManager< WeaponClass > * ObjectFactory::GetWeaponClassManager(void)
{
	return _WeaponClassManager;
}

#endif
