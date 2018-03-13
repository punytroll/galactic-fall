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

#include "template.h"

class VisualizationPrototype;

namespace Graphics
{
	class Model;
}

class WeaponClass : public Template
{
public:
	// constructor & destructor
	WeaponClass(const std::string & Identifier);
	~WeaponClass(void);
	// getters
	const std::string & GetIdentifier(void) const;
	const Vector3f & GetMuzzlePosition(void) const;
	const Quaternion & GetOrientation(void) const;
	const VisualizationPrototype * GetShotVisualizationPrototype(void) const;
	const VisualizationPrototype * GetWeaponVisualizationPrototype(void) const;
	// setters
	void SetMuzzlePosition(const Vector3f & MuzzlePosition);
	void SetOrientation(const Quaternion & Orientation);
	void SetShotVisualizationPrototype(const VisualizationPrototype & ShotVisualizationPrototype);
	void SetWeaponVisualizationPrototype(const VisualizationPrototype & WeaponVisualizationPrototype);
private:
	Vector3f _MuzzlePosition;
	Quaternion _Orientation;
	VisualizationPrototype * _ShotVisualizationPrototype;
	VisualizationPrototype * _WeaponVisualizationPrototype;
};

inline const std::string & WeaponClass::GetIdentifier(void) const
{
	return GetSubTypeIdentifier();
}

inline const Vector3f & WeaponClass::GetMuzzlePosition(void) const
{
	return _MuzzlePosition;
}

inline const Quaternion & WeaponClass::GetOrientation(void) const
{
	return _Orientation;
}

inline const VisualizationPrototype * WeaponClass::GetShotVisualizationPrototype(void) const
{
	return _ShotVisualizationPrototype;
}

inline const VisualizationPrototype * WeaponClass::GetWeaponVisualizationPrototype(void) const
{
	return _WeaponVisualizationPrototype;
}

inline void WeaponClass::SetMuzzlePosition(const Vector3f & MuzzlePosition)
{
	_MuzzlePosition = MuzzlePosition;
}

inline void WeaponClass::SetOrientation(const Quaternion & Orientation)
{
	_Orientation = Orientation;
}

#endif
