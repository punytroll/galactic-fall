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

#ifndef GRAPHICS_PARTICLE_SYSTEM_H
#define GRAPHICS_PARTICLE_SYSTEM_H

#include <list>
#include <string>
#include <vector>

#include "../color.h"
#include "../math/vector3f.h"
#include "node.h"

namespace Graphics
{
	class ParticleSystem : public Graphics::Node
	{
	public:
		struct Particle
		{
			Vector3f m_Position;
			Vector3f m_Velocity;
			Color m_Color;
			double m_TimeOfDeath;
			float m_Size;
		};
		
		ParticleSystem(void);
		bool Update(float Seconds);
		virtual void Begin(void);
		virtual void Draw(void);
		virtual void End(void);
		// setters
		void SetVelocity(const Vector3f & Velocity);
		void SetTimeOfDeath(double TimeOfDeath);
		// modifiers
		void AddParticle(const Graphics::ParticleSystem::Particle & Particle);
		void AddSystemScriptLine(const std::string & SystemScriptLine);
		void AddParticleScriptLine(const std::string & ParticleScriptLine);
	protected:
		Vector3f m_Velocity;
		double m_TimeOfDeath;
		std::list< Graphics::ParticleSystem::Particle > m_Particles;
		std::vector< std::string > m_SystemScript;
		std::vector< std::string > m_ParticleScript;
	};
	
	inline void ParticleSystem::SetVelocity(const Vector3f & Velocity)
	{
		m_Velocity = Velocity;
	}
	
	inline void ParticleSystem::SetTimeOfDeath(double TimeOfDeath)
	{
		m_TimeOfDeath = TimeOfDeath;
	}
}

#endif
