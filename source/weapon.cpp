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
#include <stdexcept>

#include <GL/gl.h>

#include "color.h"
#include "game_time.h"
#include "globals.h"
#include "graphics_model_object.h"
#include "math.h"
#include "math/matrix4f.h"
#include "math/quaternion.h"
#include "math/vector4f.h"
#include "model.h"
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
	m_Orientation(true),
	m_Slot(0),
	m_Fire(false),
	m_NextTimeToFire(0.0)
{
	SetName(m_WeaponClass->GetName());
	SetSpaceRequirement(m_WeaponClass->GetSpaceRequirement());
	SetOrientation(m_WeaponClass->GetOrientation());
	SetPosition(Vector3f(0.0f, 0.0f, 0.0f));
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
		
		// calculating the shot's position in the world coordinate system
		const Vector3f & ParticleExitPosition(GetWeaponClass()->GetParticleExitPosition());
		const Vector3f & SlotPosition(GetSlot()->GetPosition());
		const Vector3f & ShipPosition(TheShip->GetPosition());
		Vector3f ShotPosition(true);
		
		ShotPosition += ParticleExitPosition;
		ShotPosition *= GetOrientation();
		ShotPosition *= GetSlot()->GetOrientation();
		ShotPosition += SlotPosition;
		ShotPosition *= TheShip->GetAngularPosition();
		ShotPosition += ShipPosition;
		NewShot->SetPosition(ShotPosition);
		
		// calculating the shot's angular position in world coordinate system
		Quaternion ShotAngularPosition(true);
		
		ShotAngularPosition *= TheShip->GetAngularPosition();
		ShotAngularPosition *= GetSlot()->GetOrientation();
		ShotAngularPosition *= GetOrientation();
		NewShot->SetAngularPosition(ShotAngularPosition);
		
		// calculate the shot's velocity
		Vector3f ParticleVelocity(GetWeaponClass()->GetParticleExitSpeed(), 0.0f, 0.0f);
		
		ParticleVelocity *= ShotAngularPosition;
		NewShot->SetVelocity(TheShip->GetVelocity() + Vector3f(ParticleVelocity[0], ParticleVelocity[1], 0.0f));
		TheShip->GetCurrentSystem()->AddContent(NewShot);
		m_NextTimeToFire = GameTime::Get() + GetWeaponClass()->GetReloadTime();
		// add visualization
		VisualizeShot(NewShot, g_ShotLayer);
	}
}

void Weapon::Draw(void) const
{
	// the OpenGL context is positioned at the containers origin and rotated according to the conatiners orientation
	if(GetWeaponClass()->GetModel() != 0)
	{
		glMaterialfv(GL_FRONT, GL_SPECULAR, Vector4f(0.0f, 0.0f, 0.0f, 1.0f).m_V.m_A);
		if(GetWeaponClass()->GetModelColor() != 0)
		{
			glMaterialfv(GL_FRONT, GL_DIFFUSE, GetWeaponClass()->GetModelColor()->GetColor().m_V.m_A);
		}
		else
		{
			glMaterialfv(GL_FRONT, GL_DIFFUSE, Vector4f(1.0f, 1.0f, 1.0f, 1.0f).m_V.m_A);
		}
		
		const Vector3f & SlotPosition(GetSlot()->GetPosition());
		
		glPushMatrix();
		glTranslatef(SlotPosition[0], SlotPosition[1], SlotPosition[2]);
		
		Quaternion WeaponOrientation(GetSlot()->GetOrientation());
		
		WeaponOrientation *= GetOrientation();
		glMultMatrixf(Matrix4f(WeaponOrientation).Transpose().Matrix());
		GetWeaponClass()->GetModel()->Draw();
		glPopMatrix();
	}
}
