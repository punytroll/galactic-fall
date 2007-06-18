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

#include <iostream>
#include <list>

#include "game_time.h"
#include "math.h"
#include "planet.h"
#include "ship.h"
#include "state_machine.h"
#include "states.h"
#include "string_cast.h"
#include "system.h"

SelectSteering::SelectSteering(Ship * ActionTarget, StateMachine * StateMachine) :
	State(ActionTarget, StateMachine)
{
	SetObjectIdentifier("::select_steering::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void SelectSteering::Enter(void)
{
}

void SelectSteering::Execute(void)
{
	if(GetActionTarget()->GetShipClass()->GetIdentifier() == "transporter")
	{
		GetStateMachine()->SetState(new TransporterPhase1(GetActionTarget(), GetStateMachine()));
	}
	else
	{
		GetStateMachine()->SetState(new Fight(GetActionTarget(), GetStateMachine()));
	}
	delete this;
}

void SelectSteering::Exit(void)
{
}

FlyOverRandomPoint::FlyOverRandomPoint(Ship * ActionTarget, StateMachine * StateMachine) :
	State(ActionTarget, StateMachine)
{
	SetObjectIdentifier("::fly_over_random_point::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void FlyOverRandomPoint::Enter(void)
{
	m_Point.set(GetRandomFloat(-200.0f, 200.0f), GetRandomFloat(-200.0f, 200.0f));
}

void FlyOverRandomPoint::Execute(void)
{
	math3d::vector2f ToDestination(m_Point - GetActionTarget()->GetPosition());
	float LengthSquared(ToDestination.length_squared());
	
	if(LengthSquared > 400.0f)
	{
		ToDestination /= sqrt(LengthSquared);
		
		float HeadingOffDestination(GetShortestRadians(GetActionTarget()->GetAngularPosition(), GetRadians(ToDestination)));
		
		if(HeadingOffDestination > 0.1)
		{
			GetActionTarget()->m_TurnRight = true;
			GetActionTarget()->m_TurnLeft = false;
			GetActionTarget()->m_Accelerate = false;
		}
		else if(HeadingOffDestination < -0.1)
		{
			GetActionTarget()->m_TurnRight = false;
			GetActionTarget()->m_TurnLeft = true;
			GetActionTarget()->m_Accelerate = false;
		}
		else
		{
			GetActionTarget()->m_TurnRight = false;
			GetActionTarget()->m_TurnLeft = false;
			GetActionTarget()->m_Accelerate = ((GetActionTarget()->GetVelocity() - math3d::vector2f(GetActionTarget()->GetShipClass()->GetMaximumSpeed(), GetActionTarget()->GetAngularPosition(), math3d::vector2f::magnitude_angle)).length_squared() > 0.1f);
		}
	}
	else
	{
		GetStateMachine()->SetState(new SelectSteering(GetActionTarget(), GetStateMachine()));
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
TransporterPhase1::TransporterPhase1(Ship * ActionTarget, StateMachine * StateMachine) :
	State(ActionTarget, StateMachine)
{
	SetObjectIdentifier("::transporter_phase_1::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void TransporterPhase1::Enter(void)
{
	const std::list< Planet * > & Planets(GetActionTarget()->GetCurrentSystem()->GetPlanets());
	
	if(Planets.size() != 0)
	{
		std::list< Planet * >::const_iterator PlanetIterator(Planets.begin());
		
		for(std::list< Planet * >::size_type Choice = GetRandomInteger(Planets.size() - 1); Choice > 0; --Choice)
		{
			++PlanetIterator;
		}
		GetActionTarget()->SetTarget((*PlanetIterator)->GetReference());
	}
	else
	{
		GetStateMachine()->SetState(new TransporterPhase4(GetActionTarget(), GetStateMachine()));
		delete this;
	}
}

void TransporterPhase1::Execute(void)
{
	math3d::vector2f ToDestination(GetActionTarget()->GetTarget()->GetPosition() - GetActionTarget()->GetPosition());
	float DistanceSquared(ToDestination.length_squared());
	float DistanceNeededToBrake(GetActionTarget()->GetShipClass()->GetMaximumSpeed() * ((M_PI / GetActionTarget()->GetShipClass()->GetTurnSpeed()) + ((GetActionTarget()->GetShipClass()->GetMaximumSpeed() / GetActionTarget()->GetShipClass()->GetForwardThrust()) / 2.0f)));
	
	// braking in phase 2 takes time (turning + accelerating)
	//  - turning: the ship will fly with maximum velocity towards the target
	//  - accelerating: the ship will fly half distance that it could with maximum velocity towards the target
	if(DistanceSquared > DistanceNeededToBrake * DistanceNeededToBrake)
	{
		ToDestination /= sqrt(DistanceSquared);
		
		float HeadingOffDestination(GetShortestRadians(GetActionTarget()->GetAngularPosition(), GetRadians(ToDestination)));
		
		if(HeadingOffDestination > 0.1)
		{
			GetActionTarget()->m_TurnRight = true;
			GetActionTarget()->m_TurnLeft = false;
			GetActionTarget()->m_Accelerate = false;
		}
		else if(HeadingOffDestination < -0.1)
		{
			GetActionTarget()->m_TurnRight = false;
			GetActionTarget()->m_TurnLeft = true;
			GetActionTarget()->m_Accelerate = false;
		}
		else
		{
			GetActionTarget()->m_TurnRight = false;
			GetActionTarget()->m_TurnLeft = false;
			GetActionTarget()->m_Accelerate = ((GetActionTarget()->GetVelocity() - math3d::vector2f(GetActionTarget()->GetShipClass()->GetMaximumSpeed(), GetActionTarget()->GetAngularPosition(), math3d::vector2f::magnitude_angle)).length_squared() > 0.1f);
		}
	}
	else
	{
		GetStateMachine()->SetState(new TransporterPhase2(GetActionTarget(), GetStateMachine()));
		delete this;
	}
}

void TransporterPhase1::Exit(void)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// TransporterPhase2: Revereses the ship and accelerates until a near stop is accomplished.      //
//                    Will finish by setting the ship to immobile.                               //
///////////////////////////////////////////////////////////////////////////////////////////////////
TransporterPhase2::TransporterPhase2(Ship * ActionTarget, StateMachine * StateMachine) :
	State(ActionTarget, StateMachine)
{
	SetObjectIdentifier("::transporter_phase_2::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void TransporterPhase2::Enter(void)
{
}

void TransporterPhase2::Execute(void)
{
	float SpeedSquared(GetActionTarget()->GetVelocity().length_squared());
	
	if(SpeedSquared > 2.0f)
	{
		float HeadingOffReverse(GetShortestRadians(GetActionTarget()->GetAngularPosition(), GetRadians(-(GetActionTarget()->GetVelocity().normalized()))));
		
		if(HeadingOffReverse > 0.1)
		{
			GetActionTarget()->m_TurnRight = true;
			GetActionTarget()->m_TurnLeft = false;
			GetActionTarget()->m_Accelerate = false;
		}
		else if(HeadingOffReverse < -0.1)
		{
			GetActionTarget()->m_TurnRight = false;
			GetActionTarget()->m_TurnLeft = true;
			GetActionTarget()->m_Accelerate = false;
		}
		else
		{
			GetActionTarget()->m_TurnRight = false;
			GetActionTarget()->m_TurnLeft = false;
			GetActionTarget()->m_Accelerate = true;
		}
	}
	else
	{
		GetStateMachine()->SetState(new TransporterPhase3(GetActionTarget(), GetStateMachine()));
		delete this;
	}
}

void TransporterPhase2::Exit(void)
{
	GetActionTarget()->SetVelocity(math3d::vector2f(0.0f, 0.0f));
	GetActionTarget()->m_TurnRight = false;
	GetActionTarget()->m_TurnLeft = false;
	GetActionTarget()->m_Accelerate = false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// TransporterPhase3: Wait for a little time.                                                    //
//                    Decide, what to do next.                                                   //
///////////////////////////////////////////////////////////////////////////////////////////////////
TransporterPhase3::TransporterPhase3(Ship * ActionTarget, StateMachine * StateMachine) :
	State(ActionTarget, StateMachine),
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
			GetStateMachine()->SetState(new TransporterPhase1(GetActionTarget(), GetStateMachine()));
			delete this;
		}
		else
		{
			GetStateMachine()->SetState(new TransporterPhase4(GetActionTarget(), GetStateMachine()));
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
TransporterPhase4::TransporterPhase4(Ship * ActionTarget, StateMachine * StateMachine) :
	State(ActionTarget, StateMachine)
{
	SetObjectIdentifier("::transporter_phase_4::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void TransporterPhase4::Enter(void)
{
	const std::list< System * > & Systems(GetActionTarget()->GetCurrentSystem()->GetLinkedSystems());
	
	if(Systems.size() != 0)
	{
		std::list< System * >::const_iterator SystemIterator(Systems.begin());
		
		for(std::list< System * >::size_type Choice = GetRandomInteger(Systems.size() - 1); Choice > 0; --Choice)
		{
			++SystemIterator;
		}
		GetActionTarget()->SetLinkedSystemTarget(*SystemIterator);
		
		// TODO: the 280.0f is a constant from main.cpp
		m_JumpPoint = GetActionTarget()->GetPosition() + (((*SystemIterator)->GetPosition() - GetActionTarget()->GetCurrentSystem()->GetPosition()).normalize() * 280.0f * GetRandomFloat(1.0f, 1.2f));
	}
	else
	{
		GetStateMachine()->SetState(new FlyOverRandomPoint(GetActionTarget(), GetStateMachine()));
		delete this;
	}
}

void TransporterPhase4::Execute(void)
{
	math3d::vector2f ToDestination(m_JumpPoint - GetActionTarget()->GetPosition());
	float LengthSquared(ToDestination.length_squared());
	
	if(LengthSquared > 400.0f)
	{
		ToDestination /= sqrt(LengthSquared);
		
		float HeadingOffDestination(GetShortestRadians(GetActionTarget()->GetAngularPosition(), GetRadians(ToDestination)));
		
		if(HeadingOffDestination > 0.1)
		{
			GetActionTarget()->m_TurnRight = true;
			GetActionTarget()->m_TurnLeft = false;
			GetActionTarget()->m_Accelerate = false;
		}
		else if(HeadingOffDestination < -0.1)
		{
			GetActionTarget()->m_TurnRight = false;
			GetActionTarget()->m_TurnLeft = true;
			GetActionTarget()->m_Accelerate = false;
		}
		else
		{
			GetActionTarget()->m_TurnRight = false;
			GetActionTarget()->m_TurnLeft = false;
			GetActionTarget()->m_Accelerate = ((GetActionTarget()->GetVelocity() - math3d::vector2f(GetActionTarget()->GetShipClass()->GetMaximumSpeed(), GetActionTarget()->GetAngularPosition(), math3d::vector2f::magnitude_angle)).length_squared() > 0.1f);
		}
	}
	else
	{
		GetActionTarget()->m_Jump = true;
		GetStateMachine()->SetState(new SelectSteering(GetActionTarget(), GetStateMachine()));
		delete this;
	}
}

void TransporterPhase4::Exit(void)
{
}

Fight::Fight(Ship * ActionTarget, StateMachine * StateMachine) :
	State(ActionTarget, StateMachine)
{
	SetObjectIdentifier("::fight::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void Fight::Enter(void)
{
	const std::list< Ship * > & Ships(GetActionTarget()->GetCurrentSystem()->GetShips());
	std::vector< Ship * > Fighters;
	
	for(std::list< Ship * >::const_iterator ShipIterator = Ships.begin(); ShipIterator != Ships.end(); ++ShipIterator)
	{
		if(((*ShipIterator)->GetShipClass()->GetIdentifier() == "fighter") && (*ShipIterator != GetActionTarget()))
		{
			Fighters.push_back(*ShipIterator);
		}
	}
	if(Fighters.size() > 0)
	{
		GetActionTarget()->SetTarget(Fighters[GetRandomInteger(Fighters.size() - 1)]->GetReference());
	}
	else
	{
		GetStateMachine()->SetState(new FlyOverRandomPoint(GetActionTarget(), GetStateMachine()));
		delete this;
	}
}

void Fight::Execute(void)
{
	if(GetActionTarget()->GetTarget() == true)
	{
		math3d::vector2f ToDestination(GetActionTarget()->GetTarget()->GetPosition() - GetActionTarget()->GetPosition());
		float LengthSquared(ToDestination.length_squared());
		
		if(LengthSquared > 400.0f)
		{
			ToDestination /= sqrt(LengthSquared);
			
			float HeadingOffDestination(GetShortestRadians(GetActionTarget()->GetAngularPosition(), GetRadians(ToDestination)));
			
			GetActionTarget()->m_Fire = false;
			if(HeadingOffDestination > 0.1)
			{
				GetActionTarget()->m_TurnRight = true;
				GetActionTarget()->m_TurnLeft = false;
				GetActionTarget()->m_Accelerate = false;
			}
			else if(HeadingOffDestination < -0.1)
			{
				GetActionTarget()->m_TurnRight = false;
				GetActionTarget()->m_TurnLeft = true;
				GetActionTarget()->m_Accelerate = false;
			}
			else
			{
				GetActionTarget()->m_TurnRight = false;
				GetActionTarget()->m_TurnLeft = false;
				GetActionTarget()->m_Accelerate = ((GetActionTarget()->GetVelocity() - math3d::vector2f(GetActionTarget()->GetShipClass()->GetMaximumSpeed(), GetActionTarget()->GetAngularPosition(), math3d::vector2f::magnitude_angle)).length_squared() > 0.1f);
				if(LengthSquared < 16000.0f)
				{
					GetActionTarget()->m_Fire = true;
				}
			}
		}
	}
	else
	{
		GetStateMachine()->SetState(new Fight(GetActionTarget(), GetStateMachine()));
		delete this;
	}
}

void Fight::Exit(void)
{
	GetActionTarget()->m_Fire = false;
}

MonitorFuel::MonitorFuel(Ship * ActionTarget, StateMachine * StateMachine) :
	State(ActionTarget, StateMachine),
	m_Refueling(false)
{
	SetObjectIdentifier("::monitor_fuel::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void MonitorFuel::Enter(void)
{
}

void MonitorFuel::Execute(void)
{
	if((m_Refueling == false) && (GetActionTarget()->GetFuel() / GetActionTarget()->GetFuelCapacity() < 0.25f))
	{
		State * OldState(GetStateMachine()->GetState());
		
		GetStateMachine()->SetState(new RefuelPhase1(GetActionTarget(), GetStateMachine()));
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

RefuelPhase1::RefuelPhase1(Ship * ActionTarget, StateMachine * StateMachine) :
	State(ActionTarget, StateMachine)
{
	SetObjectIdentifier("::refuel_phase_1::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void RefuelPhase1::Enter(void)
{
	const std::list< Planet * > & Planets(GetActionTarget()->GetCurrentSystem()->GetPlanets());
	float MinimumDistance(0.0f);
	Planet * NearestPlanet(0);
	
	for(std::list< Planet * >::const_iterator PlanetIterator = Planets.begin(); PlanetIterator != Planets.end(); ++PlanetIterator)
	{
		if(NearestPlanet == 0)
		{
			NearestPlanet = *PlanetIterator;
			MinimumDistance = (NearestPlanet->GetPosition() - GetActionTarget()->GetPosition()).length_squared();
		}
		else
		{
			float Distance(((*PlanetIterator)->GetPosition() - GetActionTarget()->GetPosition()).length_squared());
			
			if(Distance < MinimumDistance)
			{
				NearestPlanet = *PlanetIterator;
				MinimumDistance = Distance;
			}
		}
	}
	if(NearestPlanet != 0)
	{
		GetActionTarget()->SetTarget(NearestPlanet->GetReference());
	}
	else
	{
		GetStateMachine()->SetState(new SelectSteering(GetActionTarget(), GetStateMachine()));
		delete this;
	}
}

void RefuelPhase1::Execute(void)
{
	math3d::vector2f ToDestination(GetActionTarget()->GetTarget()->GetPosition() - GetActionTarget()->GetPosition());
	float DistanceSquared(ToDestination.length_squared());
	float DistanceNeededToBrake(GetActionTarget()->GetShipClass()->GetMaximumSpeed() * ((M_PI / GetActionTarget()->GetShipClass()->GetTurnSpeed()) + ((GetActionTarget()->GetShipClass()->GetMaximumSpeed() / GetActionTarget()->GetShipClass()->GetForwardThrust()) / 2.0f)));
	
	// braking in phase 2 takes time (turning + accelerating)
	//  - turning: the ship will fly with maximum velocity towards the target
	//  - accelerating: the ship will fly half distance that it could with maximum velocity towards the target
	if(DistanceSquared > DistanceNeededToBrake * DistanceNeededToBrake)
	{
		ToDestination /= sqrt(DistanceSquared);
		
		float HeadingOffDestination(GetShortestRadians(GetActionTarget()->GetAngularPosition(), GetRadians(ToDestination)));
		
		if(HeadingOffDestination > 0.1)
		{
			GetActionTarget()->m_TurnRight = true;
			GetActionTarget()->m_TurnLeft = false;
			GetActionTarget()->m_Accelerate = false;
		}
		else if(HeadingOffDestination < -0.1)
		{
			GetActionTarget()->m_TurnRight = false;
			GetActionTarget()->m_TurnLeft = true;
			GetActionTarget()->m_Accelerate = false;
		}
		else
		{
			GetActionTarget()->m_TurnRight = false;
			GetActionTarget()->m_TurnLeft = false;
			GetActionTarget()->m_Accelerate = ((GetActionTarget()->GetVelocity() - math3d::vector2f(GetActionTarget()->GetShipClass()->GetMaximumSpeed(), GetActionTarget()->GetAngularPosition(), math3d::vector2f::magnitude_angle)).length_squared() > 0.1f);
		}
	}
	else
	{
		GetStateMachine()->SetState(new RefuelPhase2(GetActionTarget(), GetStateMachine()));
		delete this;
	}
}

void RefuelPhase1::Exit(void)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// RefuelPhase2: Revereses the ship and accelerates until a near stop is accomplished.           //
//               Will finish by setting the ship to immobile.                                    //
///////////////////////////////////////////////////////////////////////////////////////////////////
RefuelPhase2::RefuelPhase2(Ship * ActionTarget, StateMachine * StateMachine) :
	State(ActionTarget, StateMachine)
{
	SetObjectIdentifier("::refuel_phase_2::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void RefuelPhase2::Enter(void)
{
}

void RefuelPhase2::Execute(void)
{
	float SpeedSquared(GetActionTarget()->GetVelocity().length_squared());
	
	if(SpeedSquared > 2.0f)
	{
		float HeadingOffReverse(GetShortestRadians(GetActionTarget()->GetAngularPosition(), GetRadians(-(GetActionTarget()->GetVelocity().normalized()))));
		
		if(HeadingOffReverse > 0.1)
		{
			GetActionTarget()->m_TurnRight = true;
			GetActionTarget()->m_TurnLeft = false;
			GetActionTarget()->m_Accelerate = false;
		}
		else if(HeadingOffReverse < -0.1)
		{
			GetActionTarget()->m_TurnRight = false;
			GetActionTarget()->m_TurnLeft = true;
			GetActionTarget()->m_Accelerate = false;
		}
		else
		{
			GetActionTarget()->m_TurnRight = false;
			GetActionTarget()->m_TurnLeft = false;
			GetActionTarget()->m_Accelerate = true;
		}
	}
	else
	{
		GetStateMachine()->SetState(new RefuelPhase3(GetActionTarget(), GetStateMachine()));
		delete this;
	}
}

void RefuelPhase2::Exit(void)
{
	GetActionTarget()->SetVelocity(math3d::vector2f(0.0f, 0.0f));
	GetActionTarget()->m_TurnRight = false;
	GetActionTarget()->m_TurnLeft = false;
	GetActionTarget()->m_Accelerate = false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// RefuelPhase3: Wait for a little time.                                                         //
//               Decide, what to do next.                                                        //
///////////////////////////////////////////////////////////////////////////////////////////////////
RefuelPhase3::RefuelPhase3(Ship * ActionTarget, StateMachine * StateMachine) :
	State(ActionTarget, StateMachine),
	m_TimeToLeave(GameTime::Get() + GetRandomFloat(2.0f, 5.0f))
{
	SetObjectIdentifier("::refuel_phase_3::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void RefuelPhase3::Enter(void)
{
}

void RefuelPhase3::Execute(void)
{
	if(GameTime::Get() >= m_TimeToLeave)
	{
		GetStateMachine()->SetState(new SelectSteering(GetActionTarget(), GetStateMachine()));
		delete this;
	}
}

void RefuelPhase3::Exit(void)
{
	GetActionTarget()->SetFuel(GetActionTarget()->GetFuelCapacity());
	
	MonitorFuel * GlobalState(dynamic_cast< MonitorFuel * >(GetStateMachine()->GetGlobalState()));
	
	assert(GlobalState != 0);
	GlobalState->SetRefueled();
}
