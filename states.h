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

#ifndef STATES_H
#define STATES_H

#include "state.h"

class Planet;
class Ship;
class StateMachine;
class System;

class SelectSteering : public State
{
public:
	SelectSteering(Ship * ActionTarget, StateMachine * StateMachine);
	virtual void Enter(void);
	virtual void Execute(void);
	virtual void Exit(void);
};

class FlyOverRandomPoint : public State
{
public:
	FlyOverRandomPoint(Ship * ActionTarget, StateMachine * StateMachine);
	virtual void Enter(void);
	virtual void Execute(void);
	virtual void Exit(void);
protected:
	math3d::vector2f m_Point;
};

class TransporterPhase1 : public State
{
public:
	TransporterPhase1(Ship * ActionTarget, StateMachine * StateMachine);
	virtual void Enter(void);
	virtual void Execute(void);
	virtual void Exit(void);
private:
	Planet * m_Planet;
};

class TransporterPhase2 : public State
{
public:
	TransporterPhase2(Ship * ActionTarget, StateMachine * StateMachine, Planet * Planet);
	virtual void Enter(void);
	virtual void Execute(void);
	virtual void Exit(void);
private:
	Planet * m_Planet;
};

class TransporterPhase3 : public State
{
public:
	TransporterPhase3(Ship * ActionTarget, StateMachine * StateMachine);
	virtual void Enter(void);
	virtual void Execute(void);
	virtual void Exit(void);
private:
	double m_TimeToLeave;
};

class TransporterPhase4 : public State
{
public:
	TransporterPhase4(Ship * ActionTarget, StateMachine * StateMachine);
	virtual void Enter(void);
	virtual void Execute(void);
	virtual void Exit(void);
private:
	math3d::vector2f m_JumpPoint;
};

#endif
