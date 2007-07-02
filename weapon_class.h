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

class Color;
class Model;

class WeaponClass
{
public:
	// constructor
	WeaponClass(const std::string & Identifier);
	// destructor
	~WeaponClass(void);
	// getters
	const std::string & GetIdentifier(void) const;
	const Quaternion & GetOrientation(void) const;
	float GetReloadTime(void) const;
	float GetParticleExitSpeed(void) const;
	float GetParticleDamage(void) const;
	float GetParticleLifeTime(void) const;
	const Model * GetParticleModel(void) const;
	const Color * GetParticleColor(void) const;
	const std::string & GetSlotType(void) const;
	// modifiers
	void SetReloadTime(float ReloadTime);
	void SetOrientation(const Quaternion & Orientation);
	void SetParticleExitSpeed(float ParticleExitSpeed);
	void SetParticleDamage(float ParticleDamage);
	void SetParticleLifeTime(float ParticleLifeTime);
	void SetParticleModel(const Model * ParticleModel);
	void SetParticleColor(const Color & ParticleColor);
	void SetSlotType(const std::string & SlotType);
private:
	std::string m_Identifier;
	std::string m_SlotType;
	float m_ReloadTime;
	float m_ParticleExitSpeed;
	float m_ParticleDamage;
	float m_ParticleLifeTime;
	const Model * m_ParticleModel;
	Color * m_ParticleColor;
	Quaternion m_Orientation;
};

inline const std::string & WeaponClass::GetIdentifier(void) const
{
	return m_Identifier;
}

inline const Quaternion & WeaponClass::GetOrientation(void) const
{
	return m_Orientation;
}

inline float WeaponClass::GetReloadTime(void) const
{
	return m_ReloadTime;
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

inline const Model * WeaponClass::GetParticleModel(void) const
{
	return m_ParticleModel;
}

inline const Color * WeaponClass::GetParticleColor(void) const
{
	return m_ParticleColor;
}

inline const std::string & WeaponClass::GetSlotType(void) const
{
	return m_SlotType;
}

inline void WeaponClass::SetOrientation(const Quaternion & Orientation)
{
	m_Orientation = Orientation;
}

inline void WeaponClass::SetReloadTime(float ReloadTime)
{
	m_ReloadTime = ReloadTime;
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

inline void WeaponClass::SetParticleModel(const Model * ParticleModel)
{
	m_ParticleModel = ParticleModel;
}

inline void WeaponClass::SetSlotType(const std::string & SlotType)
{
	m_SlotType = SlotType;
}

#endif
