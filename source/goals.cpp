/**
 * galactic-fall
 * Copyright (C) 2008-2018  Aram Altschudjian, Hagen Möbius
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

#include <string_cast/string_cast.h>

#include "character.h"
#include "commodity.h"
#include "faction.h"
#include "game_time.h"
#include "globals.h"
#include "goal_mind.h"
#include "goals.h"
#include "math.h"
#include "message.h"
#include "messages.h"
#include "object_aspect_position.h"
#include "planet.h"
#include "planet_assets.h"
#include "ship.h"
#include "system.h"
#include "threat.h"

int WantToJump(Ship * Ship, System * System);

///////////////////////////////////////////////////////////////////////////////////////////////////
// GoalApproachTarget                                                                            //
///////////////////////////////////////////////////////////////////////////////////////////////////
GoalApproachTarget::GoalApproachTarget(GoalMind * GoalMind) :
	Goal(GoalMind, "approach_target"),
	_FlyInDirection(nullptr)
{
}

void GoalApproachTarget::Activate(void)
{
	assert(GetSubGoals().empty() == true);
	_FlyInDirection = new GoalFlyInDirection(GetMind());
	AddSubGoal(_FlyInDirection);
	_FlyInDirection->Activate();
	SetState(Goal::ACTIVE);
}

void GoalApproachTarget::Process(void)
{
	assert(GetState() == Goal::ACTIVE);
	
	Vector3f ToDestination(GetMind()->GetCharacter()->GetShip()->GetTarget()->GetAspectPosition()->GetPosition() - GetMind()->GetCharacter()->GetShip()->GetAspectPosition()->GetPosition());
	float DistanceSquared(ToDestination.SquaredLength());
	float DistanceNeededToBrake(GetMind()->GetCharacter()->GetShip()->GetMaximumSpeed() * ((M_PI / GetMind()->GetCharacter()->GetShip()->GetMaximumTurnSpeed()) + ((GetMind()->GetCharacter()->GetShip()->GetMaximumSpeed() / GetMind()->GetCharacter()->GetShip()->GetMaximumForwardThrust()) / 2.0f)));
	
	// braking to do a full stop takes time (turning + accelerating)
	//  - turning: the ship will fly with maximum velocity towards the target
	//  - accelerating: the ship will fly half distance that it could with maximum velocity towards the target
	if(DistanceSquared > DistanceNeededToBrake * DistanceNeededToBrake)
	{
		_FlyInDirection->SetDirection(ToDestination / sqrt(DistanceSquared));
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
	_FlyInDirection->Terminate();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// GoalDestroyTarget                                                                             //
///////////////////////////////////////////////////////////////////////////////////////////////////
GoalDestroyTarget::GoalDestroyTarget(GoalMind * GoalMind) :
	Goal(GoalMind, "destroy_target"),
	_FlyInDirection(nullptr)
{
}

void GoalDestroyTarget::Activate(void)
{
	assert(GetSubGoals().empty() == true);
	_FlyInDirection = new GoalFlyInDirection(GetMind());
	AddSubGoal(_FlyInDirection);
	_FlyInDirection->Activate();
	SetState(Goal::ACTIVE);
}

void GoalDestroyTarget::Process(void)
{
	assert(GetState() == Goal::ACTIVE);
	if(GetMind()->GetCharacter()->GetShip()->GetTarget() != nullptr)
	{
		Vector3f ToDestination(GetMind()->GetCharacter()->GetShip()->GetTarget()->GetAspectPosition()->GetPosition() - GetMind()->GetCharacter()->GetShip()->GetAspectPosition()->GetPosition());
		float Length(ToDestination.Length());
		// TODO: shot speed
		float SecondsForShot(Length / 30.0f);
		
		ToDestination -= GetMind()->GetCharacter()->GetShip()->GetVelocity() * SecondsForShot;
		Length = ToDestination.Length();
		_FlyInDirection->SetDirection(ToDestination / Length);
		assert(GetSubGoals().size() == 1);
		assert(GetSubGoals().front()->GetState() == Goal::ACTIVE);
		GetSubGoals().front()->Process();
		// TODO: shot reach
		GetMind()->GetCharacter()->GetShip()->SetFire((_FlyInDirection->GetFacesDirection() == true) && (Length < 50.0f));
	}
	else
	{
		SetState(Goal::COMPLETED);
	}
}

void GoalDestroyTarget::Terminate(void)
{
	GetMind()->GetCharacter()->GetShip()->SetFire(false);
	_FlyInDirection->Terminate();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// GoalFighterThink                                                                              //
///////////////////////////////////////////////////////////////////////////////////////////////////
GoalFighterThink::GoalFighterThink(GoalMind * GoalMind) :
	Goal(GoalMind, "fighter_think")
{
}

void GoalFighterThink::Activate(void)
{
	assert(GetSubGoals().empty() == true);
	AddSubGoal(new GoalFightSomeTarget(GetMind()));
	SetState(Goal::ACTIVE);
}

bool GoalFighterThink::OnMessageReceived(Message * Message)
{
	if(Message->GetTypeIdentifier() == "threat")
	{
		auto TheThreatMessage{dynamic_cast< ThreatMessage * >(Message)};
		
		if((TheThreatMessage->GetSender() != nullptr) && (TheThreatMessage->GetSender()->GetTypeIdentifier() == "ship") && (dynamic_cast< Ship * >(TheThreatMessage->GetSender())->GetFaction()->GetSubTypeIdentifier() != GetMind()->GetCharacter()->GetShip()->GetFaction()->GetSubTypeIdentifier()))
		{
			GetMind()->GetCharacter()->GetThreat()->ModifyThreat(TheThreatMessage->GetSender(), TheThreatMessage->GetDeltaThreat());
		}
		
		return true;
	}
	else if(Message->GetTypeIdentifier() == "fuel_low")
	{
		if(HasSubGoal("visit_planet") == false)
		{
			AddSubGoal(new GoalVisitPlanet(GetMind(), GoalVisitPlanet::VISIT_NEAREST_PLANET));
		}
		
		return true;
	}
	else if(Message->GetTypeIdentifier() == "hull_low")
	{
		if(HasSubGoal("visit_planet") == false)
		{
			AddSubGoal(new GoalVisitPlanet(GetMind(), GoalVisitPlanet::VISIT_NEAREST_PLANET_IN_SYSTEM));
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
		RemoveSubGoal(SubGoal);
		// other actions may depend on the SubGoal variable
		if((SubGoal->GetName() == "fight_some_target") && (GetMind()->GetCharacter()->GetShip()->GetHull() < 0.5 * GetMind()->GetCharacter()->GetShip()->GetHullCapacity()))
		{
			// after a hard fight: queue a repair
			if(HasSubGoal("visit_planet") == false)
			{
				AddSubGoal(new GoalVisitPlanet(GetMind(), GoalVisitPlanet::VISIT_NEAREST_PLANET));
			}
		}
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
		RemoveSubGoal(SubGoal);
		// other actions may depend on the SubGoal variable
		// FightSomeTarget may fail if no fighter is present in the system: fly around
		if(SubGoal->GetName() == "fight_some_target")
		{
			AddSubGoal(new GoalFlyOverRandomPointWhileNoThreat(GetMind()));
		}
		if(GetSubGoals().empty() == true)
		{
			SetState(Goal::INACTIVE);
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
}

void GoalFightSomeTarget::Activate(void)
{
	assert(GetSubGoals().empty() == true);
	AddSubGoal(new GoalDestroyTarget(GetMind()));
	AddSubGoal(new GoalSelectFightableTarget(GetMind()));
	SetState(Goal::ACTIVE);
}

void GoalFightSomeTarget::Process(void)
{
	assert(GetState() == Goal::ACTIVE);
	assert(GetSubGoals().empty() == false);
	
	auto ObjectWithHighestThreat{GetMind()->GetCharacter()->GetThreat()->GetObjectWithHighestThreat()};
	
	if(ObjectWithHighestThreat != nullptr)
	{
		GetMind()->GetCharacter()->GetShip()->SetTarget(ObjectWithHighestThreat);
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
		RemoveSubGoal(SubGoal);
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
		RemoveSubGoal(SubGoal);
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
	_Direction(Vector3f::CreateZero()),
	_FacesDirection(false)
{
}

bool GoalFlyInDirection::GetFacesDirection(void) const
{
	return _FacesDirection;
}

void GoalFlyInDirection::SetDirection(const Vector3f & Direction)
{
	_Direction = Direction;
}

void GoalFlyInDirection::Activate(void)
{
	assert(GetSubGoals().empty() == true);
	SetState(Goal::ACTIVE);
}

void GoalFlyInDirection::Process(void)
{
	assert(GetState() == Goal::ACTIVE);
	
	Vector3f LocalizedDirection(_Direction);
	
	LocalizedDirection.Rotate(GetMind()->GetCharacter()->GetShip()->GetAspectPosition()->GetOrientation().Conjugated());
	if((LocalizedDirection[1] > 0.1f) || ((LocalizedDirection[0] < 0.0f) && (LocalizedDirection[1] >= 0.0f)))
	{
		GetMind()->GetCharacter()->GetShip()->SetTurnLeft(1.0f);
		GetMind()->GetCharacter()->GetShip()->SetTurnRight(0.0f);
		GetMind()->GetCharacter()->GetShip()->SetAccelerate(false);
		_FacesDirection = false;
	}
	else if((LocalizedDirection[1] < -0.1f) || ((LocalizedDirection[0] < 0.0f) && (LocalizedDirection[1] < 0.0f)))
	{
		GetMind()->GetCharacter()->GetShip()->SetTurnLeft(0.0f);
		GetMind()->GetCharacter()->GetShip()->SetTurnRight(1.0f);
		GetMind()->GetCharacter()->GetShip()->SetAccelerate(false);
		_FacesDirection = false;
	}
	else
	{
		GetMind()->GetCharacter()->GetShip()->SetTurnLeft(0.0f);
		GetMind()->GetCharacter()->GetShip()->SetTurnRight(0.0f);
		GetMind()->GetCharacter()->GetShip()->SetAccelerate(GetMind()->GetCharacter()->GetShip()->GetMaximumSpeed() - _Direction.Dot(GetMind()->GetCharacter()->GetShip()->GetVelocity()) > 0.1f);
		_FacesDirection = true;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// GoalFlyInSystemDirection                                                                      //
///////////////////////////////////////////////////////////////////////////////////////////////////
GoalFlyInSystemDirection::GoalFlyInSystemDirection(GoalMind * GoalMind) :
	Goal(GoalMind, "fly_in_system_direction"),
	_FlyInDirection(nullptr)
{
}

void GoalFlyInSystemDirection::Activate(void)
{
	assert(GetSubGoals().empty() == true);
	_FlyInDirection = new GoalFlyInDirection(GetMind());
	AddSubGoal(_FlyInDirection);
	_FlyInDirection->Activate();
	
	const System * CurrentSystem(dynamic_cast< System * >(GetMind()->GetCharacter()->GetShip()->GetContainer()));
	
	assert(CurrentSystem != nullptr);
	_FlyInDirection->SetDirection((GetMind()->GetCharacter()->GetShip()->GetLinkedSystemTarget()->GetAspectPosition()->GetPosition() - CurrentSystem->GetAspectPosition()->GetPosition()).Normalize());
	SetState(Goal::ACTIVE);
}

void GoalFlyInSystemDirection::Process(void)
{
	assert(GetState() == Goal::ACTIVE);
	assert(GetSubGoals().empty() == false);
	assert(GetSubGoals().front()->GetState() == Goal::ACTIVE);
	GetSubGoals().front()->Process();
}

void GoalFlyInSystemDirection::Terminate(void)
{
	_FlyInDirection->Terminate();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// GoalFlyOverRandomPointWhileNoThreat                                                           //
///////////////////////////////////////////////////////////////////////////////////////////////////
GoalFlyOverRandomPointWhileNoThreat::GoalFlyOverRandomPointWhileNoThreat(GoalMind * GoalMind) :
	Goal(GoalMind, "fly_over_random_point_while_no_threat"),
	_FlyInDirection(nullptr)
{
}

void GoalFlyOverRandomPointWhileNoThreat::Activate(void)
{
	assert(GetSubGoals().empty() == true);
	_FlyInDirection = new GoalFlyInDirection(GetMind());
	AddSubGoal(_FlyInDirection);
	_FlyInDirection->Activate();
	_Point.Set(GetRandomFloat(-200.0f, 200.0f), GetRandomFloat(-200.0f, 200.0f), 0.0f);
	SetState(Goal::ACTIVE);
}

void GoalFlyOverRandomPointWhileNoThreat::Process(void)
{
	assert(GetState() == Goal::ACTIVE);
	if(GetMind()->GetCharacter()->GetThreat()->GetObjectWithHighestThreat() != 0)
	{
		SetState(Goal::FAILED);
	}
	else
	{
		Vector3f ToDestination(_Point - GetMind()->GetCharacter()->GetShip()->GetAspectPosition()->GetPosition());
		float LengthSquared(ToDestination.SquaredLength());
		
		if(LengthSquared > 400.0f)
		{
			_FlyInDirection->SetDirection(ToDestination / sqrt(LengthSquared));
			assert(GetSubGoals().front()->GetState() == Goal::ACTIVE);
			GetSubGoals().front()->Process();
		}
		else
		{
			SetState(Goal::COMPLETED);
		}
	}
}

void GoalFlyOverRandomPointWhileNoThreat::Terminate(void)
{
	_FlyInDirection->Terminate();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// GoalFullStop                                                                                  //
///////////////////////////////////////////////////////////////////////////////////////////////////
GoalFullStop::GoalFullStop(GoalMind * GoalMind) :
	Goal(GoalMind, "full_stop"),
	_FlyInDirection(nullptr)
{
}

void GoalFullStop::Activate(void)
{
	assert(GetSubGoals().empty() == true);
	_FlyInDirection = new GoalFlyInDirection(GetMind());
	AddSubGoal(_FlyInDirection);
	_FlyInDirection->Activate();
	SetState(Goal::ACTIVE);
}

void GoalFullStop::Process(void)
{
	assert(GetState() == Goal::ACTIVE);
	
	float SpeedSquared(GetMind()->GetCharacter()->GetShip()->GetVelocity().SquaredLength());
	
	if(SpeedSquared > 2.0f)
	{
		_FlyInDirection->SetDirection(-GetMind()->GetCharacter()->GetShip()->GetVelocity().Normalized());
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
	_FlyInDirection->Terminate();
	GetMind()->GetCharacter()->GetShip()->SetAccelerate(false);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// GoalJump                                                                                      //
///////////////////////////////////////////////////////////////////////////////////////////////////
GoalJump::GoalJump(GoalMind * GoalMind) :
	Goal(GoalMind, "jump")
{
}

void GoalJump::Activate(void)
{
	assert(GetSubGoals().empty() == true);
	SetState(Goal::ACTIVE);
}

void GoalJump::Process(void)
{
	assert(GetState() == Goal::ACTIVE);
	GetMind()->GetCharacter()->GetShip()->SetJump(true);
	SetState(Goal::COMPLETED);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// GoalJumpToSystem                                                                              //
///////////////////////////////////////////////////////////////////////////////////////////////////
GoalJumpToSystem::GoalJumpToSystem(GoalMind * GoalMind, GoalJumpToSystem::SystemPolicy SystemPolicy) :
	Goal(GoalMind, "jump_to_system"),
	_SystemPolicy(SystemPolicy)
{
}

void GoalJumpToSystem::Activate(void)
{
	assert(GetSubGoals().empty() == true);
	AddSubGoal(new GoalJump(GetMind()));
	AddSubGoal(new GoalFlyInSystemDirection(GetMind()));
	if(_SystemPolicy == GoalJumpToSystem::JUMP_TO_RANDOM_SYSTEM)
	{
		AddSubGoal(new GoalSelectRandomSystem(GetMind()));
	}
	SetState(Goal::ACTIVE);
}

void GoalJumpToSystem::Process(void)
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
		RemoveSubGoal(SubGoal);
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
		RemoveSubGoal(SubGoal);
		// other actions may depend on the SubGoal variable
		SetState(Goal::FAILED);
		// now destroy the SubGoal
		SubGoal->Destroy();
		delete SubGoal;
	}
	else if(SubGoal->GetState() == Goal::ACTIVE)
	{
		if(SubGoal->GetName() == "fly_in_system_direction")
		{
			switch(WantToJump(GetMind()->GetCharacter()->GetShip(), GetMind()->GetCharacter()->GetShip()->GetLinkedSystemTarget()))
			{
			case NOT_ENOUGH_FUEL:
				{
					SetState(Goal::FAILED);
					
					break;
				}
			case OK:
				{
					// terminate and remove sub goal
					SubGoal->Terminate();
					RemoveSubGoal(SubGoal);
					// now destroy the SubGoal
					SubGoal->Destroy();
					delete SubGoal;
					
					break;
				}
			case TOO_NEAR_TO_STELLAR_OBJECT:
				{
					// this is the common case which requires us to continue with the goal processing
					
					break;
				}
			default:
				{
					assert("TODO: Unhandled WantToJump() retur code." == 0);
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// GoalLand                                                                                      //
///////////////////////////////////////////////////////////////////////////////////////////////////
GoalLand::GoalLand(GoalMind * GoalMind) :
	Goal(GoalMind, "land")
{
}

void GoalLand::Activate(void)
{
	assert(GetSubGoals().empty() == true);
	SetState(Goal::ACTIVE);
}

void GoalLand::Process(void)
{
	assert(GetState() == Goal::ACTIVE);
	GetMind()->GetCharacter()->GetShip()->SetLand(true);
	SetState(Goal::COMPLETED);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// GoalRefuel                                                                                    //
///////////////////////////////////////////////////////////////////////////////////////////////////
GoalRefuel::GoalRefuel(GoalMind * GoalMind, Planet * Planet) :
	Goal(GoalMind, "refuel"),
	_Planet(Planet)
{
	assert(_Planet != nullptr);
	_PlanetDestroyingConnection = _Planet->ConnectDestroyingCallback(std::bind(&GoalRefuel::_OnPlanetDestroying, this));
}

GoalRefuel::~GoalRefuel(void)
{
	if(_Planet != nullptr)
	{
		assert(_PlanetDestroyingConnection.IsValid() == true);
		_PlanetDestroyingConnection.Disconnect();
		_Planet = nullptr;
	}
	assert(_PlanetDestroyingConnection.IsValid() == false);
}

void GoalRefuel::Activate(void)
{
	assert(GetSubGoals().empty() == true);
	SetState(Goal::ACTIVE);
}

void GoalRefuel::Process(void)
{
	assert(GetState() == Goal::ACTIVE);
	
	bool SuccessfullyRefueled(false);
	
	for(auto PlanetAssets : _Planet->GetPlanetAssets())
	{
		if((PlanetAssets->GetTypeIdentifier() == "commodity") && (PlanetAssets->GetSubTypeIdentifier() == "fuel"))
		{
			auto FuelPrice{PlanetAssets->GetPrice()};
			float CanBuy(GetMind()->GetCharacter()->GetCredits() / FuelPrice);
			float Need(GetMind()->GetCharacter()->GetShip()->GetFuelCapacity() - GetMind()->GetCharacter()->GetShip()->GetFuel());
			float Buy((CanBuy > Need) ? (Need) : (CanBuy));
			
			GetMind()->GetCharacter()->GetShip()->SetFuel(GetMind()->GetCharacter()->GetShip()->GetFuel() + Buy);
			GetMind()->GetCharacter()->RemoveCredits(static_cast< std::uint32_t >(Buy * FuelPrice));
			SuccessfullyRefueled = true;
			
			break;
		}
	}
	SetState((SuccessfullyRefueled == true) ? (Goal::COMPLETED) : (Goal::FAILED));
}

void GoalRefuel::_OnPlanetDestroying(void)
{
	assert(_Planet != nullptr);
	assert(_PlanetDestroyingConnection.IsValid() == true);
	_PlanetDestroyingConnection.Disconnect();
	assert(_PlanetDestroyingConnection.IsValid() == false);
	_Planet = nullptr;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// GoalSelectFightableTarget                                                                     //
///////////////////////////////////////////////////////////////////////////////////////////////////
GoalSelectFightableTarget::GoalSelectFightableTarget(GoalMind * GoalMind) :
	Goal(GoalMind, "select_fightable_target")
{
}

void GoalSelectFightableTarget::Activate(void)
{
	assert(GetSubGoals().empty() == true);
	AddSubGoal(new GoalSelectEnemy(GetMind()));
	AddSubGoal(new GoalSelectStrandedShip(GetMind()));
	AddSubGoal(new GoalSelectMeasuredCargo(GetMind()));
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
		RemoveSubGoal(SubGoal);
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
		RemoveSubGoal(SubGoal);
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
// GoalSelectEnemy                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////
GoalSelectEnemy::GoalSelectEnemy(GoalMind * GoalMind) :
	Goal(GoalMind, "select_enemy")
{
}

void GoalSelectEnemy::Activate(void)
{
	assert(GetSubGoals().empty() == true);
	SetState(Goal::ACTIVE);
}

void GoalSelectEnemy::Process(void)
{
	assert(GetState() == Goal::ACTIVE);
	
	auto CurrentSystem(dynamic_cast< System * >(GetMind()->GetCharacter()->GetSystem()));
	
	assert(CurrentSystem != nullptr);
	
	std::vector< Ship * > AttackPossibilities;
	
	for(auto Ship : CurrentSystem->GetShips())
	{
		assert(Ship != nullptr);
		assert(Ship->GetFaction() != nullptr);
		// attack everything with a below neutral standing
		if((Ship != GetMind()->GetCharacter()->GetShip()) && (GetMind()->GetCharacter()->GetShip()->GetFaction()->GetStanding(Ship->GetFaction()) < 0.5f))
		{
			AttackPossibilities.push_back(Ship);
		}
	}
	if(AttackPossibilities.size() > 0)
	{
		GetMind()->GetCharacter()->GetShip()->SetTarget(AttackPossibilities[GetRandomInteger(AttackPossibilities.size() - 1)]);
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
}

void GoalSelectMeasuredCargo::Activate(void)
{
	assert(GetSubGoals().empty() == true);
	SetState(Goal::ACTIVE);
}

void GoalSelectMeasuredCargo::Process(void)
{
	assert(GetState() == Goal::ACTIVE);
	
	const System * CurrentSystem(dynamic_cast< System * >(GetMind()->GetCharacter()->GetShip()->GetContainer()));
	
	assert(CurrentSystem != 0);
	
	const std::list< Commodity * > & Commodities(CurrentSystem->GetCommodities());
	float MinimumCost(FLT_MAX);
	Commodity * MinimumCostCommodity(0);
	
	for(std::list< Commodity * >::const_iterator CommodityIterator = Commodities.begin(); CommodityIterator != Commodities.end(); ++CommodityIterator)
	{
		// what this formula does is effectively calculating the distance of the cargo from a point that is twice a much away from the system center as the ship
		// Why is this sensible? Because fighters will prefer cargo that is on "their" side of the system and is farther away from the system center than they are.
		float Cost(((*CommodityIterator)->GetAspectPosition()->GetPosition() - GetMind()->GetCharacter()->GetShip()->GetAspectPosition()->GetPosition().Scaled(2.0f)).SquaredLength());
		
		if(Cost < MinimumCost)
		{
			MinimumCostCommodity = *CommodityIterator;
			MinimumCost = Cost;
		}
	}
	if(MinimumCostCommodity != nullptr)
	{
		GetMind()->GetCharacter()->GetShip()->SetTarget(MinimumCostCommodity);
		SetState(Goal::COMPLETED);
	}
	else
	{
		SetState(Goal::FAILED);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// GoalSelectNearestPlanetInSystem                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////
GoalSelectNearestPlanetInSystem::GoalSelectNearestPlanetInSystem(GoalMind * GoalMind) :
	Goal(GoalMind, "select_nearest_planet_in_system")
{
}

void GoalSelectNearestPlanetInSystem::Activate(void)
{
	assert(GetSubGoals().empty() == true);
	SetState(Goal::ACTIVE);
}

void GoalSelectNearestPlanetInSystem::Process(void)
{
	assert(GetState() == Goal::ACTIVE);
	
	auto CurrentSystem(dynamic_cast< System * >(GetMind()->GetCharacter()->GetSystem()));
	
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
		SetState(Goal::COMPLETED);
	}
	else
	{
		SetState(Goal::FAILED);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// GoalSelectRandomSystem                                                                        //
///////////////////////////////////////////////////////////////////////////////////////////////////
GoalSelectRandomSystem::GoalSelectRandomSystem(GoalMind * GoalMind) :
	Goal(GoalMind, "select_random_system")
{
}

void GoalSelectRandomSystem::Activate(void)
{
	assert(GetSubGoals().empty() == true);
	SetState(Goal::ACTIVE);
}

void GoalSelectRandomSystem::Process(void)
{
	assert(GetState() == Goal::ACTIVE);
	assert(GetSubGoals().empty() == true);
	
	const System * CurrentSystem(dynamic_cast< System * >(GetMind()->GetCharacter()->GetShip()->GetContainer()));
	
	assert(CurrentSystem != 0);
	
	const std::list< System * > & Systems(CurrentSystem->GetLinkedSystems());
	
	if(Systems.size() != 0)
	{
		std::list< System * >::const_iterator SystemIterator(Systems.begin());
		
		for(std::list< System * >::size_type Choice = GetRandomInteger(Systems.size() - 1); Choice > 0; --Choice)
		{
			++SystemIterator;
		}
		GetMind()->GetCharacter()->GetShip()->SetLinkedSystemTarget(*SystemIterator);
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
}

void GoalSelectStrandedShip::Activate(void)
{
	assert(GetSubGoals().empty() == true);
	SetState(Goal::ACTIVE);
}

void GoalSelectStrandedShip::Process(void)
{
	assert(GetState() == Goal::ACTIVE);
	
	const System * CurrentSystem(dynamic_cast< System * >(GetMind()->GetCharacter()->GetShip()->GetContainer()));
	
	assert(CurrentSystem != 0);
	
	const std::list< Ship * > & Ships(CurrentSystem->GetShips());
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
		GetMind()->GetCharacter()->GetShip()->SetTarget(AttackPossibilities[GetRandomInteger(AttackPossibilities.size() - 1)]);
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
}

void GoalStopAtTarget::Activate(void)
{
	assert(GetSubGoals().empty() == true);
	AddSubGoal(new GoalFullStop(GetMind()));
	AddSubGoal(new GoalApproachTarget(GetMind()));
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
		RemoveSubGoal(SubGoal);
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
		RemoveSubGoal(SubGoal);
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
	_PlanetPolicy(PlanetPolicy)
{
}

void GoalVisitPlanet::Activate(void)
{
	assert(GetSubGoals().empty() == true);
	AddSubGoal(new GoalTakeOffFromPlanet(GetMind()));
	AddSubGoal(new GoalLand(GetMind()));
	AddSubGoal(new GoalStopAtTarget(GetMind()));
	if((_PlanetPolicy == GoalVisitPlanet::VISIT_NEAREST_PLANET_IN_SYSTEM) || (_PlanetPolicy == GoalVisitPlanet::VISIT_NEAREST_PLANET))
	{
		AddSubGoal(new GoalSelectNearestPlanetInSystem(GetMind()));
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
		AddSubGoal(new GoalWait(GetMind(), GetRandomFloat(2.0f, 5.0f)));
		AddSubGoal(new GoalRefuel(GetMind(), dynamic_cast< Planet * >(Message->GetSender())));
		
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
		RemoveSubGoal(SubGoal);
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
		RemoveSubGoal(SubGoal);
		// other actions may depend on the SubGoal variable
		if((SubGoal->GetName() == "select_nearest_planet_in_system") && (_PlanetPolicy == GoalVisitPlanet::VISIT_NEAREST_PLANET))
		{
			AddSubGoal(new GoalSelectNearestPlanetInSystem(GetMind()));
			AddSubGoal(new GoalJumpToSystem(GetMind(), GoalJumpToSystem::JUMP_TO_RANDOM_SYSTEM));
		}
		else
		{
			SetState(Goal::FAILED);
		}
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
	_SecondsToWait(Seconds)
{
}

void GoalWait::Activate(void)
{
	assert(GetSubGoals().empty() == true);
	_TimeToLeave = GameTime::Get() + _SecondsToWait;
	SetState(Goal::ACTIVE);
}

void GoalWait::Process(void)
{
	if(GameTime::Get() >= _TimeToLeave)
	{
		SetState(Goal::COMPLETED);
	}
}
