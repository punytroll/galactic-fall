/**
 * galactic-fall
 * Copyright (C) 2013-2019  Hagen Möbius
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
#include <set>
#include <string>
#include <vector>

#include <algebra/vector3f.h>

#include <graphics/color_rgbo.h>

namespace Graphics
{
	class ParticleSystemNode;
	
	class ParticleSystem
	{
		friend class ParticleSystemNode;
	public:
		struct Particle
		{
			Vector3f _Position;
			Vector3f _Velocity;
			Graphics::ColorRGBO _Color;
			double _Lifetime;
			double _TimeUntilDeath;
			float _Opacity;
			float _Size;
		};
		
		ParticleSystem(void);
		virtual ~ParticleSystem(void);
		// getters
		bool IsDone(void) const;
		const std::list< Graphics::ParticleSystem::Particle > & GetParticles(void) const;
		const Vector3f & GetPosition(void) const;
		// setters
		void SetPosition(const Vector3f & Position);
		void SetVelocity(const Vector3f & Velocity);
		void SetTimeUntilDeath(double TimeUntilDeath);
		// modifiers
		void AddParticle(const Graphics::ParticleSystem::Particle & Particle);
		void AddSystemScriptLine(const std::string & SystemScriptLine);
		void AddParticleScriptLine(const std::string & ParticleScriptLine);
		void Update(float Seconds);
	protected:
		bool _IsDone;
		Vector3f _Position;
		Vector3f _Velocity;
		double _TimeUntilDeath;
		std::list< Graphics::ParticleSystem::Particle > _Particles;
		std::vector< std::string > _SystemScript;
		std::vector< std::string > _ParticleScript;
		std::set< Graphics::ParticleSystemNode * > _ParticleSystemNodes;
	};
	
	inline bool ParticleSystem::IsDone(void) const
	{
		return _IsDone;
	}
	
	inline const std::list< Graphics::ParticleSystem::Particle > & ParticleSystem::GetParticles(void) const
	{
		return _Particles;
	}
	
	inline const Vector3f & ParticleSystem::GetPosition(void) const
	{
		return _Position;
	}
	
	inline void ParticleSystem::SetPosition(const Vector3f & Position)
	{
		_Position = Position;
	}
	
	inline void ParticleSystem::SetVelocity(const Vector3f & Velocity)
	{
		_Velocity = Velocity;
	}
	
	inline void ParticleSystem::SetTimeUntilDeath(double TimeUntilDeath)
	{
		_TimeUntilDeath = TimeUntilDeath;
	}
}

#endif
