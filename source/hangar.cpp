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

#include "character.h"
#include "hangar.h"
#include "object_aspect_object_container.h"

Hangar::Hangar(void) :
	_Character(nullptr)
{
	// initialize object aspects
	AddAspectObjectContainer();
}

Hangar::~Hangar(void)
{
	if(_Character != nullptr)
	{
		_Character->DisconnectDestroyingCallback(_CharacterDestroyingConnectionHandle);
	}
}

unsigned_numeric Hangar::GetAmount(const std::string & TypeIdentifier, const std::string & ClassIdentifier) const
{
	assert(GetAspectObjectContainer() != nullptr);
	
	unsigned_numeric Amount(0);
	
	for(auto Content : GetAspectObjectContainer()->GetContent())
	{
		if((Content->GetTypeIdentifier() == TypeIdentifier) && (Content->GetClassIdentifier() == ClassIdentifier))
		{
			Amount += 1;
		}
	}
	
	return Amount;
}

void Hangar::SetCharacter(Character * Character)
{
	assert(_Character == nullptr);
	assert(Character != nullptr);
	_Character = Character;
	_CharacterDestroyingConnectionHandle = _Character->ConnectDestroyingCallback(Callback(this, &Hangar::_OnCharacterDestroying));
}

void Hangar::_OnCharacterDestroying(void)
{
	_Character = nullptr;
}
