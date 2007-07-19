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

#include <iostream>
#include <sstream>

#include "game_time.h"
#include "math.h"
#include "math/quaternion.h"
#include "math/vector4f.h"
#include "ship.h"
#include "shot.h"
#include "slot.h"
#include "string_cast.h"
#include "system.h"
#include "weapon.h"
#include "weapon_class.h"

Weapon::Weapon(const WeaponClass * WeaponClass) :
	m_WeaponClass(WeaponClass),
	m_Slot(0),
	m_Fire(false),
	m_NextTimeToFire(0.0)
{
	SetName(m_WeaponClass->GetName());
	SetSpaceRequirement(m_WeaponClass->GetSpaceRequirement());
	SetOrientation(m_WeaponClass->GetOrientation());
}

void Weapon::Update(float Seconds)
{
	if((m_Fire == true) && (m_NextTimeToFire <= GameTime::Get()))
	{
		Ship * TheShip(dynamic_cast< Ship * >(GetContainer()));
		
		assert(TheShip != 0);
		
		std::stringstream IdentifierStream;
		
		IdentifierStream << "::shot::created_at_game_time(" << to_string_cast(GameTime::Get(), 2) << ")::created_by(" << GetContainer()->GetObjectIdentifier() << ")::in_system(" << TheShip->GetCurrentSystem()->GetIdentifier() << ")";
		
		Shot * NewShot(new Shot(GetWeaponClass()));
		
		NewShot->SetObjectIdentifier(IdentifierStream.str());
		NewShot->SetShooter(TheShip);
		
		const Vector3f & SlotPosition(GetSlot()->GetPosition());
		
		NewShot->SetPosition(TheShip->GetPosition() + Vector2f(SlotPosition.m_V.m_A[0], SlotPosition.m_V.m_A[1]).Turned(TheShip->GetAngularPosition()));
		
		Quaternion ShipOrientation(Vector4f(0.0f, 1.0f, 0.0f, 0.0f).m_V, TheShip->GetAngularPosition());
		Quaternion SlotOrientation(GetSlot()->GetOrientation());
		Quaternion WeaponOrientation(GetOrientation());
		Quaternion OrientationModifier(ShipOrientation * SlotOrientation * WeaponOrientation);
		Vector4f ShotDirection(1.0f, 0.0f, 0.0f, 0.0f);
		
		ShotDirection *= OrientationModifier;
		
		float ShotAngularPosition(GetRadians(Vector2f(ShotDirection[0], -ShotDirection[2])));
		std::cout << "Ship's Angular Position: " << TheShip->GetAngularPosition() << std::endl << "ShipOrientation = (" << ShipOrientation[0] << ", " << ShipOrientation[1] << ", " << ShipOrientation[2] << ", " << ShipOrientation[3] << ")" << std::endl << "OrientationModifier = (" << OrientationModifier[0] << ", " << OrientationModifier[1] << ", " << OrientationModifier[2] << ", " << OrientationModifier[3] << ")" << std::endl << "ShotDirection = (" << ShotDirection[0] << ", " << ShotDirection[1] << ", " << ShotDirection[2] << ", " << ShotDirection[3] << ")" << std::endl << "ShotAngularPosition = " << ShotAngularPosition << std::endl << "-----------------------------" << std::endl;
		NewShot->SetAngularPosition(ShotAngularPosition);
		NewShot->SetVelocity(TheShip->GetVelocity() + Vector2f(GetWeaponClass()->GetParticleExitSpeed(), ShotAngularPosition, Vector2f::InitializeMagnitudeAngle));
		TheShip->GetCurrentSystem()->AddContent(NewShot);
		m_NextTimeToFire = GameTime::Get() + GetWeaponClass()->GetReloadTime();
	}
}

void Weapon::Draw(void) const
{
}
