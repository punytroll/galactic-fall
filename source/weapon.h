/**
 * galactic-fall
 * Copyright (C) 2019  Hagen Möbius
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
#include "physics/energy/device.h"

class Visualization;
class VisualizationPrototype;

class Weapon : public Object, public Physics::Energy::Device
{
public:
	Weapon(void);
	virtual ~Weapon(void) override;
	// setters
	void SetFire(bool Fire);
	void SetMuzzlePosition(const Vector3f & MuzzlePosition);
	void SetShotDamage(float ShotDamage);
	void SetShotExitSpeed(float ShotExitSpeed);
	void SetShotLifeTime(float ShotLifeTime);
	void SetShotVisualizationPrototype(const VisualizationPrototype & ShotVisualizationPrototype);
protected:
	// variables
	bool _Fire;
	Vector3f _MuzzlePosition;
	float _ShotDamage;
	float _ShotExitSpeed;
	float _ShotLifeTime;
	VisualizationPrototype * _ShotVisualizationPrototype;
};

inline void Weapon::SetFire(bool Fire)
{
	_Fire = Fire;
}

inline void Weapon::SetMuzzlePosition(const Vector3f & MuzzlePosition)
{
	_MuzzlePosition = MuzzlePosition;
}

inline void Weapon::SetShotDamage(float ShotDamage)
{
	_ShotDamage = ShotDamage;
}

inline void Weapon::SetShotExitSpeed(float ShotExitSpeed)
{
	_ShotExitSpeed = ShotExitSpeed;
}

inline void Weapon::SetShotLifeTime(float ShotLifeTime)
{
	_ShotLifeTime = ShotLifeTime;
}

#endif
