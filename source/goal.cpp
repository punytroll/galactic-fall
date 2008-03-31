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
#include "object_aspect_object_container.h"

Goal::Goal(GoalMind * GoalMind, const std::string & Name) :
	m_GoalMind(GoalMind),
	m_Name(Name),
	m_State(Goal::INACTIVE)
{
	// initialize object aspects
	AddAspectObjectContainer();
	GetAspectObjectContainer()->SetAllowAddingCallback(Callback(this, &Goal::AllowAdding));
	GetAspectObjectContainer()->SetAllowRemovingCallback(Callback(this, &Goal::AllowRemoving));
	GetAspectObjectContainer()->SetOnAddedCallback(Callback(this, &Goal::OnAdded));
	GetAspectObjectContainer()->SetOnRemovedCallback(Callback(this, &Goal::OnRemoved));
}

Goal::~Goal(void)
{
}

bool Goal::HasSubGoal(const std::string & Name) const
{
	std::deque< Goal * >::const_iterator SubGoalIterator(m_SubGoals.begin());
	
	while(SubGoalIterator != m_SubGoals.end())
	{
		if((*SubGoalIterator)->GetName() == Name)
		{
			return true;
		}
		++SubGoalIterator;
	}
	
	return false;
}

void Goal::Activate(void)
{
}

bool Goal::OnMessageReceived(Message * Message)
{
	return false;
}

void Goal::Process(void)
{
}

void Goal::Terminate(void)
{
}

bool Goal::AddSubGoal(Goal * SubGoal)
{
	return GetAspectObjectContainer()->AddContent(SubGoal);
}

bool Goal::HandleMessage(Message * Message)
{
	if((m_SubGoals.size() != 0) && (m_SubGoals.front()->GetState() == Goal::ACTIVE) && (m_SubGoals.front()->HandleMessage(Message) == true))
	{
		return true;
	}
	else
	{
		return OnMessageReceived(Message);
	}
}

bool Goal::RemoveSubGoal(Goal * SubGoal)
{
	return GetAspectObjectContainer()->RemoveContent(SubGoal);
}

bool Goal::AllowAdding(Object * Content)
{
	return dynamic_cast< Goal * >(Content) != 0;
}

bool Goal::AllowRemoving(Object * Content)
{
	return true;
}

void Goal::OnAdded(Object * Content)
{
	Goal * TheGoal(dynamic_cast< Goal * >(Content));
	
	assert(TheGoal != 0);
	m_SubGoals.push_front(TheGoal);
}

void Goal::OnRemoved(Object * Content)
{
	Goal * TheGoal(dynamic_cast< Goal * >(Content));
	
	assert(TheGoal != 0);
	
	std::deque< Goal * >::iterator SubGoalIterator(std::find(m_SubGoals.begin(), m_SubGoals.end(), TheGoal));
	
	assert(SubGoalIterator != m_SubGoals.end());
	m_SubGoals.erase(SubGoalIterator);
}
