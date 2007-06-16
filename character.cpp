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
#include "mind.h"

std::set< Character * > Character::m_Characters;

Character::Character(void) :
	m_Credits(0.0f),
	m_MapKnowledge(new MapKnowledge()),
	m_Ship(0)
{
	m_Characters.insert(this);
}

Character::~Character(void)
{
	m_Characters.erase(m_Characters.find(this));
}

void Character::Update(void)
{
	if(m_Minds.empty() == false)
	{
		m_Minds.top()->Update();
	}
}

void Character::AddCredits(const float & Credits)
{
	m_Credits += Credits;
}

bool Character::RemoveCredits(const float & Credits)
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

void Character::PossessByMind(Mind * Mind)
{
	m_Minds.push(Mind);
}

Mind * Character::ReleaseMind(void)
{
	Mind * Result(0);
	
	if(m_Minds.empty() == false)
	{
		Result = m_Minds.top();
		m_Minds.pop();
	}
	
	return Result;
}

CharacterWithShip::CharacterWithShip(const Ship * Ship) :
	m_Ship(Ship)
{
}
	
bool CharacterWithShip::operator()(const Character * Character) const
{
	return m_Ship == Character->GetShip();
}
