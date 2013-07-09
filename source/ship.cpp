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

#include "battery.h"
#include "character.h"
#include "commodity.h"
#include "game_time.h"
#include "generator.h"
#include "globals.h"
#include "graphics/particle_system.h"
#include "map_knowledge.h"
#include "math.h"
#include "message.h"
#include "message_dispatcher.h"
#include "object_aspect_accessory.h"
#include "object_aspect_object_container.h"
#include "object_aspect_outfitting.h"
#include "object_aspect_position.h"
#include "object_aspect_update.h"
#include "object_aspect_visualization.h"
#include "planet.h"
#include "ship.h"
#include "slot.h"
#include "slot_class.h"
#include "storage.h"
#include "system.h"
#include "visualizations.h"
#include "weapon.h"

Ship::Ship(void) :
	m_Accelerate(false),
	m_Battery(0),
	m_CargoHold(0),
	m_ExhaustRadius(0.0f),
	m_Fuel(0.0f),
	m_FuelCapacity(0.0f),
	m_FuelNeededToAccelerate(0.0f),
	m_FuelNeededToJump(0.0f),
	m_FuelNeededToTurn(0.0f),
	m_Generator(0),
	m_Hull(0.0f),
	m_HullCapacity(0.0f),
	m_Jettison(false),
	m_Jump(false),
	m_Land(false),
	m_LinkedSystemTarget(0),
	m_MaximumForwardThrust(0.0f),
	m_MaximumSpeed(0.0f),
	m_MaximumTurnSpeed(0.0f),
	m_Refuel(false),
	m_Scoop(false),
	m_TakeOff(false),
	m_TurnLeft(0.0f),
	m_TurnRight(0.0f),
	m_Velocity(true)
{
	// initialize object aspects
	AddAspectName();
	AddAspectObjectContainer();
	GetAspectObjectContainer()->SetOnAddedCallback(Callback(this, &Ship::OnAdded));
	GetAspectObjectContainer()->SetOnRemovedCallback(Callback(this, &Ship::OnRemoved));
	AddAspectOutfitting();
	AddAspectPhysical();
	AddAspectPosition();
	AddAspectUpdate();
	GetAspectUpdate()->SetCallback(Callback(this, &Ship::Update));
	AddAspectVisualization();
}

Ship::~Ship(void)
{
}

Battery * Ship::GetBattery(void)
{
	if(m_Battery != 0)
	{
		assert(m_Battery->GetAspectAccessory() != 0);
		if(m_Battery->GetAspectAccessory()->GetSlot() != 0)
		{
			return m_Battery;
		}
	}
	
	return 0;
}

void Ship::SetFire(bool Fire)
{
	for(std::map< std::string, Slot * >::const_iterator SlotIterator = GetAspectOutfitting()->GetSlots().begin(); SlotIterator != GetAspectOutfitting()->GetSlots().end(); ++SlotIterator)
	{
		// only update *mounted* *weapons*
		Object * MountedObject(SlotIterator->second->GetMountedObject().Get());
		
		if((MountedObject != 0) && (MountedObject->GetTypeIdentifier() == "weapon"))
		{
			dynamic_cast< Weapon * >(MountedObject)->SetFire(Fire);
		}
	}
}

