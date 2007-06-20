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

#include "character.h"
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
		GetMind()->GetStateMachine()->SetState(new Fight(GetMind()));
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
	m_Point.set(GetRandomFloat(-200.0f, 200.0f), GetRandomFloat(-200.0f, 200.0f));
}

void FlyOverRandomPoint::Execute(void)
{
	math3d::vector2f ToDestination(m_Point - GetMind()->GetCharacter()->GetShip()->GetPosition());
	float LengthSquared(ToDestination.length_squared());
	
	if(LengthSquared > 400.0f)
	{
		ToDestination /= sqrt(LengthSquared);
		
		float HeadingOffDestination(GetShortestRadians(GetMind()->GetCharacter()->GetShip()->GetAngularPosition(), GetRadians(ToDestination)));
		
		if(HeadingOffDestination > 0.1)
		{
			GetMind()->GetCharacter()->GetShip()->m_TurnRight = true;
			GetMind()->GetCharacter()->GetShip()->m_TurnLeft = false;
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = false;
		}
		else if(HeadingOffDestination < -0.1)
		{
			GetMind()->GetCharacter()->GetShip()->m_TurnRight = false;
			GetMind()->GetCharacter()->GetShip()->m_TurnLeft = true;
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = false;
		}
		else
		{
			GetMind()->GetCharacter()->GetShip()->m_TurnRight = false;
			GetMind()->GetCharacter()->GetShip()->m_TurnLeft = false;
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = ((GetMind()->GetCharacter()->GetShip()->GetVelocity() - math3d::vector2f(GetMind()->GetCharacter()->GetShip()->GetShipClass()->GetMaximumSpeed(), GetMind()->GetCharacter()->GetShip()->GetAngularPosition(), math3d::vector2f::magnitude_angle)).length_squared() > 0.1f);
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
	const std::list< Planet * > & Planets(GetMind()->GetCharacter()->GetShip()->GetCurrentSystem()->GetPlanets());
	
	if(Planets.size() != 0)
	{
		std::list< Planet * >::const_iterator PlanetIterator(Planets.begin());
		
		for(std::list< Planet * >::size_type Choice = GetRandomInteger(Planets.size() - 1); Choice > 0; --Choice)
		{
			++PlanetIterator;
		}
		GetMind()->GetCharacter()->GetShip()->SetTarget((*PlanetIterator)->GetReference());
	}
	else
	{
		GetMind()->GetStateMachine()->SetState(new TransporterPhase4(GetMind()));
		delete this;
	}
}

void TransporterPhase1::Execute(void)
{
	math3d::vector2f ToDestination(GetMind()->GetCharacter()->GetShip()->GetTarget()->GetPosition() - GetMind()->GetCharacter()->GetShip()->GetPosition());
	float DistanceSquared(ToDestination.length_squared());
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
			GetMind()->GetCharacter()->GetShip()->m_TurnRight = true;
			GetMind()->GetCharacter()->GetShip()->m_TurnLeft = false;
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = false;
		}
		else if(HeadingOffDestination < -0.1)
		{
			GetMind()->GetCharacter()->GetShip()->m_TurnRight = false;
			GetMind()->GetCharacter()->GetShip()->m_TurnLeft = true;
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = false;
		}
		else
		{
			GetMind()->GetCharacter()->GetShip()->m_TurnRight = false;
			GetMind()->GetCharacter()->GetShip()->m_TurnLeft = false;
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = ((GetMind()->GetCharacter()->GetShip()->GetVelocity() - math3d::vector2f(GetMind()->GetCharacter()->GetShip()->GetShipClass()->GetMaximumSpeed(), GetMind()->GetCharacter()->GetShip()->GetAngularPosition(), math3d::vector2f::magnitude_angle)).length_squared() > 0.1f);
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
	float SpeedSquared(GetMind()->GetCharacter()->GetShip()->GetVelocity().length_squared());
	
	if(SpeedSquared > 2.0f)
	{
		float HeadingOffReverse(GetShortestRadians(GetMind()->GetCharacter()->GetShip()->GetAngularPosition(), GetRadians(-(GetMind()->GetCharacter()->GetShip()->GetVelocity().normalized()))));
		
		if(HeadingOffReverse > 0.1)
		{
			GetMind()->GetCharacter()->GetShip()->m_TurnRight = true;
			GetMind()->GetCharacter()->GetShip()->m_TurnLeft = false;
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = false;
		}
		else if(HeadingOffReverse < -0.1)
		{
			GetMind()->GetCharacter()->GetShip()->m_TurnRight = false;
			GetMind()->GetCharacter()->GetShip()->m_TurnLeft = true;
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = false;
		}
		else
		{
			GetMind()->GetCharacter()->GetShip()->m_TurnRight = false;
			GetMind()->GetCharacter()->GetShip()->m_TurnLeft = false;
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = true;
		}
	}
	else
	{
		GetMind()->GetCharacter()->GetShip()->m_Land = true;
		GetMind()->GetStateMachine()->SetState(new TransporterPhase3(GetMind()));
		delete this;
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
		m_JumpPoint = GetMind()->GetCharacter()->GetShip()->GetPosition() + (((*SystemIterator)->GetPosition() - GetMind()->GetCharacter()->GetShip()->GetCurrentSystem()->GetPosition()).normalize() * 280.0f * GetRandomFloat(1.0f, 1.2f));
	}
	else
	{
		GetMind()->GetStateMachine()->SetState(new FlyOverRandomPoint(GetMind()));
		delete this;
	}
}

void TransporterPhase4::Execute(void)
{
	math3d::vector2f ToDestination(m_JumpPoint - GetMind()->GetCharacter()->GetShip()->GetPosition());
	float LengthSquared(ToDestination.length_squared());
	
	if(LengthSquared > 400.0f)
	{
		ToDestination /= sqrt(LengthSquared);
		
		float HeadingOffDestination(GetShortestRadians(GetMind()->GetCharacter()->GetShip()->GetAngularPosition(), GetRadians(ToDestination)));
		
		if(HeadingOffDestination > 0.1)
		{
			GetMind()->GetCharacter()->GetShip()->m_TurnRight = true;
			GetMind()->GetCharacter()->GetShip()->m_TurnLeft = false;
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = false;
		}
		else if(HeadingOffDestination < -0.1)
		{
			GetMind()->GetCharacter()->GetShip()->m_TurnRight = false;
			GetMind()->GetCharacter()->GetShip()->m_TurnLeft = true;
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = false;
		}
		else
		{
			GetMind()->GetCharacter()->GetShip()->m_TurnRight = false;
			GetMind()->GetCharacter()->GetShip()->m_TurnLeft = false;
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = ((GetMind()->GetCharacter()->GetShip()->GetVelocity() - math3d::vector2f(GetMind()->GetCharacter()->GetShip()->GetShipClass()->GetMaximumSpeed(), GetMind()->GetCharacter()->GetShip()->GetAngularPosition(), math3d::vector2f::magnitude_angle)).length_squared() > 0.1f);
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

Fight::Fight(StateMachineMind * Mind) :
	State(Mind)
{
	SetObjectIdentifier("::fight::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void Fight::Enter(void)
{
	const std::list< Ship * > & Ships(GetMind()->GetCharacter()->GetShip()->GetCurrentSystem()->GetShips());
	std::vector< Ship * > Fighters;
	
	for(std::list< Ship * >::const_iterator ShipIterator = Ships.begin(); ShipIterator != Ships.end(); ++ShipIterator)
	{
		if(((*ShipIterator)->GetShipClass()->GetIdentifier() == "fighter") && (*ShipIterator != GetMind()->GetCharacter()->GetShip()))
		{
			Fighters.push_back(*ShipIterator);
		}
	}
	if(Fighters.size() > 0)
	{
		GetMind()->GetCharacter()->GetShip()->SetTarget(Fighters[GetRandomInteger(Fighters.size() - 1)]->GetReference());
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
		math3d::vector2f ToDestination(GetMind()->GetCharacter()->GetShip()->GetTarget()->GetPosition() - GetMind()->GetCharacter()->GetShip()->GetPosition());
		float Length(ToDestination.length());
		
		ToDestination /= Length;
		
		float HeadingOffDestination(GetShortestRadians(GetMind()->GetCharacter()->GetShip()->GetAngularPosition(), GetRadians(ToDestination)));
		
		GetMind()->GetCharacter()->GetShip()->m_Fire = false;
		if(HeadingOffDestination > 0.1)
		{
			GetMind()->GetCharacter()->GetShip()->m_TurnRight = true;
			GetMind()->GetCharacter()->GetShip()->m_TurnLeft = false;
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = false;
		}
		else if(HeadingOffDestination < -0.1)
		{
			GetMind()->GetCharacter()->GetShip()->m_TurnRight = false;
			GetMind()->GetCharacter()->GetShip()->m_TurnLeft = true;
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = false;
		}
		else
		{
			GetMind()->GetCharacter()->GetShip()->m_TurnRight = false;
			GetMind()->GetCharacter()->GetShip()->m_TurnLeft = false;
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = ((GetMind()->GetCharacter()->GetShip()->GetVelocity() - math3d::vector2f(GetMind()->GetCharacter()->GetShip()->GetShipClass()->GetMaximumSpeed(), GetMind()->GetCharacter()->GetShip()->GetAngularPosition(), math3d::vector2f::magnitude_angle)).length_squared() > 0.1f);
			if(Length < 150.0f)
			{
				GetMind()->GetCharacter()->GetShip()->m_Fire = true;
			}
		}
	}
	else
	{
		GetMind()->GetStateMachine()->SetState(new Fight(GetMind()));
		delete this;
	}
}

void Fight::Exit(void)
{
	GetMind()->GetCharacter()->GetShip()->m_Fire = false;
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
	const std::list< Planet * > & Planets(GetMind()->GetCharacter()->GetShip()->GetCurrentSystem()->GetPlanets());
	float MinimumDistance(FLT_MAX);
	Planet * NearestPlanet(0);
	
	for(std::list< Planet * >::const_iterator PlanetIterator = Planets.begin(); PlanetIterator != Planets.end(); ++PlanetIterator)
	{
		float Distance(((*PlanetIterator)->GetPosition() - GetMind()->GetCharacter()->GetShip()->GetPosition()).length_squared());
		
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
	math3d::vector2f ToDestination(GetMind()->GetCharacter()->GetShip()->GetTarget()->GetPosition() - GetMind()->GetCharacter()->GetShip()->GetPosition());
	float DistanceSquared(ToDestination.length_squared());
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
			GetMind()->GetCharacter()->GetShip()->m_TurnRight = true;
			GetMind()->GetCharacter()->GetShip()->m_TurnLeft = false;
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = false;
		}
		else if(HeadingOffDestination < -0.1)
		{
			GetMind()->GetCharacter()->GetShip()->m_TurnRight = false;
			GetMind()->GetCharacter()->GetShip()->m_TurnLeft = true;
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = false;
		}
		else
		{
			GetMind()->GetCharacter()->GetShip()->m_TurnRight = false;
			GetMind()->GetCharacter()->GetShip()->m_TurnLeft = false;
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = ((GetMind()->GetCharacter()->GetShip()->GetVelocity() - math3d::vector2f(GetMind()->GetCharacter()->GetShip()->GetShipClass()->GetMaximumSpeed(), GetMind()->GetCharacter()->GetShip()->GetAngularPosition(), math3d::vector2f::magnitude_angle)).length_squared() > 0.1f);
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
	float SpeedSquared(GetMind()->GetCharacter()->GetShip()->GetVelocity().length_squared());
	
	if(SpeedSquared > 2.0f)
	{
		float HeadingOffReverse(GetShortestRadians(GetMind()->GetCharacter()->GetShip()->GetAngularPosition(), GetRadians(-(GetMind()->GetCharacter()->GetShip()->GetVelocity().normalized()))));
		
		if(HeadingOffReverse > 0.1)
		{
			GetMind()->GetCharacter()->GetShip()->m_TurnRight = true;
			GetMind()->GetCharacter()->GetShip()->m_TurnLeft = false;
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = false;
		}
		else if(HeadingOffReverse < -0.1)
		{
			GetMind()->GetCharacter()->GetShip()->m_TurnRight = false;
			GetMind()->GetCharacter()->GetShip()->m_TurnLeft = true;
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = false;
		}
		else
		{
			GetMind()->GetCharacter()->GetShip()->m_TurnRight = false;
			GetMind()->GetCharacter()->GetShip()->m_TurnLeft = false;
			GetMind()->GetCharacter()->GetShip()->m_Accelerate = true;
		}
	}
	else
	{
		GetMind()->GetCharacter()->GetShip()->m_Land = true;
		GetMind()->GetStateMachine()->SetState(new RefuelPhase3(GetMind()));
		delete this;
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
	GetMind()->GetCharacter()->GetShip()->SetFuel(GetMind()->GetCharacter()->GetShip()->GetFuelCapacity());
	
	MonitorFuel * GlobalState(dynamic_cast< MonitorFuel * >(GetMind()->GetStateMachine()->GetGlobalState()));
	
	assert(GlobalState != 0);
	GlobalState->SetRefueled();
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
