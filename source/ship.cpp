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
#include "graphics/engine.h"
#include "graphics/node.h"
#include "graphics/particle_system.h"
#include "map_knowledge.h"
#include "math.h"
#include "message.h"
#include "message_dispatcher.h"
#include "object_aspect_accessory.h"
#include "object_aspect_object_container.h"
#include "object_aspect_outfitting.h"
#include "object_aspect_physical.h"
#include "object_aspect_position.h"
#include "object_aspect_update.h"
#include "object_aspect_visualization.h"
#include "planet.h"
#include "ship.h"
#include "slot.h"
#include "slot_class.h"
#include "storage.h"
#include "system.h"
#include "turret.h"
#include "visualization.h"
#include "visualizations.h"
#include "weapon.h"

// this function defined in main.cpp
Graphics::ParticleSystem * CreateParticleSystem(const std::string & ParticleSystemClassIdentifier);

Ship::Ship(void) :
	m_Accelerate(false),
	_Battery(nullptr),
	_CargoHold(nullptr),
	_EngineGlowParticleSystem(0),
	m_ExhaustRadius(0.0f),
	m_Fuel(0.0f),
	m_FuelCapacity(0.0f),
	m_FuelNeededToAccelerate(0.0f),
	m_FuelNeededToJump(0.0f),
	m_FuelNeededToTurn(0.0f),
	_Generator(nullptr),
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
	_Target(nullptr),
	m_TurnLeft(0.0f),
	m_TurnRight(0.0f),
	m_Velocity(Vector3f::CreateZero())
{
	// initialize object aspects
	AddAspectName();
	AddAspectObjectContainer();
	GetAspectObjectContainer()->ConnectContentAddedCallback(std::bind(&Ship::OnAdded, this, std::placeholders::_1));
	GetAspectObjectContainer()->ConnectContentRemovedCallback(std::bind(&Ship::OnRemoved, this, std::placeholders::_1));
	AddAspectOutfitting();
	AddAspectPhysical();
	AddAspectPosition();
	AddAspectUpdate();
	GetAspectUpdate()->SetCallback(std::bind(&Ship::Update, this, std::placeholders::_1));
	AddAspectVisualization();
	GetAspectVisualization()->SetUpdateVisualizationCallback(std::bind(&Ship::_UpdateVisualization, this, std::placeholders::_1));
}

Ship::~Ship(void)
{
	assert(_Battery == nullptr);
	assert(_CargoHold == nullptr);
	assert(_Generator == nullptr);
	if(_Target != nullptr)
	{
		_Target->DisconnectDestroyingCallback(_TargetDestroyingConnection);
	}
	g_GraphicsEngine->RemoveParticleSystem(_EngineGlowParticleSystem);
	delete _EngineGlowParticleSystem;
	_EngineGlowParticleSystem = 0;
}

Battery * Ship::GetBattery(void)
{
	if(_Battery != nullptr)
	{
		assert(_Battery->GetAspectAccessory() != 0);
		assert(_Battery->GetAspectAccessory()->GetSlot() != 0);
	}
	
	return _Battery;
}

System * Ship::GetSystem(void)
{
	auto Container(GetContainer());
	
	while((Container != nullptr) && (Container->GetTypeIdentifier() != "system"))
	{
		Container = Container->GetContainer();
	}
	
	return dynamic_cast< System * >(Container);
}

