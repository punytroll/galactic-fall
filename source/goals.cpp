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

#include <list>

#include "character.h"
#include "game_time.h"
#include "goals.h"
#include "math.h"
#include "mind.h"
#include "ship.h"
#include "string_cast.h"
#include "system.h"

GoalFlyInDirection::GoalFlyInDirection(GoalMind * GoalMind) :
	Goal(GoalMind),
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

GoalFlyOverRandomPoint::GoalFlyOverRandomPoint(GoalMind * GoalMind) :
	Goal(GoalMind)
{
	SetObjectIdentifier("::goal(fly_over_random_point)::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void GoalFlyOverRandomPoint::Activate(void)
{
	SetState(Goal::ACTIVE);
	m_FlyInDirection = new GoalFlyInDirection(GetMind());
	AddContent(m_FlyInDirection);
	m_FlyInDirection->Activate();
	m_Point.Set(GetRandomFloat(-200.0f, 200.0f), GetRandomFloat(-200.0f, 200.0f), 0.0f);
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

GoalFighterThink::GoalFighterThink(GoalMind * GoalMind) :
	Goal(GoalMind)
{
	SetObjectIdentifier("::goal(fighter_think)::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void GoalFighterThink::Activate(void)
{
	assert(GetSubGoals().empty() == true);
	AddContent(new GoalFightFighter(GetMind()));
	SetState(Goal::ACTIVE);
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
		SubGoal->Terminate();
		RemoveContent(SubGoal);
		SubGoal->Destroy();
		delete SubGoal;
		Activate();
	}
	else if(SubGoal->GetState() == Goal::FAILED)
	{
		SubGoal->Terminate();
		RemoveContent(SubGoal);
		SubGoal->Destroy();
		delete SubGoal;
		AddContent(new GoalFlyOverRandomPoint(GetMind()));
	}
}

GoalFightFighter::GoalFightFighter(GoalMind * GoalMind) :
	Goal(GoalMind)
{
	SetObjectIdentifier("::goal(fight_fighter)::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void GoalFightFighter::Activate(void)
{
	AddContent(new GoalDestroyTarget(GetMind()));
	if((GetMind()->GetCharacter()->GetShip()->GetTarget() == false) || (dynamic_cast< const Ship * >(GetMind()->GetCharacter()->GetShip()->GetTarget().Get())->GetShipClass()->GetIdentifier() != "fighter"))
	{
		AddContent(new GoalSelectFighter(GetMind()));
	}
	SetState(Goal::ACTIVE);
}

void GoalFightFighter::Process(void)
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
		SubGoal->Terminate();
		RemoveContent(SubGoal);
		SubGoal->Destroy();
		delete SubGoal;
		if(GetSubGoals().empty() == true)
		{
			SetState(Goal::COMPLETED);
		}
	}
	else if(SubGoal->GetState() == Goal::FAILED)
	{
		SubGoal->Terminate();
		SetState(Goal::FAILED);
	}
}

GoalSelectFighter::GoalSelectFighter(GoalMind * GoalMind) :
	Goal(GoalMind)
{
	SetObjectIdentifier("::goal(select_fighter)::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void GoalSelectFighter::Activate(void)
{
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

GoalDestroyTarget::GoalDestroyTarget(GoalMind * GoalMind) :
	Goal(GoalMind)
{
	SetObjectIdentifier("::goal(destroy_target)::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void GoalDestroyTarget::Activate(void)
{
	SetState(Goal::ACTIVE);
	m_FlyInDirection = new GoalFlyInDirection(GetMind());
	AddContent(m_FlyInDirection);
	m_FlyInDirection->Activate();
}

void GoalDestroyTarget::Process(void)
{
	assert(GetState() == Goal::ACTIVE);
	if(GetMind()->GetCharacter()->GetShip()->GetTarget() == true)
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
