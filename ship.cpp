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

#include <assert.h>

#include <sstream>

#include <GL/gl.h>

#include "cargo.h"
#include "character.h"
#include "color.h"
#include "commodity.h"
#include "game_time.h"
#include "globals.h"
#include "map_knowledge.h"
#include "math.h"
#include "model.h"
#include "model_manager.h"
#include "ship.h"
#include "shot.h"
#include "string_cast.h"
#include "system.h"
#include "weapon.h"

Ship::Ship(ShipClass * ShipClass) :
	m_Accelerate(false),
	m_TurnLeft(false),
	m_TurnRight(false),
	m_Jump(false),
	m_Jettison(false),
	m_Land(false),
	m_Scoop(false),
	m_Velocity(true),
	m_AngularPosition(0.0f),
	m_ShipClass(ShipClass),
	m_Fuel(0.0f),
	m_Hull(m_ShipClass->GetHull()),
	m_LinkedSystemTarget(0),
	m_CurrentSystem(0)
{
	SetRadialSize(m_ShipClass->GetModel()->GetRadialSize());
}

void Ship::Draw(void) const
{
	glPushMatrix();
	glTranslatef(m_Position.m_V.m_A[0], m_Position.m_V.m_A[1], 0.0f);
	glRotatef(m_AngularPosition * 180.0f / M_PI, 0.0f, 0.0f, 1.0f);
	glMaterialfv(GL_FRONT, GL_SPECULAR, math3d::vector4f(0.0f, 0.0f, 0.0f, 1.0f).m_V.m_A);
	if(m_ShipClass->GetColor() != 0)
	{
		glMaterialfv(GL_FRONT, GL_DIFFUSE, m_ShipClass->GetColor()->GetColor().m_V.m_A);
	}
	else
	{
		glMaterialfv(GL_FRONT, GL_DIFFUSE, math3d::vector4f(1.0f, 1.0f, 1.0f, 1.0f).m_V.m_A);
	}
	m_ShipClass->GetModel()->Draw();
	// draw acceleration visualization
	if(m_Accelerate == true)
	{
		glTranslatef(m_ShipClass->GetExhaustOffset().m_V.m_A[0], m_ShipClass->GetExhaustOffset().m_V.m_A[1], m_ShipClass->GetExhaustOffset().m_V.m_A[2]);
		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_LIGHTING);
		glEnable(GL_BLEND);
		glBegin(GL_QUADS);
		glColor4f(1.0f, 1.0f, 1.0f, 0.4f);
		glVertex2f(0.0f, -0.3f);
		glVertex2f(0.0f, 0.3f);
		glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
		glVertex2f(-20.0f, 2.0f);
		glVertex2f(-20.0f, -2.0f);
		glEnd();
		glPopAttrib();
	}
	glPopMatrix();
}

