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

#include "game_time.h"
#include "object_aspect_position.h"
#include "shot.h"
#include "weapon_class.h"

Shot::Shot(const WeaponClass * WeaponClass) :
	m_WeaponClass(WeaponClass),
	m_Shooter(0),
	m_TimeOfDeath(GameTime::Get() + WeaponClass->GetParticleLifeTime()),
	m_Velocity(true),
	m_AngularPosition(0.0f),
	m_Damage(WeaponClass->GetParticleDamage())
{
	// initialize object aspects
	AddAspectPosition();
	// other
	SetRadialSize(0.54f);
}

Shot::~Shot(void)
{
}

bool Shot::Update(float Seconds)
{
	if(m_TimeOfDeath < GameTime::Get())
	{
		return false;
	}
	GetAspectPosition()->ModifyPosition(m_Velocity * Seconds);
	
	return true;
}
