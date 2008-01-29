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

#include <iostream>

#include "goal.h"
#include "mind.h"
#include "state_machine.h"

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

GoalMind::GoalMind(void) :
	m_Goal(0)
{
}

GoalMind::~GoalMind(void)
{
}

void GoalMind::Update(void)
{
	assert(m_Goal != 0);
	if((m_Goal->GetState() == Goal::COMPLETED) || (m_Goal->GetState() == Goal::FAILED))
	{
		m_Goal->Terminate();
	}
	if(m_Goal->GetState() != Goal::ACTIVE)
	{
		m_Goal->Activate();
	}
	m_Goal->Process();
}

bool GoalMind::OnAddContent(Object * Content)
{
	Goal * TheGoal(dynamic_cast< Goal * >(Content));
	
	if((TheGoal != 0) && (m_Goal == 0))
	{
		if(Object::OnAddContent(TheGoal) == true)
		{
			m_Goal = TheGoal;
			
			return true;
		}
	}
	
	return false;
}

bool GoalMind::OnRemoveContent(Object * Content)
{
	Goal * TheGoal(dynamic_cast< Goal * >(Content));
	
	if((TheGoal != 0) && (m_Goal == TheGoal))
	{
		if(Object::OnRemoveContent(TheGoal) == true)
		{
			m_Goal = 0;
			
			return true;
		}
		
		return false;
	}
	
	return Object::OnRemoveContent(Content);
}