bool Ship::Update(float Seconds)
{
	if(m_Jump == true)
	{
		if((GetFuel() >= GetFuelNeededToJump()) && (GetLinkedSystemTarget() != 0))
		{
			Object * OldSystem(GetContainer());
			System * NewSystem(GetLinkedSystemTarget());
			
			// remove the ship from the old system
			assert(OldSystem->GetAspectObjectContainer() != 0);
			OldSystem->GetAspectObjectContainer()->RemoveContent(this);
			SetFuel(GetFuel() - GetFuelNeededToJump());
			
			// set the ship's position according to the old system
			Vector3f Direction(NewSystem->GetAspectPosition()->GetPosition() - OldSystem->GetAspectPosition()->GetPosition());
			
			Direction.Normalize();
			GetAspectPosition()->SetPosition(Direction * -300.0f);
			m_Velocity = Direction * GetMaximumSpeed();
			GetAspectPosition()->SetOrientation(Quaternion::CreateAsRotationZ(GetRadians(Vector2f(Direction[0], Direction[1]))));
			// set up the ship in the new system
			assert(NewSystem->GetAspectObjectContainer() != 0);
			NewSystem->GetAspectObjectContainer()->AddContent(this);
			assert(GetAspectObjectContainer() != 0);
			
			const std::set< Object * > & Content(GetAspectObjectContainer()->GetContent());
			
			for(std::set< Object * >::iterator ContentIterator = Content.begin(); ContentIterator != Content.end(); ++ContentIterator)
			{
				Object * Content(*ContentIterator);
				
				if(Content->GetTypeIdentifier() == "character")
				{
					dynamic_cast< Character * >(Content)->GetMapKnowledge()->AddExploredSystem(NewSystem);
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
		
		Object * ThePlanet(GetTarget().Get());
		
		assert(ThePlanet != 0);
		assert(ThePlanet->GetTypeIdentifier() == "planet");
		dynamic_cast< Planet * >(ThePlanet)->Land(this);
		m_Accelerate = false;
		m_TurnLeft = 0.0f;
		m_TurnRight = 0.0f;
	}
	else if(m_TakeOff == true)
	{
		m_TakeOff = false;
		
		Object * ThePlanet(GetTarget().Get());
		
		assert(ThePlanet != 0);
		assert(ThePlanet->GetTypeIdentifier() == "planet");
		dynamic_cast< Planet * >(ThePlanet)->TakeOff(this);
		SetTarget(0);
		SetLinkedSystemTarget(0);
	}
	else
	{
		for(std::map< std::string, Slot * >::const_iterator SlotIterator = GetAspectOutfitting()->GetSlots().begin(); SlotIterator != GetAspectOutfitting()->GetSlots().end(); ++SlotIterator)
		{
			// only update *mounted* *weapons*
			Object * MountedObject(SlotIterator->second->GetMountedObject().Get());
			
			if((MountedObject != 0) && (MountedObject->GetTypeIdentifier() == "weapon"))
			{
				dynamic_cast< Weapon * >(MountedObject)->Update(Seconds);
			}
		}
		if((GetGenerator() != 0) && (GetBattery() != 0))
		{
			float Energy(GetBattery()->GetEnergy() + Seconds * GetGenerator()->GetEnergyProvisionPerSecond());
			
			GetBattery()->SetEnergy((Energy < GetBattery()->GetEnergyCapacity()) ? (Energy) : (GetBattery()->GetEnergyCapacity()));
		}
		if(m_Refuel == true)
		{
			assert(GetAspectObjectContainer() != 0);
			
			const std::set< Object * > & Content(GetAspectObjectContainer()->GetContent());
			
			for(std::set< Object * >::const_iterator ContentIterator = Content.begin(); ContentIterator != Content.end(); ++ContentIterator)
			{
				Object * Content(*ContentIterator);
				
				if((Content->GetTypeIdentifier() == "commodity") && (Content->GetClassIdentifier() == "fuel"))
				{
					SetFuel(Clamp(GetFuel() + 1.0f, 0.0f, GetFuelCapacity()));
					Content->Destroy();
					delete Content;
					
					break;
				}
			}
			m_Refuel = false;
		}
		if(m_TurnLeft > 0.0f)
		{
			float FuelConsumption(GetFuelNeededToTurn() * Seconds * m_TurnLeft);
			
			if(GetFuel() >= FuelConsumption)
			{
				GetAspectPosition()->ModifyOrientation(Quaternion::CreateAsRotationZ(GetMaximumTurnSpeed() * Seconds * m_TurnLeft));
				SetFuel(GetFuel() - FuelConsumption);
			}
		}
		if(m_TurnRight > 0.0f)
		{
			float FuelConsumption(GetFuelNeededToTurn() * Seconds * m_TurnRight);
			
			if(GetFuel() >= FuelConsumption)
			{
				GetAspectPosition()->ModifyOrientation(Quaternion::CreateAsRotationZ(-GetMaximumTurnSpeed() * Seconds * m_TurnRight));
				SetFuel(GetFuel() - FuelConsumption);
			}
		}
		GetAspectPosition()->ModifyPosition(m_Velocity * Seconds);
		if(m_Accelerate == true)
		{
			float FuelConsumption(GetFuelNeededToAccelerate() * Seconds);
			
			if(GetFuel() >= FuelConsumption)
			{
				Vector3f ForwardThrust(GetMaximumForwardThrust(), 0.0f, 0.0f);
				
				ForwardThrust *= GetAspectPosition()->GetOrientation();
				ForwardThrust *= Seconds;
				m_Velocity += Vector3f(ForwardThrust[0], ForwardThrust[1], 0.0f);
				ForwardThrust *= 0.5f * Seconds;
				GetAspectPosition()->ModifyPosition(Vector3f(ForwardThrust[0], ForwardThrust[1], 0.0f));
				if(m_Velocity.Length() > GetMaximumSpeed())
				{
					m_Velocity.Normalize();
					m_Velocity *= GetMaximumSpeed();
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
						X *= GetExhaustRadius();
						Y *= GetExhaustRadius();
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
			assert(GetCargoHold() != 0);
			assert(GetCargoHold()->GetAspectObjectContainer() != 0);
			
			std::set< Object * >::const_iterator ContentIterator(GetCargoHold()->GetAspectObjectContainer()->GetContent().begin());
			
			while(ContentIterator != GetCargoHold()->GetAspectObjectContainer()->GetContent().end())
			{
				Object * Content(*ContentIterator);
				
				++ContentIterator;
				if(Content->GetTypeIdentifier() == "commodity")
				{
					GetCargoHold()->GetAspectObjectContainer()->RemoveContent(Content);
					assert(GetAspectPosition() != 0);
					assert(Content->GetAspectPosition() != 0);
					Content->GetAspectPosition()->SetPosition(GetAspectPosition()->GetPosition());
					
					Commodity * TheCommodity(dynamic_cast< Commodity * >(Content));
					Vector2f Velocity(Vector2f::CreateFromMagnitudeAndAngle(GetRandomFloat(0.0f, 0.5f), GetRandomFloat(0.0f, 2 * M_PI)));
					
					TheCommodity->SetVelocity(Vector3f(GetVelocity()[0] * 0.8f + Velocity[0], GetVelocity()[1] * 0.8 + Velocity[1], 0.0f));
					
					Vector3f RotationAxis(GetRandomFloat(-1.0f, 1.0f), GetRandomFloat(-1.0f, 1.0f), GetRandomFloat(-1.0f, 1.0f));
					
					RotationAxis.Normalize();
					TheCommodity->SetAngularVelocity(AxisAngle(RotationAxis[0], RotationAxis[1], RotationAxis[2], GetRandomFloat(0.0f, 0.7f)));
					assert(GetContainer()->GetAspectObjectContainer() != 0);
					GetContainer()->GetAspectObjectContainer()->AddContent(TheCommodity);
					VisualizeObject(TheCommodity, g_CommodityLayer);
				}
			}
			m_Jettison = false;
		}
		if(m_Scoop == true)
		{
			Object * Target(m_Target.Get());
			
			if((Target != 0) && (Target->GetTypeIdentifier() == "commodity"))
			{
				assert(GetContainer()->GetAspectObjectContainer() != 0);
				// the following is a typical occasion of bad practise: a transaction would be great here
				if(GetContainer()->GetAspectObjectContainer()->RemoveContent(Target) == true)
				{
					assert(GetCargoHold() != 0);
					assert(GetCargoHold()->GetAspectObjectContainer() != 0);
					if(GetCargoHold()->GetAspectObjectContainer()->AddContent(Target) == true)
					{
						SetTarget(0);
						if((Target->GetAspectVisualization() != 0) && (Target->GetAspectVisualization()->GetVisualization().IsValid() == true))
						{
							UnvisualizeObject(Target);
						}
					}
					else
					{
						GetContainer()->GetAspectObjectContainer()->AddContent(Target);
					}
				}
			}
			m_Scoop = false;
		}
	}
	
	return true;
}

void Ship::SetFuel(float Fuel)
{
	float FuelThreshold(GetFuelCapacity() * 0.2);
	bool FuelLow((Fuel < FuelThreshold) && (GetFuel() > FuelThreshold));
	
	m_Fuel = Fuel;
	if(FuelLow == true)
	{
		// send message to all characters on the ship
		assert(GetAspectObjectContainer() != 0);
		
		const std::set< Object * > & ShipContent(GetAspectObjectContainer()->GetContent());
		
		for(std::set< Object * >::iterator ContentIterator = ShipContent.begin(); ContentIterator != ShipContent.end(); ++ContentIterator)
		{
			Object * Content(*ContentIterator);
			
			if(Content->GetTypeIdentifier() == "character")
			{
				g_MessageDispatcher->PushMessage(new Message("fuel_low", GetReference(), Content->GetReference()));
			}
		}
	}
}

void Ship::SetHull(float Hull)
{
	float HullThreshold(GetHullCapacity() * 0.33);
	bool HullLow((Hull < HullThreshold) && (GetHull() > HullThreshold));
	
	m_Hull = Hull;
	if(HullLow == true)
	{
		// send message to all characters on the ship
		assert(GetAspectObjectContainer() != 0);
		
		const std::set< Object * > & ShipContent(GetAspectObjectContainer()->GetContent());
		
		for(std::set< Object * >::iterator ContentIterator = ShipContent.begin(); ContentIterator != ShipContent.end(); ++ContentIterator)
		{
			Object * Content(*ContentIterator);
			
			if(Content->GetTypeIdentifier() == "character")
			{
				g_MessageDispatcher->PushMessage(new Message("hull_low", GetReference(), Content->GetReference()));
			}
		}
	}
}

void Ship::OnAdded(Object * Content)
{
	if(Content->GetTypeIdentifier() == "battery")
	{
		assert(m_Battery == 0);
		m_Battery = dynamic_cast< Battery * >(Content);
	}
	else if(Content->GetTypeIdentifier() == "generator")
	{
		assert(m_Generator == 0);
		m_Generator = dynamic_cast< Generator * >(Content);
	}
	else if(Content->GetTypeIdentifier() == "storage")
	{
		assert(m_CargoHold == 0);
		m_CargoHold = dynamic_cast< Storage * >(Content);
	}
}

void Ship::OnRemoved(Object * Content)
{
	if(Content->GetTypeIdentifier() == "battery")
	{
		assert(m_Battery == Content);
		m_Battery = 0;
	}
	else if(Content->GetTypeIdentifier() == "generator")
	{
		assert(m_Generator == Content);
		m_Generator = 0;
	}
	else if(Content->GetTypeIdentifier() == "storage")
	{
		assert(m_CargoHold == Content);
		m_CargoHold = 0;
	}
}
