/**
 * galactic-fall
 * Copyright (C) 2007-2018  Hagen Möbius
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
	const Vector3f & GetMuzzlePosition(void) const;
	const std::string & GetName(void) const;
	const Quaternion & GetOrientation(void) const;
	float GetShotExitSpeed(void) const;
	float GetShotDamage(void) const;
	float GetShotLifeTime(void) const;
	const VisualizationPrototype * GetShotVisualizationPrototype(void) const;
	float GetReloadTime(void) const;
	const std::string & GetSlotClassIdentifier(void) const;
	std::uint32_t GetSpaceRequirement(void) const;
	const VisualizationPrototype * GetWeaponVisualizationPrototype(void) const;
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
	void SetWeaponVisualizationPrototype(const VisualizationPrototype & WeaponVisualizationPrototype);
private:
	float _EnergyUsagePerShot;
	std::string _Identifier;
	Vector3f _MuzzlePosition;
	std::string _Name;
	Quaternion _Orientation;
	float _ReloadTime;
	float _ShotExitSpeed;
	float _ShotDamage;
	float _ShotLifeTime;
	VisualizationPrototype * _ShotVisualizationPrototype;
	std::string _SlotClassIdentifier;
	std::uint32_t _SpaceRequirement;
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

inline const Vector3f & WeaponClass::GetMuzzlePosition(void) const
{
	return _MuzzlePosition;
}

inline const std::string & WeaponClass::GetName(void) const
{
	return _Name;
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

inline std::uint32_t WeaponClass::GetSpaceRequirement(void) const
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

inline void WeaponClass::SetMuzzlePosition(const Vector3f & MuzzlePosition)
{
	_MuzzlePosition = MuzzlePosition;
}

inline void WeaponClass::SetName(const std::string & Name)
{
	_Name = Name;
}

inline void WeaponClass::SetOrientation(const Quaternion & Orientation)
{
	_Orientation = Orientation;
}

inline void WeaponClass::SetReloadTime(float ReloadTime)
{
	_ReloadTime = ReloadTime;
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

inline void WeaponClass::SetSlotClassIdentifier(const std::string & SlotClassIdentifier)
{
	_SlotClassIdentifier = SlotClassIdentifier;
}

inline void WeaponClass::SetSpaceRequirement(std::uint32_t SpaceRequirement)
{
	_SpaceRequirement = SpaceRequirement;
}

#endif
