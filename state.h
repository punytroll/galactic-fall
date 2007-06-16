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
