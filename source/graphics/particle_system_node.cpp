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

const Graphics::Texture * g_ParticleTexture(nullptr);

Graphics::ParticleSystemNode::ParticleSystemNode(void) :
	_ParticleSystem(nullptr)
{
	SetBlendFunction(GL_SRC_ALPHA, GL_ONE);
	SetUseBlending(true);
	SetUseDepthTest(false);
	SetUseLighting(false);
	SetUse2DTexture(true);
}

Graphics::ParticleSystemNode::~ParticleSystemNode(void)
{
	if(_ParticleSystem != nullptr)
	{
		_ParticleSystem->_ParticleSystemNodes.erase(this);
		_ParticleSystem = nullptr;
	}
}
	
void Graphics::ParticleSystemNode::SetParticleSystem(Graphics::ParticleSystem * ParticleSystem)
{
	assert(_ParticleSystem == nullptr);
	assert(ParticleSystem != nullptr);
	_ParticleSystem = ParticleSystem;
	_ParticleSystem->_ParticleSystemNodes.insert(this);
}

void Graphics::ParticleSystemNode::Begin(Graphics::RenderContext * RenderContext)
{
	if(g_ParticleTexture == nullptr)
	{
		g_ParticleTexture = GetScene()->GetEngine()->GetTextureManager()->Get("particle");
		assert(g_ParticleTexture != nullptr);
	}
	assert(_ParticleSystem != nullptr);
	GLPushMatrix();
	GLMultMatrixf(GetSpacialMatrix().GetPointer());
	Graphics::Node::Begin(RenderContext);
	g_ParticleTexture->Activate();
}

void Graphics::ParticleSystemNode::Draw(Graphics::RenderContext * RenderContext)
{
	g_SystemStatistics->SetParticleSystemsDrawnThisFrame(g_SystemStatistics->GetParticleSystemsDrawnThisFrame() + 1);
	assert(_ParticleSystem != nullptr);
	GLBegin(GL_QUADS);
	for(auto Particle : _ParticleSystem->GetParticles())
	{
		g_SystemStatistics->SetParticlesDrawnThisFrame(g_SystemStatistics->GetParticlesDrawnThisFrame() + 1);
		GLColor4fv(Particle._Color.GetColor().GetPointer());
		
		const Vector3f & Position(Particle._Position);
		const float & Size(Particle._Size);
		
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
}

void Graphics::ParticleSystemNode::End(Graphics::RenderContext * RenderContext)
{
	Graphics::Node::End(RenderContext);
	GLPopMatrix();
}
