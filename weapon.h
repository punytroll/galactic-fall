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

class Ship;
class WeaponClass;

class Weapon : public Object
{
public:
	Weapon(WeaponClass * WeaponClass);
	void Update(float Seconds);
	// getters
	const WeaponClass * GetWeaponClass(void) const;
	Ship * GetShip(void);
	// setters
	void SetFire(bool Fire);
	void SetPosition(const math3d::vector2f & Position);
	void SetShip(Ship * Ship);
private:
	WeaponClass * m_WeaponClass;
	Ship * m_Ship;
	bool m_Fire;
	double m_NextTimeToFire;
	math3d::vector2f m_Position;
};

inline const WeaponClass * Weapon::GetWeaponClass(void) const
{
	return m_WeaponClass;
}

inline Ship * Weapon::GetShip(void)
{
	return m_Ship;
}

inline void Weapon::SetFire(bool Fire)
{
	m_Fire = Fire;
}

inline void Weapon::SetPosition(const math3d::vector2f & Position)
{
	m_Position = Position;
}

inline void Weapon::SetShip(Ship * Ship)
{
	m_Ship = Ship;
}

#endif
