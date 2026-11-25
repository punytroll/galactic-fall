/**
 * galactic-fall
 * Copyright (C) 2018-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__SOURCE__GAME_SAVER_H
#define GALACTIC_FALL__SOURCE__GAME_SAVER_H

#include <unordered_map>

#include <algebra/vector3f.h>

#include <xml_stream/xml_stream.h>

#include <ostream>

class Ammunition;
class Battery;
class Character;
class CommandMind;
class Commodity;
class Galaxy;
class Generator;
class Hangar;
class Mind;
class Object;
class ProjectileWeapon;
class Ship;
class Storage;
class System;
class Turret;

class GameSaver
{
public:
    GameSaver(std::ostream & OStream);
    auto Save(Galaxy * Galaxy, System * CurrentSystem, float TimeWarp, Vector3f const & CameraOffset, float FieldOfViewY, CommandMind * CommandMind, Character * ObservedCharacter) -> void;
    
private:
    auto m_GetOrRegisterObjectIdentifier(Object * Object) -> std::string const &;
    auto m_WriteAmmunition(Ammunition * Ammunition) -> void;
    auto m_WriteBattery(Battery * Battery) -> void;
    auto m_WriteCharacter(Character * Character) -> void;
    auto m_WriteCommodity(Commodity * Commodity) -> void;
    auto m_WriteGenerator(Generator * Generator) -> void;
    auto m_WriteHangar(Hangar * Hangar) -> void;
    auto m_WriteMind(Mind * Mind) -> void;
    auto m_WriteObject(Object * Object) -> void;
    auto m_WriteObjectRecursive(Object * Object) -> void;
    auto m_WriteProjectileWeapon(ProjectileWeapon * ProjectileWeapon) -> void;
    auto m_WriteShip(Ship * Ship) -> void;
    auto m_WriteStorage(Storage * Storage) -> void;
    auto m_WriteTurret(Turret * Turret) -> void;
    
    std::unordered_map<Object *, std::string> m_IdentifiersByObject;
    std::ostream & m_OStream;
    XMLStream m_XMLStream;
};

#endif
