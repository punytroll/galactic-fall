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

#include "character.h"
#include "color.h"
#include "commodity.h"
#include "commodity_class.h"
#include "game_time.h"
#include "globals.h"
#include "map_knowledge.h"
#include "math.h"
#include "model.h"
#include "model_manager.h"
#include "ship.h"
#include "shot.h"
#include "slot.h"
#include "string_cast.h"
#include "system.h"
#include "weapon.h"
#include "weapon_class.h"

Ship::Ship(const ShipClass * ShipClass) :
	m_Accelerate(false),
	m_Accelerating(false),
	m_Jettison(false),
	m_Jump(false),
	m_Land(false),
	m_Refuel(false),
	m_Scoop(false),
	m_AngularPosition(0.0f),
	m_ShipClass(ShipClass),
	m_Fuel(0.0f),
	m_Hull(m_ShipClass->GetHull()),
	m_TurnLeft(0.0f),
	m_TurnRight(0.0f),
	m_LinkedSystemTarget(0),
	m_CurrentSystem(0),
	m_Velocity(true)
{
	SetRadialSize(m_ShipClass->GetModel()->GetRadialSize());
	
	const std::map< std::string, Slot * > & ShipClassSlots(GetShipClass()->GetSlots());
	
	for(std::map< std::string, Slot * >::const_iterator SlotIterator = ShipClassSlots.begin(); SlotIterator != ShipClassSlots.end(); ++SlotIterator)
	{
		Slot * NewSlot(CreateSlot(SlotIterator->first));
		
		NewSlot->SetType(SlotIterator->second->GetType());
		NewSlot->SetPosition(SlotIterator->second->GetPosition());
	}
}

