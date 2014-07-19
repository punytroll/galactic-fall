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
#include "graphics/material.h"
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
		
		const std::set< Object * > & Content(TheObject->GetAspectObjectContainer()->GetContent());
		
		for(std::set< Object * >::const_iterator ContentIterator = Content.begin(); ContentIterator != Content.end(); ++ContentIterator)
		{
			XMLStream << element << "content" << attribute << "object-identifier" << value << (*ContentIterator)->GetObjectIdentifier() << end;
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
		
		const std::map< std::string, Graphics::Material * > & PartMaterials(TheObject->GetAspectVisualization()->GetVisualizationPrototype()->GetPartMaterials());
		
		for(std::map< std::string, Graphics::Material * >::const_iterator PartMaterialIterator = PartMaterials.begin(); PartMaterialIterator != PartMaterials.end(); ++PartMaterialIterator)
		{
			XMLStream << element << "part" << attribute << "identifier" << value << PartMaterialIterator->first;
			if(PartMaterialIterator->second->GetDiffuseColor() != nullptr)
			{
				XMLStream << element << "material-diffuse-color" << attribute << "red" << value << PartMaterialIterator->second->GetDiffuseColor()->GetColor()[0] << attribute << "green" << value << PartMaterialIterator->second->GetDiffuseColor()->GetColor()[1] << attribute << "blue" << value << PartMaterialIterator->second->GetDiffuseColor()->GetColor()[2] << attribute << "opacity" << value << PartMaterialIterator->second->GetDiffuseColor()->GetColor()[3] << end;
			}
			if(PartMaterialIterator->second->GetSpecularColor() != nullptr)
			{
				XMLStream << element << "material-specular-color" << attribute << "red" << value << PartMaterialIterator->second->GetSpecularColor()->GetColor()[0] << attribute << "green" << value << PartMaterialIterator->second->GetSpecularColor()->GetColor()[1] << attribute << "blue" << value << PartMaterialIterator->second->GetSpecularColor()->GetColor()[2] << attribute << "opacity" << value << PartMaterialIterator->second->GetSpecularColor()->GetColor()[3] << end;
			}
			XMLStream << element << "material-shininess" << attribute << "value" << value << PartMaterialIterator->second->GetShininess() << end;
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
	assert(TheBattery != 0);
	XMLStream << element << "energy" << attribute << "value" << value << TheBattery->GetEnergy() << end;
	XMLStream << element << "energy-capacity" << attribute << "value" << value << TheBattery->GetEnergyCapacity() << end;
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
}

static void WriteCommodityToXMLStream(XMLStream & XMLStream, Commodity * TheCommodity)
{
	assert(TheCommodity != 0);
	XMLStream << element << "angular-velocity" << attribute << "axis-x" << value << TheCommodity->GetAngularVelocity()[0] << attribute << "axis-y" << value << TheCommodity->GetAngularVelocity()[1] << attribute << "axis-z" << value << TheCommodity->GetAngularVelocity()[2] << attribute << "angle" << value << TheCommodity->GetAngularVelocity()[3] << end;
	XMLStream << element << "hull" << attribute << "value" << value << TheCommodity->GetHull() << end;
	XMLStream << element << "velocity" << attribute << "x" << value << TheCommodity->GetVelocity()[0] << attribute << "y" << value << TheCommodity->GetVelocity()[1] << attribute << "z" << value << TheCommodity->GetVelocity()[2] << end;
}

static void WriteGeneratorToXMLStream(XMLStream & XMLStream, Generator * TheGenerator)
{
	assert(TheGenerator != 0);
	XMLStream << element << "energy-provision-per-second" << attribute << "value" << value << TheGenerator->GetEnergyProvisionPerSecond() << end;
}

static void WriteHangarToXMLStream(XMLStream & XMLStream, Hangar * Hangar)
{
	assert(Hangar != nullptr);
	XMLStream << element << "character" << attribute << "object-identifier" << value << Hangar->GetCharacter()->GetObjectIdentifier() << end;
}

static void WriteMindToXMLStream(XMLStream & XMLStream, Mind * TheMind)
{
	assert(TheMind != 0);
	XMLStream << element << "character" << attribute << "object-identifier" << value << TheMind->GetCharacter()->GetObjectIdentifier() << end;
}

static void WriteShipToXMLStream(XMLStream & XMLStream, Ship * TheShip)
{
	assert(TheShip != 0);
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
	assert(TheStorage != 0);
	XMLStream << element << "space-capacity" << attribute << "value" << value << TheStorage->GetSpaceCapacity() << end;
}

static void WriteWeaponToXMLStream(XMLStream & XMLStream, Weapon * TheWeapon)
{
	assert(TheWeapon != 0);
}
