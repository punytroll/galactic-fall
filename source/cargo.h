/**
 * galactic-fall
 * Copyright (C) 2006  Hagen Möbius
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

#ifndef CARGO_H
#define CARGO_H

#include "math/vector2f.h"
#include "math/vector3f.h"
#include "physical_object.h"

class CommodityClass;
class Model;

class Cargo : public PhysicalObject
{
public:
	Cargo(const CommodityClass * CommodityClass);
	virtual ~Cargo(void);
	// modifiers
	virtual void Draw(void) const;
	void Move(float Seconds);
	// getters
	const CommodityClass * GetCommodityClass(void) const;
	float GetHull(void) const;
	const Vector2f & GetVelocity(void) const;
	// setters
	void SetHull(float Hull);
	void SetVelocity(const Vector2f & Velocity);
private:
	const CommodityClass * m_CommodityClass;
	float m_Hull;
	Vector2f m_Velocity;
	float m_AngularPosition;
	Vector3f m_RotationAxis;
	float m_AngularVelocity;
	// no copy constructor
	Cargo(const Cargo & Cargo);
};

inline const CommodityClass * Cargo::GetCommodityClass(void) const
{
	return m_CommodityClass;
}

inline float Cargo::GetHull(void) const
{
	return m_Hull;
}

inline const Vector2f & Cargo::GetVelocity(void) const
{
	return m_Velocity;
}

inline void Cargo::SetHull(float Hull)
{
	m_Hull = Hull;
}

inline void Cargo::SetVelocity(const Vector2f & Velocity)
{
	m_Velocity = Velocity;
}

#endif
