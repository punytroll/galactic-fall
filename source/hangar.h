/**
 * galactic-fall
 * Copyright (C) 2014  Hagen Möbius
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

#ifndef HANGAR_H
#define HANGAR_H

#include "object.h"
#include "type_definitions.h"

class Character;

class Hangar : public Object
{
public:
	// constructor & destructor
	Hangar(void);
	virtual ~Hangar(void) override;
	// getters
	unsigned_numeric GetAmount(const std::string & TypeIdentifier, const std::string & ClassIdentifier) const;
	Character * GetCharacter(void);
	// setters
	void SetCharacter(Character * Character);
private:
	void _OnCharacterDestroying(void);
	// variables
	Character * _Character;
	ConnectionHandle _CharacterDestroyingConnectionHandle;
};

inline Character * Hangar::GetCharacter(void)
{
	return _Character;
}

#endif
