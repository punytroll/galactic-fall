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

#ifndef GOALS_H
#define GOALS_H

#include <algebra/vector3f.h>

#include "goal.h"

class GoalFlyInDirection;
class Planet;
class System;

class GoalApproachTarget : public Goal
{
public:
	GoalApproachTarget(GoalMind * GoalMind);
	virtual void Activate(void);
	virtual void Process(void);
	virtual void Terminate(void);
private:
	GoalFlyInDirection * _FlyInDirection;
};

class GoalDestroyTarget : public Goal
{
public:
	GoalDestroyTarget(GoalMind * GoalMind);
	virtual void Activate(void);
	virtual void Process(void);
	virtual void Terminate(void);
private:
	GoalFlyInDirection * _FlyInDirection;
};

class GoalFighterThink : public Goal
{
public:
	GoalFighterThink(GoalMind * GoalMind);
	virtual void Activate(void);
	virtual bool OnMessageReceived(Message * Message);
	virtual void Process(void);
};

class GoalFightSomeTarget : public Goal
{
public:
	GoalFightSomeTarget(GoalMind * GoalMind);
	virtual void Activate(void);
	virtual void Process(void);
};

class GoalFlyInDirection : public Goal
{
public:
	GoalFlyInDirection(GoalMind * GoalMind);
	// getters
	bool GetFacesDirection(void) const;
	// setters
	void SetDirection(const Vector3f & Direction);
	virtual void Activate(void);
	virtual void Process(void);
private:
	Vector3f _Direction;
	bool _FacesDirection;
};

class GoalFlyInSystemDirection : public Goal
{
public:
	GoalFlyInSystemDirection(GoalMind * GoalMind);
	virtual void Activate(void);
	virtual void Process(void);
	virtual void Terminate(void);
private:
	GoalFlyInDirection * _FlyInDirection;
};

class GoalFlyOverRandomPointWhileNoThreat : public Goal
{
public:
	GoalFlyOverRandomPointWhileNoThreat(GoalMind * GoalMind);
	virtual void Activate(void);
	virtual void Process(void);
	virtual void Terminate(void);
private:
	Vector3f _Point;
	GoalFlyInDirection * _FlyInDirection;
};

class GoalFullStop : public Goal
{
public:
	GoalFullStop(GoalMind * GoalMind);
	virtual void Activate(void);
	virtual void Process(void);
	virtual void Terminate(void);
private:
	GoalFlyInDirection * _FlyInDirection;
};

class GoalJump : public Goal
{
public:
	GoalJump(GoalMind * GoalMind);
	virtual void Activate(void);
	virtual void Process(void);
};

class GoalJumpToSystem : public Goal
{
public:
	enum SystemPolicy
	{
		JUMP_TO_RANDOM_SYSTEM
	};
	
	GoalJumpToSystem(GoalMind * GoalMind, GoalJumpToSystem::SystemPolicy SystemPolicy);
	virtual void Activate(void);
	virtual void Process(void);
private:
	GoalJumpToSystem::SystemPolicy _SystemPolicy;
};

class GoalLand : public Goal
{
public:
	GoalLand(GoalMind * GoalMind);
	virtual void Activate(void);
	virtual void Process(void);
};

class GoalRefuel : public Goal
{
public:
	GoalRefuel(GoalMind * GoalMind, const Reference< Planet > & Planet);
	virtual void Activate(void);
	virtual void Process(void);
private:
	Reference< Planet > _Planet;
};

class GoalSelectFightableTarget : public Goal
{
public:
	GoalSelectFightableTarget(GoalMind * GoalMind);
	virtual void Activate(void);
	virtual void Process(void);
};

class GoalSelectEnemy : public Goal
{
public:
	GoalSelectEnemy(GoalMind * GoalMind);
	virtual void Activate(void);
	virtual void Process(void);
};

class GoalSelectMeasuredCargo : public Goal
{
public:
	GoalSelectMeasuredCargo(GoalMind * GoalMind);
	virtual void Activate(void);
	virtual void Process(void);
};

class GoalSelectNearestPlanetInSystem : public Goal
{
public:
	GoalSelectNearestPlanetInSystem(GoalMind * GoalMind);
	virtual void Activate(void);
	virtual void Process(void);
};

class GoalSelectRandomSystem : public Goal
{
public:
	GoalSelectRandomSystem(GoalMind * GoalMind);
	virtual void Activate(void);
	virtual void Process(void);
};

class GoalSelectStrandedShip : public Goal
{
public:
	GoalSelectStrandedShip(GoalMind * GoalMind);
	virtual void Activate(void);
	virtual void Process(void);
};

class GoalStopAtTarget : public Goal
{
public:
	GoalStopAtTarget(GoalMind * GoalMind);
	virtual void Activate(void);
	virtual void Process(void);
};

class GoalTakeOffFromPlanet : public Goal
{
public:
	GoalTakeOffFromPlanet(GoalMind * GoalMind);
	virtual void Activate(void);
	virtual void Process(void);
};

class GoalVisitPlanet : public Goal
{
public:
	enum PlanetPolicy
	{
		VISIT_NEAREST_PLANET,
		VISIT_NEAREST_PLANET_IN_SYSTEM
	};
	
	GoalVisitPlanet(GoalMind * GoalMind, GoalVisitPlanet::PlanetPolicy PlanetPolicy);
	virtual void Activate(void);
	bool OnMessageReceived(Message * Message);
	virtual void Process(void);
private:
	GoalVisitPlanet::PlanetPolicy _PlanetPolicy;
};

class GoalWait : public Goal
{
public:
	GoalWait(GoalMind * GoalMind, float Seconds);
	virtual void Activate(void);
	virtual void Process(void);
private:
	float _SecondsToWait;
	float _TimeToLeave;
};

#endif
