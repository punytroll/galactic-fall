/**
 * galactic-fall
 * Copyright (C) 2008  Aram Altschudjian
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

#ifndef GOAL_H
#define GOAL_H

#include <deque>

#include "object.h"

class GoalMind;

class Goal : public Object
{
public:
	enum State
	{
		INACTIVE,
		ACTIVE,
		COMPLETED,
		FAILED
	};
	
	Goal(GoalMind * GoalMind, const std::string & Name);
	~Goal(void);
	// getters
	GoalMind * GetMind(void) const;
	const std::string & GetName(void) const;
	const int GetState(void) const;
	std::deque< Goal * > & GetSubGoals(void);
	const std::deque< Goal * > & GetSubGoals(void) const;
	bool HasSubGoal(const std::string & Name) const;
	// actors
	virtual void Activate(void);
	virtual bool OnMessageReceived(Message * Message); 
	virtual void Process(void);
	virtual void Terminate(void);
	// modifiers
	bool AddSubGoal(Goal * SubGoal);
	bool HandleMessage(Message * Message);
	bool RemoveSubGoal(Goal * SubGoal);
protected:
	// setters
	void SetState(int State);
	bool AllowAdding(Object * Content);
	bool AllowRemoving(Object * Content);
	void OnAdded(Object * Content);
	void OnRemoved(Object * Content);
private:
	GoalMind * m_GoalMind;
	std::string m_Name;
	int m_State;
	std::deque< Goal * > m_SubGoals;
};

inline GoalMind * Goal::GetMind(void) const
{
	return m_GoalMind;
}

inline const std::string & Goal::GetName(void) const
{
	return m_Name;
}

inline const int Goal::GetState(void) const
{
	return m_State;
}

inline std::deque< Goal * > & Goal::GetSubGoals(void)
{
	return m_SubGoals;
}

inline const std::deque< Goal * > & Goal::GetSubGoals(void) const
{
	return m_SubGoals;
}

inline void Goal::SetState(int State)
{
	m_State = State;
}

#endif
