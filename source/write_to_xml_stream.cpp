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

#include <assert.h>

#include <iostream>

#include "character.h"
#include "commodity.h"
#include "map_knowledge.h"
#include "mind.h"
#include "object.h"
#include "object_aspect_accessory.h"
#include "object_aspect_name.h"
#include "object_aspect_object_container.h"
#include "object_aspect_position.h"
#include "ship.h"
#include "slot.h"
#include "system.h"
#include "weapon.h"
#include "write_to_xml_stream.h"
#include "xml_stream.h"

static void WriteToXMLStream(XMLStream & XMLStream, Object * TheObject);
static void WriteToXMLStream(XMLStream & XMLStream, Object * TheObject, std::stack< Object * > & ObjectStack);

static void WriteCharacterToXMLStream(XMLStream & XMLStream, Character * TheCharacter);
static void WriteCommodityToXMLStream(XMLStream & XMLStream, Commodity * TheCommodity);
static void WriteMindToXMLStream(XMLStream & XMLStream, Mind * TheMind);
static void WriteShipToXMLStream(XMLStream & XMLStream, Ship * TheShip);
static void WriteWeaponToXMLStream(XMLStream & XMLStream, Weapon * TheWeapon);

void WriteToXMLStream(XMLStream & XMLStream, Object * TheObject, bool Recursive)
{
	if(Recursive == true)
	{
		std::stack< Object * > ObjectStack;
		
		ObjectStack.push(TheObject);
		while(ObjectStack.empty() == false)
		{
			Object * TopObject(ObjectStack.top());
			
			ObjectStack.pop();
			WriteToXMLStream(XMLStream, TopObject, ObjectStack);
		}
	}
	else
	{
		WriteToXMLStream(XMLStream, TheObject);
	}
}

static void WriteToXMLStream(XMLStream & XMLStream, Object * TheObject, std::stack< Object * > & ObjectStack)
{
	if(TheObject->GetAspectObjectContainer() != 0)
	{
		const std::set< Object * > & Content(TheObject->GetAspectObjectContainer()->GetContent());
		
		for(std::set< Object * >::const_iterator ContentIterator =  Content.begin(); ContentIterator != Content.end(); ++ContentIterator)
		{
			ObjectStack.push(*ContentIterator);
		}
	}
	WriteToXMLStream(XMLStream, TheObject);
}

void WriteToXMLStream(XMLStream & XMLStream, Object * TheObject)
{
	assert(TheObject != 0);
	XMLStream << element << "object" << attribute << "type-identifier" << value << TheObject->GetTypeIdentifier() << attribute << "class-identifier" << value << TheObject->GetClassIdentifier() << attribute << "object-identifier" << value << TheObject->GetObjectIdentifier();
	if(TheObject->GetAspectAccessory() != 0)
	{
		XMLStream << element << "aspect-accessory";
		if(TheObject->GetAspectAccessory()->GetSlot() != 0)
		{
			XMLStream << element << "slot" << attribute << "slot-identifier" << value << TheObject->GetAspectAccessory()->GetSlot()->GetIdentifier() << end;
		}
		XMLStream << end;
	}
	if(TheObject->GetAspectName() != 0)
	{
		XMLStream << element << "aspect-name";
		XMLStream << element << "name" << attribute << "value" << value << TheObject->GetAspectName()->GetName() << end;
		XMLStream << end;
	}
	if(TheObject->GetAspectObjectContainer() != 0)
	{
		XMLStream << element << "aspect-object-container";
		
		const std::set< Object * > & Content(TheObject->GetAspectObjectContainer()->GetContent());
		
		for(std::set< Object * >::const_iterator ContentIterator = Content.begin(); ContentIterator != Content.end(); ++ContentIterator)
		{
			XMLStream << element << "content" << attribute << "object-identifier" << value << (*ContentIterator)->GetObjectIdentifier() << end;
		}
		XMLStream << end;
	}
	if(TheObject->GetAspectPosition() != 0)
	{
		XMLStream << element << "aspect-position";
		XMLStream << element << "orientation" << attribute << "w" << value << TheObject->GetAspectPosition()->GetOrientation()[0] << attribute << "x" << value << TheObject->GetAspectPosition()->GetOrientation()[1] << attribute << "y" << value << TheObject->GetAspectPosition()->GetOrientation()[2] << attribute << "z" << value << TheObject->GetAspectPosition()->GetOrientation()[3] << end;
		XMLStream << element << "position" << attribute << "x" << value << TheObject->GetAspectPosition()->GetPosition().m_V.m_A[0] << attribute << "y" << value << TheObject->GetAspectPosition()->GetPosition().m_V.m_A[1] << attribute << "z" << value << TheObject->GetAspectPosition()->GetPosition().m_V.m_A[2] << end;
		XMLStream << end;
	}
	XMLStream << element << "type-specific";
	if(TheObject->GetTypeIdentifier() == "character")
	{
		WriteCharacterToXMLStream(XMLStream, dynamic_cast< Character * >(TheObject));
	}
	else if(TheObject->GetTypeIdentifier() == "commodity")
	{
		WriteCommodityToXMLStream(XMLStream, dynamic_cast< Commodity * >(TheObject));
	}
	else if(TheObject->GetTypeIdentifier() == "mind")
	{
		WriteMindToXMLStream(XMLStream, dynamic_cast< Mind * >(TheObject));
	}
	else if(TheObject->GetTypeIdentifier() == "ship")
	{
		WriteShipToXMLStream(XMLStream, dynamic_cast< Ship * >(TheObject));
	}
	else if(TheObject->GetTypeIdentifier() == "weapon")
	{
		WriteWeaponToXMLStream(XMLStream, dynamic_cast< Weapon * >(TheObject));
	}
	else
	{
		std::cout << "Unknown object type: " << TheObject->GetTypeIdentifier() << std::endl;
		assert(false);
	}
	XMLStream << end;
	XMLStream << end;
}

