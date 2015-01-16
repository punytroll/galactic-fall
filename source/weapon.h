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

class Visualization;
class VisualizationPrototype;
class WeaponClass;

class Weapon : public Object
{
public:
	Weapon(void);
	virtual ~Weapon(void) override;
	// getters
	float GetEnergyUsagePerShot(void) const;
	float GetReloadTime(void) const;
	float GetShotDamage(void) const;
	const Vector3f & GetShotExitPosition(void) const;
	float GetShotExitSpeed(void) const;
	float GetShotLifeTime(void) const;
	const VisualizationPrototype * GetShotVisualizationPrototype(void) const;
	// setters
	void SetEnergyUsagePerShot(float EnergyUsagePerShot);
	void SetFire(bool Fire);
	void SetReloadTime(float ReloadTime);
	void SetShotDamage(float ShotDamage);
	void SetShotExitPosition(const Vector3f & ShotExitPosition);
	void SetShotExitSpeed(float ShotExitSpeed);
	void SetShotLifeTime(float ShotLifeTime);
	void SetShotVisualizationPrototype(const VisualizationPrototype * ShotVisualizationPrototype);
private:
	// modifiers
	bool _Update(float Seconds);
	void _UpdateVisualization(Visualization * Visualization);
	// variables
	float _EnergyUsagePerShot;
	bool _Fire;
	double _NextTimeToFire;
	float _ReloadTime;
	float _ShotDamage;
	Vector3f _ShotExitPosition;
	float _ShotExitSpeed;
	float _ShotLifeTime;
	VisualizationPrototype * _ShotVisualizationPrototype;
};

inline float Weapon::GetEnergyUsagePerShot(void) const
{
	return _EnergyUsagePerShot;
}

inline float Weapon::GetReloadTime(void) const
{
	return _ReloadTime;
}

inline float Weapon::GetShotDamage(void) const
{
	return _ShotDamage;
}

inline const Vector3f & Weapon::GetShotExitPosition(void) const
{
	return _ShotExitPosition;
}

inline float Weapon::GetShotExitSpeed(void) const
{
	return _ShotExitSpeed;
}

inline float Weapon::GetShotLifeTime(void) const
{
	return _ShotLifeTime;
}

inline const VisualizationPrototype * Weapon::GetShotVisualizationPrototype(void) const
{
	return _ShotVisualizationPrototype;
}

inline void Weapon::SetEnergyUsagePerShot(float EnergyUsagePerShot)
{
	_EnergyUsagePerShot = EnergyUsagePerShot;
}

inline void Weapon::SetFire(bool Fire)
{
	_Fire = Fire;
}

inline void Weapon::SetReloadTime(float ReloadTime)
{
	_ReloadTime = ReloadTime;
}

inline void Weapon::SetShotDamage(float ShotDamage)
{
	_ShotDamage = ShotDamage;
}

inline void Weapon::SetShotExitPosition(const Vector3f & ShotExitPosition)
{
	_ShotExitPosition = ShotExitPosition;
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
