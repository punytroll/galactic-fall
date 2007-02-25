#include <iostream>

#include "math.h"
#include "ship.h"
#include "state_machine.h"
#include "states.h"

SelectSteering::SelectSteering(Ship * ActionTarget, StateMachine * StateMachine) :
	State(ActionTarget, StateMachine)
{
}

void SelectSteering::Enter(void)
{
}

void SelectSteering::Execute(void)
{
	GetStateMachine()->SetState(new FlyOverRandomPoint(GetActionTarget(), GetStateMachine()));
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
	m_Point.m_V.m_A[0] = (-0.5f + static_cast< float >(random()) / RAND_MAX) * 200.0f;
	m_Point.m_V.m_A[1] = (-0.5f + static_cast< float >(random()) / RAND_MAX) * 200.0f;
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
			GetActionTarget()->m_Accelerate = true;
		}
	}
	else
	{
		GetStateMachine()->SetState(new SelectSteering(GetActionTarget(), GetStateMachine()));
	}
}

void FlyOverRandomPoint::Exit(void)
{
}
