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
#include "threat.h"

std::set< Character * > Character::m_Characters;

Character::Character(void) :
	m_Credits(0),
	m_MapKnowledge(new MapKnowledge()),
	m_Ship(0),
	m_Threat(new Threat())
{
	// static initialization
	m_Characters.insert(this);
	// initialize object aspects
	AddAspectMessages();
	AddAspectObjectContainer();
	GetAspectObjectContainer()->SetOnAddedCallback(Callback(this, &Character::OnAdded));
	GetAspectObjectContainer()->SetOnRemovedCallback(Callback(this, &Character::OnRemoved));
}

Character::~Character(void)
{
	delete m_MapKnowledge;
	m_MapKnowledge = 0;
	delete m_Threat;
	m_Threat = 0;
	m_Characters.erase(m_Characters.find(this));
}

void Character::Update(void)
{
	// forward all messages in the message queue
	Message * Message(0);
	
	// take responsibility for the popped message
	while((Message = GetAspectMessages()->PopMessage()) != 0)
	{
		// forward message to all registered CharacterObservers
		for(std::set< CharacterObserver * >::iterator ObserverIterator = m_Observers.begin(); ObserverIterator != m_Observers.end(); ++ObserverIterator)
		{
			// we would like to push the messages because handling them brings us into another subsystem, but the delete down there is a stopper
			(*ObserverIterator)->HandleMessage(Message);
		}
		// forward message to the active mind
		if(m_Minds.empty() == false)
		{
			m_Minds.front()->HandleMessage(Message);
		}
		delete Message;
	}
	// let the mind act
	if(m_Minds.empty() == false)
	{
		m_Minds.front()->Update();
	}
}

void Character::AddCredits(unsigned_numeric Credits)
{
	m_Credits += Credits;
}

void Character::AddObserver(CharacterObserver * CharacterObserver)
{
	m_Observers.insert(CharacterObserver);
}

bool Character::RemoveCredits(unsigned_numeric Credits)
{
	if(m_Credits < Credits)
	{
		return false;
	}
	else
	{
		m_Credits -= Credits;
		
		return true;
	}
}

void Character::RemoveObserver(CharacterObserver * CharacterObserver)
{
	m_Observers.erase(m_Observers.find(CharacterObserver));
}

void Character::OnAdded(Object * Content)
{
	assert(Content != nullptr);
	
	Mind * TheMind(dynamic_cast< Mind * >(Content));
	
	assert(TheMind != 0);
	m_Minds.push_front(TheMind);
}

void Character::OnRemoved(Object * Content)
{
	assert(Content != nullptr);
	
	Mind * TheMind(dynamic_cast< Mind * >(Content));
	
	assert(TheMind != 0);
	
	std::deque< Mind * >::iterator MindIterator(std::find(m_Minds.begin(), m_Minds.end(), TheMind));
	
	assert(MindIterator != m_Minds.end());
	m_Minds.erase(MindIterator);
}
