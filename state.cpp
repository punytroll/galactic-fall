#include "state.h"

State::State(Ship * ActionTarget, StateMachine * StateMachine) :
	m_ActionTarget(ActionTarget),
	m_StateMachine(StateMachine)
{
}
