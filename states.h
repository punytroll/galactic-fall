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

class StateMachineMind;

class SelectSteering : public State
{
public:
	SelectSteering(StateMachineMind * Mind);
	virtual void Enter(void);
	virtual void Execute(void);
	virtual void Exit(void);
};

class FlyOverRandomPoint : public State
{
public:
	FlyOverRandomPoint(StateMachineMind * Mind);
	virtual void Enter(void);
	virtual void Execute(void);
	virtual void Exit(void);
protected:
	math3d::vector2f m_Point;
};

class TransporterPhase1 : public State
{
public:
	TransporterPhase1(StateMachineMind * Mind);
	virtual void Enter(void);
	virtual void Execute(void);
	virtual void Exit(void);
};

class TransporterPhase2 : public State
{
public:
	TransporterPhase2(StateMachineMind * Mind);
	virtual void Enter(void);
	virtual void Execute(void);
	virtual void Exit(void);
};

class TransporterPhase3 : public State
{
public:
	TransporterPhase3(StateMachineMind * Mind);
	virtual void Enter(void);
	virtual void Execute(void);
	virtual void Exit(void);
private:
	double m_TimeToLeave;
};

class TransporterPhase4 : public State
{
public:
	TransporterPhase4(StateMachineMind * Mind);
	virtual void Enter(void);
	virtual void Execute(void);
	virtual void Exit(void);
private:
	math3d::vector2f m_JumpPoint;
};

class Fight : public State
{
public:
	Fight(StateMachineMind * Mind);
	virtual void Enter(void);
	virtual void Execute(void);
	virtual void Exit(void);
};

class MonitorFuel : public State
{
public:
	MonitorFuel(StateMachineMind * Mind);
	virtual void Enter(void);
	virtual void Execute(void);
	virtual void Exit(void);
	void SetRefueled(void);
private:
	bool m_Refueling;
};

class RefuelPhase1 : public State
{
public:
	RefuelPhase1(StateMachineMind * Mind);
	virtual void Enter(void);
	virtual void Execute(void);
	virtual void Exit(void);
};

class RefuelPhase2 : public State
{
public:
	RefuelPhase2(StateMachineMind * Mind);
	virtual void Enter(void);
	virtual void Execute(void);
	virtual void Exit(void);
};

class RefuelPhase3 : public State
{
public:
	RefuelPhase3(StateMachineMind * Mind);
	virtual void Enter(void);
	virtual void Execute(void);
	virtual void Exit(void);
private:
	double m_TimeToLeave;
};

#endif
