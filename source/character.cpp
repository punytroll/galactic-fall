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

#include "character.h"
#include "map_knowledge.h"
#include "message.h"
#include "mind.h"

std::set< Character * > Character::m_Characters;

Character::Character(void) :
	m_Credits(0),
	m_MapKnowledge(new MapKnowledge()),
	m_Ship(0)
{
	m_Characters.insert(this);
	SetAcceptMessages(true);
}

Character::~Character(void)
{
	delete m_MapKnowledge;
	m_MapKnowledge = 0;
	m_Characters.erase(m_Characters.find(this));
}

void Character::Update(void)
{
	// forward messages to the active mind
	Message * Message(0);
	
	// take responsibility for the popped message
	while((Message = PopMessage()) != 0)
	{
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

void Character::AddCredits(u4byte Credits)
{
	m_Credits += Credits;
}

bool Character::RemoveCredits(u4byte Credits)
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

bool Character::IsAddingAllowed(Object * Content)
{
	return dynamic_cast< Mind * >(Content) != 0;
}

bool Character::IsRemovingAllowed(Object * Content)
{
	return Object::IsRemovingAllowed(Content);
}

void Character::OnContentAdded(Object * Content)
{
	Object::OnContentAdded(Content);
	
	Mind * TheMind(dynamic_cast< Mind * >(Content));
	
	assert(TheMind != 0);
	m_Minds.push_front(TheMind);
}

void Character::OnContentRemoved(Object * Content)
{
	Mind * TheMind(dynamic_cast< Mind * >(Content));
	
	assert(TheMind != 0);
	
	std::deque< Mind * >::iterator MindIterator(std::find(m_Minds.begin(), m_Minds.end(), TheMind));
	
	assert(MindIterator != m_Minds.end());
	m_Minds.erase(MindIterator);
	Object::OnContentRemoved(Content);
}
