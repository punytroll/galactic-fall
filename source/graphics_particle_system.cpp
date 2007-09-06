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

#include <GL/gl.h>

#include "color.h"
#include "game_time.h"
#include "globals.h"
#include "graphics_particle_system.h"
#include "graphics_texture.h"
#include "graphics_texture_manager.h"
#include "math.h"

const Graphics::Texture * g_ParticleTexture(0);

Graphics::ParticleSystem::ParticleSystem(void)
{
	if(g_ParticleTexture == 0)
	{
		g_ParticleTexture = g_TextureManager->Get("particle");
		assert(g_ParticleTexture != 0);
	}
}

bool Graphics::ParticleSystem::Update(float Seconds)
{
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

void Graphics::ParticleSystem::Draw(void)
{
	glPushMatrix();
	glTranslatef(GetPosition()[0], GetPosition()[1], GetPosition()[2]);
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	g_ParticleTexture->Activate();
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	for(std::list< Graphics::ParticleSystem::Particle >::iterator ParticleIterator = m_Particles.begin(); ParticleIterator != m_Particles.end(); ++ParticleIterator)
	{
		glColor4fv(ParticleIterator->m_Color.GetColor().m_V.m_A);
		
		Vector3f & Position(ParticleIterator->m_Position);
		
		// TODO: billboarding
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(Position[0] - ParticleIterator->m_Size, Position[1] - ParticleIterator->m_Size, Position[2]);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(Position[0] + ParticleIterator->m_Size, Position[1] - ParticleIterator->m_Size, Position[2]);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(Position[0] + ParticleIterator->m_Size, Position[1] + ParticleIterator->m_Size, Position[2]);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(Position[0] - ParticleIterator->m_Size, Position[1] + ParticleIterator->m_Size, Position[2]);
	}
	glEnd();
	glPopAttrib();
	glPopMatrix();
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
