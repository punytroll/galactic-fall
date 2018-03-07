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

#ifndef TURRET_CLASS_H
#define TURRET_CLASS_H

#include <string>

#include <algebra/quaternion.h>
#include <algebra/vector3f.h>

class VisualizationPrototype;

namespace Graphics
{
	class Model;
}

class TurretClass
{
public:
	// constructor
	TurretClass(const std::string & Identifier);
	// destructor
	~TurretClass(void);
	// getters
	float GetEnergyUsagePerShot(void) const;
	const std::string & GetIdentifier(void) const;
	const Vector3f & GetMuzzlePosition(void) const;
	const std::string & GetName(void) const;
	const Quaternion & GetOrientation(void) const;
	float GetReloadTime(void) const;
	float GetShotExitSpeed(void) const;
	float GetShotDamage(void) const;
	float GetShotLifeTime(void) const;
	const VisualizationPrototype * GetShotVisualizationPrototype(void) const;
	const std::string & GetSlotClassIdentifier(void) const;
	std::uint32_t GetSpaceRequirement(void) const;
	const VisualizationPrototype * GetTurretVisualizationPrototype(void) const;
	// setters
	void SetEnergyUsagePerShot(float EnergyUsagePerShot);
	void SetMuzzlePosition(const Vector3f & MuzzlePosition);
	void SetName(const std::string & Name);
	void SetOrientation(const Quaternion & Orientation);
	void SetReloadTime(float ReloadTime);
	void SetShotExitSpeed(float ShotExitSpeed);
	void SetShotDamage(float ShotDamage);
	void SetShotLifeTime(float ShotLifeTime);
	void SetShotVisualizationPrototype(const VisualizationPrototype & ShotVisualizationPrototype);
	void SetSlotClassIdentifier(const std::string & SlotClassIdentifier);
	void SetSpaceRequirement(std::uint32_t SpaceRequirement);
	void SetTurretVisualizationPrototype(const VisualizationPrototype & TurretVisualizationPrototype);
private:
	float _EnergyUsagePerShot;
	std::string _Identifier;
	Vector3f _MuzzlePosition;
	std::string _Name;
	Quaternion _Orientation;
	float _ReloadTime;
	float _ShotDamage;
	float _ShotExitSpeed;
	float _ShotLifeTime;
	VisualizationPrototype * _ShotVisualizationPrototype;
	std::string _SlotClassIdentifier;
	std::uint32_t _SpaceRequirement;
	VisualizationPrototype * _TurretVisualizationPrototype;
};

inline float TurretClass::GetEnergyUsagePerShot(void) const
{
	return _EnergyUsagePerShot;
}

inline const std::string & TurretClass::GetIdentifier(void) const
{
	return _Identifier;
}

inline const Vector3f & TurretClass::GetMuzzlePosition(void) const
{
	return _MuzzlePosition;
}

inline const std::string & TurretClass::GetName(void) const
{
	return _Name;
}

inline const Quaternion & TurretClass::GetOrientation(void) const
{
	return _Orientation;
}

inline float TurretClass::GetReloadTime(void) const
{
	return _ReloadTime;
}

inline float TurretClass::GetShotDamage(void) const
{
	return _ShotDamage;
}

inline float TurretClass::GetShotExitSpeed(void) const
{
	return _ShotExitSpeed;
}

inline float TurretClass::GetShotLifeTime(void) const
{
	return _ShotLifeTime;
}

inline const VisualizationPrototype * TurretClass::GetShotVisualizationPrototype(void) const
{
	return _ShotVisualizationPrototype;
}

inline const std::string & TurretClass::GetSlotClassIdentifier(void) const
{
	return _SlotClassIdentifier;
}

inline std::uint32_t TurretClass::GetSpaceRequirement(void) const
{
	return _SpaceRequirement;
}

inline const VisualizationPrototype * TurretClass::GetTurretVisualizationPrototype(void) const
{
	return _TurretVisualizationPrototype;
}

inline void TurretClass::SetEnergyUsagePerShot(float EnergyUsagePerShot)
{
	_EnergyUsagePerShot = EnergyUsagePerShot;
}

inline void TurretClass::SetMuzzlePosition(const Vector3f & MuzzlePosition)
{
	_MuzzlePosition = MuzzlePosition;
}

inline void TurretClass::SetName(const std::string & Name)
{
	_Name = Name;
}

inline void TurretClass::SetOrientation(const Quaternion & Orientation)
{
	_Orientation = Orientation;
}

inline void TurretClass::SetReloadTime(float ReloadTime)
{
	_ReloadTime = ReloadTime;
}

inline void TurretClass::SetShotExitSpeed(float ShotExitSpeed)
{
	_ShotExitSpeed = ShotExitSpeed;
}

inline void TurretClass::SetShotDamage(float ShotDamage)
{
	_ShotDamage = ShotDamage;
}

inline void TurretClass::SetShotLifeTime(float ShotLifeTime)
{
	_ShotLifeTime = ShotLifeTime;
}

inline void TurretClass::SetSlotClassIdentifier(const std::string & SlotClassIdentifier)
{
	_SlotClassIdentifier = SlotClassIdentifier;
}

inline void TurretClass::SetSpaceRequirement(std::uint32_t SpaceRequirement)
{
	_SpaceRequirement = SpaceRequirement;
}

#endif
