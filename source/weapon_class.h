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

#include "math/quaternion.h"
#include "math/vector3f.h"
#include "type_definitions.h"

class Color;

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
	const Graphics::Model * GetModel(void) const;
	const Color * GetModelColor(void) const;
	const std::string & GetName(void) const;
	const Quaternion & GetOrientation(void) const;
	const Vector3f & GetParticleExitPosition(void) const;
	float GetParticleExitSpeed(void) const;
	float GetParticleDamage(void) const;
	float GetParticleLifeTime(void) const;
	const Graphics::Model * GetParticleModel(void) const;
	const Color * GetParticleColor(void) const;
	float GetReloadTime(void) const;
	const std::string & GetSlotClassIdentifier(void) const;
	unsigned_numeric GetSpaceRequirement(void) const;
	// setters
	void SetEnergyUsagePerShot(float EnergyUsagePerShot);
	void SetModel(const Graphics::Model * Model);
	void SetModelColor(const Color & ModelColor);
	void SetName(const std::string & Name);
	void SetOrientation(const Quaternion & Orientation);
	void SetParticleExitPosition(const Vector3f & ParticleExitPosition);
	void SetParticleExitSpeed(float ParticleExitSpeed);
	void SetParticleDamage(float ParticleDamage);
	void SetParticleLifeTime(float ParticleLifeTime);
	void SetParticleModel(const Graphics::Model * ParticleModel);
	void SetParticleColor(const Color & ParticleColor);
	void SetReloadTime(float ReloadTime);
	void SetSlotClassIdentifier(const std::string & SlotClassIdentifier);
	void SetSpaceRequirement(unsigned_numeric SpaceRequirement);
private:
	float m_EnergyUsagePerShot;
	std::string m_Identifier;
	const Graphics::Model * m_Model;
	Color * m_ModelColor;
	std::string m_Name;
	std::string m_SlotClassIdentifier;
	Quaternion m_Orientation;
	float m_ReloadTime;
	Vector3f m_ParticleExitPosition;
	float m_ParticleExitSpeed;
	float m_ParticleDamage;
	float m_ParticleLifeTime;
	const Graphics::Model * m_ParticleModel;
	Color * m_ParticleColor;
	unsigned_numeric m_SpaceRequirement;
};

inline float WeaponClass::GetEnergyUsagePerShot(void) const
{
	return m_EnergyUsagePerShot;
}

inline const Color * WeaponClass::GetModelColor(void) const
{
	return m_ModelColor;
}

inline const std::string & WeaponClass::GetIdentifier(void) const
{
	return m_Identifier;
}

inline const Graphics::Model * WeaponClass::GetModel(void) const
{
	return m_Model;
}

inline const std::string & WeaponClass::GetName(void) const
{
	return m_Name;
}

inline const Vector3f & WeaponClass::GetParticleExitPosition(void) const
{
	return m_ParticleExitPosition;
}

inline const Quaternion & WeaponClass::GetOrientation(void) const
{
	return m_Orientation;
}

inline float WeaponClass::GetParticleExitSpeed(void) const
{
	return m_ParticleExitSpeed;
}

inline float WeaponClass::GetParticleDamage(void) const
{
	return m_ParticleDamage;
}

inline float WeaponClass::GetParticleLifeTime(void) const
{
	return m_ParticleLifeTime;
}

inline const Graphics::Model * WeaponClass::GetParticleModel(void) const
{
	return m_ParticleModel;
}

inline const Color * WeaponClass::GetParticleColor(void) const
{
	return m_ParticleColor;
}

inline float WeaponClass::GetReloadTime(void) const
{
	return m_ReloadTime;
}

inline const std::string & WeaponClass::GetSlotClassIdentifier(void) const
{
	return m_SlotClassIdentifier;
}

inline unsigned_numeric WeaponClass::GetSpaceRequirement(void) const
{
	return m_SpaceRequirement;
}

inline void WeaponClass::SetEnergyUsagePerShot(float EnergyUsagePerShot)
{
	m_EnergyUsagePerShot = EnergyUsagePerShot;
}

inline void WeaponClass::SetModel(const Graphics::Model * Model)
{
	m_Model = Model;
}

inline void WeaponClass::SetName(const std::string & Name)
{
	m_Name = Name;
}

inline void WeaponClass::SetOrientation(const Quaternion & Orientation)
{
	m_Orientation = Orientation;
}

inline void WeaponClass::SetParticleExitPosition(const Vector3f & ParticleExitPosition)
{
	m_ParticleExitPosition = ParticleExitPosition;
}

inline void WeaponClass::SetParticleExitSpeed(float ParticleExitSpeed)
{
	m_ParticleExitSpeed = ParticleExitSpeed;
}

inline void WeaponClass::SetParticleDamage(float ParticleDamage)
{
	m_ParticleDamage = ParticleDamage;
}

inline void WeaponClass::SetParticleLifeTime(float ParticleLifeTime)
{
	m_ParticleLifeTime = ParticleLifeTime;
}

inline void WeaponClass::SetParticleModel(const Graphics::Model * ParticleModel)
{
	m_ParticleModel = ParticleModel;
}

inline void WeaponClass::SetReloadTime(float ReloadTime)
{
	m_ReloadTime = ReloadTime;
}

inline void WeaponClass::SetSlotClassIdentifier(const std::string & SlotClassIdentifier)
{
	m_SlotClassIdentifier = SlotClassIdentifier;
}

inline void WeaponClass::SetSpaceRequirement(unsigned_numeric SpaceRequirement)
{
	m_SpaceRequirement = SpaceRequirement;
}

#endif
