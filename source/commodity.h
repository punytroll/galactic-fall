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

#ifndef COMMODITY_H
#define COMMODITY_H

#include "math/axis_angle.h"
#include "math/quaternion.h"
#include "math/vector3f.h"
#include "physical_object.h"

class CommodityClass;
class Model;

class Commodity : public PhysicalObject
{
public:
	Commodity(const CommodityClass * CommodityClass);
	virtual ~Commodity(void);
	// modifiers
	virtual void Draw(void) const;
	void Move(float Seconds);
	// getters
	const Quaternion & GetAngularPosition(void) const;
	const CommodityClass * GetCommodityClass(void) const;
	float GetHull(void) const;
	const Vector3f & GetVelocity(void) const;
	// setters
	void SetHull(float Hull);
	void SetVelocity(const Vector3f & Velocity);
private:
	const CommodityClass * m_CommodityClass;
	float m_Hull;
	Vector3f m_Velocity;
	Quaternion m_AngularPosition;
	AxisAngle m_AngularVelocity;
	// no copy constructor
	Commodity(const Commodity & Commodity);
};

inline const Quaternion & Commodity::GetAngularPosition(void) const
{
	return m_AngularPosition;
}

inline const CommodityClass * Commodity::GetCommodityClass(void) const
{
	return m_CommodityClass;
}

inline float Commodity::GetHull(void) const
{
	return m_Hull;
}

inline const Vector3f & Commodity::GetVelocity(void) const
{
	return m_Velocity;
}

inline void Commodity::SetHull(float Hull)
{
	m_Hull = Hull;
}

inline void Commodity::SetVelocity(const Vector3f & Velocity)
{
	m_Velocity = Velocity;
}

#endif
