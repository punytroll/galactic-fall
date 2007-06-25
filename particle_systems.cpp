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

ParticleSystemHit::ParticleSystemHit(void)
{
	SetTimeOfDeath(GameTime::Get() + 3.0);
	for(int Index = 0; Index < 30; ++Index)
	{
		Particle NewParticle;
		
		NewParticle.m_Position.Set(0.0f, 0.0f);
		NewParticle.m_Velocity = Vector2f(GetRandomFloat(0.0f, 0.5f) + GetRandomFloatFromExponentialDistribution(1.0f), GetRandomFloat(0.0f, 2 * M_PI), Vector2f::InitializeMagnitudeAngle);
		NewParticle.m_TimeOfDeath = GameTime::Get() + GetRandomDouble(0.3f, 0.8f);
		m_Particles.push_back(NewParticle);
	}
}

bool ParticleSystemHit::Update(float Seconds)
{
	if(GameTime::Get() >= m_TimeOfDeath)
	{
		return false;
	}
	else
	{
		m_Position += m_Velocity * Seconds;
		
		std::list< Particle >::iterator ParticleIterator(m_Particles.begin());
		
		while(ParticleIterator != m_Particles.end())
		{
			if(GameTime::Get() >= ParticleIterator->m_TimeOfDeath)
			{
				ParticleIterator = m_Particles.erase(ParticleIterator);
			}
			else
			{
				ParticleIterator->m_Position += ParticleIterator->m_Velocity * Seconds;
				++ParticleIterator;
			}
		}
		
		return true;
	}
}

void ParticleSystemHit::Draw(void)
{
	glPushMatrix();
	glTranslatef(m_Position.m_V.m_A[0], m_Position.m_V.m_A[1], 0.0f);
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glColor4fv(Color(0.5f, 0.4f, 0.5f, 1.0f).GetColor().m_V.m_A);
	glBegin(GL_POINTS);
	for(std::list< Particle >::iterator ParticleIterator = m_Particles.begin(); ParticleIterator != m_Particles.end(); ++ParticleIterator)
	{
		glVertex2fv(ParticleIterator->m_Position.m_V.m_A);
	}
	glEnd();
	glPopAttrib();
	glPopMatrix();
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
		m_Particles.push_back(NewParticle);
	}
}

bool ParticleSystemExplosion::Update(float Seconds)
{
	if(GameTime::Get() >= m_TimeOfDeath)
	{
		return false;
	}
	else
	{
		m_Position += m_Velocity * Seconds;
		
		std::list< Particle >::iterator ParticleIterator(m_Particles.begin());
		
		while(ParticleIterator != m_Particles.end())
		{
			if(GameTime::Get() >= ParticleIterator->m_TimeOfDeath)
			{
				ParticleIterator = m_Particles.erase(ParticleIterator);
			}
			else
			{
				ParticleIterator->m_Position += ParticleIterator->m_Velocity * Seconds;
				++ParticleIterator;
			}
		}
		
		return true;
	}
}

void ParticleSystemExplosion::Draw(void)
{
	glPushMatrix();
	glTranslatef(m_Position.m_V.m_A[0], m_Position.m_V.m_A[1], 0.0f);
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glColor4fv(Color(0.6f, 0.3f, 0.1f, 1.0f).GetColor().m_V.m_A);
	glBegin(GL_POINTS);
	for(std::list< Particle >::iterator ParticleIterator = m_Particles.begin(); ParticleIterator != m_Particles.end(); ++ParticleIterator)
	{
		glVertex2fv(ParticleIterator->m_Position.m_V.m_A);
	}
	glEnd();
	glPopAttrib();
	glPopMatrix();
}
