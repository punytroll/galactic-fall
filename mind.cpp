#include <iostream>

#include "mind.h"
#include "state_machine.h"

StateMachineMind::StateMachineMind(void) :
	m_StateMachine(new StateMachine())
{
}

StateMachineMind::~StateMachineMind(void)
{
	delete m_StateMachine;
	m_StateMachine = 0;
}

void StateMachineMind::Update(void)
{
	m_StateMachine->Update();
}