void Ship::SetFire(bool Fire)
{
	assert(GetAspectOutfitting() != nullptr);
	for(auto & SlotPair : GetAspectOutfitting()->GetSlots())
	{
		// only update *mounted* *weapons*
		auto MountedObject(SlotPair.second->GetMountedObject());
		
		if(MountedObject != nullptr)
		{
			if(MountedObject->GetTypeIdentifier() == "weapon")
			{
				dynamic_cast< Weapon * >(MountedObject)->SetFire(Fire);
			}
			else if(MountedObject->GetTypeIdentifier() == "turret")
			{
				dynamic_cast< Turret * >(MountedObject)->SetFire(Fire);
			}
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
			
			// display jump partice system in old system
			auto NewJumpParticleSystem(CreateParticleSystem("jump"));
			
			NewJumpParticleSystem->SetPosition(GetAspectPosition()->GetPosition());
			NewJumpParticleSystem->SetVelocity(Vector3f::CreateZero());
			VisualizeParticleSystem(NewJumpParticleSystem, dynamic_cast< System * >(OldSystem));
			
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
		assert(_Target != nullptr);
		assert(_Target->GetTypeIdentifier() == "planet");
		assert(GetAspectObjectContainer() != nullptr);
		
		Character * TheCharacter(nullptr);
		
		for(auto Content : GetAspectObjectContainer()->GetContent())
		{
			if(Content->GetTypeIdentifier() == "character")
			{
				TheCharacter = dynamic_cast< Character * >(Content);
				
				break;
			}
		}
		dynamic_cast< Planet * >(_Target)->Land(this, TheCharacter);
		SetTarget(nullptr);
		SetLinkedSystemTarget(nullptr);
		SetFire(false);
		m_Accelerate = false;
		m_TurnLeft = 0.0f;
		m_TurnRight = 0.0f;
	}
	else if(m_TakeOff == true)
	{
		m_TakeOff = false;
		
		auto Container(GetContainer());
		
		while((Container != nullptr) && (Container->GetTypeIdentifier() != "planet"))
		{
			Container = Container->GetContainer();
		}
		assert(Container != nullptr);
		assert(Container->GetTypeIdentifier() == "planet");
		
		Character * TheCharacter(nullptr);
		
		for(auto Content : GetAspectObjectContainer()->GetContent())
		{
			if(Content->GetTypeIdentifier() == "character")
			{
				TheCharacter = dynamic_cast< Character * >(Content);
				
				break;
			}
		}
		dynamic_cast< Planet * >(Container)->TakeOff(this, TheCharacter);
	}
	else
	{
		if((GetGenerator() != 0) && (GetBattery() != 0))
		{
			float Energy(GetBattery()->GetEnergy() + Seconds * GetGenerator()->GetEnergyProvisionPerSecond());
			
			GetBattery()->SetEnergy((Energy < GetBattery()->GetEnergyCapacity()) ? (Energy) : (GetBattery()->GetEnergyCapacity()));
		}
		if(m_Refuel == true)
		{
			if(_CargoHold != nullptr)
			{
				assert(_CargoHold->GetAspectObjectContainer() != nullptr);
				for(auto Content : _CargoHold->GetAspectObjectContainer()->GetContent())
				{
					if((Content->GetTypeIdentifier() == "commodity") && (Content->GetClassIdentifier() == "fuel"))
					{
						SetFuel(Clamp(GetFuel() + 1.0f, 0.0f, GetFuelCapacity()));
						Content->Destroy();
						delete Content;
						
						break;
					}
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
				auto ForwardThrust(Vector3f::CreateTranslationX(GetMaximumForwardThrust()));
				
				ForwardThrust.Rotate(GetAspectPosition()->GetOrientation());
				ForwardThrust *= Seconds;
				m_Velocity.Translate(Vector3f::CreateFromComponents(ForwardThrust[0], ForwardThrust[1], 0.0f));
				ForwardThrust *= 0.5f * Seconds;
				GetAspectPosition()->ModifyPosition(Vector3f::CreateFromComponents(ForwardThrust[0], ForwardThrust[1], 0.0f));
				if(m_Velocity.Length() > GetMaximumSpeed())
				{
					m_Velocity.Normalize();
					m_Velocity *= GetMaximumSpeed();
				}
				SetFuel(GetFuel() - FuelConsumption);
				assert(_EngineGlowParticleSystem != 0);
				for(int I = 0; I < 4; ++I)
				{
					Graphics::ParticleSystem::Particle Particle;
					
					Particle._TimeOfDeath = GameTime::Get() + GetRandomFloat(0.5f, 0.9f);
					Particle._Color = Color(GetRandomFloat(0.8f, 1.0f), GetRandomFloat(0.7f, 0.8f), 0.5f, 0.08f);
					
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
					Particle._Position = Vector3f::CreateFromComponents(0.0f, X, Y);
					Particle._Velocity = Vector3f::CreateFromComponents(GetRandomFloat(-0.3f * VelocityFactor, 0.0f), 0.0f, 0.0f);
					Particle._Size = 0.2f;
					_EngineGlowParticleSystem->AddParticle(Particle);
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
					auto Velocity(Vector2f::CreateFromMagnitudeAndAngle(GetRandomFloat(0.0f, 0.5f), GetRandomFloat(0.0f, 2 * M_PI)));
					
					TheCommodity->SetVelocity(Vector3f::CreateFromComponents(GetVelocity()[0] * 0.8f + Velocity[0], GetVelocity()[1] * 0.8 + Velocity[1], 0.0f));
					
					auto RotationAxis(Vector3f::CreateFromComponents(GetRandomFloat(-1.0f, 1.0f), GetRandomFloat(-1.0f, 1.0f), GetRandomFloat(-1.0f, 1.0f)));
					
					RotationAxis.Normalize();
					TheCommodity->SetAngularVelocity(AxisAngle(RotationAxis[0], RotationAxis[1], RotationAxis[2], GetRandomFloat(0.0f, 0.7f)));
					assert(GetContainer()->GetAspectObjectContainer() != 0);
					GetContainer()->GetAspectObjectContainer()->AddContent(TheCommodity);
				}
			}
			m_Jettison = false;
		}
		if(m_Scoop == true)
		{
			if((_CargoHold != nullptr) && (_Target != nullptr))
			{
				assert(_CargoHold->GetAspectObjectContainer() != nullptr);
				
				auto Target(_Target);
				
				assert(Target != nullptr);
				if((Target->GetAspectPhysical() != nullptr) && (Target->GetAspectPhysical()->GetSpaceRequirement() <= _CargoHold->GetSpace()))
				{
					assert(Target->GetContainer() != nullptr);
					assert(Target->GetContainer()->GetAspectObjectContainer() != nullptr);
					Target->GetContainer()->GetAspectObjectContainer()->RemoveContent(Target);
					_CargoHold->GetAspectObjectContainer()->AddContent(Target);
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

void Ship::SetTarget(Object * Target)
{
	if(_Target != Target)
	{
		if(_Target != nullptr)
		{
			_Target->DisconnectDestroyingCallback(_TargetDestroyingConnection);
			_Target = nullptr;
		}
		if(Target != nullptr)
		{
			_Target = Target;
			_TargetDestroyingConnection = _Target->ConnectDestroyingCallback(std::bind(&Ship::_OnTargetDestroying, this));
		}
	}
}

void Ship::OnAdded(Object * Content)
{
	assert(Content != nullptr);
	if(Content->GetTypeIdentifier() == "battery")
	{
		assert(_Battery == nullptr);
		
		auto TheBattery(dynamic_cast< Battery * >(Content));
		
		assert(TheBattery != nullptr);
		_Battery = TheBattery;
	}
	else if(Content->GetTypeIdentifier() == "generator")
	{
		assert(_Generator == nullptr);
		
		auto TheGenerator(dynamic_cast< Generator * >(Content));
		
		assert(TheGenerator != nullptr);
		_Generator = TheGenerator;
	}
	else if(Content->GetTypeIdentifier() == "storage")
	{
		assert(_CargoHold == nullptr);
		
		auto TheCargoHold(dynamic_cast< Storage * >(Content));
		
		assert(TheCargoHold != nullptr);
		_CargoHold = TheCargoHold;
	}
}

void Ship::OnRemoved(Object * Content)
{
	assert(Content != nullptr);
	if(Content->GetTypeIdentifier() == "battery")
	{
		assert(_Battery == Content);
		_Battery = nullptr;
	}
	else if(Content->GetTypeIdentifier() == "generator")
	{
		assert(_Generator == Content);
		_Generator = nullptr;
	}
	else if(Content->GetTypeIdentifier() == "storage")
	{
		assert(_CargoHold == Content);
		_CargoHold = nullptr;
	}
}

void Ship::_OnTargetDestroying(void)
{
	assert(_Target != nullptr);
	_Target = nullptr;
}

void Ship::_UpdateVisualization(Visualization * Visualization)
{
	assert(Visualization != nullptr);
	assert(Visualization->GetGraphics() != nullptr);
	assert(GetAspectPosition() != nullptr);
	if(Visualization->GetUpdateOrientation() == true)
	{
		Visualization->GetGraphics()->SetOrientation(GetAspectPosition()->GetOrientation());
	}
	if(Visualization->GetUpdatePosition() == true)
	{
		Visualization->GetGraphics()->SetPosition(GetAspectPosition()->GetPosition());
	}
}
