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

#ifndef MIND_H
#define MIND_H

#include "object.h"

class Character;
class Message;

/**
 * A Mind is the aggregation of the internal state of a character.
 * The Mind is meant to be inherited by classes which implement the resulting behavior.
 **/

class Mind : public Object
{
public:
	void SetCharacter(Character * Character);
	// getters
	Character * GetCharacter(void);
	const Character * GetCharacter(void) const;
	// actors
	virtual void HandleMessage(Message * Message) = 0;
	virtual void Update(void) = 0;
protected:
	/// no internal state yet
	Character * m_Character;
};

inline void Mind::SetCharacter(Character * Character)
{
	m_Character = Character;
}

inline Character * Mind::GetCharacter(void)
{
	return m_Character;
}

inline const Character * Mind::GetCharacter(void) const
{
	return m_Character;
}

class StateMachine;

class StateMachineMind : public Mind
{
public:
	StateMachineMind(void);
	~StateMachineMind(void);
	// getters
	StateMachine * GetStateMachine(void);
	// actors
	virtual void HandleMessage(Message * Message);
	virtual void Update(void);
private:
	StateMachine * m_StateMachine;
};

inline StateMachine * StateMachineMind::GetStateMachine(void)
{
	return m_StateMachine;
}

class Goal;

class GoalMind : public Mind
{
public:
	GoalMind(void);
	virtual ~GoalMind(void);
	// getters
	Goal * GetGoal(void) const;
	// actors
	virtual void HandleMessage(Message * Message);
	virtual void Update(void);
private:
	bool AllowAdding(Object * Content);
	bool AllowRemoving(Object * Content);
	void OnAdded(Object * Content);
	void OnRemoved(Object * Content);
	Goal * m_Goal;
};

inline Goal * GoalMind::GetGoal(void) const
{
	return m_Goal;
}

#endif
