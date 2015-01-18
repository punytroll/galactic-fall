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

#include "battery.h"
#include "character.h"
#include "color.h"
#include "commodity.h"
#include "faction.h"
#include "generator.h"
#include "graphics/style.h"
#include "hangar.h"
#include "map_knowledge.h"
#include "mind.h"
#include "object.h"
#include "object_aspect_accessory.h"
#include "object_aspect_name.h"
#include "object_aspect_object_container.h"
#include "object_aspect_physical.h"
#include "object_aspect_position.h"
#include "object_aspect_visualization.h"
#include "ship.h"
#include "slot.h"
#include "storage.h"
#include "system.h"
#include "visualization_prototype.h"
#include "weapon.h"
#include "write_to_xml_stream.h"
#include "xml_stream.h"

static void WriteToXMLStream(XMLStream & XMLStream, Object * TheObject);
static void WriteToXMLStream(XMLStream & XMLStream, Object * TheObject, std::stack< Object * > & ObjectStack);

static void WriteBatteryToXMLStream(XMLStream & XMLStream, Battery * TheBattery);
static void WriteCharacterToXMLStream(XMLStream & XMLStream, Character * TheCharacter);
static void WriteCommodityToXMLStream(XMLStream & XMLStream, Commodity * TheCommodity);
static void WriteGeneratorToXMLStream(XMLStream & XMLStream, Generator * TheGenerator);
static void WriteHangarToXMLStream(XMLStream & XMLStream, Hangar * Hangar);
static void WriteMindToXMLStream(XMLStream & XMLStream, Mind * TheMind);
static void WriteShipToXMLStream(XMLStream & XMLStream, Ship * TheShip);
static void WriteStorageToXMLStream(XMLStream & XMLStream, Storage * TheStorage);
static void WriteWeaponToXMLStream(XMLStream & XMLStream, Weapon * TheWeapon);

void WriteToXMLStream(XMLStream & XMLStream, Object * TheObject, bool Recursive)
{
	assert(TheObject != nullptr);
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
	if(TheObject->GetAspectObjectContainer() != nullptr)
	{
		for(auto Content : TheObject->GetAspectObjectContainer()->GetContent())
		{
			ObjectStack.push(Content);
		}
	}
	WriteToXMLStream(XMLStream, TheObject);
}

