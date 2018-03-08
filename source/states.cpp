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
#include <float.h>

#include <list>
#include <stdexcept>

#include <string_cast/string_cast.h>

#include "character.h"
#include "commodity.h"
#include "commodity_class.h"
#include "faction.h"
#include "game_time.h"
#include "globals.h"
#include "math.h"
#include "message.h"
#include "mind.h"
#include "object_aspect_object_container.h"
#include "object_aspect_physical.h"
#include "object_aspect_position.h"
#include "object_factory.h"
#include "planet.h"
#include "planet_assets.h"
#include "ship.h"
#include "state_machine.h"
#include "states.h"
#include "storage.h"
#include "system.h"

int WantToJump(Ship * Ship, System * System);

static bool FlyTo(Ship * Ship, const Vector3f & Direction)
{
	Vector3f LocalizedDirection(Direction);
	
	LocalizedDirection.Rotate(Ship->GetAspectPosition()->GetOrientation().Conjugated());
	if((LocalizedDirection[1] > 0.1f) || ((LocalizedDirection[0] < 0.0f) && (LocalizedDirection[1] >= 0.0f)))
	{
		Ship->SetTurnLeft(1.0f);
		Ship->SetTurnRight(0.0f);
		Ship->SetAccelerate(false);
		
		return false;
	}
	else if((LocalizedDirection[1] < -0.1f) || ((LocalizedDirection[0] < 0.0f) && (LocalizedDirection[1] < 0.0f)))
	{
		Ship->SetTurnLeft(0.0f);
		Ship->SetTurnRight(1.0f);
		Ship->SetAccelerate(false);
		
		return false;
	}
	else
	{
		Ship->SetTurnLeft(0.0f);
		Ship->SetTurnRight(0.0f);
		Ship->SetAccelerate(Ship->GetMaximumSpeed() - Direction.Dot(Ship->GetVelocity()) > 0.1f);
		
		return true;
	}
}

