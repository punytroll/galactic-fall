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

#include <algebra/vector3f.h>

#include "object.h"

class Shot : public Object
{
public:
	Shot(void);
	virtual ~Shot(void);
	// getters
	float GetDamage(void) const;
	const Reference< Object > & GetShooter(void) const;
	const Vector3f & GetVelocity(void) const;
	// setters
	void SetDamage(float Damage);
	void SetShooter(Reference< Object > Shooter);
	void SetTimeOfDeath(float TimeOfDeath);
	void SetVelocity(const Vector3f & Velocity);
private:
	// modifers
	bool _Update(float Seconds);
	// variables
	Reference< Object > _Shooter;
	double _TimeOfDeath;
	Vector3f _Velocity;
	float _Damage;
};

inline float Shot::GetDamage(void) const
{
	return _Damage;
}

inline const Reference< Object > & Shot::GetShooter(void) const
{
	return _Shooter;
}

inline const Vector3f & Shot::GetVelocity(void) const
{
	return _Velocity;
}

inline void Shot::SetDamage(float Damage)
{
	_Damage = Damage;
}

inline void Shot::SetShooter(Reference< Object > Shooter)
{
	_Shooter = Shooter;
}

inline void Shot::SetTimeOfDeath(float TimeOfDeath)
{
	_TimeOfDeath = TimeOfDeath;
}

inline void Shot::SetVelocity(const Vector3f & Velocity)
{
	_Velocity = Velocity;
}

#endif

