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

#include <iostream>
#include <list>
#include <stdexcept>

#include "asset_class.h"
#include "character.h"
#include "commodity.h"
#include "commodity_class.h"
#include "game_time.h"
#include "globals.h"
#include "math.h"
#include "mind.h"
#include "object_factory.h"
#include "planet.h"
#include "ship.h"
#include "state_machine.h"
#include "states.h"
#include "string_cast.h"
#include "system.h"

int WantToJump(Ship * Ship, System * System);

static bool FlyTo(Ship * Ship, const Vector3f & Direction)
{
	Vector3f LocalizedDirection(Direction);
	
	LocalizedDirection *= Ship->GetAngularPosition().Conjugated();
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
	if(GetMind()->GetCharacter()->GetShip()->GetShipClass()->GetIdentifier() == "transporter")
	{
		GetMind()->GetStateMachine()->SetState(new TransporterPhase1(GetMind()));
	}
	else
	{
		if(GetRandomBoolean() == true)
		{
			GetMind()->GetStateMachine()->SetState(new Fight(GetMind()));
		}
		else
		{
			GetMind()->GetStateMachine()->SetState(new ShootFarthestCargo(GetMind()));
		}
	}
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
	Vector3f ToDestination(m_Point - GetMind()->GetCharacter()->GetShip()->GetPosition());
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
	const std::vector< Planet * > & Planets(GetMind()->GetCharacter()->GetShip()->GetCurrentSystem()->GetPlanets());
	
	if(Planets.size() != 0)
	{
		GetMind()->GetCharacter()->GetShip()->SetTarget((*(Planets.begin() + GetRandomInteger(Planets.size() - 1)))->GetReference());
	}
	else
	{
		GetMind()->GetStateMachine()->SetState(new TransporterPhase4(GetMind()));
		delete this;
	}
}

