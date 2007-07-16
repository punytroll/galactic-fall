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
#include "slot.h"
#include "string_cast.h"
#include "system.h"
#include "weapon.h"
#include "weapon_class.h"

Weapon::Weapon(const WeaponClass * WeaponClass) :
	m_WeaponClass(WeaponClass),
	m_Ship(0),
	m_Slot(0),
	m_Fire(false),
	m_NextTimeToFire(0.0)
{
}

void Weapon::Update(float Seconds)
{
	if((m_Fire == true) && (m_NextTimeToFire <= GameTime::Get()))
	{
		std::stringstream IdentifierStream;
		
		IdentifierStream << "::shot::created_at_game_time(" << to_string_cast(GameTime::Get(), 2) << ")::created_by(" << GetShip()->GetObjectIdentifier() << ")::in_system(" << GetShip()->GetCurrentSystem()->GetIdentifier() << ")";
		
		Shot * NewShot(new Shot(GetWeaponClass()));
		
		NewShot->SetObjectIdentifier(IdentifierStream.str());
		NewShot->SetShooter(GetShip());
		
		const Vector3f & SlotPosition(GetSlot()->GetPosition());
		
		NewShot->SetPosition(GetShip()->GetPosition() + Vector2f(SlotPosition.m_V.m_A[0], SlotPosition.m_V.m_A[1]).Turned(GetShip()->GetAngularPosition()));
		NewShot->SetAngularPosition(GetShip()->GetAngularPosition());
		NewShot->SetVelocity(GetShip()->GetVelocity() + Vector2f(GetWeaponClass()->GetParticleExitSpeed(), GetShip()->GetAngularPosition(), Vector2f::InitializeMagnitudeAngle));
		GetShip()->GetCurrentSystem()->AddContent(NewShot);
		m_NextTimeToFire = GameTime::Get() + GetWeaponClass()->GetReloadTime();
	}
}
