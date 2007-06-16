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

#ifndef SHOT_H
#define SHOT_H

#include <math3d/vector2f.h>

#include "physical_object.h"

class Model;

class Shot : public PhysicalObject
{
public:
	Shot(PhysicalObject * Shooter, float AngularPosition, const math3d::vector2f & Velocity);
	virtual ~Shot(void);
	virtual void Draw(void) const;
	bool Update(float Seconds);
	const PhysicalObject * GetShooter(void) const;
	const math3d::vector2f & GetVelocity(void) const;
	float GetDamage(void) const;
private:
	PhysicalObject * m_Shooter;
	double m_TimeOfDeath;
	math3d::vector2f m_Velocity;
	float m_AngularPosition;
	float m_Damage;
};

inline const PhysicalObject * Shot::GetShooter(void) const
{
	return m_Shooter;
}

inline const math3d::vector2f & Shot::GetVelocity(void) const
{
	return m_Velocity;
}

inline float Shot::GetDamage(void) const
{
	return m_Damage;
}

#endif

