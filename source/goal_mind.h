/**
 * galactic-fall
 * Copyright (C) 2018  Hagen Möbius
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

#ifndef GOAL_MIND_H
#define GOAL_MIND_H

#include "mind.h"

class Goal;

class GoalMind : public Mind
{
public:
	// constructor and destructor
	GoalMind(void);
	virtual ~GoalMind(void);
	// queries
	Goal * GetGoal(void);
	// modifiers
	virtual void HandleMessage(Message * Message);
	virtual void Update(void);
private:
	// callbacks
	void _OnAdded(Object * Content);
	void _OnRemoved(Object * Content);
	// member variables
	Goal * _Goal;
};

inline Goal * GoalMind::GetGoal(void)
{
	return _Goal;
}

#endif
 
