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

#ifndef WEAPON_CLASS_H
#define WEAPON_CLASS_H

#include <string>

#include <algebra/quaternion.h>
#include <algebra/vector3f.h>

#include "type_definitions.h"

class Color;
class VisualizationPrototype;

namespace Graphics
{
	class Model;
}

class WeaponClass
{
public:
	// constructor
	WeaponClass(const std::string & Identifier);
	// destructor
	~WeaponClass(void);
	// getters
	float GetEnergyUsagePerShot(void) const;
	const std::string & GetIdentifier(void) const;
	const std::string & GetName(void) const;
	const Quaternion & GetOrientation(void) const;
	const Vector3f & GetShotExitPosition(void) const;
	float GetShotExitSpeed(void) const;
	float GetShotDamage(void) const;
	float GetShotLifeTime(void) const;
	const VisualizationPrototype * GetShotVisualizationPrototype(void) const;
	float GetReloadTime(void) const;
	const std::string & GetSlotClassIdentifier(void) const;
	unsigned_numeric GetSpaceRequirement(void) const;
	const VisualizationPrototype * GetWeaponVisualizationPrototype(void) const;
	// setters
	void SetEnergyUsagePerShot(float EnergyUsagePerShot);
	void SetName(const std::string & Name);
	void SetOrientation(const Quaternion & Orientation);
	void SetReloadTime(float ReloadTime);
	void SetShotExitPosition(const Vector3f & ShotExitPosition);
	void SetShotExitSpeed(float ShotExitSpeed);
	void SetShotDamage(float ShotDamage);
	void SetShotLifeTime(float ShotLifeTime);
	void SetShotVisualizationPrototype(const VisualizationPrototype & ShotVisualizationPrototype);
	void SetSlotClassIdentifier(const std::string & SlotClassIdentifier);
	void SetSpaceRequirement(unsigned_numeric SpaceRequirement);
	void SetWeaponVisualizationPrototype(const VisualizationPrototype & WeaponVisualizationPrototype);
private:
	float _EnergyUsagePerShot;
	std::string _Identifier;
	std::string _Name;
	std::string _SlotClassIdentifier;
	Quaternion _Orientation;
	float _ReloadTime;
	Vector3f _ShotExitPosition;
	float _ShotExitSpeed;
	float _ShotDamage;
	float _ShotLifeTime;
	VisualizationPrototype * _ShotVisualizationPrototype;
	unsigned_numeric _SpaceRequirement;
	VisualizationPrototype * _WeaponVisualizationPrototype;
};

inline float WeaponClass::GetEnergyUsagePerShot(void) const
{
	return _EnergyUsagePerShot;
}

inline const std::string & WeaponClass::GetIdentifier(void) const
{
	return _Identifier;
}

inline const std::string & WeaponClass::GetName(void) const
{
	return _Name;
}

inline const Vector3f & WeaponClass::GetShotExitPosition(void) const
{
	return _ShotExitPosition;
}

inline const Quaternion & WeaponClass::GetOrientation(void) const
{
	return _Orientation;
}

inline float WeaponClass::GetShotExitSpeed(void) const
{
	return _ShotExitSpeed;
}

inline float WeaponClass::GetShotDamage(void) const
{
	return _ShotDamage;
}

inline float WeaponClass::GetShotLifeTime(void) const
{
	return _ShotLifeTime;
}

inline const VisualizationPrototype * WeaponClass::GetShotVisualizationPrototype(void) const
{
	return _ShotVisualizationPrototype;
}

inline float WeaponClass::GetReloadTime(void) const
{
	return _ReloadTime;
}

inline const std::string & WeaponClass::GetSlotClassIdentifier(void) const
{
	return _SlotClassIdentifier;
}

inline unsigned_numeric WeaponClass::GetSpaceRequirement(void) const
{
	return _SpaceRequirement;
}

inline const VisualizationPrototype * WeaponClass::GetWeaponVisualizationPrototype(void) const
{
	return _WeaponVisualizationPrototype;
}

inline void WeaponClass::SetEnergyUsagePerShot(float EnergyUsagePerShot)
{
	_EnergyUsagePerShot = EnergyUsagePerShot;
}

inline void WeaponClass::SetName(const std::string & Name)
{
	_Name = Name;
}

inline void WeaponClass::SetOrientation(const Quaternion & Orientation)
{
	_Orientation = Orientation;
}

inline void WeaponClass::SetShotExitPosition(const Vector3f & ShotExitPosition)
{
	_ShotExitPosition = ShotExitPosition;
}

inline void WeaponClass::SetShotExitSpeed(float ShotExitSpeed)
{
	_ShotExitSpeed = ShotExitSpeed;
}

inline void WeaponClass::SetShotDamage(float ShotDamage)
{
	_ShotDamage = ShotDamage;
}

inline void WeaponClass::SetShotLifeTime(float ShotLifeTime)
{
	_ShotLifeTime = ShotLifeTime;
}

inline void WeaponClass::SetReloadTime(float ReloadTime)
{
	_ReloadTime = ReloadTime;
}

inline void WeaponClass::SetSlotClassIdentifier(const std::string & SlotClassIdentifier)
{
	_SlotClassIdentifier = SlotClassIdentifier;
}

inline void WeaponClass::SetSpaceRequirement(unsigned_numeric SpaceRequirement)
{
	_SpaceRequirement = SpaceRequirement;
}

#endif
