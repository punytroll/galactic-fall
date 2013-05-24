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

#include <stdexcept>

#include "../color.h"
#include "../game_time.h"
#include "../globals.h"
#include "../graphics/gl.h"
#include "../math.h"
#include "../system_statistics.h"
#include "engine.h"
#include "particle_system.h"
#include "scene.h"
#include "texture.h"
#include "texture_manager.h"

const Graphics::Texture * g_ParticleTexture(0);

Graphics::ParticleSystem::ParticleSystem(void)
{
	SetBlendFunction(GL_SRC_ALPHA, GL_ONE);
	SetUseBlending(true);
	SetUseDepthTest(false);
	SetUseLighting(false);
	SetUse2DTexture(true);
}

bool Graphics::ParticleSystem::Update(float Seconds)
{
	g_SystemStatistics->SetParticleSystemsThisFrame(g_SystemStatistics->GetParticleSystemsThisFrame() + 1);
	for(std::vector< std::string >::const_iterator ScriptLine = m_SystemScript.begin(); ScriptLine != m_SystemScript.end(); ++ScriptLine)
	{
		if(*ScriptLine == "kill-old")
		{
			if(GameTime::Get() >= m_TimeOfDeath)
			{
				return false;
			}
		}
		else if(*ScriptLine == "move")
		{
			SetPosition(GetPosition() + Seconds * m_Velocity);
		}
		else if(*ScriptLine == "update-particles")
		{
			g_SystemStatistics->SetParticlesThisFrame(g_SystemStatistics->GetParticlesThisFrame() + m_Particles.size());
			
			std::list< Graphics::ParticleSystem::Particle >::iterator ParticleIterator(m_Particles.begin());
			
			while(ParticleIterator != m_Particles.end())
			{
				bool Forward(true);
				
				for(std::vector< std::string >::const_iterator ScriptLine = m_ParticleScript.begin(); ScriptLine != m_ParticleScript.end(); ++ScriptLine)
				{
					if(*ScriptLine == "kill-old")
					{
						if(GameTime::Get() >= ParticleIterator->m_TimeOfDeath)
						{
							ParticleIterator = m_Particles.erase(ParticleIterator);
							Forward = false;
							
							break;
						}
					}
					else if(*ScriptLine == "move")
					{
						ParticleIterator->m_Position += ParticleIterator->m_Velocity * Seconds;
					}
					else
					{
						throw std::runtime_error("Unknown particle command '" + *ScriptLine + "'.");
					}
				}
				if(Forward == true)
				{
					++ParticleIterator;
				}
			}
		}
		else
		{
			throw std::runtime_error("Unknown particle system command '" + *ScriptLine + "'.");
		}
	}
	
	return true;
}

void Graphics::ParticleSystem::Begin(void)
{
	if(g_ParticleTexture == 0)
	{
		g_ParticleTexture = GetScene()->GetEngine()->GetTextureManager()->Get("particle");
		assert(g_ParticleTexture != 0);
	}
	Graphics::Node::Begin();
	g_ParticleTexture->Activate();
}

void Graphics::ParticleSystem::Draw(void)
{
	GLBegin(GL_QUADS);
	for(std::list< Graphics::ParticleSystem::Particle >::iterator ParticleIterator = m_Particles.begin(); ParticleIterator != m_Particles.end(); ++ParticleIterator)
	{
		GLColor4fv(ParticleIterator->m_Color.GetColor().m_V.m_A);
		
		const Vector3f & Position(ParticleIterator->m_Position);
		const float & Size(ParticleIterator->m_Size);
		
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

void Graphics::ParticleSystem::End(void)
{
	Graphics::Node::End();
}

void Graphics::ParticleSystem::AddParticle(const Graphics::ParticleSystem::Particle & Particle)
{
	m_Particles.push_back(Particle);
}

void Graphics::ParticleSystem::AddSystemScriptLine(const std::string & SystemScriptLine)
{
	m_SystemScript.push_back(SystemScriptLine);
}

void Graphics::ParticleSystem::AddParticleScriptLine(const std::string & ParticleScriptLine)
{
	m_ParticleScript.push_back(ParticleScriptLine);
}
