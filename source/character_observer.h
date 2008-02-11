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

class CharacterObserver
{
public:
	CharacterObserver(void);
	~CharacterObserver(void);
	// getters
	Reference< Character > & GetObservedCharacter(void);
	const Reference< Character > & GetObservedCharacter(void) const;
	// setters
	void SetObservedCharacter(Reference< Character > ObservedCharacter);
private:
	Reference< Character > m_ObservedCharacter;
};

inline Reference< Character > & CharacterObserver::GetObservedCharacter(void)
{
	return m_ObservedCharacter;
}

inline const Reference< Character > & CharacterObserver::GetObservedCharacter(void) const
{
	return m_ObservedCharacter;
}

#endif
