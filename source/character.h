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

#ifndef CHARACTER_H
#define CHARACTER_H

#include <deque>
#include <set>

#include "object.h"
#include "type_definitions.h"

class CharacterObserver;
class MapKnowledge;
class Mind;
class Ship;
class System;
class Threat;

class Character : public Object
{
/** static management **/
public:
	static std::set< Character * > & GetCharacters(void);
private:
	static std::set< Character * > _Characters;

/** individual **/
public:
	Character(void);
	virtual ~Character(void) override;
	// getters
	Mind * GetActiveMind(void);
	unsigned_numeric GetCredits(void) const;
	MapKnowledge * GetMapKnowledge(void);
	Ship * GetShip(void);
	System * GetSystem(void);
	Threat * GetThreat(void);
	// setters
	void SetCredits(unsigned_numeric Credits);
	// modifiers
	void Update(void);
	void AddCredits(unsigned_numeric Credits);
	void AddObserver(CharacterObserver * Reference);
	bool RemoveCredits(unsigned_numeric Credits);
	void RemoveObserver(CharacterObserver * Reference);
private:
	void OnAdded(Object * Content);
	void OnRemoved(Object * Content);
	std::deque< Mind * > _Minds;
	std::set< CharacterObserver * > _Observers;
	unsigned_numeric _Credits;
	MapKnowledge * _MapKnowledge;
	Threat * _Threat;
};

inline Mind * Character::GetActiveMind(void)
{
	if(_Minds.empty() == false)
	{
		return _Minds.front();
	}
	else
	{
		return nullptr;
	}
}

inline std::set< Character * > & Character::GetCharacters(void)
{
	return _Characters;
}

inline unsigned_numeric Character::GetCredits(void) const
{
	return _Credits;
}

inline MapKnowledge * Character::GetMapKnowledge(void)
{
	return _MapKnowledge;
}

inline Threat * Character::GetThreat(void)
{
	return _Threat;
}

inline void Character::SetCredits(unsigned_numeric Credits)
{
	_Credits = Credits;
}

#endif
