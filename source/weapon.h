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

#ifndef WEAPON_H
#define WEAPON_H

#include "object.h"

class WeaponClass;

class Weapon : public Object
{
public:
	Weapon(const WeaponClass * WeaponClass);
	void Update(float Seconds);
	// getters
	float GetEnergyUsagePerShot(void) const;
	const WeaponClass * GetWeaponClass(void) const;
	// setters
	void SetEnergyUsagePerShot(float EnergyUsagePerShot);
	void SetFire(bool Fire);
private:
	const WeaponClass * m_WeaponClass;
	float m_EnergyUsagePerShot;
	bool m_Fire;
	double m_NextTimeToFire;
};

inline float Weapon::GetEnergyUsagePerShot(void) const
{
	return m_EnergyUsagePerShot;
}

inline const WeaponClass * Weapon::GetWeaponClass(void) const
{
	return m_WeaponClass;
}

inline void Weapon::SetEnergyUsagePerShot(float EnergyUsagePerShot)
{
	m_EnergyUsagePerShot = EnergyUsagePerShot;
}

inline void Weapon::SetFire(bool Fire)
{
	m_Fire = Fire;
}

#endif
