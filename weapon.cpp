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

#include <sstream>

#include "game_time.h"
#include "ship.h"
#include "shot.h"
#include "string_cast.h"
#include "system.h"
#include "weapon.h"

Weapon::Weapon(WeaponClass * WeaponClass) :
	m_WeaponClass(WeaponClass),
	m_NextTimeToFire(0.0)
{
}

void Weapon::Update(float Seconds, bool Fire)
{
	if((Fire == true) && (m_NextTimeToFire <= GameTime::Get()))
	{
		std::stringstream IdentifierStream;
		
		IdentifierStream << "::shot::created_at_game_time(" << to_string_cast(GameTime::Get(), 2) << ")::created_by(" << GetShip()->GetObjectIdentifier() << ")::in_system(" << GetShip()->GetCurrentSystem()->GetIdentifier() << ")";
		
		Shot * NewShot(new Shot(GetShip(), GetShip()->GetAngularPosition(), GetShip()->GetVelocity() + math3d::vector2f(30.0f, GetShip()->GetAngularPosition(), math3d::vector2f::magnitude_angle)));
		
		NewShot->SetObjectIdentifier(IdentifierStream.str());
		NewShot->SetPosition(GetShip()->GetPosition());
		GetShip()->GetCurrentSystem()->AddShot(NewShot);
		m_NextTimeToFire = GameTime::Get() + 0.63;
	}
}
