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

#include "math/quaternion.h"
#include "physical_object.h"

class Ship;
class Slot;
class WeaponClass;

class Weapon : public PhysicalObject
{
public:
	Weapon(const WeaponClass * WeaponClass);
	void Update(float Seconds);
	// getters
	const Quaternion & GetOrientation(void) const;
	Slot * GetSlot(void);
	const Slot * GetSlot(void) const;
	const WeaponClass * GetWeaponClass(void) const;
	// setters
	void SetFire(bool Fire);
	void SetOrientation(const Quaternion & Orientation);
	void SetSlot(Slot * Slot);
private:
	const WeaponClass * m_WeaponClass;
	Quaternion m_Orientation;
	Slot * m_Slot;
	bool m_Fire;
	double m_NextTimeToFire;
};

inline const Quaternion & Weapon::GetOrientation(void) const
{
	return m_Orientation;
}

inline Slot * Weapon::GetSlot(void)
{
	return m_Slot;
}

inline const Slot * Weapon::GetSlot(void) const
{
	return m_Slot;
}

inline const WeaponClass * Weapon::GetWeaponClass(void) const
{
	return m_WeaponClass;
}

inline void Weapon::SetFire(bool Fire)
{
	m_Fire = Fire;
}

inline void Weapon::SetOrientation(const Quaternion & Orientation)
{
	m_Orientation = Orientation;
}

inline void Weapon::SetSlot(Slot * Slot)
{
	m_Slot = Slot;
}

#endif
