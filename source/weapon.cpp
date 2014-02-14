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

#include <algebra/quaternion.h>

#include "battery.h"
#include "game_time.h"
#include "globals.h"
#include "math.h"
#include "object_aspect_accessory.h"
#include "object_aspect_object_container.h"
#include "object_aspect_position.h"
#include "object_aspect_update.h"
#include "object_factory.h"
#include "ship.h"
#include "shot.h"
#include "slot.h"
#include "string_cast.h"
#include "weapon.h"

Weapon::Weapon(void) :
	m_EnergyUsagePerShot(0.0f),
	m_Fire(false),
	m_NextTimeToFire(0.0),
	m_ParticleExitPosition(true),
	m_ParticleExitSpeed(0.0f),
	m_ReloadTime(0.0f)
{
	// initialize object aspects
	AddAspectAccessory();
	AddAspectName();
	AddAspectPhysical();
	AddAspectPosition();
	AddAspectVisualization();
	AddAspectUpdate();
	GetAspectUpdate()->SetCallback(Callback(this, &Weapon::_Update));
}

bool Weapon::_Update(float Seconds)
{
	if((m_Fire == true) && (m_NextTimeToFire <= GameTime::Get()))
	{
		Object * Container(GetContainer());
		
		assert(Container != 0);
		assert(Container->GetTypeIdentifier() == "ship");
		
		Ship * TheShip(dynamic_cast< Ship * >(Container));
		
		assert(TheShip != 0);
		if((TheShip->GetBattery() != 0) && (TheShip->GetBattery()->GetEnergy() >= GetEnergyUsagePerShot()))
		{
			TheShip->GetBattery()->SetEnergy(TheShip->GetBattery()->GetEnergy() - GetEnergyUsagePerShot());
			assert(Container->GetAspectPosition() != 0);
			assert(Container->GetContainer() != 0);
			
			std::stringstream IdentifierStream;
			
			IdentifierStream << "::shot::created_at_game_time(" << to_string_cast(GameTime::Get(), 2) << ")::created_by(" << GetContainer()->GetObjectIdentifier() << ")::in_system(" << Container->GetContainer()->GetObjectIdentifier() << ")";
			
			Shot * NewShot(dynamic_cast< Shot * >(g_ObjectFactory->Create("shot", m_ShotClassIdentifier)));
			
			assert(NewShot->GetAspectPosition() != 0);
			NewShot->SetObjectIdentifier(IdentifierStream.str());
			NewShot->SetShooter(Container->GetReference());
			
			// calculating the shot's position in the world coordinate system
			const Vector3f & ParticleExitPosition(GetParticleExitPosition());
			const Vector3f & SlotPosition(GetAspectAccessory()->GetSlot()->GetPosition());
			const Vector3f & ShipPosition(Container->GetAspectPosition()->GetPosition());
			Vector3f ShotPosition(true);
			
			ShotPosition += ParticleExitPosition;
			ShotPosition *= GetAspectPosition()->GetOrientation();
			ShotPosition *= GetAspectAccessory()->GetSlot()->GetOrientation();
			ShotPosition += SlotPosition;
			ShotPosition *= Container->GetAspectPosition()->GetOrientation();
			ShotPosition += ShipPosition;
			NewShot->GetAspectPosition()->SetPosition(ShotPosition);
			
			// calculating the shot's angular position in world coordinate system
			Quaternion ShotOrientation;
			
			ShotOrientation.Identity();
			ShotOrientation *= Container->GetAspectPosition()->GetOrientation();
			ShotOrientation *= GetAspectAccessory()->GetSlot()->GetOrientation();
			ShotOrientation *= GetAspectPosition()->GetOrientation();
			NewShot->GetAspectPosition()->SetOrientation(ShotOrientation);
			
			// calculate the shot's velocity
			Vector3f ParticleVelocity(GetParticleExitSpeed(), 0.0f, 0.0f);
			
			ParticleVelocity *= ShotOrientation;
			assert(dynamic_cast< Ship * >(Container) != 0);
			NewShot->SetVelocity(dynamic_cast< Ship * >(Container)->GetVelocity() + Vector3f(ParticleVelocity[0], ParticleVelocity[1], 0.0f));
			Container->GetContainer()->GetAspectObjectContainer()->AddContent(NewShot);
			m_NextTimeToFire = GameTime::Get() + GetReloadTime();
		}
	}
	
	return true;
}
