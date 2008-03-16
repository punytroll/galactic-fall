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

#include "character.h"
#include "color.h"
#include "commodity.h"
#include "commodity_class.h"
#include "game_time.h"
#include "globals.h"
#include "graphics_model.h"
#include "graphics_particle_system.h"
#include "map_knowledge.h"
#include "math.h"
#include "message.h"
#include "message_dispatcher.h"
#include "object_aspect_accessory.h"
#include "object_aspect_object_container.h"
#include "object_aspect_physical.h"
#include "object_aspect_position.h"
#include "object_aspect_update.h"
#include "object_aspect_visualization.h"
#include "planet.h"
#include "ship.h"
#include "shot.h"
#include "slot.h"
#include "slot_class.h"
#include "system.h"
#include "visualizations.h"
#include "weapon.h"
#include "weapon_class.h"

Ship::Ship(const ShipClass * ShipClass) :
	m_Jettison(false),
	m_Jump(false),
	m_Land(false),
	m_Scoop(false),
	m_Accelerate(false),
	m_CurrentSystem(0),
	m_Fuel(0.0f),
	m_Hull(0.0f),
	m_LinkedSystemTarget(0),
	m_Refuel(false),
	m_ShipClass(ShipClass),
	m_TakeOff(false),
	m_TurnLeft(0.0f),
	m_TurnRight(0.0f),
	m_Velocity(true)
{
	// initialize object aspects
	AddAspectName();
	AddAspectObjectContainer();
	AddAspectPhysical();
	GetAspectPhysical()->SetRadialSize(m_ShipClass->GetModel()->GetRadialSize());
	AddAspectPosition();
	AddAspectUpdate();
	GetAspectUpdate()->SetCallback(Method(this, &Ship::Update));
	AddAspectVisualization();
	// other
	SetHull(m_ShipClass->GetHull());
	
	const std::map< std::string, Slot * > & ShipClassSlots(GetShipClass()->GetSlots());
	
	for(std::map< std::string, Slot * >::const_iterator SlotIterator = ShipClassSlots.begin(); SlotIterator != ShipClassSlots.end(); ++SlotIterator)
	{
		Slot * NewSlot(CreateSlot(SlotIterator->second->GetSlotClass(), SlotIterator->first));
		
		NewSlot->SetName(SlotIterator->second->GetName());
		NewSlot->SetPosition(SlotIterator->second->GetPosition());
		NewSlot->SetOrientation(SlotIterator->second->GetOrientation());
	}
}

Ship::~Ship(void)
{
	while(m_Slots.empty() == false)
	{
		delete m_Slots.begin()->second;
		m_Slots.erase(m_Slots.begin());
	}
}

float Ship::GetAvailableSpace(void) const
{
	assert(GetAspectObjectContainer() != 0);
	
	float AvailableSpace(m_ShipClass->GetMaximumAvailableSpace());
	const std::set< Object * > & Content(GetAspectObjectContainer()->GetContent());
	
	for(std::set< Object * >::const_iterator ContentIterator = Content.begin(); ContentIterator != Content.end(); ++ContentIterator)
	{
		if((*ContentIterator)->GetAspectPhysical() != 0)
		{
			if((*ContentIterator)->GetAspectAccessory() != 0)
			{
				if((*ContentIterator)->GetAspectAccessory()->GetSlot() == 0)
				{
					AvailableSpace -= (*ContentIterator)->GetAspectPhysical()->GetSpaceRequirement();
				}
			}
			else
			{
				AvailableSpace -= (*ContentIterator)->GetAspectPhysical()->GetSpaceRequirement();
			}
		}
	}
	
	return AvailableSpace;
}

unsigned_numeric Ship::GetContentAmount(const std::string & TypeIdentifier, const std::string & ClassIdentifier) const
{
	assert(GetAspectObjectContainer() != 0);
	
	unsigned_numeric Amount(0);
	const std::set< Object * > & Content(GetAspectObjectContainer()->GetContent());
	
	for(std::set< Object * >::const_iterator ContentIterator = Content.begin(); ContentIterator != Content.end(); ++ContentIterator)
	{
		const Object * Content(*ContentIterator);
		
		if((Content->GetTypeIdentifier() == TypeIdentifier) && (Content->GetClassIdentifier() == ClassIdentifier) && ((Content->GetAspectAccessory() == 0) || (Content->GetAspectAccessory()->GetSlot() == 0)))
		{
			Amount += 1;
		}
	}
	
	return Amount;
}

float Ship::GetFuelCapacity(void) const
{
	return m_ShipClass->GetFuelHoldSize();
}

