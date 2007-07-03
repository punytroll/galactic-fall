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

#include "cargo.h"
#include "character.h"
#include "commodity.h"
#include "game_time.h"
#include "math.h"
#include "mind.h"
#include "planet.h"
#include "ship.h"
#include "state_machine.h"
#include "states.h"
#include "string_cast.h"
#include "system.h"

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
	m_Point.Set(GetRandomFloat(-200.0f, 200.0f), GetRandomFloat(-200.0f, 200.0f));
}

void FlyOverRandomPoint::Execute(void)
{
	Vector2f ToDestination(m_Point - GetMind()->GetCharacter()->GetShip()->GetPosition());
	float LengthSquared(ToDestination.SquaredLength());
	
	if(LengthSquared > 400.0f)
	{
		ToDestination /= sqrt(LengthSquared);
		
		float HeadingOffDestination(GetShortestRadians(GetMind()->GetCharacter()->GetShip()->GetAngularPosition(), GetRadians(ToDestination)));
		
		if(HeadingOffDestination > 0.1)
		{
			GetMind()->GetCharacter()->GetShip()->SetTurnRight(1.0f);
			GetMind()->GetCharacter()->GetShip()->SetTurnLeft(0.0f);
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = false;
		}
		else if(HeadingOffDestination < -0.1)
		{
			GetMind()->GetCharacter()->GetShip()->SetTurnRight(0.0f);
			GetMind()->GetCharacter()->GetShip()->SetTurnLeft(1.0f);
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = false;
		}
		else
		{
			GetMind()->GetCharacter()->GetShip()->SetTurnRight(0.0f);
			GetMind()->GetCharacter()->GetShip()->SetTurnLeft(0.0f);
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = ((GetMind()->GetCharacter()->GetShip()->GetVelocity() - Vector2f(GetMind()->GetCharacter()->GetShip()->GetShipClass()->GetMaximumSpeed(), GetMind()->GetCharacter()->GetShip()->GetAngularPosition(), Vector2f::InitializeMagnitudeAngle)).SquaredLength() > 0.1f);
		}
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
	Vector2f ToDestination(GetMind()->GetCharacter()->GetShip()->GetTarget()->GetPosition() - GetMind()->GetCharacter()->GetShip()->GetPosition());
	float DistanceSquared(ToDestination.SquaredLength());
	float DistanceNeededToBrake(GetMind()->GetCharacter()->GetShip()->GetShipClass()->GetMaximumSpeed() * ((M_PI / GetMind()->GetCharacter()->GetShip()->GetShipClass()->GetTurnSpeed()) + ((GetMind()->GetCharacter()->GetShip()->GetShipClass()->GetMaximumSpeed() / GetMind()->GetCharacter()->GetShip()->GetShipClass()->GetForwardThrust()) / 2.0f)));
	
	// braking in phase 2 takes time (turning + accelerating)
	//  - turning: the ship will fly with maximum velocity towards the target
	//  - accelerating: the ship will fly half distance that it could with maximum velocity towards the target
	if(DistanceSquared > DistanceNeededToBrake * DistanceNeededToBrake)
	{
		ToDestination /= sqrt(DistanceSquared);
		
		float HeadingOffDestination(GetShortestRadians(GetMind()->GetCharacter()->GetShip()->GetAngularPosition(), GetRadians(ToDestination)));
		
		if(HeadingOffDestination > 0.1)
		{
			GetMind()->GetCharacter()->GetShip()->SetTurnRight(1.0f);
			GetMind()->GetCharacter()->GetShip()->SetTurnLeft(0.0f);
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = false;
		}
		else if(HeadingOffDestination < -0.1)
		{
			GetMind()->GetCharacter()->GetShip()->SetTurnRight(0.0f);
			GetMind()->GetCharacter()->GetShip()->SetTurnLeft(1.0f);
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = false;
		}
		else
		{
			GetMind()->GetCharacter()->GetShip()->SetTurnRight(0.0f);
			GetMind()->GetCharacter()->GetShip()->SetTurnLeft(0.0f);
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = ((GetMind()->GetCharacter()->GetShip()->GetVelocity() - Vector2f(GetMind()->GetCharacter()->GetShip()->GetShipClass()->GetMaximumSpeed(), GetMind()->GetCharacter()->GetShip()->GetAngularPosition(), Vector2f::InitializeMagnitudeAngle)).SquaredLength() > 0.1f);
		}
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
		float HeadingOffReverse(GetShortestRadians(GetMind()->GetCharacter()->GetShip()->GetAngularPosition(), GetRadians(-(GetMind()->GetCharacter()->GetShip()->GetVelocity().Normalized()))));
		
		if(HeadingOffReverse > 0.1)
		{
			GetMind()->GetCharacter()->GetShip()->SetTurnRight(1.0f);
			GetMind()->GetCharacter()->GetShip()->SetTurnLeft(0.0f);
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = false;
		}
		else if(HeadingOffReverse < -0.1)
		{
			GetMind()->GetCharacter()->GetShip()->SetTurnRight(0.0f);
			GetMind()->GetCharacter()->GetShip()->SetTurnLeft(1.0f);
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = false;
		}
		else
		{
			GetMind()->GetCharacter()->GetShip()->SetTurnRight(0.0f);
			GetMind()->GetCharacter()->GetShip()->SetTurnLeft(0.0f);
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = true;
		}
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
	
	// build a lookup map for commodities
	std::map< const Commodity *, PlanetCommodity * > Commodities;
	std::vector< PlanetCommodity * > BuyCommodities;
	
	{
		const std::vector< PlanetCommodity * > & PlanetCommodities(Planet->GetCommodities());
		
		for(std::vector< PlanetCommodity * >::size_type Index = 0; Index < PlanetCommodities.size(); ++Index)
		{
			Commodities[PlanetCommodities[Index]->GetCommodity()] = PlanetCommodities[Index];
			if(PlanetCommodities[Index]->GetBasePriceModifier() < 1.0f)
			{
				BuyCommodities.push_back(PlanetCommodities[Index]);
			}
		}
	}
	if(Commodities.empty() == false)
	{
		const std::set< Object * > & Manifest(GetMind()->GetCharacter()->GetShip()->GetContent());
		std::set< Object * >::const_iterator ManifestIterator(Manifest.begin());
		std::map< const Commodity *, PlanetCommodity * >::iterator PlanetCommodityIterator(Commodities.begin());
		
		// first sell stuff ... but only stuff with base price modifier above 1
		while(ManifestIterator != Manifest.end())
		{
			Cargo * TheCargo(dynamic_cast< Cargo * >(*ManifestIterator));
			
			if(TheCargo != 0)
			{
				// update the planet commodity cache, hoping it will be right more than once
				if(PlanetCommodityIterator == Commodities.end())
				{
					PlanetCommodityIterator = Commodities.find(TheCargo->GetCommodity());
				}
				else
				{
					if(PlanetCommodityIterator->first != TheCargo->GetCommodity())
					{
						PlanetCommodityIterator = Commodities.find(TheCargo->GetCommodity());
					}
				}
				// work with the cached planet commodity
				if((PlanetCommodityIterator != Commodities.end()) && (PlanetCommodityIterator->second->GetBasePriceModifier() > 1.0))
				{
					std::set< Object * >::iterator SaveIterator(ManifestIterator);
					
					++SaveIterator;
					GetMind()->GetCharacter()->GetShip()->RemoveContent(*ManifestIterator);
					ManifestIterator = SaveIterator;
					delete TheCargo;
					GetMind()->GetCharacter()->AddCredits(PlanetCommodityIterator->second->GetPrice());
					
					continue;
				}
			}
			++ManifestIterator;
		}
		// second buy stuff ... but only stuff with base price modifier below 1
		if(BuyCommodities.empty() == false)
		{
			for(int NumberOfCommoditiesToBuy = GetRandomIntegerFromExponentialDistribution(2); NumberOfCommoditiesToBuy > 0; --NumberOfCommoditiesToBuy)
			{
				PlanetCommodity * CommodityToBuy(BuyCommodities[GetRandomInteger(BuyCommodities.size() - 1)]);
				
				for(int NumberOfCargosToBuy = GetRandomIntegerFromExponentialDistribution(GetMind()->GetCharacter()->GetShip()->GetShipClass()->GetCargoHoldSize() / 2); NumberOfCargosToBuy > 0; --NumberOfCargosToBuy)
				{
					// TODO: the 400.0f is a safety margin for landing fees and fuel
					if((GetMind()->GetCharacter()->GetShip()->GetFreeCargoHoldSize() >= 1.0f) && (GetMind()->GetCharacter()->GetCredits() - 400.0f >= CommodityToBuy->GetPrice()))
					{
						GetMind()->GetCharacter()->RemoveCredits(CommodityToBuy->GetPrice());
						
						Cargo * NewCargo(new Cargo(CommodityToBuy->GetCommodity()));
						
						NewCargo->SetObjectIdentifier("::cargo(" + NewCargo->GetCommodity()->GetIdentifier() + ")::buy_index(" + to_string_cast(NumberOfCommoditiesToBuy) + "|" + to_string_cast(NumberOfCargosToBuy) + ")::bought_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::bought_by(" + GetMind()->GetObjectIdentifier() + ")::bought_on(" + Planet->GetObjectIdentifier() + ")");
						GetMind()->GetCharacter()->GetShip()->AddContent(NewCargo);
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
		
		// TODO: the 280.0f is a constant from main.cpp
		m_JumpPoint = GetMind()->GetCharacter()->GetShip()->GetPosition() + (((*SystemIterator)->GetPosition() - GetMind()->GetCharacter()->GetShip()->GetCurrentSystem()->GetPosition()).Normalize() * 280.0f * GetRandomFloat(1.0f, 1.2f));
	}
	else
	{
		GetMind()->GetStateMachine()->SetState(new FlyOverRandomPoint(GetMind()));
		delete this;
	}
}

void TransporterPhase4::Execute(void)
{
	Vector2f ToDestination(m_JumpPoint - GetMind()->GetCharacter()->GetShip()->GetPosition());
	float LengthSquared(ToDestination.SquaredLength());
	
	if(LengthSquared > 400.0f)
	{
		ToDestination /= sqrt(LengthSquared);
		
		float HeadingOffDestination(GetShortestRadians(GetMind()->GetCharacter()->GetShip()->GetAngularPosition(), GetRadians(ToDestination)));
		
		if(HeadingOffDestination > 0.1)
		{
			GetMind()->GetCharacter()->GetShip()->SetTurnRight(1.0f);
			GetMind()->GetCharacter()->GetShip()->SetTurnLeft(0.0f);
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = false;
		}
		else if(HeadingOffDestination < -0.1)
		{
			GetMind()->GetCharacter()->GetShip()->SetTurnRight(0.0f);
			GetMind()->GetCharacter()->GetShip()->SetTurnLeft(1.0f);
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = false;
		}
		else
		{
			GetMind()->GetCharacter()->GetShip()->SetTurnRight(0.0f);
			GetMind()->GetCharacter()->GetShip()->SetTurnLeft(0.0f);
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = ((GetMind()->GetCharacter()->GetShip()->GetVelocity() - Vector2f(GetMind()->GetCharacter()->GetShip()->GetShipClass()->GetMaximumSpeed(), GetMind()->GetCharacter()->GetShip()->GetAngularPosition(), Vector2f::InitializeMagnitudeAngle)).SquaredLength() > 0.1f);
		}
	}
	else
	{
		GetMind()->GetCharacter()->GetShip()->m_Jump = true;
		GetMind()->GetStateMachine()->SetState(new SelectSteering(GetMind()));
		delete this;
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
		Vector2f ToDestination(GetMind()->GetCharacter()->GetShip()->GetTarget()->GetPosition() - GetMind()->GetCharacter()->GetShip()->GetPosition());
		float Length(ToDestination.Length());
		// TODO: shot speed
		float SecondsForShot(Length / 30.0f);
		
		ToDestination -= GetMind()->GetCharacter()->GetShip()->GetVelocity() * SecondsForShot;
		Length = ToDestination.Length();
		ToDestination /= Length;
		
		float HeadingOffDestination(GetShortestRadians(GetMind()->GetCharacter()->GetShip()->GetAngularPosition(), GetRadians(ToDestination)));
		
		GetMind()->GetCharacter()->GetShip()->SetFire(false);
		if(HeadingOffDestination > 0.1)
		{
			GetMind()->GetCharacter()->GetShip()->SetTurnRight(1.0f);
			GetMind()->GetCharacter()->GetShip()->SetTurnLeft(0.0f);
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = false;
		}
		else if(HeadingOffDestination < -0.1)
		{
			GetMind()->GetCharacter()->GetShip()->SetTurnRight(0.0f);
			GetMind()->GetCharacter()->GetShip()->SetTurnLeft(1.0f);
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = false;
		}
		else
		{
			GetMind()->GetCharacter()->GetShip()->SetTurnRight(0.0f);
			GetMind()->GetCharacter()->GetShip()->SetTurnLeft(0.0f);
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = ((GetMind()->GetCharacter()->GetShip()->GetVelocity() - Vector2f(GetMind()->GetCharacter()->GetShip()->GetShipClass()->GetMaximumSpeed(), GetMind()->GetCharacter()->GetShip()->GetAngularPosition(), Vector2f::InitializeMagnitudeAngle)).SquaredLength() > 0.1f);
			if(Length < 50.0f)
			{
				GetMind()->GetCharacter()->GetShip()->SetFire(true);
			}
		}
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
	const std::list< Cargo * > & Cargos(GetMind()->GetCharacter()->GetShip()->GetCurrentSystem()->GetCargos());
	float MaximumDistance(FLT_MIN);
	Cargo * FarthestCargo(0);
	
	for(std::list< Cargo * >::const_iterator CargoIterator = Cargos.begin(); CargoIterator != Cargos.end(); ++CargoIterator)
	{
		float Distance((*CargoIterator)->GetPosition().SquaredLength());
		
		if(Distance > MaximumDistance)
		{
			FarthestCargo = *CargoIterator;
			MaximumDistance = Distance;
		}
	}
	if(FarthestCargo != 0)
	{
		GetMind()->GetCharacter()->GetShip()->SetTarget(FarthestCargo->GetReference());
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
		Vector2f ToDestination(GetMind()->GetCharacter()->GetShip()->GetTarget()->GetPosition() - GetMind()->GetCharacter()->GetShip()->GetPosition());
		float Length(ToDestination.Length());
		// TODO: shot speed
		float SecondsForShot(Length / 30.0f);
		
		ToDestination -= GetMind()->GetCharacter()->GetShip()->GetVelocity() * SecondsForShot;
		Length = ToDestination.Length();
		ToDestination /= Length;
		
		float HeadingOffDestination(GetShortestRadians(GetMind()->GetCharacter()->GetShip()->GetAngularPosition(), GetRadians(ToDestination)));
		
		GetMind()->GetCharacter()->GetShip()->SetFire(false);
		if(HeadingOffDestination > 0.1)
		{
			GetMind()->GetCharacter()->GetShip()->SetTurnRight(1.0f);
			GetMind()->GetCharacter()->GetShip()->SetTurnLeft(0.0f);
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = false;
		}
		else if(HeadingOffDestination < -0.1)
		{
			GetMind()->GetCharacter()->GetShip()->SetTurnRight(0.0f);
			GetMind()->GetCharacter()->GetShip()->SetTurnLeft(1.0f);
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = false;
		}
		else
		{
			GetMind()->GetCharacter()->GetShip()->SetTurnRight(0.0f);
			GetMind()->GetCharacter()->GetShip()->SetTurnLeft(0.0f);
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = ((GetMind()->GetCharacter()->GetShip()->GetVelocity() - Vector2f(GetMind()->GetCharacter()->GetShip()->GetShipClass()->GetMaximumSpeed(), GetMind()->GetCharacter()->GetShip()->GetAngularPosition(), Vector2f::InitializeMagnitudeAngle)).SquaredLength() > 0.1f);
			if(Length < 50.0f)
			{
				GetMind()->GetCharacter()->GetShip()->SetFire(true);
			}
		}
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
	Vector2f ToDestination(GetMind()->GetCharacter()->GetShip()->GetTarget()->GetPosition() - GetMind()->GetCharacter()->GetShip()->GetPosition());
	float DistanceSquared(ToDestination.SquaredLength());
	float DistanceNeededToBrake(GetMind()->GetCharacter()->GetShip()->GetShipClass()->GetMaximumSpeed() * ((M_PI / GetMind()->GetCharacter()->GetShip()->GetShipClass()->GetTurnSpeed()) + ((GetMind()->GetCharacter()->GetShip()->GetShipClass()->GetMaximumSpeed() / GetMind()->GetCharacter()->GetShip()->GetShipClass()->GetForwardThrust()) / 2.0f)));
	
	// braking in phase 2 takes time (turning + accelerating)
	//  - turning: the ship will fly with maximum velocity towards the target
	//  - accelerating: the ship will fly half distance that it could with maximum velocity towards the target
	if(DistanceSquared > DistanceNeededToBrake * DistanceNeededToBrake)
	{
		ToDestination /= sqrt(DistanceSquared);
		
		float HeadingOffDestination(GetShortestRadians(GetMind()->GetCharacter()->GetShip()->GetAngularPosition(), GetRadians(ToDestination)));
		
		if(HeadingOffDestination > 0.1)
		{
			GetMind()->GetCharacter()->GetShip()->SetTurnRight(1.0f);
			GetMind()->GetCharacter()->GetShip()->SetTurnLeft(0.0f);
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = false;
		}
		else if(HeadingOffDestination < -0.1)
		{
			GetMind()->GetCharacter()->GetShip()->SetTurnRight(0.0f);
			GetMind()->GetCharacter()->GetShip()->SetTurnLeft(1.0f);
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = false;
		}
		else
		{
			GetMind()->GetCharacter()->GetShip()->SetTurnRight(0.0f);
			GetMind()->GetCharacter()->GetShip()->SetTurnLeft(0.0f);
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = ((GetMind()->GetCharacter()->GetShip()->GetVelocity() - Vector2f(GetMind()->GetCharacter()->GetShip()->GetShipClass()->GetMaximumSpeed(), GetMind()->GetCharacter()->GetShip()->GetAngularPosition(), Vector2f::InitializeMagnitudeAngle)).SquaredLength() > 0.1f);
		}
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
		float HeadingOffReverse(GetShortestRadians(GetMind()->GetCharacter()->GetShip()->GetAngularPosition(), GetRadians(-(GetMind()->GetCharacter()->GetShip()->GetVelocity().Normalized()))));
		
		if(HeadingOffReverse > 0.1)
		{
			GetMind()->GetCharacter()->GetShip()->SetTurnRight(1.0f);
			GetMind()->GetCharacter()->GetShip()->SetTurnLeft(0.0f);
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = false;
		}
		else if(HeadingOffReverse < -0.1)
		{
			GetMind()->GetCharacter()->GetShip()->SetTurnRight(0.0f);
			GetMind()->GetCharacter()->GetShip()->SetTurnLeft(1.0f);
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = false;
		}
		else
		{
			GetMind()->GetCharacter()->GetShip()->SetTurnRight(0.0f);
			GetMind()->GetCharacter()->GetShip()->SetTurnLeft(0.0f);
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = true;
		}
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
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = false;
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
	
	const std::vector< PlanetCommodity * > & PlanetCommodities(Planet->GetCommodities());
	
	for(std::vector< PlanetCommodity * >::const_iterator PlanetCommodityIterator = PlanetCommodities.begin(); PlanetCommodityIterator != PlanetCommodities.end(); ++PlanetCommodityIterator)
	{
		if((*PlanetCommodityIterator)->GetCommodity()->GetIdentifier() == "fuel")
		{
			float FuelPrice((*PlanetCommodityIterator)->GetPrice());
			float CanBuy(GetMind()->GetCharacter()->GetCredits() / FuelPrice);
			float Need(GetMind()->GetCharacter()->GetShip()->GetFuelCapacity() - GetMind()->GetCharacter()->GetShip()->GetFuel());
			float Buy((CanBuy > Need) ? (Need) : (CanBuy));
			
			GetMind()->GetCharacter()->GetShip()->SetFuel(GetMind()->GetCharacter()->GetShip()->GetFuel() + Buy);
			GetMind()->GetCharacter()->RemoveCredits(Buy * FuelPrice);
			
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
