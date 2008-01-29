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

#include "character.h"
#include "game_time.h"
#include "goals.h"
#include "math.h"
#include "mind.h"
#include "ship.h"
#include "string_cast.h"

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

FlyOverRandomPointGoal::FlyOverRandomPointGoal(GoalMind * GoalMind) :
	Goal(GoalMind)
{
	SetObjectIdentifier("::goal(fly_over_random_point)::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::at(" + to_string_cast(reinterpret_cast< void * >(this)) + ")");
}

void FlyOverRandomPointGoal::Activate(void)
{
	SetState(Goal::ACTIVE);
	
	m_Point.Set(GetRandomFloat(-200.0f, 200.0f), GetRandomFloat(-200.0f, 200.0f), 0.0f);
}

void FlyOverRandomPointGoal::Process(void)
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
		SetState(Goal::COMPLETED);
	}
}

void FlyOverRandomPointGoal::Terminate(void)
{
}
