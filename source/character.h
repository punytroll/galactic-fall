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

class CharacterObserver;
class MapKnowledge;
class Mind;
class Planet;
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
	std::uint32_t GetCredits(void) const;
	MapKnowledge * GetMapKnowledge(void);
	Planet * GetPlanet(void);
	Ship * GetShip(void);
	System * GetSystem(void);
	Threat * GetThreat(void);
	// setters
	void SetCredits(std::uint32_t Credits);
	// modifiers
	void Update(void);
	void AddCredits(std::uint32_t Credits);
	void AddObserver(CharacterObserver * Reference);
	bool RemoveCredits(std::uint32_t Credits);
	void RemoveObserver(CharacterObserver * Reference);
private:
	void _OnAdded(Object * Content);
	void _OnRemoved(Object * Content);
	std::uint32_t _Credits;
	MapKnowledge * _MapKnowledge;
	std::deque< Mind * > _Minds;
	std::set< CharacterObserver * > _Observers;
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

inline std::uint32_t Character::GetCredits(void) const
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

inline void Character::SetCredits(std::uint32_t Credits)
{
	_Credits = Credits;
}

#endif