void Ship::Update(float Seconds)
{
	if(m_Jump == true)
	{
		if((GetFuel() >= GetShipClass()->GetJumpFuel()) && (GetLinkedSystemTarget() != 0))
		{
			System * OldSystem(GetCurrentSystem());
			System * NewSystem(GetLinkedSystemTarget());
			
			// remove the ship from the old system
			OldSystem->GetShips().erase(std::find(OldSystem->GetShips().begin(), OldSystem->GetShips().end(), this));
			SetCurrentSystem(0);
			SetFuel(GetFuel() - GetShipClass()->GetJumpFuel());
			
			// set the ship's position according to the old system
			math3d::vector2f Direction(NewSystem->GetPosition() - OldSystem->GetPosition());
			
			Direction.normalize();
			m_Position = Direction * -300.0f;
			m_Velocity = Direction * GetShipClass()->GetMaximumSpeed();
			m_AngularPosition = GetRadians(Direction);
			// set up the ship in the new system
			SetCurrentSystem(GetLinkedSystemTarget());
			NewSystem->AddShip(this);
			for(std::set< Object * >::iterator ManifestIterator = m_Manifest.begin(); ManifestIterator != m_Manifest.end(); ++ManifestIterator)
			{
				Character * ManifestCharacter(dynamic_cast< Character * >(*ManifestIterator));
				
				if(ManifestCharacter != 0)
				{
					ManifestCharacter->GetMapKnowledge()->AddExploredSystem(NewSystem);
				}
			}
			SetLinkedSystemTarget(0);
			SetTarget(0);
			m_Jump = false;
		}
	}
	else if(m_Land == true)
	{
		SetLinkedSystemTarget(0);
		SetTarget(0);
		m_Velocity.set(0.0f, 0.0f);
		m_Accelerate = false;
		m_TurnLeft = false;
		m_TurnRight = false;
		m_Land = false;
	}
	else
	{
		for(std::vector< Weapon * >::size_type WeaponIndex = 0; WeaponIndex < m_Weapons.size(); ++WeaponIndex)
		{
			m_Weapons[WeaponIndex]->Update(Seconds);
		}
		if(m_TurnLeft == true)
		{
			float FuelConsumption(m_ShipClass->GetTurnFuel() * Seconds);
			
			if(m_Fuel >= FuelConsumption)
			{
				m_AngularPosition += GetTurnSpeed() * Seconds;
				m_Fuel -= FuelConsumption;
			}
		}
		if(m_TurnRight == true)
		{
			float FuelConsumption(m_ShipClass->GetTurnFuel() * Seconds);
			
			if(m_Fuel >= FuelConsumption)
			{
				m_AngularPosition -= GetTurnSpeed() * Seconds;
				m_Fuel -= FuelConsumption;
			}
		}
		m_Position += m_Velocity * Seconds;
		if(m_Accelerate == true)
		{
			float FuelConsumption(m_ShipClass->GetForwardFuel() * Seconds);
			
			if(m_Fuel >= FuelConsumption)
			{
				math3d::vector2f ForwardThrust(GetForwardThrust(), m_AngularPosition, math3d::vector2f::magnitude_angle);
				
				m_Position += ForwardThrust * (Seconds * Seconds / 2.0f);
				m_Velocity += ForwardThrust * Seconds;
				if(m_Velocity.length() > GetMaximumSpeed())
				{
					m_Velocity.normalize();
					m_Velocity.scale(GetMaximumSpeed());
				}
				m_Fuel -= FuelConsumption;
			}
		}
		if(m_Jettison == true)
		{
			std::set< Object * >::iterator ManifestIterator;
			std::set< Object * >::iterator NextIterator(GetManifest().begin());
			
			while(NextIterator != GetManifest().end())
			{
				ManifestIterator = NextIterator;
				++NextIterator;
				
				Cargo * TheCargo(dynamic_cast< Cargo * >(*ManifestIterator));
				
				if(TheCargo != 0)
				{
					GetManifest().erase(ManifestIterator);
					TheCargo->SetPosition(GetPosition());
					TheCargo->SetVelocity(GetVelocity() * 0.8f + math3d::vector2f(GetRandomFloat(-0.5f, 0.5f), GetRandomFloat(-0.5f, 0.5f)));
					GetCurrentSystem()->AddCargo(TheCargo);
				}
			}
			m_Jettison = false;
		}
		if(m_Scoop == true)
		{
			Cargo * SelectedCargo(dynamic_cast< Cargo * >(GetTarget().Get()));
			
			if(SelectedCargo != 0)
			{
				if(AddObject(SelectedCargo) == true)
				{
					GetCurrentSystem()->GetCargos().erase(std::find(GetCurrentSystem()->GetCargos().begin(), GetCurrentSystem()->GetCargos().end(), SelectedCargo));
					SetTarget(0);
				}
			}
			m_Scoop = false;
		}
	}
}

float Ship::GetFuelCapacity(void) const
{
	return m_ShipClass->GetFuelHoldSize();
}

float Ship::GetFreeCargoHoldSize(void) const
{
	float CargoHoldSize(m_ShipClass->GetCargoHoldSize());
	std::set< Object * >::const_iterator ManifestIterator(GetManifest().begin());
	
	while(ManifestIterator != GetManifest().end())
	{
		if(dynamic_cast< Cargo * >(*ManifestIterator) != 0)
		{
			CargoHoldSize -= 1.0f;
		}
		++ManifestIterator;
	}
	
	return CargoHoldSize;
}

float Ship::GetCommodityAmount(const Commodity * CargoCommodity) const
{
	float Amount(0.0f);
	std::set< Object * >::const_iterator ManifestIterator(GetManifest().begin());
	
	while(ManifestIterator != GetManifest().end())
	{
		Cargo * TheCargo(dynamic_cast< Cargo * >(*ManifestIterator));
		
		if((TheCargo != 0) && (TheCargo->GetCommodity() == CargoCommodity))
		{
			Amount += 1.0f;
		}
		++ManifestIterator;
	}
	
	return Amount;
}

void Ship::SetFire(bool Fire)
{
	for(std::vector< Weapon * >::size_type WeaponIndex = 0; WeaponIndex < m_Weapons.size(); ++WeaponIndex)
	{
		m_Weapons[WeaponIndex]->SetFire(Fire);
	}
}

bool Ship::AddObject(Object * Add)
{
	Cargo * TheCargo(dynamic_cast< Cargo * >(Add));
	
	if(TheCargo != 0)
	{
		if(GetFreeCargoHoldSize() < 1.0f)
		{
			return false;
		}
	}
	
	Weapon * TheWeapon(dynamic_cast< Weapon * >(Add));
	
	if(TheWeapon != 0)
	{
		m_Weapons.push_back(TheWeapon);
		TheWeapon->SetShip(this);
	}
	m_Manifest.insert(Add);
	
	return true;
}

bool Ship::RemoveObject(Object * Remove)
{
	std::set< Object * >::iterator ManifestIterator(m_Manifest.find(Remove));
	
	if(ManifestIterator != m_Manifest.end())
	{
		Weapon * TheWeapon(dynamic_cast< Weapon * >(Remove));
		
		if(TheWeapon != 0)
		{
			TheWeapon->SetShip(0);
			m_Weapons.erase(std::find(m_Weapons.begin(), m_Weapons.end(), TheWeapon));
		}
		m_Manifest.erase(ManifestIterator);
		
		return true;
	}
	else
	{
		return false;
	}
}
