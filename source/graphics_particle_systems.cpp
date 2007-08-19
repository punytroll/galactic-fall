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

#include <GL/gl.h>

#include "color.h"
#include "game_time.h"
#include "globals.h"
#include "graphics_particle_systems.h"
#include "graphics_texture.h"
#include "graphics_texture_manager.h"
#include "math.h"

const Graphics::Texture * g_ParticleTexture(0);

Graphics::ParticleSystem::ParticleSystem(void)
{
	if(g_ParticleTexture == 0)
	{
		g_ParticleTexture = g_TextureManager->Get("particle");
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
				}
				if(Forward == true)
				{
					++ParticleIterator;
				}
			}
		}
	}
	
	return true;
}

void Graphics::ParticleSystem::Draw(void)
{
	glPushMatrix();
	glTranslatef(GetPosition()[0], GetPosition()[1], 0.0f);
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
		glVertex2f(Position.m_V.m_A[0] - ParticleIterator->m_Size, Position.m_V.m_A[1] - ParticleIterator->m_Size);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(Position.m_V.m_A[0] + ParticleIterator->m_Size, Position.m_V.m_A[1] - ParticleIterator->m_Size);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(Position.m_V.m_A[0] + ParticleIterator->m_Size, Position.m_V.m_A[1] + ParticleIterator->m_Size);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(Position.m_V.m_A[0] - ParticleIterator->m_Size, Position.m_V.m_A[1] + ParticleIterator->m_Size);
	}
	glEnd();
	glPopAttrib();
	glPopMatrix();
}

Graphics::ParticleSystemHit::ParticleSystemHit(void)
{
	SetTimeOfDeath(GameTime::Get() + 3.0);
	for(int Index = 0; Index < 30; ++Index)
	{
		Graphics::ParticleSystem::Particle NewParticle;
		
		NewParticle.m_Position.Set(0.0f, 0.0f, 0.0f);
		
		Vector2f Velocity(GetRandomFloat(0.0f, 0.5f) + GetRandomFloatFromExponentialDistribution(1.0f), GetRandomFloat(0.0f, 2 * M_PI), Vector2f::InitializeMagnitudeAngle);
		
		NewParticle.m_Velocity = Vector3f(Velocity[0], Velocity[1], 0.0f);
		NewParticle.m_TimeOfDeath = GameTime::Get() + GetRandomDouble(0.3f, 0.8f);
		NewParticle.m_Color = Color(GetRandomFloat(0.4f, 0.5f), GetRandomFloat(0.35f, 0.45f), GetRandomFloat(0.35f, 0.65f), 0.3f);
		NewParticle.m_Size = GetRandomFloat(0.25f, 0.4f);
		m_Particles.push_back(NewParticle);
	}
	m_SystemScript.push_back("kill-old");
	m_SystemScript.push_back("move");
	m_SystemScript.push_back("update-particles");
	m_ParticleScript.push_back("kill-old");
	m_ParticleScript.push_back("move");
}

Graphics::ParticleSystemExplosion::ParticleSystemExplosion(void)
{
	SetTimeOfDeath(GameTime::Get() + 3.0);
	for(int Index = 0; Index < 500; ++Index)
	{
		Graphics::ParticleSystem::Particle NewParticle;
		
		NewParticle.m_Position.Set(0.0f, 0.0f, 0.0f);
		
		Vector2f Velocity(GetRandomFloat(0.0f, 2.5f) + GetRandomFloatFromExponentialDistribution(2.0f), GetRandomFloat(0.0f, 2 * M_PI), Vector2f::InitializeMagnitudeAngle);
		
		NewParticle.m_Velocity = Vector3f(Velocity[0], Velocity[1], 0.0f);
		NewParticle.m_TimeOfDeath = GameTime::Get() + GetRandomDouble(1.0f, 2.5f);
		NewParticle.m_Color = Color(GetRandomFloat(0.4f, 0.8f), GetRandomFloat(0.2f, 0.4f), GetRandomFloat(0.05f, 0.15f), 0.5f);
		NewParticle.m_Size = 1.0f;
		m_Particles.push_back(NewParticle);
	}
	for(int Index = 0; Index < 5; ++Index)
	{
		Graphics::ParticleSystem::Particle FlashParticle;
		
		Vector2f Position(GetRandomFloat(0.0f, 6.0f), GetRandomFloat(0.0f, 2 * M_PI), Vector2f::InitializeMagnitudeAngle);
		
		FlashParticle.m_Position = Vector3f(Position[0], Position[1], 0.0f);
		FlashParticle.m_Velocity.Set(0.0f, 0.0f, 0.0f);
		FlashParticle.m_TimeOfDeath = GameTime::Get() + GetRandomDouble(0.1f, 0.22f);
		FlashParticle.m_Color = Color(0.9f, GetRandomFloat(0.9f, 1.0f), GetRandomFloat(0.95f, 1.0f), 0.15f);
		FlashParticle.m_Size = 20.0f;
		m_Particles.push_back(FlashParticle);
	}
	
	Graphics::ParticleSystem::Particle BigFlashParticle;
	
	Vector2f Position(GetRandomFloat(0.0f, 6.0f), GetRandomFloat(0.0f, 2 * M_PI), Vector2f::InitializeMagnitudeAngle);
	
	BigFlashParticle.m_Position = Vector3f(Position[0], Position[1], 0.0f);
	BigFlashParticle.m_Velocity.Set(0.0f, 0.0f, 0.0f);
	BigFlashParticle.m_TimeOfDeath = GameTime::Get() + GetRandomDouble(0.1f, 0.15f);
	BigFlashParticle.m_Color = Color(0.9f, GetRandomFloat(0.9f, 1.0f), GetRandomFloat(0.95f, 1.0f), 0.3f);
	BigFlashParticle.m_Size = 200.0f;
	m_Particles.push_back(BigFlashParticle);
	m_SystemScript.push_back("kill-old");
	m_SystemScript.push_back("move");
	m_SystemScript.push_back("update-particles");
	m_ParticleScript.push_back("kill-old");
	m_ParticleScript.push_back("move");
}
