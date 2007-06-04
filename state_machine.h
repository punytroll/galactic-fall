#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

class State;

class StateMachine
{
public:
	StateMachine(void);
	~StateMachine(void);
	void SetGlobalState(State * State);
	void SetState(State * State);
	void Update(void);
private:
	State * m_State;
	State * m_GlobalState;
};

#endif