void TransporterPhase1::Execute(void)
{
	Vector3f ToDestination(GetMind()->GetCharacter()->GetShip()->GetTarget()->GetPosition() - GetMind()->GetCharacter()->GetShip()->GetPosition());
	float DistanceSquared(ToDestination.SquaredLength());
	float DistanceNeededToBrake(GetMind()->GetCharacter()->GetShip()->GetShipClass()->GetMaximumSpeed() * ((M_PI / GetMind()->GetCharacter()->GetShip()->GetShipClass()->GetTurnSpeed()) + ((GetMind()->GetCharacter()->GetShip()->GetShipClass()->GetMaximumSpeed() / GetMind()->GetCharacter()->GetShip()->GetShipClass()->GetForwardThrust()) / 2.0f)));
	
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
		Reference< Planet > Planet(GetMind()->GetCharacter()->GetShip()->GetTarget());
		
		if(GetMind()->GetCharacter()->GetCredits() >= Planet->GetLandingFee())
		{
			GetMind()->GetCharacter()->RemoveCredits(Planet->GetLandingFee());
			GetMind()->GetCharacter()->GetShip()->m_Land = true;
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
	Reference< Planet > Planet(GetMind()->GetCharacter()->GetShip()->GetTarget());
	
	// build a lookup map for asset classes
	std::map< std::string, PlanetAssetClass * > PlanetAssetClasses;
	std::vector< PlanetAssetClass * > BuyPlanetAssetClasses;
	
	{
		const std::vector< PlanetAssetClass * > & OfferedPlanetAssetClasses(Planet->GetPlanetAssetClasses());
		
		for(std::vector< PlanetAssetClass * >::size_type Index = 0; Index < OfferedPlanetAssetClasses.size(); ++Index)
		{
			PlanetAssetClasses[OfferedPlanetAssetClasses[Index]->GetAssetClass()->GetIdentifier()] = OfferedPlanetAssetClasses[Index];
			if(OfferedPlanetAssetClasses[Index]->GetBasePriceModifier() < 1.0f)
			{
				BuyPlanetAssetClasses.push_back(OfferedPlanetAssetClasses[Index]);
			}
		}
	}
	if(PlanetAssetClasses.empty() == false)
	{
		const std::set< Object * > & Content(GetMind()->GetCharacter()->GetShip()->GetContent());
		std::set< Object * >::const_iterator ContentIterator(Content.begin());
		std::map< std::string, PlanetAssetClass * >::iterator PlanetAssetClassIterator(PlanetAssetClasses.end());
		
		// first sell commodities ... but only those with base price modifier above 1
		while(ContentIterator != Content.end())
		{
			Commodity * SellCommodity(dynamic_cast< Commodity * >(*ContentIterator));
			
			if(SellCommodity != 0)
			{
				// update the asset class cache, hoping it will be right more than once
				if((PlanetAssetClassIterator == PlanetAssetClasses.end()) || (PlanetAssetClassIterator->first != SellCommodity->GetCommodityClass()->GetIdentifier()))
				{
					PlanetAssetClassIterator = PlanetAssetClasses.find(SellCommodity->GetCommodityClass()->GetIdentifier());
				}
				// work with the cached asset class
				if((PlanetAssetClassIterator != PlanetAssetClasses.end()) && (PlanetAssetClassIterator->second->GetBasePriceModifier() > 1.0))
				{
					std::set< Object * >::iterator SaveIterator(ContentIterator);
					
					++SaveIterator;
					GetMind()->GetCharacter()->GetShip()->RemoveContent(SellCommodity);
					ContentIterator = SaveIterator;
					delete SellCommodity;
					GetMind()->GetCharacter()->AddCredits(PlanetAssetClassIterator->second->GetPrice());
					
					continue;
				}
			}
			++ContentIterator;
		}
		// second buy stuff ... but only stuff with base price modifier below 1
		if(BuyPlanetAssetClasses.empty() == false)
		{
			for(int NumberOfPlanetAssetClassesToBuy = GetRandomIntegerFromExponentialDistribution(2); NumberOfPlanetAssetClassesToBuy > 0; --NumberOfPlanetAssetClassesToBuy)
			{
				PlanetAssetClass * PlanetAssetClassToBuy(BuyPlanetAssetClasses[GetRandomInteger(BuyPlanetAssetClasses.size() - 1)]);
				
				for(int NumberOfAssetsToBuy = GetRandomIntegerFromExponentialDistribution(GetMind()->GetCharacter()->GetShip()->GetShipClass()->GetMaximumAvailableSpace() / g_ObjectFactory->GetSpaceRequirement(PlanetAssetClassToBuy->GetAssetClass()->GetObjectType(), PlanetAssetClassToBuy->GetAssetClass()->GetObjectClass())); NumberOfAssetsToBuy > 0; --NumberOfAssetsToBuy)
				{
					// TODO: the 400.0f is a safety margin for landing fees and fuel
					if((GetMind()->GetCharacter()->GetShip()->GetAvailableSpace() >= 1.0f) && (GetMind()->GetCharacter()->GetCredits() - 400.0f >= PlanetAssetClassToBuy->GetPrice()))
					{
						GetMind()->GetCharacter()->RemoveCredits(PlanetAssetClassToBuy->GetPrice());
						
						Object * NewCommodity(g_ObjectFactory->Create(PlanetAssetClassToBuy->GetAssetClass()->GetObjectType(), PlanetAssetClassToBuy->GetAssetClass()->GetObjectClass()));
						
						NewCommodity->SetObjectIdentifier("::" + PlanetAssetClassToBuy->GetAssetClass()->GetObjectType() + "(" + PlanetAssetClassToBuy->GetAssetClass()->GetObjectClass() + ")::buy_index(" + to_string_cast(NumberOfPlanetAssetClassesToBuy) + "|" + to_string_cast(NumberOfAssetsToBuy) + ")::bought_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::bought_by(" + GetMind()->GetObjectIdentifier() + ")::bought_on(" + Planet->GetObjectIdentifier() + ")");
						GetMind()->GetCharacter()->GetShip()->AddContent(NewCommodity);
					}
					else
					{
						break;
					}
				}
			}
		}
	}
}

void TransporterPhase3::Execute(void)
{
	if(GameTime::Get() >= m_TimeToLeave)
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
	}
}

