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
#include "math.h"
#include "particle_systems.h"

bool ParticleSystem::Update(float Seconds)
{
	if(GameTime::Get() >= m_TimeOfDeath)
	{
		return false;
	}
	else
	{
		m_Position += m_Velocity * Seconds;
		
		std::list< ParticleSystem::Particle >::iterator ParticleIterator(m_Particles.begin());
		
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
		
		return true;
	}
}

void ParticleSystem::Draw(void)
{
	glPushMatrix();
	glTranslatef(m_Position.m_V.m_A[0], m_Position.m_V.m_A[1], 0.0f);
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glBegin(GL_POINTS);
	for(std::list< Particle >::iterator ParticleIterator = m_Particles.begin(); ParticleIterator != m_Particles.end(); ++ParticleIterator)
	{
		glColor4fv(ParticleIterator->m_Color.GetColor().m_V.m_A);
		glVertex2fv(ParticleIterator->m_Position.m_V.m_A);
	}
	glEnd();
	glPopAttrib();
	glPopMatrix();
}

ParticleSystemHit::ParticleSystemHit(void)
{
	SetTimeOfDeath(GameTime::Get() + 3.0);
	for(int Index = 0; Index < 30; ++Index)
	{
		Particle NewParticle;
		
		NewParticle.m_Position.Set(0.0f, 0.0f);
		NewParticle.m_Velocity = Vector2f(GetRandomFloat(0.0f, 0.5f) + GetRandomFloatFromExponentialDistribution(1.0f), GetRandomFloat(0.0f, 2 * M_PI), Vector2f::InitializeMagnitudeAngle);
		NewParticle.m_TimeOfDeath = GameTime::Get() + GetRandomDouble(0.3f, 0.8f);
		NewParticle.m_Color = Color(GetRandomFloat(0.35f, 0.65f), GetRandomFloat(0.3f, 0.5f), GetRandomFloat(0.35f, 0.65f), 1.0f);
		m_Particles.push_back(NewParticle);
	}
	m_ParticleScript.push_back("kill-old");
	m_ParticleScript.push_back("move");
}

ParticleSystemExplosion::ParticleSystemExplosion(void)
{
	SetTimeOfDeath(GameTime::Get() + 3.0);
	for(int Index = 0; Index < 500; ++Index)
	{
		Particle NewParticle;
		
		NewParticle.m_Position.Set(0.0f, 0.0f);
		NewParticle.m_Velocity = Vector2f(GetRandomFloat(0.0f, 2.5f) + GetRandomFloatFromExponentialDistribution(2.0f), GetRandomFloat(0.0f, 2 * M_PI), Vector2f::InitializeMagnitudeAngle);
		NewParticle.m_TimeOfDeath = GameTime::Get() + GetRandomDouble(1.0f, 1.6f);
		NewParticle.m_Color = Color(GetRandomFloat(0.4f, 0.8f), GetRandomFloat(0.2f, 0.4f), GetRandomFloat(0.05f, 0.15f), 1.0f);
		m_Particles.push_back(NewParticle);
	}
	m_ParticleScript.push_back("kill-old");
	m_ParticleScript.push_back("move");
}
