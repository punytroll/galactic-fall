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
#include "graphics_node.h"
#include "graphics_particle_systems.h"

void Graphics::Engine::Update(float Seconds)
{
	std::vector< Graphics::ParticleSystem * >::size_type ParticleSystemIndex(0);
	
	while(ParticleSystemIndex < m_ParticleSystems.size())
	{
		if(m_ParticleSystems[ParticleSystemIndex]->Update(Seconds) == false)
		{
			Graphics::ParticleSystem * ParticleSystem(m_ParticleSystems[ParticleSystemIndex]);
			
			RemoveNode(ParticleSystem);
			delete ParticleSystem;
		}
		else
		{
			++ParticleSystemIndex;
		}
	}
}

void Graphics::Engine::Render(void)
{
	for(std::vector< Graphics::Node * >::iterator NodeIterator = m_Nodes.begin(); NodeIterator != m_Nodes.end(); ++NodeIterator)
	{
		(*NodeIterator)->Draw();
	}
}

void Graphics::Engine::AddNode(Graphics::Node * Node)
{
	m_Nodes.push_back(Node);
	
	Graphics::ParticleSystem * ParticleSystem(dynamic_cast< Graphics::ParticleSystem * >(Node));
	
	if(ParticleSystem != 0)
	{
		m_ParticleSystems.push_back(ParticleSystem);
	}
}

void Graphics::Engine::RemoveNode(Graphics::Node * Node)
{
	m_Nodes.erase(std::find(m_Nodes.begin(), m_Nodes.end(), Node));
	
	Graphics::ParticleSystem * ParticleSystem(dynamic_cast< Graphics::ParticleSystem * >(Node));
	
	if(ParticleSystem != 0)
	{
		m_ParticleSystems.erase(std::find(m_ParticleSystems.begin(), m_ParticleSystems.end(), ParticleSystem));
	}
}

void Graphics::Engine::Clear(void)
{
	while(m_Nodes.empty() == false)
	{
		Graphics::Node * Node(m_Nodes.front());
		
		m_Nodes.erase(m_Nodes.begin());
		delete Node;
	}
	m_ParticleSystems.clear();
}
