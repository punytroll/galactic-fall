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