static void WriteCharacterToXMLStream(XMLStream & XMLStream, Character * TheCharacter)
{
	assert(TheCharacter != 0);
	XMLStream << element << "credits" << attribute << "value" << value << TheCharacter->GetCredits() << end;
	XMLStream << element << "map-knowledge";
	
	const std::set< System * > & ExploredSystems(TheCharacter->GetMapKnowledge()->GetExploredSystems());
	
	for(std::set< System * >::const_iterator ExploredSystemIterator = ExploredSystems.begin(); ExploredSystemIterator != ExploredSystems.end(); ++ExploredSystemIterator)
	{
		XMLStream << element << "explored-system" << attribute << "object-identifier" << value << (*ExploredSystemIterator)->GetObjectIdentifier() << end;
	}
	XMLStream << end;
	if(TheCharacter->GetShip() != 0)
	{
		XMLStream << element << "ship" << attribute << "object-identifier" << value << TheCharacter->GetShip()->GetObjectIdentifier() << end;
	}
}

static void WriteCommodityToXMLStream(XMLStream & XMLStream, Commodity * TheCommodity)
{
	assert(TheCommodity != 0);
	XMLStream << element << "angular-velocity" << attribute << "axis-x" << value << TheCommodity->GetAngularVelocity().m_V.m_A[0] << attribute << "axis-y" << value << TheCommodity->GetAngularVelocity().m_V.m_A[1] << attribute << "axis-z" << value << TheCommodity->GetAngularVelocity().m_V.m_A[2] << attribute << "angle" << value << TheCommodity->GetAngularVelocity().m_V.m_A[3] << end;
	XMLStream << element << "hull" << attribute << "value" << value << TheCommodity->GetHull() << end;
	XMLStream << element << "velocity" << attribute << "x" << value << TheCommodity->GetVelocity().m_V.m_A[0] << attribute << "y" << value << TheCommodity->GetVelocity().m_V.m_A[1] << attribute << "z" << value << TheCommodity->GetVelocity().m_V.m_A[2] << end;
}

static void WriteMindToXMLStream(XMLStream & XMLStream, Mind * TheMind)
{
	assert(TheMind != 0);
	XMLStream << element << "character" << attribute << "object-identifier" << value << TheMind->GetCharacter()->GetObjectIdentifier() << end;
}

static void WriteShipToXMLStream(XMLStream & XMLStream, Ship * TheShip)
{
	assert(TheShip != 0);
	XMLStream << element << "current-system" << attribute << "object-identifier" << value << TheShip->GetCurrentSystem()->GetObjectIdentifier() << end;
	XMLStream << element << "fuel" << attribute << "value" << value << TheShip->GetFuel() << end;
	XMLStream << element << "fuel-capacity" << attribute << value << TheShip->GetFuelCapacity() << end;
	XMLStream << element << "hull" << attribute << "value" << value << TheShip->GetHull() << end;
	XMLStream << element << "velocity" << attribute << "x" << value << TheShip->GetVelocity().m_V.m_A[0] << attribute << "y" << value << TheShip->GetVelocity().m_V.m_A[1] << attribute << "z" << value << TheShip->GetVelocity().m_V.m_A[2] << end;
}

static void WriteWeaponToXMLStream(XMLStream & XMLStream, Weapon * TheWeapon)
{
	assert(TheWeapon != 0);
}
