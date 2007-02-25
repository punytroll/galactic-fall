#include "state.h"
#include "state_machine.h"

StateMachine::StateMachine(void) :
	m_State(0),
	m_GlobalState(0)
{
}

void StateMachine::SetGlobalState(State * State)
{
	if(m_GlobalState != 0)
	{
		m_GlobalState->Exit();
		m_GlobalState = 0;
	}
	if(State != 0)
	{
		m_GlobalState = State;
		m_GlobalState->Enter();
	}
}

void StateMachine::SetState(State * State)
{
	if(m_State != 0)
	{
		m_State->Exit();
		m_State = 0;
	}
	if(State != 0)
	{
		m_State = State;
		m_State->Enter();
	}
}

void StateMachine::Update(void)
{
	if(m_GlobalState != 0)
	{
		m_GlobalState->Execute();
	}
	if(m_State != 0)
	{
		m_State->Execute();
	}
}
