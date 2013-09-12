/**
 * galactic-fall
 * Copyright (C) 2013  Hagen Möbius
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

#include <list>

#include "../globals.h"
#include "../system_statistics.h"
#include "engine.h"
#include "gl.h"
#include "particle_system.h"
#include "particle_system_node.h"
#include "scene.h"
#include "texture.h"
#include "texture_manager.h"

const Graphics::Texture * g_ParticleTexture(0);

Graphics::ParticleSystemNode::ParticleSystemNode(void) :
	_ParticleSystem(0)
{
	SetBlendFunction(GL_SRC_ALPHA, GL_ONE);
	SetUseBlending(true);
	SetUseDepthTest(false);
	SetUseLighting(false);
	SetUse2DTexture(true);
}

Graphics::ParticleSystemNode::~ParticleSystemNode(void)
{
	if(_ParticleSystem != 0)
	{
		_ParticleSystem->_ParticleSystemNodes.erase(this);
		_ParticleSystem = 0;
	}
}
	
void Graphics::ParticleSystemNode::SetParticleSystem(Graphics::ParticleSystem * ParticleSystem)
{
	assert(_ParticleSystem == 0);
	assert(ParticleSystem != 0);
	_ParticleSystem = ParticleSystem;
	_ParticleSystem->_ParticleSystemNodes.insert(this);
}

void Graphics::ParticleSystemNode::Begin(void)
{
	if(g_ParticleTexture == 0)
	{
		g_ParticleTexture = GetScene()->GetEngine()->GetTextureManager()->Get("particle");
		assert(g_ParticleTexture != 0);
	}
	assert(_ParticleSystem != 0);
	SetPosition(_ParticleSystem->GetPosition());
	Graphics::Node::Begin();
	g_ParticleTexture->Activate();
}

void Graphics::ParticleSystemNode::Draw(void)
{
	g_SystemStatistics->SetParticleSystemsDrawnThisFrame(g_SystemStatistics->GetParticleSystemsDrawnThisFrame() + 1);
	assert(_ParticleSystem != 0);
	
	const std::list< Graphics::ParticleSystem::Particle > & Particles(_ParticleSystem->GetParticles());
	
	GLBegin(GL_QUADS);
	for(std::list< Graphics::ParticleSystem::Particle >::const_iterator ParticleIterator = Particles.begin(); ParticleIterator != Particles.end(); ++ParticleIterator)
	{
		g_SystemStatistics->SetParticlesDrawnThisFrame(g_SystemStatistics->GetParticlesDrawnThisFrame() + 1);
		GLColor4fv(ParticleIterator->_Color.GetColor().GetPointer());
		
		const Vector3f & Position(ParticleIterator->_Position);
		const float & Size(ParticleIterator->_Size);
		
		// TODO: billboarding
		GLTexCoord2f(0.0f, 0.0f);
		GLVertex3f(Position[0] - Size, Position[1] - Size, Position[2]);
		GLTexCoord2f(1.0f, 0.0f);
		GLVertex3f(Position[0] + Size, Position[1] - Size, Position[2]);
		GLTexCoord2f(1.0f, 1.0f);
		GLVertex3f(Position[0] + Size, Position[1] + Size, Position[2]);
		GLTexCoord2f(0.0f, 1.0f);
		GLVertex3f(Position[0] - Size, Position[1] + Size, Position[2]);
	}
	GLEnd();
	Graphics::Node::Draw();
}

void Graphics::ParticleSystemNode::End(void)
{
	Graphics::Node::End();
}
