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

#include <assert.h>

#include <sstream>

#include "color.h"
#include "game_time.h"
#include "globals.h"
#include "math.h"
#include "math/quaternion.h"
#include "object_aspect_accessory.h"
#include "object_aspect_name.h"
#include "object_aspect_position.h"
#include "ship.h"
#include "shot.h"
#include "slot.h"
#include "string_cast.h"
#include "system.h"
#include "weapon.h"
#include "weapon_class.h"
#include "visualizations.h"

Weapon::Weapon(const WeaponClass * WeaponClass) :
	m_WeaponClass(WeaponClass),
	m_Fire(false),
	m_NextTimeToFire(0.0)
{
	// initialize object aspects
	AddAspectAccessory();
	GetAspectAccessory()->SetSlotClassIdentifier(m_WeaponClass->GetSlotClassIdentifier());
	AddAspectName();
	GetAspectName()->SetName(m_WeaponClass->GetName());
	AddAspectPosition();
	GetAspectPosition()->SetOrientation(m_WeaponClass->GetOrientation());
	AddAspectVisualization();
	// other
	SetSpaceRequirement(m_WeaponClass->GetSpaceRequirement());
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
		NewShot->SetShooter(TheShip->GetReference());
		
		// calculating the shot's position in the world coordinate system
		const Vector3f & ParticleExitPosition(GetWeaponClass()->GetParticleExitPosition());
		const Vector3f & SlotPosition(GetAspectAccessory()->GetSlot()->GetPosition());
		const Vector3f & ShipPosition(TheShip->GetAspectPosition()->GetPosition());
		Vector3f ShotPosition(true);
		
		ShotPosition += ParticleExitPosition;
		ShotPosition *= GetAspectPosition()->GetOrientation();
		ShotPosition *= GetAspectAccessory()->GetSlot()->GetOrientation();
		ShotPosition += SlotPosition;
		ShotPosition *= TheShip->GetAspectPosition()->GetOrientation();
		ShotPosition += ShipPosition;
		NewShot->GetAspectPosition()->SetPosition(ShotPosition);
		
		// calculating the shot's angular position in world coordinate system
		Quaternion ShotOrientation(true);
		
		ShotOrientation *= TheShip->GetAspectPosition()->GetOrientation();
		ShotOrientation *= GetAspectAccessory()->GetSlot()->GetOrientation();
		ShotOrientation *= GetAspectPosition()->GetOrientation();
		NewShot->GetAspectPosition()->SetOrientation(ShotOrientation);
		
		// calculate the shot's velocity
		Vector3f ParticleVelocity(GetWeaponClass()->GetParticleExitSpeed(), 0.0f, 0.0f);
		
		ParticleVelocity *= ShotOrientation;
		NewShot->SetVelocity(TheShip->GetVelocity() + Vector3f(ParticleVelocity[0], ParticleVelocity[1], 0.0f));
		TheShip->GetCurrentSystem()->AddContent(NewShot);
		m_NextTimeToFire = GameTime::Get() + GetWeaponClass()->GetReloadTime();
		// add visualization
		VisualizeShot(NewShot, g_ShotLayer);
	}
}
