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

#include <algorithm>

#include "character.h"
#include "character_observer.h"
#include "map_knowledge.h"
#include "message.h"
#include "mind.h"
#include "object_aspect_messages.h"
#include "object_aspect_object_container.h"
#include "ship.h"
#include "system.h"
#include "threat.h"

std::set< Character * > Character::_Characters;

Character::Character(void) :
	_Credits(0),
	_MapKnowledge(new MapKnowledge()),
	_Threat(new Threat())
{
	// static initialization
	_Characters.insert(this);
	// initialize object aspects
	AddAspectMessages();
	AddAspectObjectContainer();
	GetAspectObjectContainer()->ConnectContentAddedCallback(std::bind(&Character::OnAdded, this, std::placeholders::_1));
	GetAspectObjectContainer()->ConnectContentRemovedCallback(std::bind(&Character::OnRemoved, this, std::placeholders::_1));
}

Character::~Character(void)
{
	assert(_Observers.empty() == true);
	delete _MapKnowledge;
	_MapKnowledge = nullptr;
	delete _Threat;
	_Threat = nullptr;
	_Characters.erase(_Characters.find(this));
}

void Character::Update(void)
{
	// forward all messages in the message queue
	Message * Message(nullptr);
	
	// take responsibility for the popped message
	while((Message = GetAspectMessages()->PopMessage()) != nullptr)
	{
		// forward message to all registered CharacterObservers
		for(auto Observer : _Observers)
		{
			// we would like to push the messages because handling them brings us into another subsystem, but the delete down there is a stopper
			Observer->HandleMessage(Message);
		}
		// forward message to the active mind
		if(_Minds.empty() == false)
		{
			_Minds.front()->HandleMessage(Message);
		}
		delete Message;
	}
	// let the mind act
	if(_Minds.empty() == false)
	{
		_Minds.front()->Update();
	}
}

void Character::AddCredits(unsigned_numeric Credits)
{
	_Credits += Credits;
}

void Character::AddObserver(CharacterObserver * CharacterObserver)
{
	_Observers.insert(CharacterObserver);
}

Ship * Character::GetShip(void)
{
	auto Container(GetContainer());
	
	while((Container != nullptr) && (Container->GetTypeIdentifier() != "ship"))
	{
		Container = Container->GetContainer();
	}
	
	return dynamic_cast< Ship * >(Container);
}

System * Character::GetSystem(void)
{
	auto Container(GetContainer());
	
	while((Container != nullptr) && (Container->GetTypeIdentifier() != "system"))
	{
		Container = Container->GetContainer();
	}
	
	return dynamic_cast< System * >(Container);
}

bool Character::RemoveCredits(unsigned_numeric Credits)
{
	if(_Credits < Credits)
	{
		return false;
	}
	else
	{
		_Credits -= Credits;
		
		return true;
	}
}

void Character::RemoveObserver(CharacterObserver * CharacterObserver)
{
	_Observers.erase(_Observers.find(CharacterObserver));
}

void Character::OnAdded(Object * Content)
{
	assert(Content != nullptr);
	
	auto TheMind(dynamic_cast< Mind * >(Content));
	
	assert(TheMind != nullptr);
	_Minds.push_front(TheMind);
}

void Character::OnRemoved(Object * Content)
{
	assert(Content != nullptr);
	
	auto TheMind(dynamic_cast< Mind * >(Content));
	
	assert(TheMind != nullptr);
	
	auto MindIterator(std::find(_Minds.begin(), _Minds.end(), TheMind));
	
	assert(MindIterator != _Minds.end());
	_Minds.erase(MindIterator);
}
