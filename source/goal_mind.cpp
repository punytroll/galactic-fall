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

#include "goal.h"
#include "goal_mind.h"
#include "object_aspect_object_container.h"

GoalMind::GoalMind(void) :
	_Goal(nullptr)
{
	// initialize object aspects
	AddAspectObjectContainer();
	GetAspectObjectContainer()->ConnectContentAddedCallback(std::bind(&GoalMind::_OnAdded, this, std::placeholders::_1));
	GetAspectObjectContainer()->ConnectContentRemovedCallback(std::bind(&GoalMind::_OnRemoved, this, std::placeholders::_1));
}

GoalMind::~GoalMind(void)
{
}

void GoalMind::HandleMessage(Message * Message)
{
	assert(_Goal != nullptr);
	_Goal->HandleMessage(Message);
}

void GoalMind::Update(void)
{
	assert(_Goal != nullptr);
	if((_Goal->GetState() == Goal::COMPLETED) || (_Goal->GetState() == Goal::FAILED))
	{
		_Goal->Terminate();
	}
	if(_Goal->GetState() != Goal::ACTIVE)
	{
		_Goal->Activate();
	}
	_Goal->Process();
}

void GoalMind::_OnAdded(Object * Content)
{
	assert(Content != nullptr);
	
	auto AddedGoal(dynamic_cast< Goal * >(Content));
	
	assert(AddedGoal != nullptr);
	assert(_Goal == nullptr);
	_Goal = AddedGoal;
}

void GoalMind::_OnRemoved(Object * Content)
{
	assert(Content != nullptr);
	
	auto RemovedGoal(dynamic_cast< Goal * >(Content));
	
	assert(RemovedGoal != nullptr);
	assert(_Goal == RemovedGoal);
	_Goal = nullptr;
}
