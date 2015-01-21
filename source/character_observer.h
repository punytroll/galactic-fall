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

#ifndef CHARACTER_OBSERVER_H
#define CHARACTER_OBSERVER_H

#include "referencing.h"

class Character;
class Message;

class CharacterObserver
{
public:
	CharacterObserver(void);
	virtual ~CharacterObserver(void);
	// getters
	Character * GetObservedCharacter(void);
	// setters
	void SetObservedCharacter(Character * ObservedCharacter);
	// modifiers
	virtual void HandleMessage(Message * Message) = 0;
private:
	void _OnObservedCharacterDestroying(void);
	Character * _ObservedCharacter;
	Connection _ObservedCharacterDestroyingConnection;
};

inline Character * CharacterObserver::GetObservedCharacter(void)
{
	return _ObservedCharacter;
}

#endif