void Ship::Draw(void) const
{
	glPushMatrix();
	glTranslatef(m_Position.m_V.m_A[0], m_Position.m_V.m_A[1], 0.0f);
	glRotatef(m_AngularPosition * 180.0f / M_PI, 0.0f, 0.0f, 1.0f);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Vector4f(0.0f, 0.0f, 0.0f, 1.0f).m_V.m_A);
	if(m_ShipClass->GetColor() != 0)
	{
		glMaterialfv(GL_FRONT, GL_DIFFUSE, m_ShipClass->GetColor()->GetColor().m_V.m_A);
	}
	else
	{
		glMaterialfv(GL_FRONT, GL_DIFFUSE, Vector4f(1.0f, 1.0f, 1.0f, 1.0f).m_V.m_A);
	}
	m_ShipClass->GetModel()->Draw();
	// draw acceleration visualization
	if(m_Accelerating == true)
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
			OldSystem->RemoveContent(this);
			SetCurrentSystem(0);
			SetFuel(GetFuel() - GetShipClass()->GetJumpFuel());
			
			// set the ship's position according to the old system
			Vector2f Direction(NewSystem->GetPosition() - OldSystem->GetPosition());
			
			Direction.Normalize();
			m_Position = Direction * -300.0f;
			m_Velocity = Direction * GetShipClass()->GetMaximumSpeed();
			m_AngularPosition = GetRadians(Direction);
			// set up the ship in the new system
			SetCurrentSystem(GetLinkedSystemTarget());
			NewSystem->AddContent(this);
			for(std::set< Object * >::iterator ManifestIterator = GetContent().begin(); ManifestIterator != GetContent().end(); ++ManifestIterator)
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
		m_Velocity.Set(0.0f, 0.0f);
		m_Accelerate = false;
		m_TurnLeft = 0.0f;
		m_TurnRight = 0.0f;
		m_Land = false;
	}
	else
	{
		for(std::map< std::string, Slot * >::iterator SlotIterator = m_Slots.begin(); SlotIterator != m_Slots.end(); ++SlotIterator)
		{
			// only update *mounted* *weapons*
			Weapon * TheWeapon(dynamic_cast< Weapon * >(SlotIterator->second->GetMountedObject().Get()));
			
			if(TheWeapon != 0)
			{
				TheWeapon->Update(Seconds);
			}
		}
		if(m_Refuel == true)
		{
			std::set< Object * >::const_iterator ContentIterator(GetContent().begin());
			
			while(ContentIterator != GetContent().end())
			{
				Commodity * TheCommodity(dynamic_cast< Commodity * >(*ContentIterator));
				
				if((TheCommodity != 0) && (TheCommodity->GetCommodityClass()->GetIdentifier() == "fuel"))
				{
					m_Fuel = Clamp(m_Fuel + 1.0f, 0.0f, GetFuelCapacity());
					TheCommodity->Destroy();
					delete TheCommodity;
					
					break;
				}
				++ContentIterator;
			}
			m_Refuel = false;
		}
		if(m_TurnLeft > 0.0f)
		{
			float FuelConsumption(m_ShipClass->GetTurnFuel() * Seconds * m_TurnLeft);
			
			if(m_Fuel >= FuelConsumption)
			{
				m_AngularPosition += GetTurnSpeed() * Seconds * m_TurnLeft;
				m_Fuel -= FuelConsumption;
			}
		}
		if(m_TurnRight > 0.0f)
		{
			float FuelConsumption(m_ShipClass->GetTurnFuel() * Seconds * m_TurnRight);
			
			if(m_Fuel >= FuelConsumption)
			{
				m_AngularPosition -= GetTurnSpeed() * Seconds * m_TurnRight;
				m_Fuel -= FuelConsumption;
			}
		}
		m_Position += m_Velocity * Seconds;
		m_Accelerating = false;
		if(m_Accelerate == true)
		{
			float FuelConsumption(m_ShipClass->GetForwardFuel() * Seconds);
			
			if(m_Fuel >= FuelConsumption)
			{
				Vector2f ForwardThrust(GetForwardThrust(), m_AngularPosition, Vector2f::InitializeMagnitudeAngle);
				
				m_Position += ForwardThrust * (Seconds * Seconds / 2.0f);
				m_Velocity += ForwardThrust * Seconds;
				if(m_Velocity.Length() > GetMaximumSpeed())
				{
					m_Velocity.Normalize();
					m_Velocity.Scale(GetMaximumSpeed());
				}
				m_Fuel -= FuelConsumption;
				m_Accelerating = true;
			}
		}
		if(m_Jettison == true)
		{
			std::set< Object * >::const_iterator ManifestIterator;
			std::set< Object * >::const_iterator NextIterator(GetContent().begin());
			
			while(NextIterator != GetContent().end())
			{
				ManifestIterator = NextIterator;
				++NextIterator;
				
				Commodity * TheCommodity(dynamic_cast< Commodity * >(*ManifestIterator));
				
				if(TheCommodity != 0)
				{
					RemoveContent(TheCommodity);
					TheCommodity->SetPosition(GetPosition());
					TheCommodity->SetVelocity(GetVelocity() * 0.8f + Vector2f(GetRandomFloat(-0.5f, 0.5f), GetRandomFloat(-0.5f, 0.5f)));
					GetCurrentSystem()->AddContent(TheCommodity);
				}
			}
			m_Jettison = false;
		}
		if(m_Scoop == true)
		{
			Commodity * SelectedCommodity(dynamic_cast< Commodity * >(GetTarget().Get()));
			
			if(SelectedCommodity != 0)
			{
				if(GetCurrentSystem()->RemoveContent(SelectedCommodity) == true)
				{
					if(AddContent(SelectedCommodity) == true)
					{
						SetTarget(0);
					}
					else
					{
						GetCurrentSystem()->AddContent(SelectedCommodity);
					}
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

float Ship::GetAvailableSpace(void) const
{
	float AvailableSpace(m_ShipClass->GetMaximumAvailableSpace());
	std::set< Object * >::const_iterator ManifestIterator(GetContent().begin());
	
	while(ManifestIterator != GetContent().end())
	{
		if(dynamic_cast< Commodity * >(*ManifestIterator) != 0)
		{
			AvailableSpace -= dynamic_cast< Commodity * >(*ManifestIterator)->GetSpaceRequirement();
		}
		else if(dynamic_cast< Weapon * >(*ManifestIterator) != 0)
		{
			if(dynamic_cast< Weapon * >(*ManifestIterator)->GetSlot() == 0)
			{
				AvailableSpace -= dynamic_cast< Weapon * >(*ManifestIterator)->GetSpaceRequirement();
			}
		}
		++ManifestIterator;
	}
	
	return AvailableSpace;
}

unsigned_numeric Ship::GetContentAmount(const std::string & Type, const std::string & Class) const
{
	unsigned_numeric Amount(0);
	std::set< Object * >::const_iterator ContentIterator(GetContent().begin());
	
	while(ContentIterator != GetContent().end())
	{
		if(Type == "commodity")
		{
			Commodity * TheCommodity(dynamic_cast< Commodity * >(*ContentIterator));
			
			if((TheCommodity != 0) && (TheCommodity->GetCommodityClass()->GetIdentifier() == Class))
			{
				Amount += 1;
			}
		}
		else if(Type == "weapon")
		{
			Weapon * TheWeapon(dynamic_cast< Weapon * >(*ContentIterator));
			
			if((TheWeapon != 0) && (TheWeapon->GetWeaponClass()->GetIdentifier() == Class))
			{
				Amount += 1;
			}
		}
		++ContentIterator;
	}
	
	return Amount;
}

void Ship::SetFire(bool Fire)
{
	for(std::map< std::string, Slot * >::iterator SlotIterator = m_Slots.begin(); SlotIterator != m_Slots.end(); ++SlotIterator)
	{
		// only set firing on *mounted* *weapons*
		Weapon * TheWeapon(dynamic_cast< Weapon * >(SlotIterator->second->GetMountedObject().Get()));
		
		if(TheWeapon != 0)
		{
			TheWeapon->SetFire(Fire);
		}
	}
}

Slot * Ship::CreateSlot(const std::string & SlotIdentifier)
{
	if(m_Slots.find(SlotIdentifier) == m_Slots.end())
	{
		Slot * NewSlot(new Slot(SlotIdentifier));
		
		m_Slots[SlotIdentifier] = NewSlot;
		
		return NewSlot;
	}
	else
	{
		return 0;
	}
}

bool Ship::Mount(Object * Object, const std::string & SlotIdentifier)
{
	std::map< std::string, Slot * >::iterator SlotIterator(m_Slots.find(SlotIdentifier));
	
	if((GetContent().find(Object) != GetContent().end()) && (SlotIterator != m_Slots.end()))
	{
		Weapon * TheWeapon(dynamic_cast< Weapon * >(Object));
		
		if((TheWeapon != 0) && (TheWeapon->GetWeaponClass()->GetSlotType() == SlotIterator->second->GetType()))
		{
			SlotIterator->second->SetMountedObject(TheWeapon->GetReference());
			TheWeapon->SetSlot(SlotIterator->second);
			
			return true;
		}
	}
	
	return false;
}
