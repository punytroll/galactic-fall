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

#include "goal.h"

Goal::Goal(GoalMind * GoalMind) :
	m_GoalMind(GoalMind),
	m_State(Goal::INACTIVE)
{
}

Goal::~Goal(void)
{
}

void Goal::Activate(void)
{
}

void Goal::Process(void)
{
}

void Goal::Terminate(void)
{
}

void Goal::ProcessSubGoals(void)
{
	bool Done(false);
	
	while(Done == false)
	{
		if(m_SubGoals.empty() == true)
		{
			m_State = Goal::COMPLETED;
			Done = true;
		}
		else
		{
			if(m_SubGoals.front()->GetState() == Goal::INACTIVE)
			{
				m_SubGoals.front()->Activate();
			}
			if(m_SubGoals.front()->GetState() == Goal::ACTIVE)
			{
				m_SubGoals.front()->Process();
				Done = true;
			}
			if((m_SubGoals.front()->GetState() == Goal::COMPLETED) || (m_SubGoals.front()->GetState() == Goal::FAILED))
			{
				m_SubGoals.front()->Terminate();
				RemoveContent(m_SubGoals.front());
			}
		}
	}
}

bool Goal::OnAddContent(Object * Content)
{
	Goal * TheGoal(dynamic_cast< Goal * >(Content));
	
	if(TheGoal != 0)
	{
		if(Object::OnAddContent(Content) == true)
		{
			m_SubGoals.push_front(TheGoal);
			
			return true;
		}
	}
	
	return false;
}

bool Goal::OnRemoveContent(Object * Content)
{
	Goal * TheGoal(dynamic_cast< Goal * >(Content));
	
	if(TheGoal != 0)
	{
		std::deque< Goal * >::iterator Iterator(std::find(m_SubGoals.begin(), m_SubGoals.end(), TheGoal));
		
		if(Iterator != m_SubGoals.end())
		{
			delete *Iterator;
			m_SubGoals.erase(Iterator);
		}
	}
	
	return Object::OnRemoveContent(Content);
}
