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

#include <set>
#include <stack>

#include "object.h"

class MapKnowledge;
class Mind;
class Ship;

class Character : public Object
{
/** static management **/
public:
	static std::set< Character * > & GetCharacters(void);
private:
	static std::set< Character * > m_Characters;

/** individual **/
public:
	Character(void);
	virtual ~Character(void);
	// getters
	float GetCredits(void);
	MapKnowledge * GetMapKnowledge(void);
	Ship * GetShip(void);
	const Ship * GetShip(void) const;
	// setters
	void SetCredits(float Credits);
	void SetShip(Ship * Ship);
	// modifiers
	void Update(void);
	void AddCredits(const float & Credits);
	bool RemoveCredits(const float & Credits);
	// mind managemant
	void PossessByMind(Mind * Mind);
	Mind * ReleaseMind(void);
private:
	std::stack< Mind * > m_Minds;
	float m_Credits;
	MapKnowledge * m_MapKnowledge;
	Ship * m_Ship;
};

class CharacterWithShip : public std::unary_function< Ship *, bool >
{
public:
	explicit CharacterWithShip(const Ship * Ship);
	bool operator()(const Character * Character) const;
private:
	const Ship * m_Ship;
};

inline std::set< Character * > & Character::GetCharacters(void)
{
	return m_Characters;
}

inline float Character::GetCredits(void)
{
	return m_Credits;
}

inline MapKnowledge * Character::GetMapKnowledge(void)
{
	return m_MapKnowledge;
}

inline Ship * Character::GetShip(void)
{
	return m_Ship;
}

inline const Ship * Character::GetShip(void) const
{
	return m_Ship;
}

inline void Character::SetCredits(float Credits)
{
	m_Credits = Credits;
}

inline void Character::SetShip(Ship * Ship)
{
	m_Ship = Ship;
}

#endif