SelectSteering::SelectSteering(StateMachineMind * Mind) :
	State(Mind)
{
	SetObjectIdentifier("::select_steering::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void SelectSteering::Enter(void)
{
}

void SelectSteering::Execute(void)
{
	GetMind()->GetStateMachine()->SetState(new TransporterPhase1(GetMind()));
	delete this;
}

void SelectSteering::Exit(void)
{
}

FlyOverRandomPoint::FlyOverRandomPoint(StateMachineMind * Mind) :
	State(Mind)
{
	SetObjectIdentifier("::fly_over_random_point::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void FlyOverRandomPoint::Enter(void)
{
	m_Point.Set(GetRandomFloat(-200.0f, 200.0f), GetRandomFloat(-200.0f, 200.0f), 0.0f);
}

void FlyOverRandomPoint::Execute(void)
{
	Vector3f ToDestination(m_Point - GetMind()->GetCharacter()->GetShip()->GetAspectPosition()->GetPosition());
	float LengthSquared(ToDestination.SquaredLength());
	
	if(LengthSquared > 400.0f)
	{
		ToDestination /= sqrt(LengthSquared);
		FlyTo(GetMind()->GetCharacter()->GetShip(), ToDestination);
	}
	else
	{
		GetMind()->GetStateMachine()->SetState(new SelectSteering(GetMind()));
		delete this;
	}
}

void FlyOverRandomPoint::Exit(void)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// TransporterPhase1: Picks a planet, aims toward it and monitors the process of approching.     //
//                    Will calculate when to go to the next phase, which does the braking.       //
///////////////////////////////////////////////////////////////////////////////////////////////////
TransporterPhase1::TransporterPhase1(StateMachineMind * Mind) :
	State(Mind)
{
	SetObjectIdentifier("::transporter_phase_1::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void TransporterPhase1::Enter(void)
{
	auto CurrentSystem{GetMind()->GetCharacter()->GetSystem()};
	
	assert(CurrentSystem != nullptr);
	if(CurrentSystem->GetPlanets().size() > 0)
	{
		Planet * NearestLandablePlanet{nullptr};
		auto MinimumDistance{FLT_MAX};
		
		for(auto Planet : CurrentSystem->GetPlanets())
		{
			assert(Planet != nullptr);
			assert(Planet->GetFaction() != nullptr);
			assert(GetMind()->GetCharacter()->GetShip()->GetFaction() != nullptr);
			if(Planet->GetFaction()->GetStanding(GetMind()->GetCharacter()->GetShip()->GetFaction()) > 0.5f)
			{
				auto Distance{(Planet->GetAspectPosition()->GetPosition() - GetMind()->GetCharacter()->GetShip()->GetAspectPosition()->GetPosition()).SquaredLength()};
				
				if(Distance < MinimumDistance)
				{
					NearestLandablePlanet = Planet;
					MinimumDistance = Distance;
				}
			}
		}
		if(NearestLandablePlanet != nullptr)
		{
			GetMind()->GetCharacter()->GetShip()->SetTarget(NearestLandablePlanet);
		}
		else
		{
			GetMind()->GetStateMachine()->SetState(new TransporterPhase4(GetMind()));
			delete this;
		}
	}
	else
	{
		GetMind()->GetStateMachine()->SetState(new TransporterPhase4(GetMind()));
		delete this;
	}
}

void TransporterPhase1::Execute(void)
{
	Vector3f ToDestination(GetMind()->GetCharacter()->GetShip()->GetTarget()->GetAspectPosition()->GetPosition() - GetMind()->GetCharacter()->GetShip()->GetAspectPosition()->GetPosition());
	float DistanceSquared(ToDestination.SquaredLength());
	float DistanceNeededToBrake(GetMind()->GetCharacter()->GetShip()->GetMaximumSpeed() * ((M_PI / GetMind()->GetCharacter()->GetShip()->GetMaximumTurnSpeed()) + ((GetMind()->GetCharacter()->GetShip()->GetMaximumSpeed() / GetMind()->GetCharacter()->GetShip()->GetMaximumForwardThrust()) / 2.0f)));
	
	// braking in phase 2 takes time (turning + accelerating)
	//  - turning: the ship will fly with maximum velocity towards the target
	//  - accelerating: the ship will fly half distance that it could with maximum velocity towards the target
	if(DistanceSquared > DistanceNeededToBrake * DistanceNeededToBrake)
	{
		ToDestination /= sqrt(DistanceSquared);
		FlyTo(GetMind()->GetCharacter()->GetShip(), ToDestination);
	}
	else
	{
		GetMind()->GetStateMachine()->SetState(new TransporterPhase2(GetMind()));
		delete this;
	}
}

void TransporterPhase1::Exit(void)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// TransporterPhase2: Revereses the ship and accelerates until a near stop is accomplished.      //
///////////////////////////////////////////////////////////////////////////////////////////////////
TransporterPhase2::TransporterPhase2(StateMachineMind * Mind) :
	State(Mind)
{
	SetObjectIdentifier("::transporter_phase_2::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void TransporterPhase2::Enter(void)
{
}

void TransporterPhase2::Execute(void)
{
	float SpeedSquared(GetMind()->GetCharacter()->GetShip()->GetVelocity().SquaredLength());
	
	if(SpeedSquared > 2.0f)
	{
		FlyTo(GetMind()->GetCharacter()->GetShip(), -GetMind()->GetCharacter()->GetShip()->GetVelocity().Normalized());
	}
	else
	{
		auto ThePlanet(dynamic_cast< Planet * >(GetMind()->GetCharacter()->GetShip()->GetTarget()));
		
		if(GetMind()->GetCharacter()->GetCredits() >= (ThePlanet->GetLandingFeePerSpace() * GetMind()->GetCharacter()->GetShip()->GetAspectPhysical()->GetSpaceRequirement()))
		{
			GetMind()->GetCharacter()->RemoveCredits(ThePlanet->GetLandingFeePerSpace() * GetMind()->GetCharacter()->GetShip()->GetAspectPhysical()->GetSpaceRequirement());
			GetMind()->GetCharacter()->GetShip()->SetLand(true);
			GetMind()->GetStateMachine()->SetState(new TransporterPhase3(GetMind()));
			delete this;
		}
		else
		{
			GetMind()->GetStateMachine()->SetState(new SelectSteering(GetMind()));
			delete this;
		}
	}
}

void TransporterPhase2::Exit(void)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// TransporterPhase3: Wait for a little time.                                                    //
//                    Decide, what to do next.                                                   //
///////////////////////////////////////////////////////////////////////////////////////////////////
TransporterPhase3::TransporterPhase3(StateMachineMind * Mind) :
	State(Mind),
	m_TimeToLeave(GameTime::Get() + GetRandomFloat(12.0f, 20.0f))
{
	SetObjectIdentifier("::transporter_phase_3::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void TransporterPhase3::Enter(void)
{
	// ATTENTION: the target is only valid because this Enter() function is called before the setting of m_Land is processed in the ship which invalidates the ship's target
	auto ThePlanet(dynamic_cast< Planet * >(GetMind()->GetCharacter()->GetShip()->GetTarget()));
	
	assert(GetMind()->GetCharacter()->GetShip()->GetAspectObjectContainer() != nullptr);
	
	auto & Content{GetMind()->GetCharacter()->GetShip()->GetAspectObjectContainer()->GetContent()};
	auto ContentIterator(Content.begin());
	
	// first sell commodities ... but only those with base price modifier above 1
	while(ContentIterator != Content.end())
	{
		auto ContentObject{*ContentIterator};
		
		if(ContentObject != nullptr)
		{
			auto PlanetAssets{ThePlanet->GetPlanetAssets(ContentObject->GetTypeIdentifier(), ContentObject->GetSubTypeIdentifier())};
			
			if((PlanetAssets != nullptr) && (PlanetAssets->GetBasePriceModifier() > 1.0))
			{
				auto NextIterator{ContentIterator};
				
				++NextIterator;
				GetMind()->GetCharacter()->GetShip()->GetCargoHold()->GetAspectObjectContainer()->RemoveContent(ContentObject);
				ContentIterator = NextIterator;
				delete ContentObject;
				GetMind()->GetCharacter()->AddCredits(PlanetAssets->GetPrice());
				
				continue;
			}
		}
		++ContentIterator;
	}
	// second buy stuff ... but only stuff with base price modifier below 1
	std::vector< PlanetAssets * > BuyablePlanetAssets;
	
	for(auto BuyPlanetAssets : ThePlanet->GetPlanetAssets())
	{
		if(BuyPlanetAssets->GetBasePriceModifier() < 1.0f)
		{
			BuyablePlanetAssets.push_back(BuyPlanetAssets);
		}
	}
	if(BuyablePlanetAssets.empty() == false)
	{
		for(int NumberOfPlanetAssetClassesToBuy = GetRandomIntegerFromExponentialDistribution(2); NumberOfPlanetAssetClassesToBuy > 0; --NumberOfPlanetAssetClassesToBuy)
		{
			auto PlanetAssetClassToBuy(BuyablePlanetAssets[GetRandomInteger(BuyablePlanetAssets.size() - 1)]);
			
			for(int NumberOfAssetsToBuy = GetRandomIntegerFromExponentialDistribution(GetMind()->GetCharacter()->GetShip()->GetCargoHold()->GetSpaceCapacity() / g_ObjectFactory->GetSpaceRequirement(PlanetAssetClassToBuy->GetTypeIdentifier(), PlanetAssetClassToBuy->GetSubTypeIdentifier())); NumberOfAssetsToBuy > 0; --NumberOfAssetsToBuy)
			{
				// TODO: the 400.0f is a safety margin for landing fees and fuel
				if((GetMind()->GetCharacter()->GetShip()->GetCargoHold()->GetSpace() >= g_ObjectFactory->GetSpaceRequirement(PlanetAssetClassToBuy->GetTypeIdentifier(), PlanetAssetClassToBuy->GetSubTypeIdentifier())) && (GetMind()->GetCharacter()->GetCredits() - 400.0f >= PlanetAssetClassToBuy->GetPrice()))
				{
					GetMind()->GetCharacter()->RemoveCredits(PlanetAssetClassToBuy->GetPrice());
					
					auto BuyObject(g_ObjectFactory->Create(PlanetAssetClassToBuy->GetTypeIdentifier(), PlanetAssetClassToBuy->GetSubTypeIdentifier(), true));
					
					BuyObject->SetObjectIdentifier("::" + PlanetAssetClassToBuy->GetTypeIdentifier() + "(" + PlanetAssetClassToBuy->GetSubTypeIdentifier() + ")::buy_index(" + to_string_cast(NumberOfPlanetAssetClassesToBuy) + "|" + to_string_cast(NumberOfAssetsToBuy) + ")::bought_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::bought_by(" + GetMind()->GetObjectIdentifier() + ")::bought_on(" + ThePlanet->GetObjectIdentifier() + ")");
					GetMind()->GetCharacter()->GetShip()->GetCargoHold()->GetAspectObjectContainer()->AddContent(BuyObject);
				}
				else
				{
					break;
				}
			}
		}
	}
}

void TransporterPhase3::Execute(void)
{
	if(GameTime::Get() >= m_TimeToLeave)
	{
		GetMind()->GetCharacter()->GetShip()->SetTakeOff(true);
	}
}

void TransporterPhase3::Exit(void)
{
}

bool TransporterPhase3::HandleMessage(Message * Message)
{
	if(Message->GetTypeIdentifier() == "taken_off")
	{
		if(GetRandomBoolean() == true)
		{
			GetMind()->GetStateMachine()->SetState(new TransporterPhase1(GetMind()));
			delete this;
		}
		else
		{
			GetMind()->GetStateMachine()->SetState(new TransporterPhase4(GetMind()));
			delete this;
		}
		
		return true;
	}
	else
	{
		return false;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// TransporterPhase4: Decide on a system to jump to. Fly away from system center to jump. Jump.  //
///////////////////////////////////////////////////////////////////////////////////////////////////
TransporterPhase4::TransporterPhase4(StateMachineMind * Mind) :
	State(Mind)
{
	SetObjectIdentifier("::transporter_phase_4::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void TransporterPhase4::Enter(void)
{
	auto CurrentSystem(GetMind()->GetCharacter()->GetShip()->GetSystem());
	
	assert(CurrentSystem != nullptr);
	
	const std::list< System * > & Systems(CurrentSystem->GetLinkedSystems());
	
	if(Systems.size() != 0)
	{
		std::list< System * >::const_iterator SystemIterator(Systems.begin());
		
		for(std::list< System * >::size_type Choice = GetRandomInteger(Systems.size() - 1); Choice > 0; --Choice)
		{
			++SystemIterator;
		}
		GetMind()->GetCharacter()->GetShip()->SetLinkedSystemTarget(*SystemIterator);
		m_JumpDirection = ((*SystemIterator)->GetAspectPosition()->GetPosition() - GetMind()->GetCharacter()->GetShip()->GetContainer()->GetAspectPosition()->GetPosition()).Normalize();
	}
	else
	{
		GetMind()->GetStateMachine()->SetState(new FlyOverRandomPoint(GetMind()));
		delete this;
	}
}

void TransporterPhase4::Execute(void)
{
	switch(WantToJump(GetMind()->GetCharacter()->GetShip(), GetMind()->GetCharacter()->GetShip()->GetLinkedSystemTarget()))
	{
	case NOT_ENOUGH_FUEL:
		{
			GetMind()->GetStateMachine()->SetState(new SelectSteering(GetMind()));
			delete this;
			
			break;
		}
	case OK:
		{
			GetMind()->GetCharacter()->GetShip()->SetJump(true);
			GetMind()->GetStateMachine()->SetState(new SelectSteering(GetMind()));
			delete this;
			
			break;
		}
	case TOO_NEAR_TO_STELLAR_OBJECT:
		{
			FlyTo(GetMind()->GetCharacter()->GetShip(), m_JumpDirection);
			
			break;
		}
	default:
		{
			throw std::runtime_error("Unhandled WantToJump return code.");
		}
	}
}

void TransporterPhase4::Exit(void)
{
}

MonitorFuel::MonitorFuel(StateMachineMind * Mind) :
	State(Mind),
	m_Refueling(false)
{
	SetObjectIdentifier("::monitor_fuel::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void MonitorFuel::Enter(void)
{
}

void MonitorFuel::Execute(void)
{
	if((m_Refueling == false) && (GetMind()->GetCharacter()->GetShip()->GetFuel() / GetMind()->GetCharacter()->GetShip()->GetFuelCapacity() < 0.25f))
	{
		State * OldState(GetMind()->GetStateMachine()->GetState());
		
		GetMind()->GetStateMachine()->SetState(new RefuelPhase1(GetMind()));
		delete OldState;
		m_Refueling = true;
	}
}

void MonitorFuel::Exit(void)
{
}

void MonitorFuel::SetRefueled(void)
{
	m_Refueling = false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// RefuelPhase1: Select the nearest planet and fly towards it.                                   //
//               Abort approch when near enough.                                                 //
///////////////////////////////////////////////////////////////////////////////////////////////////
RefuelPhase1::RefuelPhase1(StateMachineMind * Mind) :
	State(Mind)
{
	SetObjectIdentifier("::refuel_phase_1::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void RefuelPhase1::Enter(void)
{
	auto CurrentSystem(GetMind()->GetCharacter()->GetSystem());
	
	assert(CurrentSystem != nullptr);
	
	Planet * NearestLandablePlanet(nullptr);
	float MinimumDistance(FLT_MAX);
	
	for(auto Planet : CurrentSystem->GetPlanets())
	{
		assert(Planet != nullptr);
		assert(Planet->GetFaction() != nullptr);
		if(Planet->GetFaction()->GetStanding(GetMind()->GetCharacter()->GetShip()->GetFaction()) >= 0.5)
		{
			float Distance((Planet->GetAspectPosition()->GetPosition() - GetMind()->GetCharacter()->GetShip()->GetAspectPosition()->GetPosition()).SquaredLength());
			
			if(Distance < MinimumDistance)
			{
				NearestLandablePlanet = Planet;
				MinimumDistance = Distance;
			}
		}
	}
	if(NearestLandablePlanet != nullptr)
	{
		GetMind()->GetCharacter()->GetShip()->SetTarget(NearestLandablePlanet);
	}
	else
	{
		GetMind()->GetStateMachine()->SetState(new SelectSteering(GetMind()));
		delete this;
	}
}

void RefuelPhase1::Execute(void)
{
	Vector3f ToDestination(GetMind()->GetCharacter()->GetShip()->GetTarget()->GetAspectPosition()->GetPosition() - GetMind()->GetCharacter()->GetShip()->GetAspectPosition()->GetPosition());
	float DistanceSquared(ToDestination.SquaredLength());
	float DistanceNeededToBrake(GetMind()->GetCharacter()->GetShip()->GetMaximumSpeed() * ((M_PI / GetMind()->GetCharacter()->GetShip()->GetMaximumTurnSpeed()) + ((GetMind()->GetCharacter()->GetShip()->GetMaximumSpeed() / GetMind()->GetCharacter()->GetShip()->GetMaximumForwardThrust()) / 2.0f)));
	
	// braking in phase 2 takes time (turning + accelerating)
	//  - turning: the ship will fly with maximum velocity towards the target
	//  - accelerating: the ship will fly half distance that it could with maximum velocity towards the target
	if(DistanceSquared > DistanceNeededToBrake * DistanceNeededToBrake)
	{
		ToDestination /= sqrt(DistanceSquared);
		FlyTo(GetMind()->GetCharacter()->GetShip(), ToDestination);
	}
	else
	{
		GetMind()->GetStateMachine()->SetState(new RefuelPhase2(GetMind()));
		delete this;
	}
}

void RefuelPhase1::Exit(void)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// RefuelPhase2: Revereses the ship and accelerates until a near stop is accomplished.           //
///////////////////////////////////////////////////////////////////////////////////////////////////
RefuelPhase2::RefuelPhase2(StateMachineMind * Mind) :
	State(Mind)
{
	SetObjectIdentifier("::refuel_phase_2::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void RefuelPhase2::Enter(void)
{
}

void RefuelPhase2::Execute(void)
{
	float SpeedSquared(GetMind()->GetCharacter()->GetShip()->GetVelocity().SquaredLength());
	
	if(SpeedSquared > 2.0f)
	{
		FlyTo(GetMind()->GetCharacter()->GetShip(), -GetMind()->GetCharacter()->GetShip()->GetVelocity().Normalized());
	}
	else
	{
		auto ThePlanet(dynamic_cast< Planet * >(GetMind()->GetCharacter()->GetShip()->GetTarget()));
		
		if(GetMind()->GetCharacter()->GetCredits() >= (ThePlanet->GetLandingFeePerSpace() * GetMind()->GetCharacter()->GetShip()->GetAspectPhysical()->GetSpaceRequirement()))
		{
			GetMind()->GetCharacter()->RemoveCredits((ThePlanet->GetLandingFeePerSpace() * GetMind()->GetCharacter()->GetShip()->GetAspectPhysical()->GetSpaceRequirement()));
			GetMind()->GetCharacter()->GetShip()->SetLand(true);
			GetMind()->GetStateMachine()->SetState(new RefuelPhase3(GetMind()));
			delete this;
		}
		else
		{
			// TODO: this line ensures that ships keep drifting in space if they have no chance of refueling
			GetMind()->GetCharacter()->GetShip()->SetAccelerate(false);
			// to ensure they are attacked by fighters
			GetMind()->GetCharacter()->GetShip()->SetFuel(0.0);
			// self destruct in dispair.
		}
	}
}

void RefuelPhase2::Exit(void)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// RefuelPhase3: Refuel.                                                                         //
//               Wait for a little time and decide, what to do next.                                //
///////////////////////////////////////////////////////////////////////////////////////////////////
RefuelPhase3::RefuelPhase3(StateMachineMind * Mind) :
	State(Mind),
	m_TimeToLeave(GameTime::Get() + GetRandomFloat(2.0f, 5.0f))
{
	SetObjectIdentifier("::refuel_phase_3::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void RefuelPhase3::Enter(void)
{
	// ATTENTION: the target is only valid because this Enter() function is called before the setting of m_Land is processed in the ship which invalidates the ship's target
	auto ThePlanet(dynamic_cast< Planet * >(GetMind()->GetCharacter()->GetShip()->GetTarget()));
	auto FuelPlanetAssets{ThePlanet->GetPlanetAssets("commodity", "fuel")};
	
	if(FuelPlanetAssets != nullptr)
	{
		auto FuelPrice{FuelPlanetAssets->GetPrice()};
		auto CanBuy{GetMind()->GetCharacter()->GetCredits() / FuelPrice};
		auto Need{GetMind()->GetCharacter()->GetShip()->GetFuelCapacity() - GetMind()->GetCharacter()->GetShip()->GetFuel()};
		auto Buy{(CanBuy > Need) ? (Need) : (CanBuy)};
		
		GetMind()->GetCharacter()->GetShip()->SetFuel(GetMind()->GetCharacter()->GetShip()->GetFuel() + Buy);
		GetMind()->GetCharacter()->RemoveCredits(static_cast< std::uint32_t >(Buy * FuelPrice));
		
		MonitorFuel * GlobalState(dynamic_cast< MonitorFuel * >(GetMind()->GetStateMachine()->GetGlobalState()));
		
		assert(GlobalState != nullptr);
		GlobalState->SetRefueled();
	}
}

void RefuelPhase3::Execute(void)
{
	if(GameTime::Get() >= m_TimeToLeave)
	{
		GetMind()->GetCharacter()->GetShip()->SetTakeOff(true);
	}
}

void RefuelPhase3::Exit(void)
{
}

bool RefuelPhase3::HandleMessage(Message * Message)
{
	if(Message->GetTypeIdentifier() == "taken_off")
	{
		GetMind()->GetStateMachine()->SetState(new SelectSteering(GetMind()));
		delete this;
		
		return true;
	}
	else
	{
		return false;
	}
}
