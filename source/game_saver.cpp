/**
 * galactic-fall
 * Copyright (C) 2018-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#include <cassert>

#include <string_cast/string_cast.h>

#include "ammunition.h"
#include "battery.h"
#include "character.h"
#include "command_mind.h"
#include "commodity.h"
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
#include "projectile_weapon.h"
#include "ship.h"
#include "slot.h"
#include "storage.h"
#include "system.h"
#include "turret.h"
#include "visualization_prototype.h"

GameSaver::GameSaver(std::ostream & OStream) :
    m_OStream{OStream},
    m_XMLStream{OStream}
{
}

auto GameSaver::Save(Galaxy * Galaxy, System * CurrentSystem, float TimeWarp, Vector3f const & CameraOffset, float FieldOfViewY, CommandMind * CommandMind, Character * ObservedCharacter) -> void
{
    assert(Galaxy != nullptr);
    m_XMLStream << element << "save" << attribute << "version" << value << "0.1";
    m_XMLStream << element << "galaxy" << attribute << "identifier" << value << Galaxy->GetSubTypeIdentifier() << end;
    m_XMLStream << element << "game-time" << attribute << "value" << value << to_string_cast(GameTime::Get(), 4) << end;
    if(CurrentSystem != nullptr)
    {
        m_XMLStream << element << "current-system" << attribute << "object-identifier" << value << m_GetOrRegisterObjectIdentifier(CurrentSystem) << end;
    }
    m_XMLStream << element << "time-warp" << attribute << "value" << value << TimeWarp << end;
    // save main camera properties
    m_XMLStream << element << "main-camera";
    m_XMLStream << element << "offset" << attribute << "x" << value << CameraOffset[0] << attribute << "y" << value << CameraOffset[1] << attribute << "z" << value << CameraOffset[2] << end;
    m_XMLStream << element << "field-of-view-y" << attribute << "radians" << value << FieldOfViewY << end;
    m_XMLStream << end; // camera
    if(ObservedCharacter != nullptr)
    {
        m_XMLStream << element << "observed-character" << attribute << "object-identifier" << value << m_GetOrRegisterObjectIdentifier(ObservedCharacter) << end;
    }
    // now save the important objects
    if(CommandMind != nullptr)
    {
        m_XMLStream << element << "command-mind" << attribute << "object-identifier" << value << m_GetOrRegisterObjectIdentifier(CommandMind) << end;
        // if no character is available
        if(CommandMind->GetCharacter() == nullptr)
        {
            // only save the input mind
            m_WriteObjectRecursive(CommandMind);
        }
        else
        {
            assert(CommandMind->GetCharacter()->GetMapKnowledge() != nullptr);
            for(auto ExploredSystem : CommandMind->GetCharacter()->GetMapKnowledge()->GetExploredSystems())
            {
                m_XMLStream << element << "object" << attribute << "type-identifier" << value << ExploredSystem->GetTypeIdentifier() << attribute << "sub-type-identifier" << value << ExploredSystem->GetSubTypeIdentifier() << attribute << "object-identifier" << value << m_GetOrRegisterObjectIdentifier(ExploredSystem);
                if((ExploredSystem == CurrentSystem) && (CommandMind->GetCharacter()->GetShip() != nullptr))
                {
                    m_XMLStream << element << "aspect-object-container";
                    m_XMLStream << element << "content" << attribute << "object-identifier" << value << m_GetOrRegisterObjectIdentifier(CommandMind->GetCharacter()->GetShip()) << end;
                    m_XMLStream << end;
                }
                m_XMLStream << end;
            }
            // if ship is available
            if(auto Ship = CommandMind->GetCharacter()->GetShip(); Ship != nullptr)
            {
                // save the complete ship
                assert(Ship->GetFaction() != nullptr);
                m_XMLStream << element << "object" << attribute << "type-identifier" << value << Ship->GetFaction()->GetTypeIdentifier() << attribute << "sub-type-identifier" << value << Ship->GetFaction()->GetSubTypeIdentifier() << attribute << "object-identifier" << value << m_GetOrRegisterObjectIdentifier(Ship->GetFaction()) << end;
                m_WriteObjectRecursive(CommandMind->GetCharacter()->GetShip());
            }
            else
            {
                // only save the character
                m_WriteObjectRecursive(CommandMind->GetCharacter());
            }
            // save hangars
            for(auto const & [SystemIdentifier, System] : Galaxy->GetSystems())
            {
                for(auto Planet : System->GetPlanets())
                {
                    auto Hangar = Planet->GetHangar(CommandMind->GetCharacter());
                    
                    if(Hangar != nullptr)
                    {
                        assert(Planet->GetAspectObjectContainer() != nullptr);
                        m_XMLStream << element << "object" << attribute << "type-identifier" << value << Planet->GetTypeIdentifier() << attribute << "sub-type-identifier" << value << Planet->GetSubTypeIdentifier() << attribute << "object-identifier" << value << m_GetOrRegisterObjectIdentifier(Planet);
                        m_XMLStream << element << "aspect-object-container";
                        m_XMLStream << element << "content" << attribute << "object-identifier" << value << m_GetOrRegisterObjectIdentifier(Hangar) << end;
                        m_XMLStream << end;
                        m_XMLStream << end;
                        m_WriteObjectRecursive(Hangar);
                    }
                }
            }
        }
    }
    m_XMLStream << end; // save
    m_OStream << std::endl;
}

auto GameSaver::m_GetOrRegisterObjectIdentifier(Object * Object) -> std::string const &
{
    auto Iterator = m_IdentifiersByObject.find(Object);
    
    if(Iterator == m_IdentifiersByObject.end())
    {
        auto NewIdentifier = std::to_string(m_IdentifiersByObject.size());
        auto Success = false;
        
        std::tie(Iterator, Success) = m_IdentifiersByObject.try_emplace(Object, NewIdentifier);
    }
    
    return Iterator->second;
}

auto GameSaver::m_WriteAmmunition(Ammunition * Ammunition) -> void
{
    assert(Ammunition != nullptr);
    m_XMLStream << element << "amount" << attribute << "value" << value << Ammunition->GetAmount() << end;
}

auto GameSaver::m_WriteBattery(Battery * Battery) -> void
{
    assert(Battery != nullptr);
    m_XMLStream << element << "energy" << attribute << "value" << value << Battery->GetEnergy() << end;
    m_XMLStream << element << "energy-capacity" << attribute << "value" << value << Battery->GetEnergyCapacity() << end;
    m_XMLStream << element << "maximum-power-input" << attribute << "value" << value << Battery->GetMaximumPowerInput() << end;
    m_XMLStream << element << "maximum-power-output" << attribute << "value" << value << Battery->GetMaximumPowerOutput() << end;
}

auto GameSaver::m_WriteCharacter(Character * Character) -> void
{
    assert(Character != nullptr);
    m_XMLStream << element << "credits" << attribute << "value" << value << Character->GetCredits() << end;
    m_XMLStream << element << "map-knowledge";
    for(auto ExploredSystem : Character->GetMapKnowledge()->GetExploredSystems())
    {
        m_XMLStream << element << "explored-system" << attribute << "object-identifier" << value << m_GetOrRegisterObjectIdentifier(ExploredSystem) << end;
    }
    m_XMLStream << end;
}

auto GameSaver::m_WriteCommodity(Commodity * Commodity) -> void
{
    assert(Commodity != nullptr);
    m_XMLStream << element << "angular-velocity" << attribute << "axis-x" << value << Commodity->GetAngularVelocity()[0] << attribute << "axis-y" << value << Commodity->GetAngularVelocity()[1] << attribute << "axis-z" << value << Commodity->GetAngularVelocity()[2] << attribute << "angle" << value << Commodity->GetAngularVelocity()[3] << end;
    m_XMLStream << element << "hull" << attribute << "value" << value << Commodity->GetHull() << end;
    m_XMLStream << element << "velocity" << attribute << "x" << value << Commodity->GetVelocity()[0] << attribute << "y" << value << Commodity->GetVelocity()[1] << attribute << "z" << value << Commodity->GetVelocity()[2] << end;
}

auto GameSaver::m_WriteGenerator(Generator * Generator) -> void
{
    assert(Generator != nullptr);
    m_XMLStream << element << "energy" << attribute << "value" << value << Generator->GetEnergy() << end;
    m_XMLStream << element << "energy-capacity" << attribute << "value" << value << Generator->GetEnergyCapacity() << end;
    m_XMLStream << element << "maximum-power-input" << attribute << "value" << value << Generator->GetMaximumPowerInput() << end;
    m_XMLStream << element << "maximum-power-output" << attribute << "value" << value << Generator->GetMaximumPowerOutput() << end;
}

auto GameSaver::m_WriteHangar(Hangar * Hangar) -> void
{
    assert(Hangar != nullptr);
    m_XMLStream << element << "character" << attribute << "object-identifier" << value << m_GetOrRegisterObjectIdentifier(Hangar->GetCharacter()) << end;
}

auto GameSaver::m_WriteMind(Mind * Mind) -> void
{
    assert(Mind != nullptr);
    m_XMLStream << element << "character" << attribute << "object-identifier" << value << m_GetOrRegisterObjectIdentifier(Mind->GetCharacter()) << end;
}

auto GameSaver::m_WriteObject(Object * WriteObject) -> void
{
    assert(WriteObject != nullptr);
    m_XMLStream << element << "object" << attribute << "type-identifier" << value << WriteObject->GetTypeIdentifier() << attribute << "sub-type-identifier" << value << WriteObject->GetSubTypeIdentifier() << attribute << "object-identifier" << value << m_GetOrRegisterObjectIdentifier(WriteObject);
    if(WriteObject->GetAspectAccessory() != nullptr)
    {
        m_XMLStream << element << "aspect-accessory";
        if(WriteObject->GetAspectAccessory()->GetSlot() != nullptr)
        {
            m_XMLStream << element << "slot" << attribute << "slot-identifier" << value << WriteObject->GetAspectAccessory()->GetSlot()->GetIdentifier() << end;
        }
        m_XMLStream << end;
    }
    if(WriteObject->GetAspectName() != nullptr)
    {
        m_XMLStream << element << "aspect-name";
        m_XMLStream << element << "name" << attribute << "value" << value << WriteObject->GetAspectName()->GetName() << end;
        m_XMLStream << end;
    }
    if(WriteObject->GetAspectObjectContainer() != nullptr)
    {
        m_XMLStream << element << "aspect-object-container";
        for(auto Content : WriteObject->GetAspectObjectContainer()->GetContent())
        {
            m_XMLStream << element << "content" << attribute << "object-identifier" << value << m_GetOrRegisterObjectIdentifier(Content) << end;
        }
        m_XMLStream << end;
    }
    if(WriteObject->GetAspectPhysical() != nullptr)
    {
        m_XMLStream << element << "aspect-physical";
        m_XMLStream << element << "radial-size" << attribute << "value" << value << WriteObject->GetAspectPhysical()->GetRadialSize() << end;
        m_XMLStream << element << "space-requirement" << attribute << "value" << value << WriteObject->GetAspectPhysical()->GetSpaceRequirement() << end;
        m_XMLStream << end;
    }
    if(WriteObject->GetAspectPosition() != nullptr)
    {
        m_XMLStream << element << "aspect-position";
        m_XMLStream << element << "orientation" << attribute << "w" << value << WriteObject->GetAspectPosition()->GetOrientation()[0] << attribute << "x" << value << WriteObject->GetAspectPosition()->GetOrientation()[1] << attribute << "y" << value << WriteObject->GetAspectPosition()->GetOrientation()[2] << attribute << "z" << value << WriteObject->GetAspectPosition()->GetOrientation()[3] << end;
        m_XMLStream << element << "position" << attribute << "x" << value << WriteObject->GetAspectPosition()->GetPosition()[0] << attribute << "y" << value << WriteObject->GetAspectPosition()->GetPosition()[1] << attribute << "z" << value << WriteObject->GetAspectPosition()->GetPosition()[2] << end;
        m_XMLStream << end;
    }
    if(WriteObject->GetAspectVisualization() != nullptr)
    {
        m_XMLStream << element << "aspect-visualization";
        for(auto PartStyle : WriteObject->GetAspectVisualization()->GetVisualizationPrototype()->GetPartStyles())
        {
            m_XMLStream << element << "part" << attribute << "identifier" << value << PartStyle.first;
            if(PartStyle.second->HasDiffuseColor() == true)
            {
                m_XMLStream << element << "style-diffuse-color" << attribute << "red" << value << PartStyle.second->GetDiffuseColor().GetRed() << attribute << "green" << value << PartStyle.second->GetDiffuseColor().GetGreen() << attribute << "blue" << value << PartStyle.second->GetDiffuseColor().GetBlue() << attribute << "opacity" << value << PartStyle.second->GetDiffuseColor().GetOpacity() << end;
            }
            if(PartStyle.second->HasSpecularColor() == true)
            {
                m_XMLStream << element << "style-specular-color" << attribute << "red" << value << PartStyle.second->GetSpecularColor().GetRed() << attribute << "green" << value << PartStyle.second->GetSpecularColor().GetGreen() << attribute << "blue" << value << PartStyle.second->GetSpecularColor().GetBlue() << attribute << "opacity" << value << PartStyle.second->GetSpecularColor().GetOpacity() << end;
            }
            if(PartStyle.second->HasShininess() == true)
            {
                m_XMLStream << element << "style-shininess" << attribute << "value" << value << PartStyle.second->GetShininess() << end;
            }
            m_XMLStream << element << "style-program-identifier" << attribute << "value" << value << PartStyle.second->GetProgramIdentifier() << end;
            m_XMLStream << end;
        }
        m_XMLStream << end;
    }
    m_XMLStream << element << "type-specific";
    if(WriteObject->GetTypeIdentifier() == "ammunition")
    {
        m_WriteAmmunition(dynamic_cast<Ammunition *>(WriteObject));
    }
    else if(WriteObject->GetTypeIdentifier() == "battery")
    {
        m_WriteBattery(dynamic_cast<Battery *>(WriteObject));
    }
    else if(WriteObject->GetTypeIdentifier() == "character")
    {
        m_WriteCharacter(dynamic_cast<Character *>(WriteObject));
    }
    else if(WriteObject->GetTypeIdentifier() == "commodity")
    {
        m_WriteCommodity(dynamic_cast<Commodity *>(WriteObject));
    }
    else if(WriteObject->GetTypeIdentifier() == "generator")
    {
        m_WriteGenerator(dynamic_cast<Generator *>(WriteObject));
    }
    else if(WriteObject->GetTypeIdentifier() == "hangar")
    {
        m_WriteHangar(dynamic_cast<Hangar *>(WriteObject));
    }
    else if(WriteObject->GetTypeIdentifier() == "mind")
    {
        m_WriteMind(dynamic_cast<Mind *>(WriteObject));
    }
    else if(WriteObject->GetTypeIdentifier() == "projectile-weapon")
    {
        m_WriteProjectileWeapon(dynamic_cast<ProjectileWeapon *>(WriteObject));
    }
    else if(WriteObject->GetTypeIdentifier() == "ship")
    {
        m_WriteShip(dynamic_cast<Ship *>(WriteObject));
    }
    else if(WriteObject->GetTypeIdentifier() == "storage")
    {
        m_WriteStorage(dynamic_cast<Storage *>(WriteObject));
    }
    else if(WriteObject->GetTypeIdentifier() == "turret")
    {
        m_WriteTurret(dynamic_cast<Turret *>(WriteObject));
    }
    else
    {
        assert(false);
    }
    m_XMLStream << end;
    m_XMLStream << end;
}

auto GameSaver::m_WriteObjectRecursive(Object * WriteObject) -> void
{
    assert(WriteObject != nullptr);
    
    auto ObjectStack = std::stack<Object *>{};
    
    ObjectStack.push(WriteObject);
    while(ObjectStack.empty() == false)
    {
        auto TopObject = ObjectStack.top();
        
        ObjectStack.pop();
        if(TopObject->GetAspectObjectContainer() != nullptr)
        {
            for(auto Content : TopObject->GetAspectObjectContainer()->GetContent())
            {
                ObjectStack.push(Content);
            }
        }
        m_WriteObject(TopObject);
    }
}

auto GameSaver::m_WriteShip(Ship * Ship) -> void
{
    assert(Ship != nullptr);
    // save maximum & capacity values
    m_XMLStream << element << "fuel-capacity" << attribute << "value" << value << Ship->GetFuelCapacity() << end;
    m_XMLStream << element << "fuel-needed-to-jump" << attribute << "value" << value << Ship->GetFuelNeededToJump() << end;
    m_XMLStream << element << "hull-capacity" << attribute << "value" << value << Ship->GetHullCapacity() << end;
    m_XMLStream << element << "maximum-forward-thrust" << attribute << "value" << value << Ship->GetMaximumForwardThrust() << end;
    m_XMLStream << element << "maximum-speed" << attribute << "value" << value << Ship->GetMaximumSpeed() << end;
    m_XMLStream << element << "maximum-turn-speed" << attribute << "value" << value << Ship->GetMaximumTurnSpeed() << end;
    // save current values
    assert(Ship->GetFaction() != nullptr);
    m_XMLStream << element << "faction" << attribute << "object-identifier" << value << m_GetOrRegisterObjectIdentifier(Ship->GetFaction()) << end;
    m_XMLStream << element << "fuel" << attribute << "value" << value << Ship->GetFuel() << end;
    m_XMLStream << element << "hull" << attribute << "value" << value << Ship->GetHull() << end;
    m_XMLStream << element << "velocity" << attribute << "x" << value << Ship->GetVelocity()[0] << attribute << "y" << value << Ship->GetVelocity()[1] << attribute << "z" << value << Ship->GetVelocity()[2] << end;
}

auto GameSaver::m_WriteStorage(Storage * Storage) -> void
{
    assert(Storage != nullptr);
    m_XMLStream << element << "space-capacity" << attribute << "value" << value << Storage->GetSpaceCapacity() << end;
}

auto GameSaver::m_WriteTurret(Turret * Turret) -> void
{
    assert(Turret != nullptr);
    m_XMLStream << element << "energy" << attribute << "value" << value << Turret->GetEnergy() << end;
    m_XMLStream << element << "energy-capacity" << attribute << "value" << value << Turret->GetEnergyCapacity() << end;
    m_XMLStream << element << "maximum-power-input" << attribute << "value" << value << Turret->GetMaximumPowerInput() << end;
    m_XMLStream << element << "maximum-power-output" << attribute << "value" << value << Turret->GetMaximumPowerOutput() << end;
}

auto GameSaver::m_WriteProjectileWeapon(ProjectileWeapon * ProjectileWeapon) -> void
{
    assert(ProjectileWeapon != nullptr);
    m_XMLStream << element << "energy" << attribute << "value" << value << ProjectileWeapon->GetEnergy() << end;
    m_XMLStream << element << "energy-capacity" << attribute << "value" << value << ProjectileWeapon->GetEnergyCapacity() << end;
    m_XMLStream << element << "maximum-power-input" << attribute << "value" << value << ProjectileWeapon->GetMaximumPowerInput() << end;
    m_XMLStream << element << "maximum-power-output" << attribute << "value" << value << ProjectileWeapon->GetMaximumPowerOutput() << end;
}
