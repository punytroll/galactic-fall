#include <iostream>
#include <list>

#include "math.h"
#include "planet.h"
#include "ship.h"
#include "state_machine.h"
#include "states.h"
#include "system.h"

SelectSteering::SelectSteering(Ship * ActionTarget, StateMachine * StateMachine) :
	State(ActionTarget, StateMachine)
{
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
		GetStateMachine()->SetState(new FlyOverRandomPoint(GetActionTarget(), GetStateMachine()));
	}
	delete this;
}

void SelectSteering::Exit(void)
{
}

FlyOverRandomPoint::FlyOverRandomPoint(Ship * ActionTarget, StateMachine * StateMachine) :
	State(ActionTarget, StateMachine)
{
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
	State(ActionTarget, StateMachine),
	m_Planet(0)
{
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
		m_Planet = *PlanetIterator;
	}
	else
	{
		// TODO: goto leave the system
	}
}

void TransporterPhase1::Execute(void)
{
	math3d::vector2f ToDestination(m_Planet->GetPosition() - GetActionTarget()->GetPosition());
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
		GetStateMachine()->SetState(new TransporterPhase2(GetActionTarget(), GetStateMachine(), m_Planet));
		delete this;
	}
}

void TransporterPhase1::Exit(void)
{
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// TransporterPhase2: Revereses the ship and accelerates until a near stop is accomplished.      //
///////////////////////////////////////////////////////////////////////////////////////////////////
TransporterPhase2::TransporterPhase2(Ship * ActionTarget, StateMachine * StateMachine, Planet * Planet) :
	State(ActionTarget, StateMachine),
	m_Planet(Planet)
{
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
		GetStateMachine()->SetState(new TransporterPhase1(GetActionTarget(), GetStateMachine()));
		delete this;
	}
}

void TransporterPhase2::Exit(void)
{
}
