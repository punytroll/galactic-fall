/**
 * galactic-fall
 * Copyright (C) 2008  Hagen Möbius
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
#include "character_observer.h"

CharacterObserver::CharacterObserver(void) :
	_ObservedCharacter(nullptr)
{
}

CharacterObserver::~CharacterObserver(void)
{
	assert(_ObservedCharacter == nullptr);
}

void CharacterObserver::_OnObservedCharacterDestroying(void)
{
	assert(_ObservedCharacter != nullptr);
	assert(m_ObservedCharacterDestroyingConnection.IsValid() == true);
	m_ObservedCharacterDestroyingConnection.Disconnect();
	assert(m_ObservedCharacterDestroyingConnection.IsValid() == false);
	_ObservedCharacter->RemoveObserver(this);
	_ObservedCharacter = nullptr;
}

void CharacterObserver::SetObservedCharacter(Character * ObservedCharacter)
{
	if(_ObservedCharacter != nullptr)
	{
		assert(m_ObservedCharacterDestroyingConnection.IsValid() == true);
		m_ObservedCharacterDestroyingConnection.Disconnect();
		_ObservedCharacter->RemoveObserver(this);
	}
	assert(m_ObservedCharacterDestroyingConnection.IsValid() == false);
	_ObservedCharacter = ObservedCharacter;
	if(_ObservedCharacter != nullptr)
	{
		m_ObservedCharacterDestroyingConnection = _ObservedCharacter->ConnectDestroyingCallback(std::bind(&CharacterObserver::_OnObservedCharacterDestroying, this));
		assert(m_ObservedCharacterDestroyingConnection.IsValid() == true);
		_ObservedCharacter->AddObserver(this);
	}
}