void Ship::SetFire(bool Fire)
{
	for(std::map< std::string, Slot * >::iterator SlotIterator = m_Slots.begin(); SlotIterator != m_Slots.end(); ++SlotIterator)
	{
		Object * Object(SlotIterator->second->GetMountedObject().Get());
		
		if((Object != 0) && (Object->GetTypeIdentifier() == "weapon"))
		{
			dynamic_cast< Weapon * >(Object)->SetFire(Fire);
		}
	}
}

bool Ship::Update(float Seconds)
{
	if(m_Jump == true)
	{
		if((GetFuel() >= GetShipClass()->GetJumpFuel()) && (GetLinkedSystemTarget() != 0))
		{
			System * OldSystem(GetCurrentSystem());
			System * NewSystem(GetLinkedSystemTarget());
			
			// remove the ship from the old system
			assert(OldSystem->GetAspectObjectContainer() != 0);
			OldSystem->GetAspectObjectContainer()->RemoveContent(this);
			SetCurrentSystem(0);
			SetFuel(GetFuel() - GetShipClass()->GetJumpFuel());
			
			// set the ship's position according to the old system
			Vector3f Direction(NewSystem->GetAspectPosition()->GetPosition() - OldSystem->GetAspectPosition()->GetPosition());
			
			Direction.Normalize();
			GetAspectPosition()->SetPosition(Direction * -300.0f);
			m_Velocity = Direction * GetShipClass()->GetMaximumSpeed();
			GetAspectPosition()->SetOrientation(Quaternion(GetRadians(Vector2f(Direction[0], Direction[1])), Quaternion::InitializeRotationZ));
			// set up the ship in the new system
			SetCurrentSystem(GetLinkedSystemTarget());
			assert(NewSystem->GetAspectObjectContainer() != 0);
			NewSystem->GetAspectObjectContainer()->AddContent(this);
			assert(GetAspectObjectContainer() != 0);
			
			const std::set< Object * > & Content(GetAspectObjectContainer()->GetContent());
			
			for(std::set< Object * >::iterator ContentIterator = Content.begin(); ContentIterator != Content.end(); ++ContentIterator)
			{
				Character * TheCharacter(dynamic_cast< Character * >(*ContentIterator));
				
				if(TheCharacter != 0)
				{
					TheCharacter->GetMapKnowledge()->AddExploredSystem(NewSystem);
				}
			}
			SetLinkedSystemTarget(0);
			SetTarget(0);
			m_Jump = false;
		}
	}
	else if(m_Land == true)
	{
		m_Land = false;
		
		Planet * ThePlanet(dynamic_cast< Planet * >(GetTarget().Get()));
		
		assert(ThePlanet != 0);
		ThePlanet->Land(this);
		m_Accelerate = false;
		m_TurnLeft = 0.0f;
		m_TurnRight = 0.0f;
	}
	else if(m_TakeOff == true)
	{
		m_TakeOff = false;
		
		Planet * ThePlanet(dynamic_cast< Planet * >(GetTarget().Get()));
		
		assert(ThePlanet != 0);
		ThePlanet->TakeOff(this);
		SetTarget(0);
		SetLinkedSystemTarget(0);
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
			assert(GetAspectObjectContainer() != 0);
			
			const std::set< Object * > & Content(GetAspectObjectContainer()->GetContent());
			
			for(std::set< Object * >::const_iterator ContentIterator = Content.begin(); ContentIterator != Content.end(); ++ContentIterator)
			{
				Commodity * TheCommodity(dynamic_cast< Commodity * >(*ContentIterator));
				
				if((TheCommodity != 0) && (TheCommodity->GetCommodityClass()->GetIdentifier() == "fuel"))
				{
					SetFuel(Clamp(GetFuel() + 1.0f, 0.0f, GetFuelCapacity()));
					TheCommodity->Destroy();
					delete TheCommodity;
					
					break;
				}
			}
			m_Refuel = false;
		}
		if(m_TurnLeft > 0.0f)
		{
			float FuelConsumption(m_ShipClass->GetTurnFuel() * Seconds * m_TurnLeft);
			
			if(GetFuel() >= FuelConsumption)
			{
				GetAspectPosition()->ModifyOrientation(Quaternion(GetShipClass()->GetTurnSpeed() * Seconds * m_TurnLeft, Quaternion::InitializeRotationZ));
				SetFuel(GetFuel() - FuelConsumption);
			}
		}
		if(m_TurnRight > 0.0f)
		{
			float FuelConsumption(m_ShipClass->GetTurnFuel() * Seconds * m_TurnRight);
			
			if(GetFuel() >= FuelConsumption)
			{
				GetAspectPosition()->ModifyOrientation(Quaternion(-GetShipClass()->GetTurnSpeed() * Seconds * m_TurnRight, Quaternion::InitializeRotationZ));
				SetFuel(GetFuel() - FuelConsumption);
			}
		}
		GetAspectPosition()->ModifyPosition(m_Velocity * Seconds);
		if(m_Accelerate == true)
		{
			float FuelConsumption(m_ShipClass->GetForwardFuel() * Seconds);
			
			if(GetFuel() >= FuelConsumption)
			{
				Vector3f ForwardThrust(GetShipClass()->GetForwardThrust(), 0.0f, 0.0f);
				
				ForwardThrust *= GetAspectPosition()->GetOrientation();
				ForwardThrust *= Seconds;
				m_Velocity += Vector3f(ForwardThrust[0], ForwardThrust[1], 0.0f);
				ForwardThrust *= 0.5f * Seconds;
				GetAspectPosition()->ModifyPosition(Vector3f(ForwardThrust[0], ForwardThrust[1], 0.0f));
				if(m_Velocity.Length() > GetShipClass()->GetMaximumSpeed())
				{
					m_Velocity.Normalize();
					m_Velocity *= GetShipClass()->GetMaximumSpeed();
				}
				SetFuel(GetFuel() - FuelConsumption);
				if(m_EngineGlowParticleSystem.IsValid() == true)
				{
					for(int I = 0; I < 4; ++I)
					{
						Graphics::ParticleSystem::Particle Particle;
						
						Particle.m_TimeOfDeath = GameTime::Get() + GetRandomFloat(0.5f, 0.9f);
						Particle.m_Color = Color(GetRandomFloat(0.8f, 1.0f), GetRandomFloat(0.7f, 0.8f), 0.5f, 0.08f);
						
						float X;
						float Y;
						float VelocityFactor;
						
						do
						{
							X = GetRandomFloat(-1.0f, 1.0f);
							Y = GetRandomFloat(-1.0f, 1.0f);
						} while(X * X + Y * Y >= 1.0f);
						VelocityFactor = 1 - X * X - Y * Y;
						X *= GetShipClass()->GetExhaustRadius();
						Y *= GetShipClass()->GetExhaustRadius();
						Particle.m_Position = Vector3f(0.0f, X, Y);
						Particle.m_Velocity = Vector3f(GetRandomFloat(-0.3f * VelocityFactor, 0.0f), 0.0f, 0.0f);
						Particle.m_Size = 0.2f;
						m_EngineGlowParticleSystem->AddParticle(Particle);
					}
				}
			}
		}
		if(m_Jettison == true)
		{
			assert(GetAspectObjectContainer() != 0);
			
			std::set< Object * >::const_iterator ContentIterator(GetAspectObjectContainer()->GetContent().begin());
			
			while(ContentIterator != GetAspectObjectContainer()->GetContent().end())
			{
				Commodity * TheCommodity(dynamic_cast< Commodity * >(*ContentIterator));
				
				++ContentIterator;
				if(TheCommodity != 0)
				{
					GetAspectObjectContainer()->RemoveContent(TheCommodity);
					TheCommodity->GetAspectPosition()->SetPosition(GetAspectPosition()->GetPosition());
					TheCommodity->SetVelocity(GetVelocity() * 0.8f + Vector3f(GetRandomFloat(-0.5f, 0.5f), GetRandomFloat(-0.5f, 0.5f), 0.0f));
					assert(GetCurrentSystem()->GetAspectObjectContainer() != 0);
					GetCurrentSystem()->GetAspectObjectContainer()->AddContent(TheCommodity);
					VisualizeCommodity(TheCommodity, g_CommodityLayer);
				}
			}
			m_Jettison = false;
		}
		if(m_Scoop == true)
		{
			Commodity * SelectedCommodity(dynamic_cast< Commodity * >(m_Target.Get()));
			
			if(SelectedCommodity != 0)
			{
				assert(GetCurrentSystem()->GetAspectObjectContainer() != 0);
				// the following is a typical occasion of bad practise: a transaction would be great here
				if(GetCurrentSystem()->GetAspectObjectContainer()->RemoveContent(SelectedCommodity) == true)
				{
					assert(GetAspectObjectContainer() != 0);
					if(GetAspectObjectContainer()->AddContent(SelectedCommodity) == true)
					{
						SetTarget(0);
						if(SelectedCommodity->GetAspectVisualization()->GetVisualization().IsValid() == true)
						{
							UnvisualizeObject(SelectedCommodity);
						}
					}
					else
					{
						GetCurrentSystem()->GetAspectObjectContainer()->AddContent(SelectedCommodity);
					}
				}
			}
			m_Scoop = false;
		}
	}
	
	return true;
}

