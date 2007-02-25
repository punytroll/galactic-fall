#ifndef STATE_H
#define STATE_H

#include "object.h"

class Ship;
class StateMachine;

class State : public Object
{
public:
	State(Ship * ActionTarget, StateMachine * StateMachine);
	// progress in the handling of the state
	virtual void Enter(void) = 0;
	virtual void Execute(void) = 0;
	virtual void Exit(void) = 0;
	// getters
	Ship * GetActionTarget(void);
	StateMachine * GetStateMachine(void);
private:
	Ship * m_ActionTarget;
	StateMachine * m_StateMachine;
};

inline Ship * State::GetActionTarget(void)
{
	return m_ActionTarget;
}

inline StateMachine * State::GetStateMachine(void)
{
	return m_StateMachine;
}

#endif