void TransporterPhase3::Exit(void)
{
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
	const std::list< System * > & Systems(GetMind()->GetCharacter()->GetShip()->GetCurrentSystem()->GetLinkedSystems());
	
	if(Systems.size() != 0)
	{
		std::list< System * >::const_iterator SystemIterator(Systems.begin());
		
		for(std::list< System * >::size_type Choice = GetRandomInteger(Systems.size() - 1); Choice > 0; --Choice)
		{
			++SystemIterator;
		}
		GetMind()->GetCharacter()->GetShip()->SetLinkedSystemTarget(*SystemIterator);
		m_JumpDirection = ((*SystemIterator)->GetPosition() - GetMind()->GetCharacter()->GetShip()->GetCurrentSystem()->GetPosition()).Normalize();
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
			GetMind()->GetCharacter()->GetShip()->m_Jump = true;
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

///////////////////////////////////////////////////////////////////////////////////////////////////
// Fight: Selects a random fighter or drifting transporter and attacks it.                       //
///////////////////////////////////////////////////////////////////////////////////////////////////
Fight::Fight(StateMachineMind * Mind) :
	State(Mind)
{
	SetObjectIdentifier("::fight::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void Fight::Enter(void)
{
	const std::list< Ship * > & Ships(GetMind()->GetCharacter()->GetShip()->GetCurrentSystem()->GetShips());
	std::vector< Ship * > AttackPossibilities;
	
	for(std::list< Ship * >::const_iterator ShipIterator = Ships.begin(); ShipIterator != Ships.end(); ++ShipIterator)
	{
		if((*ShipIterator != GetMind()->GetCharacter()->GetShip()) && (((*ShipIterator)->GetShipClass()->GetIdentifier() == "fighter") || (((*ShipIterator)->GetShipClass()->GetIdentifier() == "transporter") && ((*ShipIterator)->GetFuel() < 0.0001))))
		{
			AttackPossibilities.push_back(*ShipIterator);
		}
	}
	if(AttackPossibilities.size() > 0)
	{
		GetMind()->GetCharacter()->GetShip()->SetTarget(AttackPossibilities[GetRandomInteger(AttackPossibilities.size() - 1)]->GetReference());
	}
	else
	{
		GetMind()->GetStateMachine()->SetState(new FlyOverRandomPoint(GetMind()));
		delete this;
	}
}

void Fight::Execute(void)
{
	if(GetMind()->GetCharacter()->GetShip()->GetTarget() == true)
	{
		Vector3f ToDestination(GetMind()->GetCharacter()->GetShip()->GetTarget()->GetPosition() - GetMind()->GetCharacter()->GetShip()->GetPosition());
		float Length(ToDestination.Length());
		// TODO: shot speed
		float SecondsForShot(Length / 30.0f);
		
		ToDestination -= GetMind()->GetCharacter()->GetShip()->GetVelocity() * SecondsForShot;
		Length = ToDestination.Length();
		ToDestination /= Length;
		
		bool Fire(false);
		
		Fire = FlyTo(GetMind()->GetCharacter()->GetShip(), ToDestination);
		GetMind()->GetCharacter()->GetShip()->SetFire(Fire && Length < 50.0f);
	}
	else
	{
		GetMind()->GetStateMachine()->SetState(new SelectSteering(GetMind()));
		delete this;
	}
}

void Fight::Exit(void)
{
	GetMind()->GetCharacter()->GetShip()->SetFire(false);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// ShootFarthestCargo: Selects the cargo that is most remote from the system center.             //
//                     Attacks it.                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////
ShootFarthestCargo::ShootFarthestCargo(StateMachineMind * Mind) :
	State(Mind)
{
	SetObjectIdentifier("::shoot_farthest_cargo::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void ShootFarthestCargo::Enter(void)
{
	const std::list< Commodity * > & Commodities(GetMind()->GetCharacter()->GetShip()->GetCurrentSystem()->GetCommodities());
	float MaximumDistance(FLT_MIN);
	Commodity * FarthestCommodity(0);
	
	for(std::list< Commodity * >::const_iterator CommodityIterator = Commodities.begin(); CommodityIterator != Commodities.end(); ++CommodityIterator)
	{
		float Distance((*CommodityIterator)->GetPosition().SquaredLength());
		
		if(Distance > MaximumDistance)
		{
			FarthestCommodity = *CommodityIterator;
			MaximumDistance = Distance;
		}
	}
	if(FarthestCommodity != 0)
	{
		GetMind()->GetCharacter()->GetShip()->SetTarget(FarthestCommodity->GetReference());
	}
	else
	{
		GetMind()->GetStateMachine()->SetState(new SelectSteering(GetMind()));
		delete this;
	}
}

void ShootFarthestCargo::Execute(void)
{
	if(GetMind()->GetCharacter()->GetShip()->GetTarget() == true)
	{
		Vector3f ToDestination(GetMind()->GetCharacter()->GetShip()->GetTarget()->GetPosition() - GetMind()->GetCharacter()->GetShip()->GetPosition());
		float Length(ToDestination.Length());
		// TODO: shot speed
		float SecondsForShot(Length / 30.0f);
		
		ToDestination -= GetMind()->GetCharacter()->GetShip()->GetVelocity() * SecondsForShot;
		Length = ToDestination.Length();
		ToDestination /= Length;
		
		bool Fire(false);
		
		Fire = FlyTo(GetMind()->GetCharacter()->GetShip(), ToDestination);
		GetMind()->GetCharacter()->GetShip()->SetFire(Fire && Length < 50.0f);
	}
	else
	{
		GetMind()->GetStateMachine()->SetState(new SelectSteering(GetMind()));
		delete this;
	}
}

void ShootFarthestCargo::Exit(void)
{
	GetMind()->GetCharacter()->GetShip()->SetFire(false);
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
	const std::vector< Planet * > & Planets(GetMind()->GetCharacter()->GetShip()->GetCurrentSystem()->GetPlanets());
	float MinimumDistance(FLT_MAX);
	Planet * NearestPlanet(0);
	
	for(std::vector< Planet * >::const_iterator PlanetIterator = Planets.begin(); PlanetIterator != Planets.end(); ++PlanetIterator)
	{
		float Distance(((*PlanetIterator)->GetPosition() - GetMind()->GetCharacter()->GetShip()->GetPosition()).SquaredLength());
		
		if(Distance < MinimumDistance)
		{
			NearestPlanet = *PlanetIterator;
			MinimumDistance = Distance;
		}
	}
	if(NearestPlanet != 0)
	{
		GetMind()->GetCharacter()->GetShip()->SetTarget(NearestPlanet->GetReference());
	}
	else
	{
		GetMind()->GetStateMachine()->SetState(new SelectSteering(GetMind()));
		delete this;
	}
}

void RefuelPhase1::Execute(void)
{
	Vector3f ToDestination(GetMind()->GetCharacter()->GetShip()->GetTarget()->GetPosition() - GetMind()->GetCharacter()->GetShip()->GetPosition());
	float DistanceSquared(ToDestination.SquaredLength());
	float DistanceNeededToBrake(GetMind()->GetCharacter()->GetShip()->GetShipClass()->GetMaximumSpeed() * ((M_PI / GetMind()->GetCharacter()->GetShip()->GetShipClass()->GetTurnSpeed()) + ((GetMind()->GetCharacter()->GetShip()->GetShipClass()->GetMaximumSpeed() / GetMind()->GetCharacter()->GetShip()->GetShipClass()->GetForwardThrust()) / 2.0f)));
	
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
		Reference< Planet > Planet(GetMind()->GetCharacter()->GetShip()->GetTarget());
		
		if(GetMind()->GetCharacter()->GetCredits() >= Planet->GetLandingFee())
		{
			GetMind()->GetCharacter()->RemoveCredits(Planet->GetLandingFee());
			GetMind()->GetCharacter()->GetShip()->m_Land = true;
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
	Reference< Planet > Planet(GetMind()->GetCharacter()->GetShip()->GetTarget());
	
	const std::vector< PlanetAssetClass * > & PlanetAssetClasses(Planet->GetPlanetAssetClasses());
	
	for(std::vector< PlanetAssetClass * >::const_iterator PlanetAssetClassIterator = PlanetAssetClasses.begin(); PlanetAssetClassIterator != PlanetAssetClasses.end(); ++PlanetAssetClassIterator)
	{
		if((*PlanetAssetClassIterator)->GetAssetClass()->GetIdentifier() == "fuel")
		{
			u4byte FuelPrice((*PlanetAssetClassIterator)->GetPrice());
			float CanBuy(GetMind()->GetCharacter()->GetCredits() / FuelPrice);
			float Need(GetMind()->GetCharacter()->GetShip()->GetFuelCapacity() - GetMind()->GetCharacter()->GetShip()->GetFuel());
			float Buy((CanBuy > Need) ? (Need) : (CanBuy));
			
			GetMind()->GetCharacter()->GetShip()->SetFuel(GetMind()->GetCharacter()->GetShip()->GetFuel() + Buy);
			GetMind()->GetCharacter()->RemoveCredits(static_cast< u4byte >(Buy * FuelPrice));
			
			MonitorFuel * GlobalState(dynamic_cast< MonitorFuel * >(GetMind()->GetStateMachine()->GetGlobalState()));
			
			assert(GlobalState != 0);
			GlobalState->SetRefueled();
			break;
		}
	}
}

void RefuelPhase3::Execute(void)
{
	if(GameTime::Get() >= m_TimeToLeave)
	{
		GetMind()->GetStateMachine()->SetState(new SelectSteering(GetMind()));
		delete this;
	}
}

void RefuelPhase3::Exit(void)
{
}