Slot * Ship::CreateSlot(const SlotClass * SlotClass, const std::string & SlotIdentifier)
{
	assert(m_Slots.find(SlotIdentifier) == m_Slots.end());
	
	Slot * NewSlot(new Slot(SlotClass, SlotIdentifier));
	
	m_Slots[SlotIdentifier] = NewSlot;
	
	return NewSlot;
}

void Ship::Mount(Object * Object, const std::string & SlotIdentifier)
{
	assert(Object != 0);
	assert(GetAspectObjectContainer() != 0);
	assert(GetAspectObjectContainer()->GetContent().find(Object) != GetAspectObjectContainer()->GetContent().end());
	
	std::map< std::string, Slot * >::iterator SlotIterator(m_Slots.find(SlotIdentifier));
	
	assert(SlotIterator != m_Slots.end());
	assert(SlotIterator->second->GetSlotClass()->AcceptsSlotClassIdentifier(Object->GetAspectAccessory()->GetSlotClassIdentifier()) == true);
	SlotIterator->second->SetMountedObject(Object->GetReference());
	Object->GetAspectAccessory()->SetSlot(SlotIterator->second);
	
	Weapon * TheWeapon(dynamic_cast< Weapon * >(Object));
	
	if((TheWeapon != 0) && (GetAspectVisualization()->GetVisualization().IsValid() == true))
	{
		// visualize all weapons on ships that are already visualized
		VisualizeWeapon(TheWeapon, GetAspectVisualization()->GetVisualization().Get());
	}
}

