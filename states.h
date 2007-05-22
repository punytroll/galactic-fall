#ifndef STATES_H
#define STATES_H

#include "state.h"

class Planet;
class Ship;
class StateMachine;

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

#endif
