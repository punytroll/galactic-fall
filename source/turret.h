/**
 * galactic-fall
 * Copyright (C) 2015  Hagen Möbius
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

#ifndef TURRET_H
#define TURRET_H

#include "object.h"

class Visualization;
class TurretClass;

class Turret : public Object
{
public:
	Turret(void);
	virtual ~Turret(void) override;
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
	void _CalculateCurrentGunProperties(Vector3f & CurrentGunExitPosition, Quaternion & CurrentGunOrientation, Vector3f & CurrentGunDirection);
	// variables
	float _EnergyUsagePerShot;
	bool _Fire;
	Quaternion _GunOrientation;
	Vector3f _GunPosition;
	bool _GunPropertiesValid;
	double _NextTimeToFire;
	float _ReloadTime;
	float _ShotDamage;
	Vector3f _ShotExitPosition;
	float _ShotExitSpeed;
	float _ShotLifeTime;
	VisualizationPrototype * _ShotVisualizationPrototype;
	float _TurretAngle;
};

inline void Turret::SetEnergyUsagePerShot(float EnergyUsagePerShot)
{
	_EnergyUsagePerShot = EnergyUsagePerShot;
}

inline void Turret::SetFire(bool Fire)
{
	_Fire = Fire;
}

inline void Turret::SetReloadTime(float ReloadTime)
{
	_ReloadTime = ReloadTime;
}

inline void Turret::SetShotDamage(float ShotDamage)
{
	_ShotDamage = ShotDamage;
}

inline void Turret::SetShotExitPosition(const Vector3f & ShotExitPosition)
{
	_ShotExitPosition = ShotExitPosition;
}

inline void Turret::SetShotExitSpeed(float ShotExitSpeed)
{
	_ShotExitSpeed = ShotExitSpeed;
}

inline void Turret::SetShotLifeTime(float ShotLifeTime)
{
	_ShotLifeTime = ShotLifeTime;
}

#endif