void Ship::Unmount(const std::string & SlotIdentifier)
{
	std::map< std::string, Slot * >::iterator SlotIterator(m_Slots.find(SlotIdentifier));
	
	assert(SlotIterator != m_Slots.end());
	
	Object * TheObject(SlotIterator->second->GetMountedObject().Get());
	
	assert(TheObject != 0);
	
	float AvailableSpace(GetAvailableSpace());
	
	assert(TheObject->GetAspectPhysical() != 0);
	assert(AvailableSpace >= TheObject->GetAspectPhysical()->GetSpaceRequirement());
	SlotIterator->second->SetMountedObject(0);
	TheObject->GetAspectAccessory()->SetSlot(0);
	if((TheObject->GetAspectVisualization() != 0) && (TheObject->GetAspectVisualization()->GetVisualization().IsValid() == true))
	{
		UnvisualizeObject(TheObject);
	}
}

void Ship::SetFuel(float Fuel)
{
	float FuelThreshold(GetShipClass()->GetFuelHoldSize() * 0.2);
	bool FuelLow((Fuel < FuelThreshold) && (GetFuel() > FuelThreshold));
	
	m_Fuel = Fuel;
	if(FuelLow == true)
	{
		// send message to all characters on the ship
		assert(GetAspectObjectContainer() != 0);
		
		const std::set< Object * > & ShipContent(GetAspectObjectContainer()->GetContent());
		
		for(std::set< Object * >::iterator ContentIterator = ShipContent.begin(); ContentIterator != ShipContent.end(); ++ContentIterator)
		{
			Character * TheCharacter(dynamic_cast< Character * >(*ContentIterator));
			
			if(TheCharacter != 0)
			{
				g_MessageDispatcher->PushMessage(new Message("fuel_low", GetReference(), TheCharacter->GetReference()));
			}
		}
	}
}

void Ship::SetHull(float Hull)
{
	float HullThreshold(GetShipClass()->GetHull() * 0.33);
	bool HullLow((Hull < HullThreshold) && (GetHull() > HullThreshold));
	
	m_Hull = Hull;
	if(HullLow == true)
	{
		// send message to all characters on the ship
		assert(GetAspectObjectContainer() != 0);
		
		const std::set< Object * > & ShipContent(GetAspectObjectContainer()->GetContent());
		
		for(std::set< Object * >::iterator ContentIterator = ShipContent.begin(); ContentIterator != ShipContent.end(); ++ContentIterator)
		{
			Character * TheCharacter(dynamic_cast< Character * >(*ContentIterator));
			
			if(TheCharacter != 0)
			{
				g_MessageDispatcher->PushMessage(new Message("hull_low", GetReference(), TheCharacter->GetReference()));
			}
		}
	}
}
