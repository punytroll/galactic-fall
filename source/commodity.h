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

#include <algebra/axis_angle.h>
#include <algebra/vector3f.h>

#include "object.h"

class Visualization;

class Commodity : public Object
{
public:
	Commodity(void);
	virtual ~Commodity(void);
	// getters
	const AxisAngle & GetAngularVelocity(void) const;
	float GetHull(void) const;
	const Vector3f & GetVelocity(void) const;
	// setters
	void SetAngularVelocity(const AxisAngle & AngularVelocity);
	void SetHull(float Hull);
	void SetVelocity(const Vector3f & Velocity);
private:
	// slot for the object aspect Update
	bool Update(float Seconds);
	void _UpdateVisualization(Visualization * Visualization);
private:
	AxisAngle m_AngularVelocity;
	float m_Hull;
	Vector3f m_Velocity;
	// no copy constructor
	Commodity(const Commodity & Commodity);
};

inline const AxisAngle & Commodity::GetAngularVelocity(void) const
{
	return m_AngularVelocity;
}

inline float Commodity::GetHull(void) const
{
	return m_Hull;
}

inline const Vector3f & Commodity::GetVelocity(void) const
{
	return m_Velocity;
}

inline void Commodity::SetAngularVelocity(const AxisAngle & AngularVelocity)
{
	m_AngularVelocity = AngularVelocity;
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
