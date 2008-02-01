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

#ifndef GOALS_H
#define GOALS_H

#include "goal.h"
#include "math/vector3f.h"

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
	Vector3f m_Direction;
	bool m_FacesDirection;
};

class GoalFlyOverRandomPoint : public Goal
{
public:
	GoalFlyOverRandomPoint(GoalMind * GoalMind);
	virtual void Activate(void);
	virtual void Process(void);
	virtual void Terminate(void);
private:
	Vector3f m_Point;
	GoalFlyInDirection * m_FlyInDirection;
};

class GoalFighterThink : public Goal
{
public:
	GoalFighterThink(GoalMind * GoalMind);
	virtual void Activate(void);
	virtual void Process(void);
};

class GoalFightFighter : public Goal
{
public:
	GoalFightFighter(GoalMind * GoalMind);
	virtual void Activate(void);
	virtual void Process(void);
};

class GoalSelectFighter : public Goal
{
public:
	GoalSelectFighter(GoalMind * GoalMind);
	virtual void Activate(void);
	virtual void Process(void);
};

class GoalDestroyTarget : public Goal
{
public:
	GoalDestroyTarget(GoalMind * GoalMind);
	virtual void Activate(void);
	virtual void Process(void);
	virtual void Terminate(void);
private:
	GoalFlyInDirection * m_FlyInDirection;
};

#endif
