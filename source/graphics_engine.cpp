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

#include "graphics_engine.h"
#include "graphics_particle_systems.h"

void Graphics::Engine::Update(float Seconds)
{
	std::vector< Graphics::ParticleSystem * >::iterator ParticleSystemIterator(m_ParticleSystems.begin());
	
	while(ParticleSystemIterator != m_ParticleSystems.end())
	{
		if((*ParticleSystemIterator)->Update(Seconds) == false)
		{
			delete *ParticleSystemIterator;
			ParticleSystemIterator = m_ParticleSystems.erase(ParticleSystemIterator);
		}
		else
		{
			++ParticleSystemIterator;
		}
	}
}

void Graphics::Engine::Render(void)
{
	for(std::vector< Graphics::ParticleSystem * >::iterator ParticleSystemIterator = m_ParticleSystems.begin(); ParticleSystemIterator != m_ParticleSystems.end(); ++ParticleSystemIterator)
	{
		(*ParticleSystemIterator)->Draw();
	}
}

void Graphics::Engine::AddParticleSystem(ParticleSystem * ParticleSystem)
{
	m_ParticleSystems.push_back(ParticleSystem);
}

void Graphics::Engine::Clear(void)
{
	while(m_ParticleSystems.empty() == false)
	{
		ParticleSystem * ParticleSystem(m_ParticleSystems.front());
		
		m_ParticleSystems.erase(m_ParticleSystems.begin());
		delete ParticleSystem;
	}
}
