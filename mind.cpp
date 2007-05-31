#include <iostream>

#include "character.h"
#include "mind.h"
#include "state_machine.h"

MindWithShip::MindWithShip(const Ship * Ship) :
	m_Ship(Ship)
{
}
	
bool MindWithShip::operator()(const Mind * Mind) const
{
	return m_Ship == Mind->GetCharacter()->GetShip();
}

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
