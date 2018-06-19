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

#ifndef OBJECT_FACTORY_H
#define OBJECT_FACTORY_H

#include <string>

template< class Class >
class ClassManager;

class Blueprint;
class Object;
class VisualizationPrototype;

class ObjectFactory
{
public:
	// constructor & destructor
	ObjectFactory(void);
	~ObjectFactory(void);
	// getters
	Object * Create(const std::string & TypeIdentifier, const std::string & SubTypeIdentifier, bool CreateNestedObjects) const;
	std::uint32_t GetBasePrice(const std::string & TypeIdentifier, const std::string & SubTypeIdentifier) const;
	const std::string & GetDescription(const std::string & TypeIdentifier, const std::string & SubTypeIdentifier) const;
	const std::string & GetName(const std::string & TypeIdentifier, const std::string & SubTypeIdentifier) const;
	std::uint32_t GetSpaceRequirement(const std::string & TypeIdentifier, const std::string & SubTypeIdentifier) const;
	const VisualizationPrototype * GetVisualizationPrototype(const std::string & TypeIdentifier, const std::string & SubTypeIdentifier) const;
	ClassManager< Blueprint > * GetAmmunitionClassManager(void);
	ClassManager< Blueprint > * GetBatteryClassManager(void);
	ClassManager< Blueprint > * GetCommodityClassManager(void);
	ClassManager< Blueprint > * GetGeneratorClassManager(void);
	ClassManager< Blueprint > * GetShipClassManager(void);
	ClassManager< Blueprint > * GetTurretClassManager(void);
	ClassManager< Blueprint > * GetWeaponClassManager(void);
	bool HasClass(const std::string & TypeIdentifier, const std::string & SubTypeIdentifier) const;
private:
	ClassManager< Blueprint > * _AmmunitionClassManager;
	ClassManager< Blueprint > * _BatteryClassManager;
	ClassManager< Blueprint > * _CommodityClassManager;
	ClassManager< Blueprint > * _GeneratorClassManager;
	ClassManager< Blueprint > * _ShipClassManager;
	ClassManager< Blueprint > * _TurretClassManager;
	ClassManager< Blueprint > * _WeaponClassManager;
};

inline ClassManager< Blueprint > * ObjectFactory::GetAmmunitionClassManager(void)
{
	return _AmmunitionClassManager;
}

inline ClassManager< Blueprint > * ObjectFactory::GetBatteryClassManager(void)
{
	return _BatteryClassManager;
}

inline ClassManager< Blueprint > * ObjectFactory::GetCommodityClassManager(void)
{
	return _CommodityClassManager;
}

inline ClassManager< Blueprint > * ObjectFactory::GetGeneratorClassManager(void)
{
	return _GeneratorClassManager;
}

inline ClassManager< Blueprint > * ObjectFactory::GetShipClassManager(void)
{
	return _ShipClassManager;
}

inline ClassManager< Blueprint > * ObjectFactory::GetTurretClassManager(void)
{
	return _TurretClassManager;
}

inline ClassManager< Blueprint > * ObjectFactory::GetWeaponClassManager(void)
{
	return _WeaponClassManager;
}

#endif
