/**
 * galactic-fall
 * Copyright (C) 2008  Aram Altschudjian
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

#include <float.h>

#include <list>

#include "asset_class.h"
#include "character.h"
#include "commodity.h"
#include "game_time.h"
#include "goals.h"
#include "math.h"
#include "message.h"
#include "messages.h"
#include "mind.h"
#include "planet.h"
#include "ship.h"
#include "string_cast.h"
#include "system.h"
#include "threat.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// GoalApproachTarget                                                                            //
///////////////////////////////////////////////////////////////////////////////////////////////////
GoalApproachTarget::GoalApproachTarget(GoalMind * GoalMind) :
	Goal(GoalMind, "approach_target"),
	m_FlyInDirection(0)
{
	SetObjectIdentifier("::goal(approach_target)::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void GoalApproachTarget::Activate(void)
{
	assert(GetSubGoals().empty() == true);
	m_FlyInDirection = new GoalFlyInDirection(GetMind());
	AddContent(m_FlyInDirection);
	m_FlyInDirection->Activate();
	SetState(Goal::ACTIVE);
}

void GoalApproachTarget::Process(void)
{
	assert(GetState() == Goal::ACTIVE);
	
	Vector3f ToDestination(GetMind()->GetCharacter()->GetShip()->GetTarget()->GetPosition() - GetMind()->GetCharacter()->GetShip()->GetPosition());
	float DistanceSquared(ToDestination.SquaredLength());
	float DistanceNeededToBrake(GetMind()->GetCharacter()->GetShip()->GetShipClass()->GetMaximumSpeed() * ((M_PI / GetMind()->GetCharacter()->GetShip()->GetShipClass()->GetTurnSpeed()) + ((GetMind()->GetCharacter()->GetShip()->GetShipClass()->GetMaximumSpeed() / GetMind()->GetCharacter()->GetShip()->GetShipClass()->GetForwardThrust()) / 2.0f)));
	
	// braking to do a full stop takes time (turning + accelerating)
	//  - turning: the ship will fly with maximum velocity towards the target
	//  - accelerating: the ship will fly half distance that it could with maximum velocity towards the target
	if(DistanceSquared > DistanceNeededToBrake * DistanceNeededToBrake)
	{
		m_FlyInDirection->SetDirection(ToDestination / sqrt(DistanceSquared));
		assert(GetSubGoals().size() == 1);
		assert(GetSubGoals().front()->GetState() == Goal::ACTIVE);
		GetSubGoals().front()->Process();
	}
	else
	{
		SetState(Goal::COMPLETED);
	}
}

void GoalApproachTarget::Terminate(void)
{
	m_FlyInDirection->Terminate();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// GoalDestroyTarget                                                                             //
///////////////////////////////////////////////////////////////////////////////////////////////////
GoalDestroyTarget::GoalDestroyTarget(GoalMind * GoalMind) :
	Goal(GoalMind, "destroy_target"),
	m_FlyInDirection(0)
{
	SetObjectIdentifier("::goal(destroy_target)::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void GoalDestroyTarget::Activate(void)
{
	assert(GetSubGoals().empty() == true);
	m_FlyInDirection = new GoalFlyInDirection(GetMind());
	AddContent(m_FlyInDirection);
	m_FlyInDirection->Activate();
	SetState(Goal::ACTIVE);
}

void GoalDestroyTarget::Process(void)
{
	assert(GetState() == Goal::ACTIVE);
	if(GetMind()->GetCharacter()->GetShip()->GetTarget().IsValid() == true)
	{
		Vector3f ToDestination(GetMind()->GetCharacter()->GetShip()->GetTarget()->GetPosition() - GetMind()->GetCharacter()->GetShip()->GetPosition());
		float Length(ToDestination.Length());
		// TODO: shot speed
		float SecondsForShot(Length / 30.0f);
		
		ToDestination -= GetMind()->GetCharacter()->GetShip()->GetVelocity() * SecondsForShot;
		Length = ToDestination.Length();
		m_FlyInDirection->SetDirection(ToDestination / Length);
		assert(GetSubGoals().size() == 1);
		assert(GetSubGoals().front()->GetState() == Goal::ACTIVE);
		GetSubGoals().front()->Process();
		// TODO: shot reach
		GetMind()->GetCharacter()->GetShip()->SetFire((m_FlyInDirection->GetFacesDirection() == true) && (Length < 50.0f));
	}
	else
	{
		SetState(Goal::COMPLETED);
	}
}

void GoalDestroyTarget::Terminate(void)
{
	GetMind()->GetCharacter()->GetShip()->SetFire(false);
	m_FlyInDirection->Terminate();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// GoalFighterThink                                                                              //
///////////////////////////////////////////////////////////////////////////////////////////////////
GoalFighterThink::GoalFighterThink(GoalMind * GoalMind) :
	Goal(GoalMind, "fighter_think")
{
	SetObjectIdentifier("::goal(fighter_think)::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void GoalFighterThink::Activate(void)
{
	assert(GetSubGoals().empty() == true);
	AddContent(new GoalFightSomeTarget(GetMind()));
	SetState(Goal::ACTIVE);
}

bool GoalFighterThink::OnMessageReceived(Message * Message)
{
	if(Message->GetTypeIdentifier() == "threat")
	{
		ThreatMessage * TheThreatMessage(dynamic_cast< ThreatMessage * >(Message));
		
		GetMind()->GetCharacter()->GetThreat()->ModifyThreat(TheThreatMessage->GetSender(), TheThreatMessage->GetDeltaThreat());
		
		return true;
	}
	else if(Message->GetTypeIdentifier() == "fuel_low")
	{
		if(HasSubGoal("visit_planet") == false)
		{
			AddContent(new GoalVisitPlanet(GetMind(), GoalVisitPlanet::VISIT_NEAREST_PLANET_IN_SYSTEM));
		}
		
		return true;
	}
	
	return false;
}

void GoalFighterThink::Process(void)
{
	assert(GetState() == Goal::ACTIVE);
	assert(GetSubGoals().empty() == false);
	
	Goal * SubGoal(GetSubGoals().front());
	
	if(SubGoal->GetState() == Goal::INACTIVE)
	{
		SubGoal->Activate();
	}
	assert(SubGoal->GetState() == Goal::ACTIVE);
	SubGoal->Process();
	if(SubGoal->GetState() == Goal::COMPLETED)
	{
		// terminate and remove sub goal
		SubGoal->Terminate();
		RemoveContent(SubGoal);
		// other actions may depend on the SubGoal variable
		if(GetSubGoals().empty() == true)
		{
			SetState(Goal::INACTIVE);
		}
		// now destroy the SubGoal
		SubGoal->Destroy();
		delete SubGoal;
	}
	else if(SubGoal->GetState() == Goal::FAILED)
	{
		// terminate and remove sub goal
		SubGoal->Terminate();
		RemoveContent(SubGoal);
		// other actions may depend on the SubGoal variable
		// FightSomeTarget may fail if no fighter is present in the system: fly around
		if(SubGoal->GetName() == "fight_some_target")
		{
			AddContent(new GoalFlyOverRandomPoint(GetMind()));
		}
		// now destroy the SubGoal
		SubGoal->Destroy();
		delete SubGoal;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// GoalFightSomeTarget                                                                           //
///////////////////////////////////////////////////////////////////////////////////////////////////
GoalFightSomeTarget::GoalFightSomeTarget(GoalMind * GoalMind) :
	Goal(GoalMind, "fight_some_target")
{
	SetObjectIdentifier("::goal(fight_some_target)::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void GoalFightSomeTarget::Activate(void)
{
	assert(GetSubGoals().empty() == true);
	AddContent(new GoalDestroyTarget(GetMind()));
	AddContent(new GoalSelectFightableTarget(GetMind()));
	SetState(Goal::ACTIVE);
}

void GoalFightSomeTarget::Process(void)
{
	assert(GetState() == Goal::ACTIVE);
	assert(GetSubGoals().empty() == false);
	
	const Reference< Object > * ObjectWithHighestThreat(GetMind()->GetCharacter()->GetThreat()->GetObjectWithHighestThreat());
	
	if(ObjectWithHighestThreat != 0)
	{
		GetMind()->GetCharacter()->GetShip()->SetTarget(*ObjectWithHighestThreat);
	}
	
	Goal * SubGoal(GetSubGoals().front());
	
	if(SubGoal->GetState() == Goal::INACTIVE)
	{
		SubGoal->Activate();
	}
	assert(SubGoal->GetState() == Goal::ACTIVE);
	SubGoal->Process();
	if(SubGoal->GetState() == Goal::COMPLETED)
	{
		// terminate and remove sub goal
		SubGoal->Terminate();
		RemoveContent(SubGoal);
		// other actions may depend on the SubGoal variable
		if(SubGoal->GetName() == "destroy_target")
		{
			SetState(Goal::COMPLETED);
		}
		// now destroy the SubGoal
		SubGoal->Destroy();
		delete SubGoal;
	}
	else if(SubGoal->GetState() == Goal::FAILED)
	{
		// terminate and remove sub goal
		SubGoal->Terminate();
		RemoveContent(SubGoal);
		// other actions may depend on the SubGoal variable
		SetState(Goal::FAILED);
		// now destroy the SubGoal
		SubGoal->Destroy();
		delete SubGoal;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// GoalFlyInDirection                                                                            //
///////////////////////////////////////////////////////////////////////////////////////////////////
GoalFlyInDirection::GoalFlyInDirection(GoalMind * GoalMind) :
	Goal(GoalMind, "fly_in_direction"),
	m_Direction(true),
	m_FacesDirection(false)
{
	SetObjectIdentifier("::goal(fly_in_direction)::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

bool GoalFlyInDirection::GetFacesDirection(void) const
{
	return m_FacesDirection;
}

void GoalFlyInDirection::SetDirection(const Vector3f & Direction)
{
	m_Direction = Direction;
}

void GoalFlyInDirection::Activate(void)
{
	assert(GetSubGoals().empty() == true);
	SetState(Goal::ACTIVE);
}

void GoalFlyInDirection::Process(void)
{
	assert(GetState() == Goal::ACTIVE);
	
	Vector3f LocalizedDirection(m_Direction);
	
	LocalizedDirection *= GetMind()->GetCharacter()->GetShip()->GetAngularPosition().Conjugated();
	if((LocalizedDirection[1] > 0.1f) || ((LocalizedDirection[0] < 0.0f) && (LocalizedDirection[1] >= 0.0f)))
	{
		GetMind()->GetCharacter()->GetShip()->SetTurnLeft(1.0f);
		GetMind()->GetCharacter()->GetShip()->SetTurnRight(0.0f);
		GetMind()->GetCharacter()->GetShip()->SetAccelerate(false);
		m_FacesDirection = false;
	}
	else if((LocalizedDirection[1] < -0.1f) || ((LocalizedDirection[0] < 0.0f) && (LocalizedDirection[1] < 0.0f)))
	{
		GetMind()->GetCharacter()->GetShip()->SetTurnLeft(0.0f);
		GetMind()->GetCharacter()->GetShip()->SetTurnRight(1.0f);
		GetMind()->GetCharacter()->GetShip()->SetAccelerate(false);
		m_FacesDirection = false;
	}
	else
	{
		GetMind()->GetCharacter()->GetShip()->SetTurnLeft(0.0f);
		GetMind()->GetCharacter()->GetShip()->SetTurnRight(0.0f);
		GetMind()->GetCharacter()->GetShip()->SetAccelerate(GetMind()->GetCharacter()->GetShip()->GetMaximumSpeed() - m_Direction.Dot(GetMind()->GetCharacter()->GetShip()->GetVelocity()) > 0.1f);
		m_FacesDirection = true;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// GoalFlyOverRandomPoint                                                                        //
///////////////////////////////////////////////////////////////////////////////////////////////////
GoalFlyOverRandomPoint::GoalFlyOverRandomPoint(GoalMind * GoalMind) :
	Goal(GoalMind, "fly_over_random_point"),
	m_FlyInDirection(0)
{
	SetObjectIdentifier("::goal(fly_over_random_point)::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void GoalFlyOverRandomPoint::Activate(void)
{
	assert(GetSubGoals().empty() == true);
	m_FlyInDirection = new GoalFlyInDirection(GetMind());
	AddContent(m_FlyInDirection);
	m_FlyInDirection->Activate();
	m_Point.Set(GetRandomFloat(-200.0f, 200.0f), GetRandomFloat(-200.0f, 200.0f), 0.0f);
	SetState(Goal::ACTIVE);
}

void GoalFlyOverRandomPoint::Process(void)
{
	assert(GetState() == Goal::ACTIVE);
	
	Vector3f ToDestination(m_Point - GetMind()->GetCharacter()->GetShip()->GetPosition());
	float LengthSquared(ToDestination.SquaredLength());
	
	if(LengthSquared > 400.0f)
	{
		m_FlyInDirection->SetDirection(ToDestination / sqrt(LengthSquared));
		assert(GetSubGoals().front()->GetState() == Goal::ACTIVE);
		GetSubGoals().front()->Process();
	}
	else
	{
		SetState(Goal::COMPLETED);
	}
}

void GoalFlyOverRandomPoint::Terminate(void)
{
	m_FlyInDirection->Terminate();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// GoalFullStop                                                                                  //
///////////////////////////////////////////////////////////////////////////////////////////////////
GoalFullStop::GoalFullStop(GoalMind * GoalMind) :
	Goal(GoalMind, "full_stop"),
	m_FlyInDirection(0)
{
	SetObjectIdentifier("::goal(full_stop)::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void GoalFullStop::Activate(void)
{
	assert(GetSubGoals().empty() == true);
	m_FlyInDirection = new GoalFlyInDirection(GetMind());
	AddContent(m_FlyInDirection);
	m_FlyInDirection->Activate();
	SetState(Goal::ACTIVE);
}

void GoalFullStop::Process(void)
{
	assert(GetState() == Goal::ACTIVE);
	
	float SpeedSquared(GetMind()->GetCharacter()->GetShip()->GetVelocity().SquaredLength());
	
	if(SpeedSquared > 2.0f)
	{
		m_FlyInDirection->SetDirection(-GetMind()->GetCharacter()->GetShip()->GetVelocity().Normalized());
		assert(GetSubGoals().size() == 1);
		assert(GetSubGoals().front()->GetState() == Goal::ACTIVE);
		GetSubGoals().front()->Process();
	}
	else
	{
		SetState(Goal::COMPLETED);
	}
}

void GoalFullStop::Terminate(void)
{
	m_FlyInDirection->Terminate();
	GetMind()->GetCharacter()->GetShip()->SetAccelerate(false);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// GoalLandOnPlanet                                                                              //
///////////////////////////////////////////////////////////////////////////////////////////////////
GoalLandOnPlanet::GoalLandOnPlanet(GoalMind * GoalMind) :
	Goal(GoalMind, "land_on_planet")
{
	SetObjectIdentifier("::goal(land_on_planet)::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void GoalLandOnPlanet::Activate(void)
{
	assert(GetSubGoals().empty() == true);
	SetState(Goal::ACTIVE);
}

void GoalLandOnPlanet::Process(void)
{
	assert(GetState() == Goal::ACTIVE);
	GetMind()->GetCharacter()->GetShip()->m_Land = true;
	SetState(Goal::COMPLETED);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// GoalRefuel                                                                                    //
///////////////////////////////////////////////////////////////////////////////////////////////////
GoalRefuel::GoalRefuel(GoalMind * GoalMind, const Reference< Planet > & Planet) :
	Goal(GoalMind, "refuel"),
	m_Planet(Planet)
{
	SetObjectIdentifier("::goal(refuel)::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void GoalRefuel::Activate(void)
{
	assert(GetSubGoals().empty() == true);
	SetState(Goal::ACTIVE);
}

void GoalRefuel::Process(void)
{
	assert(GetState() == Goal::ACTIVE);
	
	const std::vector< PlanetAssetClass * > & PlanetAssetClasses(m_Planet->GetPlanetAssetClasses());
	bool SuccessfullyRefueled(false);
	
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
			SuccessfullyRefueled = true;
			
			break;
		}
	}
	SetState((SuccessfullyRefueled == true) ? (Goal::COMPLETED) : (Goal::FAILED));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// GoalSelectFightableTarget                                                                     //
///////////////////////////////////////////////////////////////////////////////////////////////////
GoalSelectFightableTarget::GoalSelectFightableTarget(GoalMind * GoalMind) :
	Goal(GoalMind, "select_fightable_target")
{
	SetObjectIdentifier("::goal(select_fightable_target)::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void GoalSelectFightableTarget::Activate(void)
{
	assert(GetSubGoals().empty() == true);
	AddContent(new GoalSelectFighter(GetMind()));
	AddContent(new GoalSelectStrandedShip(GetMind()));
	AddContent(new GoalSelectMeasuredCargo(GetMind()));
	SetState(Goal::ACTIVE);
}

void GoalSelectFightableTarget::Process(void)
{
	assert(GetState() == Goal::ACTIVE);
	assert(GetSubGoals().empty() == false);
	
	Goal * SubGoal(GetSubGoals().front());
	
	if(SubGoal->GetState() == Goal::INACTIVE)
	{
		SubGoal->Activate();
	}
	assert(SubGoal->GetState() == Goal::ACTIVE);
	SubGoal->Process();
	if(SubGoal->GetState() == Goal::COMPLETED)
	{
		// terminate and remove sub goal
		SubGoal->Terminate();
		RemoveContent(SubGoal);
		// other actions may depend on the SubGoal variable
		// if one subgoal succeeds the goal has done its job
		SetState(Goal::COMPLETED);
		// now destroy the SubGoal
		SubGoal->Destroy();
		delete SubGoal;
	}
	else if(SubGoal->GetState() == Goal::FAILED)
	{
		// terminate and remove sub goal
		SubGoal->Terminate();
		RemoveContent(SubGoal);
		// other actions may depend on the SubGoal variable
		// if no subgoal succeeded, this goal has failed
		if(GetSubGoals().empty() == true)
		{
			SetState(Goal::FAILED);
		}
		// now destroy the SubGoal
		SubGoal->Destroy();
		delete SubGoal;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// GoalSelectFighter                                                                             //
///////////////////////////////////////////////////////////////////////////////////////////////////
GoalSelectFighter::GoalSelectFighter(GoalMind * GoalMind) :
	Goal(GoalMind, "select_fighter")
{
	SetObjectIdentifier("::goal(select_fighter)::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void GoalSelectFighter::Activate(void)
{
	assert(GetSubGoals().empty() == true);
	SetState(Goal::ACTIVE);
}

void GoalSelectFighter::Process(void)
{
	assert(GetState() == Goal::ACTIVE);
	
	const std::list< Ship * > & Ships(GetMind()->GetCharacter()->GetShip()->GetCurrentSystem()->GetShips());
	std::vector< Ship * > AttackPossibilities;
	
	for(std::list< Ship * >::const_iterator ShipIterator = Ships.begin(); ShipIterator != Ships.end(); ++ShipIterator)
	{
		if((*ShipIterator != GetMind()->GetCharacter()->GetShip()) && ((*ShipIterator)->GetShipClass()->GetIdentifier() == "fighter"))
		{
			AttackPossibilities.push_back(*ShipIterator);
		}
	}
	if(AttackPossibilities.size() > 0)
	{
		GetMind()->GetCharacter()->GetShip()->SetTarget(AttackPossibilities[GetRandomInteger(AttackPossibilities.size() - 1)]->GetReference());
		SetState(Goal::COMPLETED);
	}
	else
	{
		SetState(Goal::FAILED);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// GoalSelectMeasuredCargo                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////
GoalSelectMeasuredCargo::GoalSelectMeasuredCargo(GoalMind * GoalMind) :
	Goal(GoalMind, "select_measured_cargo")
{
	SetObjectIdentifier("::goal(select_measured_cargo)::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void GoalSelectMeasuredCargo::Activate(void)
{
	assert(GetSubGoals().empty() == true);
	SetState(Goal::ACTIVE);
}

void GoalSelectMeasuredCargo::Process(void)
{
	assert(GetState() == Goal::ACTIVE);
	
	const std::list< Commodity * > & Commodities(GetMind()->GetCharacter()->GetShip()->GetCurrentSystem()->GetCommodities());
	float MinimumCost(FLT_MAX);
	Commodity * MinimumCostCommodity(0);
	
	for(std::list< Commodity * >::const_iterator CommodityIterator = Commodities.begin(); CommodityIterator != Commodities.end(); ++CommodityIterator)
	{
		// what this formula does is effectively calculating the distance of the cargo from a point that is twice a much away from the system center as the ship
		// Why is this sensible? Because fighters will prefer cargo that is on "their" side of the system and is farther away from the system center than they are.
		float Cost(((*CommodityIterator)->GetPosition() - 2.0f * GetMind()->GetCharacter()->GetShip()->GetPosition()).SquaredLength());
		
		if(Cost < MinimumCost)
		{
			MinimumCostCommodity = *CommodityIterator;
			MinimumCost = Cost;
		}
	}
	if(MinimumCostCommodity != 0)
	{
		GetMind()->GetCharacter()->GetShip()->SetTarget(MinimumCostCommodity->GetReference());
		SetState(Goal::COMPLETED);
	}
	else
	{
		SetState(Goal::FAILED);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// GoalSelectNearestPlanet                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////
GoalSelectNearestPlanet::GoalSelectNearestPlanet(GoalMind * GoalMind) :
	Goal(GoalMind, "select_nearest_planet")
{
	SetObjectIdentifier("::goal(select_nearest_planet)::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void GoalSelectNearestPlanet::Activate(void)
{
	assert(GetSubGoals().empty() == true);
	SetState(Goal::ACTIVE);
}

void GoalSelectNearestPlanet::Process(void)
{
	assert(GetState() == Goal::ACTIVE);
	
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
		SetState(Goal::COMPLETED);
	}
	else
	{
		SetState(Goal::FAILED);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// GoalSelectStrandedShip                                                                        //
///////////////////////////////////////////////////////////////////////////////////////////////////
GoalSelectStrandedShip::GoalSelectStrandedShip(GoalMind * GoalMind) :
	Goal(GoalMind, "select_stranded_ship")
{
	SetObjectIdentifier("::goal(select_stranded_ship)::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void GoalSelectStrandedShip::Activate(void)
{
	assert(GetSubGoals().empty() == true);
	SetState(Goal::ACTIVE);
}

void GoalSelectStrandedShip::Process(void)
{
	assert(GetState() == Goal::ACTIVE);
	
	const std::list< Ship * > & Ships(GetMind()->GetCharacter()->GetShip()->GetCurrentSystem()->GetShips());
	std::vector< Ship * > AttackPossibilities;
	
	for(std::list< Ship * >::const_iterator ShipIterator = Ships.begin(); ShipIterator != Ships.end(); ++ShipIterator)
	{
		if((*ShipIterator != GetMind()->GetCharacter()->GetShip()) && ((*ShipIterator)->GetFuel() < 0.01))
		{
			AttackPossibilities.push_back(*ShipIterator);
		}
	}
	if(AttackPossibilities.size() > 0)
	{
		GetMind()->GetCharacter()->GetShip()->SetTarget(AttackPossibilities[GetRandomInteger(AttackPossibilities.size() - 1)]->GetReference());
		SetState(Goal::COMPLETED);
	}
	else
	{
		SetState(Goal::FAILED);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// GoalStopAtTarget                                                                              //
///////////////////////////////////////////////////////////////////////////////////////////////////
GoalStopAtTarget::GoalStopAtTarget(GoalMind * GoalMind) :
	Goal(GoalMind, "stop_at_target")
{
	SetObjectIdentifier("::goal(stop_at_target)::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void GoalStopAtTarget::Activate(void)
{
	assert(GetSubGoals().empty() == true);
	AddContent(new GoalFullStop(GetMind()));
	AddContent(new GoalApproachTarget(GetMind()));
	SetState(Goal::ACTIVE);
}

void GoalStopAtTarget::Process(void)
{
	assert(GetState() == Goal::ACTIVE);
	assert(GetSubGoals().empty() == false);
	
	Goal * SubGoal(GetSubGoals().front());
	
	if(SubGoal->GetState() == Goal::INACTIVE)
	{
		SubGoal->Activate();
	}
	assert(SubGoal->GetState() == Goal::ACTIVE);
	SubGoal->Process();
	if(SubGoal->GetState() == Goal::COMPLETED)
	{
		// terminate and remove sub goal
		SubGoal->Terminate();
		RemoveContent(SubGoal);
		// other actions may depend on the SubGoal variable
		if(SubGoal->GetName() == "full_stop")
		{
			SetState(Goal::COMPLETED);
		}
		// now destroy the SubGoal
		SubGoal->Destroy();
		delete SubGoal;
	}
	else if(SubGoal->GetState() == Goal::FAILED)
	{
		// terminate and remove sub goal
		SubGoal->Terminate();
		RemoveContent(SubGoal);
		// other actions may depend on the SubGoal variable
		SetState(Goal::FAILED);
		// now destroy the SubGoal
		SubGoal->Destroy();
		delete SubGoal;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// GoalTakeOffFromPlanet                                                                         //
///////////////////////////////////////////////////////////////////////////////////////////////////
GoalTakeOffFromPlanet::GoalTakeOffFromPlanet(GoalMind * GoalMind) :
	Goal(GoalMind, "take_off_from_planet")
{
	SetObjectIdentifier("::goal(take_off_from_planet)::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void GoalTakeOffFromPlanet::Activate(void)
{
	assert(GetSubGoals().empty() == true);
	SetState(Goal::ACTIVE);
}

void GoalTakeOffFromPlanet::Process(void)
{
	assert(GetState() == Goal::ACTIVE);
	GetMind()->GetCharacter()->GetShip()->SetTakeOff(true);
	SetState(Goal::COMPLETED);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// GoalVisitPlanet                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////
GoalVisitPlanet::GoalVisitPlanet(GoalMind * GoalMind, GoalVisitPlanet::PlanetPolicy PlanetPolicy) :
	Goal(GoalMind, "visit_planet"),
	m_PlanetPolicy(PlanetPolicy)
{
	SetObjectIdentifier("::goal(visit_planet)::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void GoalVisitPlanet::Activate(void)
{
	assert(GetSubGoals().empty() == true);
	AddContent(new GoalTakeOffFromPlanet(GetMind()));
	AddContent(new GoalLandOnPlanet(GetMind()));
	AddContent(new GoalStopAtTarget(GetMind()));
	if(m_PlanetPolicy == GoalVisitPlanet::VISIT_NEAREST_PLANET_IN_SYSTEM)
	{
		AddContent(new GoalSelectNearestPlanet(GetMind()));
	}
	else
	{
		assert(false);
	}
	SetState(Goal::ACTIVE);
}

bool GoalVisitPlanet::OnMessageReceived(Message * Message)
{
	if(Message->GetTypeIdentifier() == "landed")
	{
		AddContent(new GoalWait(GetMind(), GetRandomFloat(2.0f, 5.0f)));
		AddContent(new GoalRefuel(GetMind(), Message->GetSender()));
		
		return true;
	}
	
	return false;
}

void GoalVisitPlanet::Process(void)
{
	assert(GetState() == Goal::ACTIVE);
	assert(GetSubGoals().empty() == false);
	
	Goal * SubGoal(GetSubGoals().front());
	
	if(SubGoal->GetState() == Goal::INACTIVE)
	{
		SubGoal->Activate();
	}
	assert(SubGoal->GetState() == Goal::ACTIVE);
	SubGoal->Process();
	if(SubGoal->GetState() == Goal::COMPLETED)
	{
		// terminate and remove sub goal
		SubGoal->Terminate();
		RemoveContent(SubGoal);
		// other actions may depend on the SubGoal variable
		if(GetSubGoals().empty() == true)
		{
			SetState(Goal::COMPLETED);
		}
		// now destroy the SubGoal
		SubGoal->Destroy();
		delete SubGoal;
	}
	else if(SubGoal->GetState() == Goal::FAILED)
	{
		// terminate and remove sub goal
		SubGoal->Terminate();
		RemoveContent(SubGoal);
		// other actions may depend on the SubGoal variable
		SetState(Goal::FAILED);
		// now destroy the SubGoal
		SubGoal->Destroy();
		delete SubGoal;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// GoalWait                                                                                      //
///////////////////////////////////////////////////////////////////////////////////////////////////
GoalWait::GoalWait(GoalMind * GoalMind, float Seconds) :
	Goal(GoalMind, "wait"),
	m_SecondsToWait(Seconds)
{
	SetObjectIdentifier("::goal(wait)::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")::seconds_to_wait(" + to_string_cast(m_SecondsToWait, 6) + ")");
}

void GoalWait::Activate(void)
{
	assert(GetSubGoals().empty() == true);
	m_TimeToLeave = GameTime::Get() + m_SecondsToWait;
	SetState(Goal::ACTIVE);
}

void GoalWait::Process(void)
{
	if(GameTime::Get() >= m_TimeToLeave)
	{
		SetState(Goal::COMPLETED);
	}
}
