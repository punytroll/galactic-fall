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

#ifndef SHOT_H
#define SHOT_H

#include "math/vector3f.h"
#include "physical_object.h"

class Model;
class WeaponClass;

class Shot : public PhysicalObject
{
public:
	Shot(const WeaponClass * WeaponClass);
	virtual ~Shot(void);
	// getters
	float GetDamage(void) const;
	const Reference< PhysicalObject > & GetShooter(void) const;
	const Vector3f & GetVelocity(void) const;
	const WeaponClass * GetWeaponClass(void) const;
	// setters
	void SetShooter(Reference< PhysicalObject > Shooter);
	void SetVelocity(const Vector3f & Velocity);
private:
	// slot for the object aspect Update
	bool Update(float Seconds);
private:
	const WeaponClass * m_WeaponClass;
	Reference< PhysicalObject > m_Shooter;
	double m_TimeOfDeath;
	Vector3f m_Velocity;
	float m_Damage;
};

inline float Shot::GetDamage(void) const
{
	return m_Damage;
}

inline const Reference< PhysicalObject > & Shot::GetShooter(void) const
{
	return m_Shooter;
}

inline const Vector3f & Shot::GetVelocity(void) const
{
	return m_Velocity;
}

inline const WeaponClass * Shot::GetWeaponClass(void) const
{
	return m_WeaponClass;
}

inline void Shot::SetShooter(Reference< PhysicalObject > Shooter)
{
	m_Shooter = Shooter;
}

inline void Shot::SetVelocity(const Vector3f & Velocity)
{
	m_Velocity = Velocity;
}

#endif

