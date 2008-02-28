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

#include "state.h"
#include "state_machine.h"

StateMachine::StateMachine(void) :
	m_State(0),
	m_GlobalState(0)
{
}

StateMachine::~StateMachine(void)
{
	delete m_GlobalState;
	delete m_State;
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

bool StateMachine::HandleMessage(Message * Message)
{
	if((m_State == 0) || (m_State->HandleMessage(Message) == false))
	{
		if(m_GlobalState != 0)
		{
			return m_GlobalState->HandleMessage(Message);
		}
		else
		{
			return false;
		}
	}
	else
	{
		return true;
	}
}