void WriteToXMLStream(XMLStream & XMLStream, Object * TheObject)
{
	assert(TheObject != nullptr);
	assert(TheObject->GetObjectIdentifier().empty() == false);
	XMLStream << element << "object" << attribute << "type-identifier" << value << TheObject->GetTypeIdentifier() << attribute << "class-identifier" << value << TheObject->GetClassIdentifier() << attribute << "object-identifier" << value << TheObject->GetObjectIdentifier();
	if(TheObject->GetAspectAccessory() != nullptr)
	{
		XMLStream << element << "aspect-accessory";
		if(TheObject->GetAspectAccessory()->GetSlot() != nullptr)
		{
			XMLStream << element << "slot" << attribute << "slot-identifier" << value << TheObject->GetAspectAccessory()->GetSlot()->GetIdentifier() << end;
		}
		XMLStream << end;
	}
	if(TheObject->GetAspectName() != nullptr)
	{
		XMLStream << element << "aspect-name";
		XMLStream << element << "name" << attribute << "value" << value << TheObject->GetAspectName()->GetName() << end;
		XMLStream << end;
	}
	if(TheObject->GetAspectObjectContainer() != nullptr)
	{
		XMLStream << element << "aspect-object-container";
		for(auto Content : TheObject->GetAspectObjectContainer()->GetContent())
		{
			if(Content->GetObjectIdentifier().empty() == true)
			{
				Content->GenerateObjectIdentifier();
			}
			XMLStream << element << "content" << attribute << "object-identifier" << value << Content->GetObjectIdentifier() << end;
		}
		XMLStream << end;
	}
	if(TheObject->GetAspectPhysical() != nullptr)
	{
		XMLStream << element << "aspect-physical";
		XMLStream << element << "radial-size" << attribute << "value" << value << TheObject->GetAspectPhysical()->GetRadialSize() << end;
		XMLStream << element << "space-requirement" << attribute << "value" << value << TheObject->GetAspectPhysical()->GetSpaceRequirement() << end;
		XMLStream << end;
	}
	if(TheObject->GetAspectPosition() != nullptr)
	{
		XMLStream << element << "aspect-position";
		XMLStream << element << "orientation" << attribute << "w" << value << TheObject->GetAspectPosition()->GetOrientation()[0] << attribute << "x" << value << TheObject->GetAspectPosition()->GetOrientation()[1] << attribute << "y" << value << TheObject->GetAspectPosition()->GetOrientation()[2] << attribute << "z" << value << TheObject->GetAspectPosition()->GetOrientation()[3] << end;
		XMLStream << element << "position" << attribute << "x" << value << TheObject->GetAspectPosition()->GetPosition()[0] << attribute << "y" << value << TheObject->GetAspectPosition()->GetPosition()[1] << attribute << "z" << value << TheObject->GetAspectPosition()->GetPosition()[2] << end;
		XMLStream << end;
	}
	if(TheObject->GetAspectVisualization() != nullptr)
	{
		XMLStream << element << "aspect-visualization";
		for(auto PartStyle : TheObject->GetAspectVisualization()->GetVisualizationPrototype()->GetPartStyles())
		{
			XMLStream << element << "part" << attribute << "identifier" << value << PartStyle.first;
			if(PartStyle.second->HasDiffuseColor() == true)
			{
				XMLStream << element << "style-diffuse-color" << attribute << "red" << value << PartStyle.second->GetDiffuseColor().GetColor()[0] << attribute << "green" << value << PartStyle.second->GetDiffuseColor().GetColor()[1] << attribute << "blue" << value << PartStyle.second->GetDiffuseColor().GetColor()[2] << attribute << "opacity" << value << PartStyle.second->GetDiffuseColor().GetColor()[3] << end;
			}
			if(PartStyle.second->HasSpecularColor() == true)
			{
				XMLStream << element << "style-specular-color" << attribute << "red" << value << PartStyle.second->GetSpecularColor().GetColor()[0] << attribute << "green" << value << PartStyle.second->GetSpecularColor().GetColor()[1] << attribute << "blue" << value << PartStyle.second->GetSpecularColor().GetColor()[2] << attribute << "opacity" << value << PartStyle.second->GetSpecularColor().GetColor()[3] << end;
			}
			if(PartStyle.second->HasShininess() == true)
			{
				XMLStream << element << "style-shininess" << attribute << "value" << value << PartStyle.second->GetShininess() << end;
			}
			XMLStream << element << "style-program-identifier" << attribute << "value" << value << PartStyle.second->GetProgramIdentifier() << end;
			XMLStream << end;
		}
		XMLStream << end;
	}
	XMLStream << element << "type-specific";
	if(TheObject->GetTypeIdentifier() == "battery")
	{
		WriteBatteryToXMLStream(XMLStream, dynamic_cast< Battery * >(TheObject));
	}
	else if(TheObject->GetTypeIdentifier() == "character")
	{
		WriteCharacterToXMLStream(XMLStream, dynamic_cast< Character * >(TheObject));
	}
	else if(TheObject->GetTypeIdentifier() == "commodity")
	{
		WriteCommodityToXMLStream(XMLStream, dynamic_cast< Commodity * >(TheObject));
	}
	else if(TheObject->GetTypeIdentifier() == "generator")
	{
		WriteGeneratorToXMLStream(XMLStream, dynamic_cast< Generator * >(TheObject));
	}
	else if(TheObject->GetTypeIdentifier() == "hangar")
	{
		WriteHangarToXMLStream(XMLStream, dynamic_cast< Hangar * >(TheObject));
	}
	else if(TheObject->GetTypeIdentifier() == "mind")
	{
		WriteMindToXMLStream(XMLStream, dynamic_cast< Mind * >(TheObject));
	}
	else if(TheObject->GetTypeIdentifier() == "ship")
	{
		WriteShipToXMLStream(XMLStream, dynamic_cast< Ship * >(TheObject));
	}
	else if(TheObject->GetTypeIdentifier() == "storage")
	{
		WriteStorageToXMLStream(XMLStream, dynamic_cast< Storage * >(TheObject));
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

static void WriteBatteryToXMLStream(XMLStream & XMLStream, Battery * TheBattery)
{
	assert(TheBattery != nullptr);
	XMLStream << element << "energy" << attribute << "value" << value << TheBattery->GetEnergy() << end;
	XMLStream << element << "energy-capacity" << attribute << "value" << value << TheBattery->GetEnergyCapacity() << end;
}

static void WriteCharacterToXMLStream(XMLStream & XMLStream, Character * TheCharacter)
{
	assert(TheCharacter != nullptr);
	XMLStream << element << "credits" << attribute << "value" << value << TheCharacter->GetCredits() << end;
	XMLStream << element << "map-knowledge";
	for(auto ExploredSystem : TheCharacter->GetMapKnowledge()->GetExploredSystems())
	{
		XMLStream << element << "explored-system" << attribute << "object-identifier" << value << ExploredSystem->GetObjectIdentifier() << end;
	}
	XMLStream << end;
}

static void WriteCommodityToXMLStream(XMLStream & XMLStream, Commodity * TheCommodity)
{
	assert(TheCommodity != nullptr);
	XMLStream << element << "angular-velocity" << attribute << "axis-x" << value << TheCommodity->GetAngularVelocity()[0] << attribute << "axis-y" << value << TheCommodity->GetAngularVelocity()[1] << attribute << "axis-z" << value << TheCommodity->GetAngularVelocity()[2] << attribute << "angle" << value << TheCommodity->GetAngularVelocity()[3] << end;
	XMLStream << element << "hull" << attribute << "value" << value << TheCommodity->GetHull() << end;
	XMLStream << element << "velocity" << attribute << "x" << value << TheCommodity->GetVelocity()[0] << attribute << "y" << value << TheCommodity->GetVelocity()[1] << attribute << "z" << value << TheCommodity->GetVelocity()[2] << end;
}

static void WriteGeneratorToXMLStream(XMLStream & XMLStream, Generator * TheGenerator)
{
	assert(TheGenerator != nullptr);
	XMLStream << element << "energy-provision-per-second" << attribute << "value" << value << TheGenerator->GetEnergyProvisionPerSecond() << end;
}

static void WriteHangarToXMLStream(XMLStream & XMLStream, Hangar * Hangar)
{
	assert(Hangar != nullptr);
	XMLStream << element << "character" << attribute << "object-identifier" << value << Hangar->GetCharacter()->GetObjectIdentifier() << end;
}

static void WriteMindToXMLStream(XMLStream & XMLStream, Mind * TheMind)
{
	assert(TheMind != nullptr);
	XMLStream << element << "character" << attribute << "object-identifier" << value << TheMind->GetCharacter()->GetObjectIdentifier() << end;
}

static void WriteShipToXMLStream(XMLStream & XMLStream, Ship * TheShip)
{
	assert(TheShip != nullptr);
	// save maximum & capacity values
	XMLStream << element << "exhaust-offset" << attribute << "x" << value << TheShip->GetExhaustOffset()[0] << attribute << "y" << value << TheShip->GetExhaustOffset()[1] << attribute << "z" << value << TheShip->GetExhaustOffset()[2] << end;
	XMLStream << element << "fuel-capacity" << attribute << "value" << value << TheShip->GetFuelCapacity() << end;
	XMLStream << element << "fuel-needed-to-jump" << attribute << "value" << value << TheShip->GetFuelNeededToJump() << end;
	XMLStream << element << "hull-capacity" << attribute << "value" << value << TheShip->GetHullCapacity() << end;
	XMLStream << element << "maximum-forward-thrust" << attribute << "value" << value << TheShip->GetMaximumForwardThrust() << end;
	XMLStream << element << "maximum-speed" << attribute << "value" << value << TheShip->GetMaximumSpeed() << end;
	XMLStream << element << "maximum-turn-speed" << attribute << "value" << value << TheShip->GetMaximumTurnSpeed() << end;
	// save current values
	assert(TheShip->GetFaction().IsValid() == true);
	XMLStream << element << "faction" << attribute << "identifier" << value << TheShip->GetFaction()->GetClassIdentifier() << end;
	XMLStream << element << "fuel" << attribute << "value" << value << TheShip->GetFuel() << end;
	XMLStream << element << "hull" << attribute << "value" << value << TheShip->GetHull() << end;
	XMLStream << element << "velocity" << attribute << "x" << value << TheShip->GetVelocity()[0] << attribute << "y" << value << TheShip->GetVelocity()[1] << attribute << "z" << value << TheShip->GetVelocity()[2] << end;
}

static void WriteStorageToXMLStream(XMLStream & XMLStream, Storage * TheStorage)
{
	assert(TheStorage != nullptr);
	XMLStream << element << "space-capacity" << attribute << "value" << value << TheStorage->GetSpaceCapacity() << end;
}

static void WriteWeaponToXMLStream(XMLStream & XMLStream, Weapon * TheWeapon)
{
	assert(TheWeapon != nullptr);
}
