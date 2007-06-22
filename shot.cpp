/**
 * galactic-fall
 * Copyright (C) 2006  Hagen Möbius
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
#include "model.h"
#include "shot.h"
#include "weapon_class.h"

Shot::Shot(const WeaponClass * WeaponClass) :
	m_WeaponClass(WeaponClass),
	m_Shooter(0),
	m_AngularPosition(0.0f),
	m_Velocity(true),
	m_TimeOfDeath(GameTime::Get() + WeaponClass->GetParticleLifeTime()),
	m_Damage(WeaponClass->GetParticleDamage())
{
	SetRadialSize(0.54f);
}

Shot::~Shot(void)
{
}

void Shot::Draw(void) const
{
	glPushAttrib(GL_ENABLE_BIT);
	glPushMatrix();
	glTranslatef(m_Position.m_V.m_A[0], m_Position.m_V.m_A[1], 0.0f);
	glRotatef(m_AngularPosition * 180.0f / M_PI, 0.0f, 0.0f, 1.0f);
	glColor4fv(m_WeaponClass->GetParticleColor()->GetColor().m_V.m_A);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	m_WeaponClass->GetParticleModel()->Draw();
	glPopMatrix();
	glPopAttrib();
}

bool Shot::Update(float Seconds)
{
	if(m_TimeOfDeath < GameTime::Get())
	{
		return false;
	}
	m_Position += m_Velocity * Seconds;
	
	return true;
}
