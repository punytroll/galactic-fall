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

#ifndef PARTICLE_SYSTEMS_H
#define PARTICLE_SYSTEMS_H

#include <list>
#include <string>
#include <vector>

#include "color.h"
#include "math/vector2f.h"

class ParticleSystem
{
public:
	ParticleSystem(void);
	bool Update(float Seconds);
	void Draw(void);
	// setters
	void SetPosition(const Vector2f & Position);
	void SetVelocity(const Vector2f & Velocity);
	void SetTimeOfDeath(double TimeOfDeath);
protected:
	struct Particle
	{
		Vector2f m_Position;
		Vector2f m_Velocity;
		Color m_Color;
		double m_TimeOfDeath;
		float m_Size;
	};
	
	Vector2f m_Position;
	Vector2f m_Velocity;
	double m_TimeOfDeath;
	std::list< Particle > m_Particles;
	std::vector< std::string > m_SystemScript;
	std::vector< std::string > m_ParticleScript;
};

inline void ParticleSystem::SetPosition(const Vector2f & Position)
{
	m_Position = Position;
}

inline void ParticleSystem::SetVelocity(const Vector2f & Velocity)
{
	m_Velocity = Velocity;
}

inline void ParticleSystem::SetTimeOfDeath(double TimeOfDeath)
{
	m_TimeOfDeath = TimeOfDeath;
}

class ParticleSystemHit : public ParticleSystem
{
public:
	ParticleSystemHit(void);
};

class ParticleSystemExplosion : public ParticleSystem
{
public:
	ParticleSystemExplosion(void);
};

#endif
