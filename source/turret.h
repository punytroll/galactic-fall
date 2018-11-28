/**
 * galactic-fall
 * Copyright (C) 2015-2018  Hagen Möbius
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
#include "physics/energy/device.h"

class Visualization;
class TurretClass;

class Turret : public Object, public Physics::Energy::Device
{
public:
	Turret(void);
	virtual ~Turret(void) override;
	// Physics::Energy::Device implementation
	virtual float GetMaximumEnergyInput(float Seconds) const;
	virtual float GetMaximumEnergyOutput(float Seconds) const;
	virtual void EnergyDelta(float EnergyDelta);
	// getters
	float GetMaximumPowerInput(void) const;
	// setters
	void SetEnergyUsagePerShot(float EnergyUsagePerShot);
	void SetFire(bool Fire);
	void SetMaximumPowerInput(float MaximumPowerInput);
	void SetMuzzlePosition(const Vector3f & MuzzlePosition);
	void SetShotDamage(float ShotDamage);
	void SetShotExitSpeed(float ShotExitSpeed);
	void SetShotLifeTime(float ShotLifeTime);
	void SetShotVisualizationPrototype(const VisualizationPrototype & ShotVisualizationPrototype);
private:
	// modifiers
	bool _Update(float Seconds);
	void _UpdateVisualization(Visualization * Visualization);
	void _CalculateMuzzleProperties(Vector3f & MuzzlePosition, Quaternion & MuzzleOrientation, Vector3f & MuzzleDirection);
	// variables
	float _Energy;
	float _EnergyUsagePerShot;
	bool _Fire;
	Quaternion _GunOrientation;
	Vector3f _GunPosition;
	bool _GunPropertiesValid;
	float _MaximumPowerInput;
	Vector3f _MuzzlePosition;
	float _ShotDamage;
	float _ShotExitSpeed;
	float _ShotLifeTime;
	VisualizationPrototype * _ShotVisualizationPrototype;
	float _TurretAngle;
};

inline float Turret::GetMaximumPowerInput(void) const
{
	return _MaximumPowerInput;
}

inline void Turret::SetEnergyUsagePerShot(float EnergyUsagePerShot)
{
	_EnergyUsagePerShot = EnergyUsagePerShot;
}

inline void Turret::SetFire(bool Fire)
{
	_Fire = Fire;
}

inline void Turret::SetMaximumPowerInput(float MaximumPowerInput)
{
	_MaximumPowerInput = MaximumPowerInput;
}

inline void Turret::SetMuzzlePosition(const Vector3f & MuzzlePosition)
{
	_MuzzlePosition = MuzzlePosition;
}

inline void Turret::SetShotDamage(float ShotDamage)
{
	_ShotDamage = ShotDamage;
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
