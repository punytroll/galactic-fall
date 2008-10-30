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
#include "object_aspect_update.h"
#include "shot.h"

Shot::Shot(void) :
	m_Shooter(0),
	m_TimeOfDeath(0.0),
	m_Velocity(true),
	m_Damage(0.0)
{
	// initialize object aspects
	AddAspectPhysical();
	AddAspectPosition();
	AddAspectUpdate();
	GetAspectUpdate()->SetCallback(Callback(this, &Shot::Update));
	AddAspectVisualization();
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
