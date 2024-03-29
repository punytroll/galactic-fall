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
	State * GetGlobalState(void);
	State * GetState(void);
	void Update(void);
	// modifiers
	bool HandleMessage(Message * Message);
private:
	State * m_State;
	State * m_GlobalState;
};

inline State * StateMachine::GetGlobalState(void)
{
	return m_GlobalState;
}

inline State * StateMachine::GetState(void)
{
	return m_State;
}

#endif
