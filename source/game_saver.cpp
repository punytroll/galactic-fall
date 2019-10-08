/**
 * galactic-fall
 * Copyright (C) 2018-2019  Hagen Möbius
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

#include <cassert>

#include <string_cast/string_cast.h>

#include "ammunition.h"
#include "battery.h"
#include "character.h"
#include "command_mind.h"
#include "commodity.h"
#include "energy_projectile_weapon.h"
#include "faction.h"
#include "galaxy.h"
#include "generator.h"
#include "game_saver.h"
#include "game_time.h"
#include "graphics/color_rgbo.h"
#include "graphics/style.h"
#include "hangar.h"
#include "map_knowledge.h"
#include "object.h"
#include "object_aspect_accessory.h"
#include "object_aspect_name.h"
#include "object_aspect_object_container.h"
#include "object_aspect_physical.h"
#include "object_aspect_position.h"
#include "object_aspect_visualization.h"
#include "planet.h"
#include "ship.h"
#include "slot.h"
#include "storage.h"
#include "system.h"
#include "turret.h"
#include "visualization_prototype.h"

GameSaver::GameSaver(std::ostream & OStream) :
	_OStream(OStream),
	_XMLStream(OStream)
{
}

void GameSaver::Save(Galaxy * Galaxy, System * CurrentSystem, float TimeWarp, const Vector3f & CameraOffset, float FieldOfViewY, CommandMind * CommandMind, Character * ObservedCharacter)
{
	assert(Galaxy != nullptr);
	_XMLStream << element << "save" << attribute << "version" << value << "0.1";
	_XMLStream << element << "galaxy" << attribute << "identifier" << value << Galaxy->GetSubTypeIdentifier() << end;
	_XMLStream << element << "game-time" << attribute << "value" << value << to_string_cast(GameTime::Get(), 4) << end;
	if(CurrentSystem != nullptr)
	{
		_XMLStream << element << "current-system" << attribute << "identifier" << value << CurrentSystem->GetSubTypeIdentifier() << end;
	}
	_XMLStream << element << "time-warp" << attribute << "value" << value << TimeWarp << end;
	// save main camera properties
	_XMLStream << element << "main-camera";
	_XMLStream << element << "offset" << attribute << "x" << value << CameraOffset[0] << attribute << "y" << value << CameraOffset[1] << attribute << "z" << value << CameraOffset[2] << end;
	_XMLStream << element << "field-of-view-y" << attribute << "radians" << value << FieldOfViewY << end;
	_XMLStream << end; // camera
	if(ObservedCharacter != nullptr)
	{
		_XMLStream << element << "observed-character" << attribute << "object-identifier" << value << _GetOrRegisterObjectIdentifier(ObservedCharacter) << end;
	}
	// now save the important objects
	if(CommandMind != nullptr)
	{
		_XMLStream << element << "command-mind" << attribute << "object-identifier" << value << _GetOrRegisterObjectIdentifier(CommandMind) << end;
		// if no character is available
		if(CommandMind->GetCharacter() == nullptr)
		{
			// only save the input mind
			_WriteObjectRecursive(CommandMind);
		}
		else
		{
			assert(CommandMind->GetCharacter()->GetMapKnowledge() != nullptr);
			for(auto ExploredSystem : CommandMind->GetCharacter()->GetMapKnowledge()->GetExploredSystems())
			{
				_XMLStream << element << "object" << attribute << "type-identifier" << value << ExploredSystem->GetTypeIdentifier() << attribute << "sub-type-identifier" << value << ExploredSystem->GetSubTypeIdentifier() << attribute << "object-identifier" << value << _GetOrRegisterObjectIdentifier(ExploredSystem) << end;
			}
			// if no ship is available
			if(CommandMind->GetCharacter()->GetShip() == nullptr)
			{
				// only save the character
				_WriteObjectRecursive(CommandMind->GetCharacter());
			}
			else
			{
				// save the complete ship
				_WriteObjectRecursive(CommandMind->GetCharacter()->GetShip());
			}
			// save hangars
			for(auto System : Galaxy->GetSystems())
			{
				for(auto Planet : System.second->GetPlanets())
				{
					auto Hangar(Planet->GetHangar(CommandMind->GetCharacter()));
					
					if(Hangar != nullptr)
					{
						assert(Planet->GetAspectObjectContainer() != nullptr);
						_XMLStream << element << "object" << attribute << "type-identifier" << value << Planet->GetTypeIdentifier() << attribute << "sub-type-identifier" << value << Planet->GetSubTypeIdentifier() << attribute << "object-identifier" << value << _GetOrRegisterObjectIdentifier(Planet);
						_XMLStream << element << "aspect-object-container";
						_XMLStream << element << "content" << attribute << "object-identifier" << value << _GetOrRegisterObjectIdentifier(Hangar) << end;
						_XMLStream << end;
						_XMLStream << end;
						_WriteObjectRecursive(Hangar);
					}
				}
			}
		}
	}
	_XMLStream << end; // save
	_OStream << std::endl;
}

std::string GameSaver::_GetOrRegisterObjectIdentifier(Object * Object)
{
	auto Iterator{_IdentifiersByObject.find(Object)};
	
	if(Iterator == _IdentifiersByObject.end())
	{
		auto NewIdentifier{to_string_cast(_IdentifiersByObject.size())};
		
		_IdentifiersByObject.emplace(Object, NewIdentifier);
		
		return NewIdentifier;
	}
	else
	{
		return Iterator->second;
	}
}

void GameSaver::_WriteAmmunition(Ammunition * Ammunition)
{
	assert(Ammunition != nullptr);
	_XMLStream << element << "amount" << attribute << "value" << value << Ammunition->GetAmount() << end;
}

void GameSaver::_WriteBattery(Battery * Battery)
{
	assert(Battery != nullptr);
	_XMLStream << element << "energy" << attribute << "value" << value << Battery->GetEnergy() << end;
	_XMLStream << element << "energy-capacity" << attribute << "value" << value << Battery->GetEnergyCapacity() << end;
	_XMLStream << element << "maximum-power-input" << attribute << "value" << value << Battery->GetMaximumPowerInput() << end;
	_XMLStream << element << "maximum-power-output" << attribute << "value" << value << Battery->GetMaximumPowerOutput() << end;
}

void GameSaver::_WriteCharacter(Character * Character)
{
	assert(Character != nullptr);
	_XMLStream << element << "credits" << attribute << "value" << value << Character->GetCredits() << end;
	_XMLStream << element << "map-knowledge";
	for(auto ExploredSystem : Character->GetMapKnowledge()->GetExploredSystems())
	{
		_XMLStream << element << "explored-system" << attribute << "object-identifier" << value << _GetOrRegisterObjectIdentifier(ExploredSystem) << end;
	}
	_XMLStream << end;
}

void GameSaver::_WriteCommodity(Commodity * Commodity)
{
	assert(Commodity != nullptr);
	_XMLStream << element << "angular-velocity" << attribute << "axis-x" << value << Commodity->GetAngularVelocity()[0] << attribute << "axis-y" << value << Commodity->GetAngularVelocity()[1] << attribute << "axis-z" << value << Commodity->GetAngularVelocity()[2] << attribute << "angle" << value << Commodity->GetAngularVelocity()[3] << end;
	_XMLStream << element << "hull" << attribute << "value" << value << Commodity->GetHull() << end;
	_XMLStream << element << "velocity" << attribute << "x" << value << Commodity->GetVelocity()[0] << attribute << "y" << value << Commodity->GetVelocity()[1] << attribute << "z" << value << Commodity->GetVelocity()[2] << end;
}

void GameSaver::_WriteGenerator(Generator * Generator)
{
	assert(Generator != nullptr);
	_XMLStream << element << "energy" << attribute << "value" << value << Generator->GetEnergy() << end;
	_XMLStream << element << "energy-capacity" << attribute << "value" << value << Generator->GetEnergyCapacity() << end;
	_XMLStream << element << "maximum-power-input" << attribute << "value" << value << Generator->GetMaximumPowerInput() << end;
	_XMLStream << element << "maximum-power-output" << attribute << "value" << value << Generator->GetMaximumPowerOutput() << end;
}

void GameSaver::_WriteHangar(Hangar * Hangar)
{
	assert(Hangar != nullptr);
	_XMLStream << element << "character" << attribute << "object-identifier" << value << _GetOrRegisterObjectIdentifier(Hangar->GetCharacter()) << end;
}

void GameSaver::_WriteMind(Mind * Mind)
{
	assert(Mind != nullptr);
	_XMLStream << element << "character" << attribute << "object-identifier" << value << _GetOrRegisterObjectIdentifier(Mind->GetCharacter()) << end;
}

void GameSaver::_WriteObject(Object * WriteObject)
{
	assert(WriteObject != nullptr);
	_XMLStream << element << "object" << attribute << "type-identifier" << value << WriteObject->GetTypeIdentifier() << attribute << "sub-type-identifier" << value << WriteObject->GetSubTypeIdentifier() << attribute << "object-identifier" << value << _GetOrRegisterObjectIdentifier(WriteObject);
	if(WriteObject->GetAspectAccessory() != nullptr)
	{
		_XMLStream << element << "aspect-accessory";
		if(WriteObject->GetAspectAccessory()->GetSlot() != nullptr)
		{
			_XMLStream << element << "slot" << attribute << "slot-identifier" << value << WriteObject->GetAspectAccessory()->GetSlot()->GetIdentifier() << end;
		}
		_XMLStream << end;
	}
	if(WriteObject->GetAspectName() != nullptr)
	{
		_XMLStream << element << "aspect-name";
		_XMLStream << element << "name" << attribute << "value" << value << WriteObject->GetAspectName()->GetName() << end;
		_XMLStream << end;
	}
	if(WriteObject->GetAspectObjectContainer() != nullptr)
	{
		_XMLStream << element << "aspect-object-container";
		for(auto Content : WriteObject->GetAspectObjectContainer()->GetContent())
		{
			_XMLStream << element << "content" << attribute << "object-identifier" << value << _GetOrRegisterObjectIdentifier(Content) << end;
		}
		_XMLStream << end;
	}
	if(WriteObject->GetAspectPhysical() != nullptr)
	{
		_XMLStream << element << "aspect-physical";
		_XMLStream << element << "radial-size" << attribute << "value" << value << WriteObject->GetAspectPhysical()->GetRadialSize() << end;
		_XMLStream << element << "space-requirement" << attribute << "value" << value << WriteObject->GetAspectPhysical()->GetSpaceRequirement() << end;
		_XMLStream << end;
	}
	if(WriteObject->GetAspectPosition() != nullptr)
	{
		_XMLStream << element << "aspect-position";
		_XMLStream << element << "orientation" << attribute << "w" << value << WriteObject->GetAspectPosition()->GetOrientation()[0] << attribute << "x" << value << WriteObject->GetAspectPosition()->GetOrientation()[1] << attribute << "y" << value << WriteObject->GetAspectPosition()->GetOrientation()[2] << attribute << "z" << value << WriteObject->GetAspectPosition()->GetOrientation()[3] << end;
		_XMLStream << element << "position" << attribute << "x" << value << WriteObject->GetAspectPosition()->GetPosition()[0] << attribute << "y" << value << WriteObject->GetAspectPosition()->GetPosition()[1] << attribute << "z" << value << WriteObject->GetAspectPosition()->GetPosition()[2] << end;
		_XMLStream << end;
	}
	if(WriteObject->GetAspectVisualization() != nullptr)
	{
		_XMLStream << element << "aspect-visualization";
		for(auto PartStyle : WriteObject->GetAspectVisualization()->GetVisualizationPrototype()->GetPartStyles())
		{
			_XMLStream << element << "part" << attribute << "identifier" << value << PartStyle.first;
			if(PartStyle.second->HasDiffuseColor() == true)
			{
				_XMLStream << element << "style-diffuse-color" << attribute << "red" << value << PartStyle.second->GetDiffuseColor().GetRed() << attribute << "green" << value << PartStyle.second->GetDiffuseColor().GetGreen() << attribute << "blue" << value << PartStyle.second->GetDiffuseColor().GetBlue() << attribute << "opacity" << value << PartStyle.second->GetDiffuseColor().GetOpacity() << end;
			}
			if(PartStyle.second->HasSpecularColor() == true)
			{
				_XMLStream << element << "style-specular-color" << attribute << "red" << value << PartStyle.second->GetSpecularColor().GetRed() << attribute << "green" << value << PartStyle.second->GetSpecularColor().GetGreen() << attribute << "blue" << value << PartStyle.second->GetSpecularColor().GetBlue() << attribute << "opacity" << value << PartStyle.second->GetSpecularColor().GetOpacity() << end;
			}
			if(PartStyle.second->HasShininess() == true)
			{
				_XMLStream << element << "style-shininess" << attribute << "value" << value << PartStyle.second->GetShininess() << end;
			}
			_XMLStream << element << "style-program-identifier" << attribute << "value" << value << PartStyle.second->GetProgramIdentifier() << end;
			_XMLStream << end;
		}
		_XMLStream << end;
	}
	_XMLStream << element << "type-specific";
	if(WriteObject->GetTypeIdentifier() == "ammunition")
	{
		_WriteAmmunition(dynamic_cast< Ammunition * >(WriteObject));
	}
	else if(WriteObject->GetTypeIdentifier() == "battery")
	{
		_WriteBattery(dynamic_cast< Battery * >(WriteObject));
	}
	else if(WriteObject->GetTypeIdentifier() == "character")
	{
		_WriteCharacter(dynamic_cast< Character * >(WriteObject));
	}
	else if(WriteObject->GetTypeIdentifier() == "commodity")
	{
		_WriteCommodity(dynamic_cast< Commodity * >(WriteObject));
	}
	else if(WriteObject->GetTypeIdentifier() == "energy-projectile-weapon")
	{
		_WriteEnergyProjectileWeapon(dynamic_cast< EnergyProjectileWeapon * >(WriteObject));
	}
	else if(WriteObject->GetTypeIdentifier() == "generator")
	{
		_WriteGenerator(dynamic_cast< Generator * >(WriteObject));
	}
	else if(WriteObject->GetTypeIdentifier() == "hangar")
	{
		_WriteHangar(dynamic_cast< Hangar * >(WriteObject));
	}
	else if(WriteObject->GetTypeIdentifier() == "mind")
	{
		_WriteMind(dynamic_cast< Mind * >(WriteObject));
	}
	else if(WriteObject->GetTypeIdentifier() == "ship")
	{
		_WriteShip(dynamic_cast< Ship * >(WriteObject));
	}
	else if(WriteObject->GetTypeIdentifier() == "storage")
	{
		_WriteStorage(dynamic_cast< Storage * >(WriteObject));
	}
	else if(WriteObject->GetTypeIdentifier() == "turret")
	{
		_WriteTurret(dynamic_cast< Turret * >(WriteObject));
	}
	else
	{
		assert(false);
	}
	_XMLStream << end;
	_XMLStream << end;
}

void GameSaver::_WriteObjectRecursive(Object * WriteObject)
{
	assert(WriteObject != nullptr);
	
	std::stack< Object * > ObjectStack;
	
	ObjectStack.push(WriteObject);
	while(ObjectStack.empty() == false)
	{
		auto TopObject{ObjectStack.top()};
		
		ObjectStack.pop();
		if(TopObject->GetAspectObjectContainer() != nullptr)
		{
			for(auto Content : TopObject->GetAspectObjectContainer()->GetContent())
			{
				ObjectStack.push(Content);
			}
		}
		_WriteObject(TopObject);
	}
}

void GameSaver::_WriteShip(Ship * Ship)
{
	assert(Ship != nullptr);
	// save maximum & capacity values
	_XMLStream << element << "fuel-capacity" << attribute << "value" << value << Ship->GetFuelCapacity() << end;
	_XMLStream << element << "fuel-needed-to-jump" << attribute << "value" << value << Ship->GetFuelNeededToJump() << end;
	_XMLStream << element << "hull-capacity" << attribute << "value" << value << Ship->GetHullCapacity() << end;
	_XMLStream << element << "maximum-forward-thrust" << attribute << "value" << value << Ship->GetMaximumForwardThrust() << end;
	_XMLStream << element << "maximum-speed" << attribute << "value" << value << Ship->GetMaximumSpeed() << end;
	_XMLStream << element << "maximum-turn-speed" << attribute << "value" << value << Ship->GetMaximumTurnSpeed() << end;
	// save current values
	assert(Ship->GetFaction() != nullptr);
	_XMLStream << element << "faction" << attribute << "identifier" << value << Ship->GetFaction()->GetSubTypeIdentifier() << end;
	_XMLStream << element << "fuel" << attribute << "value" << value << Ship->GetFuel() << end;
	_XMLStream << element << "hull" << attribute << "value" << value << Ship->GetHull() << end;
	_XMLStream << element << "velocity" << attribute << "x" << value << Ship->GetVelocity()[0] << attribute << "y" << value << Ship->GetVelocity()[1] << attribute << "z" << value << Ship->GetVelocity()[2] << end;
}

void GameSaver::_WriteStorage(Storage * Storage)
{
	assert(Storage != nullptr);
	_XMLStream << element << "space-capacity" << attribute << "value" << value << Storage->GetSpaceCapacity() << end;
}

void GameSaver::_WriteTurret(Turret * Turret)
{
	assert(Turret != nullptr);
	_XMLStream << element << "energy" << attribute << "value" << value << Turret->GetEnergy() << end;
	_XMLStream << element << "energy-capacity" << attribute << "value" << value << Turret->GetEnergyCapacity() << end;
	_XMLStream << element << "maximum-power-input" << attribute << "value" << value << Turret->GetMaximumPowerInput() << end;
	_XMLStream << element << "maximum-power-output" << attribute << "value" << value << Turret->GetMaximumPowerOutput() << end;
}

void GameSaver::_WriteEnergyProjectileWeapon(EnergyProjectileWeapon * EnergyProjectileWeapon)
{
	assert(EnergyProjectileWeapon != nullptr);
	_XMLStream << element << "energy" << attribute << "value" << value << EnergyProjectileWeapon->GetEnergy() << end;
	_XMLStream << element << "energy-capacity" << attribute << "value" << value << EnergyProjectileWeapon->GetEnergyCapacity() << end;
	_XMLStream << element << "maximum-power-input" << attribute << "value" << value << EnergyProjectileWeapon->GetMaximumPowerInput() << end;
	_XMLStream << element << "maximum-power-output" << attribute << "value" << value << EnergyProjectileWeapon->GetMaximumPowerOutput() << end;
}
